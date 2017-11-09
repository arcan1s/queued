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
 * @file QueuedControlGroupsAdaptor.cpp
 * Source code of queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>

#include <QDir>


/**
 * @class QueuedControlGroupsAdaptor
 */
/**
 * @fn QueuedControlGroupsAdaptor
 */
QueuedControlGroupsAdaptor::QueuedControlGroupsAdaptor(QObject *_parent,
                                                       QString _name)
    : QObject(_parent)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    m_name = std::move(_name);
    createGroup();
}


/**
 * @fn ~QueuedControlGroupsAdaptor
 */
QueuedControlGroupsAdaptor::~QueuedControlGroupsAdaptor()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    removeGroup();
}


/**
 * @fn groupPath
 */
QString QueuedControlGroupsAdaptor::groupPath(const QString &_base) const
{
    qCDebug(LOG_LIB) << "Get group path for base" << _base;

    return QDir(_base).filePath(name());
}


/**
 * @fn controlPaths
 */
QStringList QueuedControlGroupsAdaptor::controlPaths()
{
    return {cpuPath(), memoryPath()};
}


/**
 * @fn cpuPath
 */
QString QueuedControlGroupsAdaptor::cpuPath()
{
    return QDir(QueuedConfig::CG_FS_PATH).filePath("cpu");
}


/**
 * @fn memoryPath
 */
QString QueuedControlGroupsAdaptor::memoryPath()
{
    return QDir(QueuedConfig::CG_FS_PATH).filePath("memory");
}


/**
 * @fn cpuLimit
 */
long long QueuedControlGroupsAdaptor::cpuLimit() const
{
    QFile file(QDir(groupPath(cpuPath())).filePath(CG_CPU_LIMIT));

    long long limit = 0;
    if (file.open(QIODevice::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        limit = stream.readAll().toLongLong();
    } else {
        qCCritical(LOG_LIB) << "Could not get CPU limit" << name();
        return 0;
    }
    file.close();

    return limit * QueuedSystemInfo::cpuCount() / 1000;
}


/**
 * @fn memoryLimit
 */
long long QueuedControlGroupsAdaptor::memoryLimit() const
{
    QFile file(QDir(groupPath(memoryPath())).filePath(CG_MEMORY_LIMIT));

    long long limit = 0;
    if (file.open(QIODevice::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        limit = stream.readAll().toLongLong();
    } else {
        qCCritical(LOG_LIB) << "Could not get memory limit" << name();
        return 0;
    }
    file.close();

    return limit;
}


/**
 * @fn name
 */
QString QueuedControlGroupsAdaptor::name() const
{
    return m_name;
}


/**
 * @fn setCpuLimit
 */
void QueuedControlGroupsAdaptor::setCpuLimit(const long long _value)
{
    qCDebug(LOG_LIB) << "Set new CPU limit to" << _value;

    QFile file(QDir(groupPath(cpuPath())).filePath(CG_CPU_LIMIT));

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << _value;
        stream.flush();
    } else {
        qCCritical(LOG_LIB)
            << "Could not set CPU limit" << name() << "to" << _value;
        return;
    }
    file.close();
}


/**
 * @fn setMemoryLimit
 */
void QueuedControlGroupsAdaptor::setMemoryLimit(const long long _value)
{
    qCDebug(LOG_LIB) << "Set new memory limit to" << _value;

    QFile file(QDir(groupPath(memoryPath())).filePath(CG_MEMORY_LIMIT));

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << _value;
        stream.flush();
    } else {
        qCCritical(LOG_LIB)
            << "Could not set memory limit" << name() << "to" << _value;
        return;
    }
    file.close();

    Q_ASSERT(_value == memoryLimit());
}


/**
 * @fn assignGroup
 */
bool QueuedControlGroupsAdaptor::addProcess(const uint _pid)
{
    qCDebug(LOG_LIB) << "Assign add process" << _pid;

    for (auto &path : controlPaths()) {
        auto proc = QDir(groupPath(path)).filePath(CG_PROC_FILE);
        QFile file(proc);

        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << _pid;
        } else {
            qCCritical(LOG_LIB)
                << "Cound not assign pid" << _pid << "to" << proc;
            return false;
        }

        file.close();
    }

    return true;
}


/**
 * @fn createGroup
 */
bool QueuedControlGroupsAdaptor::createGroup()
{
    qCDebug(LOG_LIB) << "Create empty group";

    auto paths = controlPaths();

    // create cgroups
    bool status = std::all_of(
        paths.cbegin(), paths.cend(),
        [this](const QString &path) { return QDir(path).mkpath(name()); });
    // apply settings
    status &= std::all_of(
        paths.cbegin(), paths.cend(), [this](const QString &path) {
            auto notify
                = QDir(groupPath(path)).filePath(CG_NOTIFY_ON_RELEASE_FILE);
            QFile file(notify);
            if (file.open(QIODevice::WriteOnly)) {
                QTextStream stream(&file);
                stream << 1;
            } else {
                qCCritical(LOG_LIB)
                    << "Could not apply rules to" << CG_NOTIFY_ON_RELEASE_FILE;
                return false;
            }
            return true;
        });
    status &= std::all_of(
        paths.cbegin(), paths.cend(), [this](const QString &path) {
            auto agent = QDir(groupPath(path)).filePath(CG_RELEASE_FILE);
            QFile file(agent);
            if (file.open(QIODevice::WriteOnly)) {
                QTextStream stream(&file);
                stream
                    << QString("rmdir \"%1\"").arg(QDir(path).filePath(name()));
            } else {
                qCCritical(LOG_LIB)
                    << "Could not apply rules to" << CG_RELEASE_FILE;
                return false;
            }
            return true;
        });


    return status;
}


/**
 * @fn removeGroup
 */
bool QueuedControlGroupsAdaptor::removeGroup()
{
    qCDebug(LOG_LIB) << "Remove group";

    auto paths = controlPaths();

    return std::all_of(
        paths.cbegin(), paths.cend(),
        [this](const QString &path) { return QDir(path).rmdir(name()); });
}
