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
 * @file QueuedProcess.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>

#include <QMetaProperty>


/**
 * @class QueuedProcess
 */
/**
 * @fn QueuedProcess
 */
QueuedProcess::QueuedProcess(QObject *parent,
                             const QueuedProcessDefinitions &definitions,
                             const long long index)
    : QProcess(parent)
    , m_definitions(definitions)
    , m_index(index)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    // update QProcess related values as well
    // TODO configuration
    setCommand(m_definitions.command);
    setCommandArguments(m_definitions.arguments);
    setWorkDirectory(m_definitions.workingDirectory);

    setStandardErrorFile(QString("%1-err.log").arg(name()), QIODevice::Append);
    setStandardOutputFile(QString("%1-out.log").arg(name()), QIODevice::Append);
}


/**
 * @fn ~QueuedProcess
 */
QueuedProcess::~QueuedProcess()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;
}


/**
 * @fn updateArguments
 */
void QueuedProcess::updateArguments()
{
    QString application = processLine();

    // replace generic properties first
    auto meta = metaObject();
    int count = meta->propertyCount();
    for (int i = 0; i < count; i++) {
        QMetaProperty prop = meta->property(i);
        auto name = prop.name();
        // replace string now
        application.replace(QString("{%1}").arg(name),
                            property(name).toString());
    }

    // replace limits now
    application.replace(
        "{cpu}", QString("%1").arg(
                     QueuedSystemInfo::cpuWeight(nativeLimits().cpu) * 100.0, 0,
                     'f', 0));
    application.replace(
        "{memory}",
        QString("%1").arg(QueuedSystemInfo::memoryWeight(nativeLimits().memory)
                              * QueuedSystemInfo::memoryCount(),
                          0, 'f', 0));

    // command line
    QString commandLine = command() + "\x01" + commandArguments().join('\x01');
    application.replace("{app}", commandLine);

    QStringList arguments = application.split('\x01');

    // set QProcess properties
    setProgram(arguments.takeFirst());
    setCommandArguments(arguments);
}


/**
 * @fn index
 */
long long QueuedProcess::index() const
{
    return m_index;
}


/**
 * @fn name
 */
QString QueuedProcess::name() const
{
    return QString("queued-task-%1").arg(index());
}


/**
 * @fn command
 */
QString QueuedProcess::command() const
{
    return m_definitions.command;
}


/**
 * @fn commandArguments
 */
QStringList QueuedProcess::commandArguments() const
{
    return m_definitions.arguments;
}


/**
 * @fn endTime
 */
QDateTime QueuedProcess::endTime() const
{
    return m_definitions.endTime;
}


/**
 * @fn gid
 */
uint QueuedProcess::gid() const
{
    return m_definitions.gid;
}


/**
 * @fn limits
 */
QString QueuedProcess::limits() const
{
    return m_definitions.limits;
}


/**
 * @fn nativeLimits
 */
QueuedLimits::Limits QueuedProcess::nativeLimits() const
{
    return QueuedLimits::Limits(limits());
}


/**
 * @fn nice
 */
uint QueuedProcess::nice() const
{
    return m_definitions.nice;
}


/**
 * @fn processLine
 */
QString QueuedProcess::processLine() const
{
    return m_processLine;
}


/**
 * @fn pstate
 */
QueuedEnums::ProcessState QueuedProcess::pstate() const
{
    return m_definitions.state;
}


/**
 * @fn startTime
 */
QDateTime QueuedProcess::startTime() const
{
    return m_definitions.startTime;
}


/**
 * @fn uid
 */
uint QueuedProcess::uid() const
{
    return m_definitions.uid;
}


/**
 * @fn user
 */
long long QueuedProcess::user() const
{
    return m_definitions.user;
}


/**
 * @fn workDirectory
 */
QString QueuedProcess::workDirectory() const
{
    return m_definitions.workingDirectory;
}


/**
 * @fn setCommand
 */
void QueuedProcess::setCommand(const QString &_command)
{
    qCDebug(LOG_LIB) << "Set command to" << _command;

    m_definitions.command = _command;
    updateArguments();
}


/**
 * @fn setCommandArguments
 */
void QueuedProcess::setCommandArguments(const QStringList &_commandArguments)
{
    qCDebug(LOG_LIB) << "Set command line arguments to" << _commandArguments;

    m_definitions.arguments = _commandArguments;
    updateArguments();
}


/**
 * @fn setEndTime
 */
void QueuedProcess::setEndTime(const QDateTime &_time)
{
    qCDebug(LOG_LIB) << "Set end time to" << _time;

    m_definitions.endTime = _time;
}


/**
 * @fn setGid
 */
void QueuedProcess::setGid(const uint _gid)
{
    qCDebug(LOG_LIB) << "Set process GID to" << _gid;

    m_definitions.gid = _gid;
    updateArguments();
}


/**
 * setLimits
 */
void QueuedProcess::setLimits(const QString &_limits)
{
    qCDebug(LOG_LIB) << "Set process limits" << _limits;

    m_definitions.limits = _limits;
    updateArguments();
}


/**
 * @fn setNice
 */
void QueuedProcess::setNice(const uint _nice)
{
    qCDebug(LOG_LIB) << "Set nice level to" << _nice;

    m_definitions.nice = _nice;
}


/**
 * @fn setProcessLine
 */
void QueuedProcess::setProcessLine(const QString &_processLine)
{
    qCDebug(LOG_LIB) << "Set process line to" << _processLine;

    m_processLine = _processLine;
    updateArguments();
}


/**
 * @fn setPState
 */
void QueuedProcess::setPState(const QueuedEnums::ProcessState _state)
{
    qCDebug(LOG_LIB) << "Set process state to" << static_cast<int>(_state);

    m_definitions.state = _state;
}


/**
 * @fn setStartTime
 */
void QueuedProcess::setStartTime(const QDateTime &_time)
{
    qCDebug(LOG_LIB) << "Set start time to" << _time;

    m_definitions.startTime = _time;
}


/**
 * @fn setUid
 */
void QueuedProcess::setUid(const uint _uid)
{
    qCDebug(LOG_LIB) << "Set process UID to" << _uid;

    m_definitions.uid = _uid;
    updateArguments();
}


/**
 * @fn setUser
 */
void QueuedProcess::setUser(const long long _user)
{
    qCDebug(LOG_LIB) << "Set user to" << _user;

    m_definitions.user = _user;
}


/**
 * @fn setWorkDirectory
 */
void QueuedProcess::setWorkDirectory(const QString &_workDirectory)
{
    qCDebug(LOG_LIB) << "Set working directory to" << _workDirectory;

    m_definitions.workingDirectory = _workDirectory;
    setWorkingDirectory(_workDirectory);
}


/**
 * @fn operator==
 */
bool QueuedProcess::operator==(const QueuedProcess &_other)
{
    return name() == _other.name();
}
