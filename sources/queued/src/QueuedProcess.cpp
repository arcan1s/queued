/*
 * Copyright (c) 2017 Queued team
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
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>

#include <QDir>
#include <QMetaProperty>
#include <QStandardPaths>

#include <cmath>
#include <csignal>

extern "C" {
#include <unistd.h>
}


/**
 * @class QueuedProcess
 */
/**
 * @fn QueuedProcess
 */
QueuedProcess::QueuedProcess(QObject *_parent, const QueuedProcessDefinitions &definitions,
                             const long long index)
    : QProcess(_parent)
    , m_definitions(definitions)
    , m_index(index)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    qRegisterMetaType<QueuedLimits::Limits>("QueuedLimits::Limits");

    m_cgroup = new QueuedControlGroupsAdaptor(this, name());

    // update QProcess related values as well
    setProgram(m_definitions.command);
    setArguments(m_definitions.arguments);
    setWorkDirectory(m_definitions.workingDirectory);

    connect(this, SIGNAL(started()), this, SLOT(applyCGroup()));
}


/**
 * @fn ~QueuedProcess
 */
QueuedProcess::~QueuedProcess()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;
}


/**
 * @fn killChildren
 */
void QueuedProcess::killChildren()
{
    auto pids = childrenPids();
    qCInfo(LOG_LIB) << "Found children pids" << pids;

    for (auto pid : pids) {
        if (::kill(pid, SIGTERM) != 0) {
            qCWarning(LOG_LIB) << "SIGTERM failed, trying to kill";
            ::kill(pid, SIGKILL);
        }
    }
}


/**
 * @fn childrenPids
 */
QList<Q_PID> QueuedProcess::childrenPids() const
{
    QStringList allDirectories
        = QDir("/proc").entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    QStringList directories = allDirectories.filter(QRegExp("(\\d+)"));

    QList<Q_PID> pids
        = std::accumulate(directories.cbegin(), directories.cend(), QList<Q_PID>(),
                          [this](QList<Q_PID> &list, const QString &dir) {
                              QFile statFile(QString("/proc/%1/stat").arg(dir));
                              if (!statFile.open(QIODevice::ReadOnly | QIODevice::Text))
                                  return list;

                              QString output = statFile.readAll();
                              output.remove(QRegExp("\\d+ \\(.*\\) . "));
                              Q_PID ppid = output.split(' ').first().toLongLong();
                              if (ppid == pid())
                                  list.append(dir.toLongLong());

                              statFile.close();

                              return list;
                          });

    return pids;
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
 * @fn logError
 */
QString QueuedProcess::logError() const
{
    return QString("%1/%2-err.log").arg(workDirectory()).arg(name());
}


/**
 * @fn logOutput
 */
QString QueuedProcess::logOutput() const
{
    return QString("%1/%2-out.log").arg(workDirectory()).arg(name());
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
}


/**
 * @fn setLimits
 */
void QueuedProcess::setLimits(const QString &_limits)
{
    qCDebug(LOG_LIB) << "Set process limits" << _limits;

    m_definitions.limits = _limits;
}


/**
 * @fn setLogError
 */
void QueuedProcess::setLogError(const QString &)
{
    setStandardErrorFile(logError(), QIODevice::Append);
}


/**
 * @fn setLogOutput
 */
void QueuedProcess::setLogOutput(const QString &)
{
    setStandardOutputFile(logOutput(), QIODevice::Append);
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

    m_definitions.workingDirectory
        = _workDirectory.isEmpty()
              ? QStandardPaths::writableLocation(QStandardPaths::StandardLocation::TempLocation)
              : _workDirectory;
    setLogError("");
    setLogOutput("");
    setWorkingDirectory(m_definitions.workingDirectory);
}


/**
 * @fn operator==
 */
bool QueuedProcess::operator==(const QueuedProcess &_other)
{
    return name() == _other.name();
}


/**
 * applyCGroup
 */
void QueuedProcess::applyCGroup()
{
    m_cgroup->addProcess(pid());
}


/**
 * @fn setupChildProcess
 */
void QueuedProcess::setupChildProcess()
{
    // configure cgroup here and apply limits
    m_cgroup->createGroup();
    auto nl = nativeLimits();
    m_cgroup->setCpuLimit(std::llround(QueuedSystemInfo::cpuWeight(nl.cpu) * 100.0));
    m_cgroup->setMemoryLimit(
        std::llround(QueuedSystemInfo::memoryWeight(nl.memory) * QueuedSystemInfo::memoryCount()));

    // setup child properties
    ::setuid(m_definitions.uid);
    ::setgid(m_definitions.gid);

    return QProcess::setupChildProcess();
}
