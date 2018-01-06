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
 * @file QueuedCorePrivate.cpp
 * Source code of queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>
#include <queued/private/QueuedCorePrivate.h>

#include "queued/QueuedDatabaseSchema.h"

#include <queued/private/QueuedCorePrivateHelper.h>


/**
 * @class QueuedCorePrivate
 */
/**
 * @fn updateSettings
 */
void QueuedCorePrivate::updateSettings(const QueuedConfig::QueuedSettings _id, const QString &_key,
                                       const QVariant &_value)
{
    qCDebug(LOG_LIB) << "Received update for" << static_cast<int>(_id) << _key << "with value"
                     << _value;

    // FIXME probably there is a better way to change settings
    switch (_id) {
    case QueuedConfig::QueuedSettings::Invalid:
        // check if it is plugin settings
        if (_key.startsWith("Plugin."))
            m_plugins->optionChanged(_key, _value);
        // do nothing otherwise
        break;
    case QueuedConfig::QueuedSettings::DatabaseInterval:
        m_databaseManager->setInterval(_value.toLongLong());
        break;
    case QueuedConfig::QueuedSettings::DatabaseVersion:
        break;
    case QueuedConfig::QueuedSettings::DefaultLimits:
        break;
    case QueuedConfig::QueuedSettings::KeepTasks:
        m_databaseManager->setKeepTasks(_value.toLongLong());
        break;
    case QueuedConfig::QueuedSettings::KeepUsers:
        m_databaseManager->setKeepUsers(_value.toLongLong());
        break;
    case QueuedConfig::QueuedSettings::OnExitAction:
        m_processes->setExitAction(static_cast<QueuedEnums::ExitAction>(_value.toInt()));
        break;
    case QueuedConfig::QueuedSettings::Plugins:
        // do nothing here
        break;
    case QueuedConfig::QueuedSettings::ServerAddress:
    case QueuedConfig::QueuedSettings::ServerMaxConnections:
    case QueuedConfig::QueuedSettings::ServerPort:
    case QueuedConfig::QueuedSettings::ServerTimeout:
        // do nothing here
        break;
    case QueuedConfig::QueuedSettings::TokenExpiration:
        m_users->setTokenExpiration(_value.toLongLong());
        break;
    }
}


/**
 * @fn updateTaskTime
 */
void QueuedCorePrivate::updateTaskTime(const long long _id, const QDateTime &_startTime,
                                       const QDateTime &_endTime)
{
    qCDebug(LOG_LIB) << "Update task" << _id << "time to" << _startTime << _endTime;

    QVariantHash record;
    if (_startTime.isValid()) {
        record["startTime"] = _startTime.toString(Qt::ISODateWithMs);
        if (m_plugins)
            emit(m_plugins->interface()->onStartTask(_id));
    }
    if (_endTime.isValid()) {
        record["endTime"] = _endTime.toString(Qt::ISODateWithMs);
        if (m_plugins)
            emit(m_plugins->interface()->onStopTask(_id));
    }

    bool status = m_database->modify(QueuedDB::TASKS_TABLE, _id, record);
    if (!status)
        qCWarning(LOG_LIB) << "Could not modify task record" << _id;
}


/**
 * @fn updateUserLoginTime
 */
void QueuedCorePrivate::updateUserLoginTime(const long long _id, const QDateTime &_time)
{
    qCDebug(LOG_LIB) << "Update user" << _id << "with login time" << _time;

    QVariantHash record = {{"lastLogin", _time.toString(Qt::ISODateWithMs)}};

    bool status = m_database->modify(QueuedDB::USERS_TABLE, _id, record);
    if (!status)
        qCWarning(LOG_LIB) << "Could not modify user record" << _id;
}
