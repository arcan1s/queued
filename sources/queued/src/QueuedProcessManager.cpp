/*
 * Copyright (c) 2017 Evgeniy Alekseev
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 *
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 */
/**
 * @file QueuedProcessManager.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>

extern "C" {
#include <unistd.h>
}


/**
 * @fn QueuedProcessManager
 */
QueuedProcessManager::QueuedProcessManager(QObject *parent,
                                           const QString &processLine,
                                           const QueuedEnums::ExitAction onExit)
    : QObject(parent)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    qRegisterMetaType<QueuedEnums::ExitAction>("QueuedEnums::ExitAction");

    setExitAction(onExit);
    setProcessLine(processLine);
}


/**
 * @fn ~QueuedProcessManager
 */
QueuedProcessManager::~QueuedProcessManager()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    QList<long long> indices = processes().keys();
    for (auto index : indices)
        remove(index);
}


/**
 * @fn add
 */
QueuedProcess *QueuedProcessManager::add(const QVariantHash &_properties,
                                         const long long _index)
{
    qCDebug(LOG_LIB) << "Add new process" << _properties << "with index"
                     << _index;

    QueuedProcess::QueuedProcessDefinitions defs;
    // parameters
    defs.command = _properties[QString("command")].toString();
    defs.arguments = _properties[QString("commandArguments")].toString().split(
        QChar('\n'));
    defs.workingDirectory = _properties[QString("workDirectory")].toString();
    defs.nice = _properties[QString("nice")].toUInt();
    defs.limits = _properties[QString("limits")].toString();
    // user data
    defs.uid = _properties[QString("uid")].toUInt();
    defs.gid = _properties[QString("gid")].toUInt();
    defs.user = _properties[QString("user")].toLongLong();
    // metadata
    defs.startTime = QDateTime::fromString(
        _properties[QString("startTime")].toString(), Qt::ISODateWithMs);
    defs.endTime = QDateTime::fromString(
        _properties[QString("endTime")].toString(), Qt::ISODateWithMs);

    return add(defs, _index);
}


/**
 * @fn add
 */
QueuedProcess *QueuedProcessManager::add(
    const QueuedProcess::QueuedProcessDefinitions &_definitions,
    const long long _index)
{
    qCDebug(LOG_LIB) << "Add new process" << _definitions.command
                     << "with index" << _index;

    if (processes().contains(_index))
        return process(_index);

    QueuedProcess *process = new QueuedProcess(this, _definitions, _index);
    process->setProcessLine(processLine());
    m_processes[_index] = process;
    // connect to signal
    m_connections[_index] = connect(
        process, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(
                     &QProcess::finished),
        [=](const int exitCode, const QProcess::ExitStatus exitStatus) {
            return taskFinished(exitCode, exitStatus, _index);
        });

    // check if we can start new task
    start();
    return process;
}


/**
 * @fn loadProcesses
 */
void QueuedProcessManager::loadProcesses(const QList<QVariantHash> &_processes)
{
    qCDebug(LOG_LIB) << "Add tasks from" << _processes;

    for (auto &processData : _processes)
        add(processData, processData[QString("_id")].toLongLong());
}


/**
 * @fn process
 */
QueuedProcess *QueuedProcessManager::process(const long long _index)
{
    qCDebug(LOG_LIB) << "Get process by index" << _index;

    return m_processes.contains(_index) ? m_processes[_index] : nullptr;
}


/**
 * @fn processes
 */
QueuedProcessManager::QueuedProcessMap QueuedProcessManager::processes()
{
    return m_processes;
}


/**
 * @fn remove
 */
void QueuedProcessManager::remove(const long long _index)
{
    qCDebug(LOG_LIB) << "Remove process by index" << _index;

    if (!processes().contains(_index))
        return;

    QueuedProcess *pr = m_processes.take(_index);
    auto connection = m_connections.take(_index);
    disconnect(connection);

    switch (onExit()) {
    case QueuedEnums::ExitAction::Kill:
        pr->kill();
        break;
    case QueuedEnums::ExitAction::Terminate:
        pr->terminate();
        break;
    }

    pr->deleteLater();
}


/**
 * @fn start
 */
void QueuedProcessManager::start()
{
    qCDebug(LOG_LIB) << "Start random task";

    long long index = -1;
    // gather used resources
    QueuedLimits::Limits limits = usedLimits();
    double weightedCpu
        = limits.cpu == 0 ? 0.0 : QueuedSystemInfo::cpuWeight(limits.cpu);
    double weightedMemory = limits.memory == 0
                                ? 0.0
                                : QueuedSystemInfo::memoryWeight(limits.memory);

    auto tasks = processes().values();
    for (auto pr : tasks) {
        // check task state
        if (pr->state() != QProcess::ProcessState::NotRunning)
            continue;
        // check limits first
        if (((1.0 - weightedCpu)
             < QueuedSystemInfo::cpuWeight(pr->nativeLimits().cpu))
            && ((1.0 - weightedMemory)
                < QueuedSystemInfo::memoryWeight(pr->nativeLimits().memory)))
            continue;
        // now check nice level
        if ((index > -1) && (pr->nice() < process(index)->nice()))
            continue;
        // now check index value
        if ((index > -1) && (pr->index() > index))
            continue;
        // hmmm, looks like we found a candidate
        index = pr->index();
    }

    if (index > -1)
        return start(index);
}


/**
 * @fn start
 */
void QueuedProcessManager::start(const long long _index)
{
    qCDebug(LOG_LIB) << "Start task" << _index;

    auto pr = process(_index);
    if (!pr) {
        qCWarning(LOG_LIB) << "No task" << _index << "found";
        return;
    }

    QDateTime start = QDateTime::currentDateTimeUtc();
    pr->start();
    pr->setStartTime(start);
    emit(taskStartTimeReceived(_index, start));
}


/**
 * @fn stop
 */
void QueuedProcessManager::stop(const long long _index)
{
    qCDebug(LOG_LIB) << "Stop task" << _index;

    auto pr = process(_index);
    if (!pr) {
        qCWarning(LOG_LIB) << "No task" << _index << "found";
        return;
    }

    switch (onExit()) {
    case QueuedEnums::ExitAction::Kill:
        pr->kill();
        break;
    case QueuedEnums::ExitAction::Terminate:
        pr->terminate();
        break;
    }
}


/**
 * @fn onExit
 */
QueuedEnums::ExitAction QueuedProcessManager::onExit() const
{
    return m_onExit;
}


/**
 * @fn processLine
 */
QString QueuedProcessManager::processLine() const
{
    return m_processLine;
}


/**
 * @fn setExitAction
 */
void QueuedProcessManager::setExitAction(const QueuedEnums::ExitAction _action)
{
    qCDebug(LOG_LIB) << "New action on exit" << static_cast<int>(_action);

    m_onExit = _action;
}


/**
 * @fn setProcessLine
 */
void QueuedProcessManager::setProcessLine(const QString _processLine)
{
    qCDebug(LOG_LIB) << "Set process line to" << _processLine;

    m_processLine = _processLine;
    for (auto process : processes().values())
        process->setProcessLine(processLine());
}


/**
 * @fn usedLimits
 */
QueuedLimits::Limits QueuedProcessManager::usedLimits()
{
    auto tasks = processes().values();
    long long cpu = std::accumulate(
        tasks.cbegin(), tasks.cend(), 0,
        [](long long value, QueuedProcess *process) {
            auto limit = process->nativeLimits().cpu == 0
                             ? QueuedSystemInfo::cpuCount()
                             : process->nativeLimits().cpu;
            return process->state() == QProcess::ProcessState::Running
                       ? value + limit
                       : value;
        });
    long long memory = std::accumulate(
        tasks.cbegin(), tasks.cend(), 0,
        [](long long value, QueuedProcess *process) {
            auto limit = process->nativeLimits().memory == 0
                             ? QueuedSystemInfo::memoryCount()
                             : process->nativeLimits().memory;
            return process->state() == QProcess::ProcessState::Running
                       ? value + limit
                       : value;
        });
    long long storage = std::accumulate(
        tasks.cbegin(), tasks.cend(), 0,
        [](long long value, QueuedProcess *process) {
            return process->state() == QProcess::ProcessState::Running
                       ? value + process->nativeLimits().storage
                       : value;
        });

    return QueuedLimits::Limits(cpu, 0, memory, 0, storage);
}


/**
 * @fn taskFinished
 */
void QueuedProcessManager::taskFinished(const int _exitCode,
                                        const QProcess::ExitStatus _exitStatus,
                                        const long long _index)
{
    qCDebug(LOG_LIB) << "Process" << _index << "finished with code" << _exitCode
                     << "and status" << _exitStatus;

    auto pr = process(_index);
    if (pr) {
        // change log rights to valid one
        ::chown(qPrintable(pr->logError()), pr->uid(), pr->gid());
        ::chown(qPrintable(pr->logOutput()), pr->uid(), pr->gid());
        // remove task
        auto endTime = QDateTime::currentDateTimeUtc();
        remove(_index);
        emit(taskStopTimeReceived(_index, endTime));
    }

    start();
}
