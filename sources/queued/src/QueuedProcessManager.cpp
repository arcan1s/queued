/*
 * Copyright (c) 2016 Evgeniy Alekseev
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


#include "queued/Queued.h"


/**
 * @fn QueuedProcessManager
 */
QueuedProcessManager::QueuedProcessManager(QObject *parent,
                                           const OnExitAction onExit)
    : QObject(parent)
    , m_onExit(onExit)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;
}


/**
 * @fn ~QueuedProcessManager
 */
QueuedProcessManager::~QueuedProcessManager()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    QList<long long> indices = m_processes.keys();
    for (auto index : indices)
        remove(index);
}


/**
 * @fn add
 */
QueuedProcess *QueuedProcessManager::add(
    const long long _index,
    const QueuedProcess::QueuedProcessDefinitions _definitions)
{
    qCDebug(LOG_LIB) << "Add new process" << _definitions.cmd << "with index"
                     << _index;

    if (m_processes.contains(_index))
        return m_processes[_index];

    QueuedProcess *process = new QueuedProcess(this, _definitions, _index);
    m_processes[_index] = process;
    // connect to signal
    m_connections[_index] = connect(
        process, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(
                     &QProcess::finished),
        [=](const int exitCode, const QProcess::ExitStatus exitStatus) {
            return taskFinished(exitCode, exitStatus, _index);
        });

    return process;
}


/**
 * @fn onExit
 */
QueuedProcessManager::OnExitAction QueuedProcessManager::onExit() const
{
    return m_onExit;
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

    if (!m_processes.contains(_index))
        return;

    QueuedProcess *pr = m_processes.take(_index);
    auto connection = m_connections.take(_index);
    disconnect(connection);

    switch (m_onExit) {
    case OnExitAction::Kill:
        pr->kill();
        break;
    case OnExitAction::Terminate:
        pr->terminate();
        break;
    }

    pr->deleteLater();
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

    switch (m_onExit) {
    case OnExitAction::Kill:
        pr->kill();
        break;
    case OnExitAction::Terminate:
        pr->terminate();
        break;
    }
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

    emit(taskStopTimeReceived(_index, QDateTime::currentDateTimeUtc()));
    // TODO implementation
    // TODO emit signal for new task here
    // emit(taskStartTimeReceived(_index, QDateTime::currentDateTimeUtc()));
}
