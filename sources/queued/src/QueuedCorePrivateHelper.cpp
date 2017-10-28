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
 * @file QueuedCorePrivateHelper.cpp
 * Source code of queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>
#include <queued/private/QueuedCorePrivateHelper.h>

#include "queued/QueuedDatabaseSchema.h"

#include <queued/private/QueuedCorePrivate.h>


/**
 * @class QueuedCorePrivateHelper
 */
/**
 * @fn QueuedCorePrivateHelper
 */
QueuedCorePrivateHelper::QueuedCorePrivateHelper(QObject *_parent)
    : QObject(_parent)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    m_core = static_cast<QueuedCorePrivate *>(parent());
}


/**
 * @fn ~QueuedCorePrivateHelper
 */
QueuedCorePrivateHelper::~QueuedCorePrivateHelper()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;
}


/**
 * @fn advancedSettings
 */
QueuedAdvancedSettings *QueuedCorePrivateHelper::advancedSettings()
{
    return m_core->m_advancedSettings;
}


/**
 * @fn database
 */
QueuedDatabase *QueuedCorePrivateHelper::database()
{
    return m_core->m_database;
}


/**
 * @fn plugins
 */
QueuedPluginManager *QueuedCorePrivateHelper::plugins()
{
    return m_core->m_plugins;
}


/**
 * @fn processes
 */
QueuedProcessManager *QueuedCorePrivateHelper::processes()
{
    return m_core->m_processes;
}


/**
 * @fn users
 */
QueuedUserManager *QueuedCorePrivateHelper::users()
{
    return m_core->m_users;
}


/**
 * @fn dropAdminFields
 */
QVariantHash
QueuedCorePrivateHelper::dropAdminFields(const QString &_table,
                                         const QVariantHash &_payload)
{
    qCDebug(LOG_LIB) << "Drop admin fields from" << _payload << "in table"
                     << _table;

    QVariantHash payload;
    for (auto &key : _payload.keys()) {
        if (QueuedDB::DBSchema[_table][key].adminField)
            continue;
        payload[key] = _payload[key];
    }

    return payload;
}


/**
 * @addTaskPrivate
 */
QueuedResult<long long> QueuedCorePrivateHelper::addTaskPrivate(
    const QString &_command, const QStringList &_arguments,
    const QString &_workingDirectory, const long long _userId,
    const QueuedLimits::Limits &_limits)
{
    qCDebug(LOG_LIB) << "Add task" << _command << "with arguments" << _arguments
                     << "from user" << _userId;

    // add to database
    auto ids = users()->ids(_userId);
    auto userObj = m_core->user(_userId);
    if (!userObj) {
        qCWarning(LOG_LIB) << "Could not find task user" << _userId;
        return QueuedError("Invalid token",
                           QueuedEnums::ReturnStatus::InvalidToken);
    }
    auto taskLimits = QueuedLimits::minimalLimits(
        _limits, userObj->nativeLimits(),
        QueuedLimits::Limits(
            advancedSettings()
                ->get(QueuedConfig::QueuedSettings::DefaultLimits)
                .toString()));
    QVariantHash properties = {{"user", _userId},
                               {"command", _command},
                               {"commandArguments", _arguments},
                               {"workDirectory", _workingDirectory},
                               {"nice", 0},
                               {"uid", ids.first},
                               {"gid", ids.second},
                               {"limits", taskLimits.toString()}};
    auto id = database()->add(QueuedDB::TASKS_TABLE, properties);
    if (id == -1) {
        qCWarning(LOG_LIB) << "Could not add task" << _command;
        return QueuedError("", QueuedEnums::ReturnStatus::Error);
    }

    // add to child object
    processes()->add(properties, id);
    // notify plugins
    if (plugins())
        emit(plugins()->interface()->onAddTask(id));

    return id;
}


/**
 * @fn addUserPrivate
 */
QueuedResult<long long> QueuedCorePrivateHelper::addUserPrivate(
    const QString &_name, const QString &_email, const QString &_password,
    const uint _permissions, const QueuedLimits::Limits &_limits)
{
    qCDebug(LOG_LIB) << "Add user" << _name << "with email" << _email
                     << "and permissions" << _permissions;
    // add to database
    QVariantHash properties = {{"name", _name},
                               {"password", _password},
                               {"email", _email},
                               {"permissions", _permissions},
                               {"limits", _limits.toString()}};
    auto id = database()->add(QueuedDB::USERS_TABLE, properties);
    if (id == -1) {
        qCWarning(LOG_LIB) << "Could not add user" << _name;
        return QueuedError("", QueuedEnums::ReturnStatus::Error);
    }

    // add to child object
    users()->add(properties, id);
    // notify plugins
    if (plugins())
        emit(plugins()->interface()->onAddUser(id));

    return id;
}


/**
 * @fn editOptionPrivate
 */
QueuedResult<bool>
QueuedCorePrivateHelper::editOptionPrivate(const QString &_key,
                                           const QVariant &_value)
{
    qCDebug(LOG_LIB) << "Set key" << _key << "to" << _value;

    // add to database
    long long id = advancedSettings()->id(_key);
    QVariantHash payload = {{"key", _key}, {"value", _value}};

    bool status;
    if (id == -1) {
        id = database()->add(QueuedDB::SETTINGS_TABLE, payload);
        qCInfo(LOG_LIB) << "Added new key with ID" << id;
        status = (id != -1);
    } else {
        status = database()->modify(QueuedDB::SETTINGS_TABLE, id, payload);
        qCInfo(LOG_LIB) << "Value for" << _key
                        << "has been modified with status" << status;
    }

    // add to child object
    if (status) {
        advancedSettings()->set(_key, _value);
        // notify plugins if required
        if (plugins()) {
            auto tryPluginOption = plugins()->convertOptionName(_key);
            if ((!tryPluginOption.first.isEmpty())
                && (!tryPluginOption.second.isEmpty()))
                plugins()->optionChanged(_key, _value);
            // notify plugins
            emit(plugins()->interface()->onEditOption(_key, _value));
        }
    }

    return status;
}


/**
 * @fn editPluginPrivate
 */
QueuedResult<bool>
QueuedCorePrivateHelper::editPluginPrivate(const QString &_plugin,
                                           const bool _add)
{
    qCDebug(LOG_LIB) << "Edit plugin" << _plugin << "add" << _add;

    QStringList pluginList = advancedSettings()
                                 ->get(QueuedConfig::QueuedSettings::Plugins)
                                 .toString()
                                 .split('\n');

    QueuedResult<bool> r;
    if (_add && !pluginList.contains(_plugin)) {
        if (plugins()->loadPlugin(_plugin, m_core->pluginSettings(_plugin))) {
            pluginList.append(_plugin);
            r = true;
        }
    } else if (!_add && pluginList.contains(_plugin)) {
        if (plugins()->unloadPlugin(_plugin)) {
            pluginList.removeAll(_plugin);
            r = true;
        }
    } else {
        qCDebug(LOG_LIB) << "Plugin" << _plugin
                         << "not loaded or already loaded";
        r = QueuedError("Plugin is not loaded or already loaded",
                        QueuedEnums::ReturnStatus::Error);
    }

    if (r.type() == Result::Content::Value) {
        editOptionPrivate(advancedSettings()->internalId(
                              QueuedConfig::QueuedSettings::Plugins),
                          pluginList.join('\n'));
        // notify plugins
        if (plugins()) {
            if (_add)
                emit(plugins()->interface()->onAddPlugin(_plugin));
            else
                emit(plugins()->interface()->onRemovePlugin(_plugin));
        }
    }

    return r;
}


/**
 * @fn editTaskPrivate
 */
QueuedResult<bool>
QueuedCorePrivateHelper::editTaskPrivate(const long long _id,
                                         const QVariantHash &_taskData)
{
    qCDebug(LOG_LIB) << "Edit task with ID" << _id;

    auto task = processes()->process(_id);
    if (!task) {
        qCWarning(LOG_LIB) << "Could not find task with ID" << _id;
        return QueuedError("Task does not exist",
                           QueuedEnums::ReturnStatus::InvalidArgument);
    }

    // modify record in database first
    bool status = database()->modify(QueuedDB::TASKS_TABLE, _id, _taskData);
    if (!status) {
        qCWarning(LOG_LIB) << "Could not modify task record" << _id
                           << "in database, do not edit it in memory";
        return QueuedError("", QueuedEnums::ReturnStatus::Error);
    }

    // modify values stored in memory
    for (auto &property : _taskData.keys())
        task->setProperty(qPrintable(property), _taskData[property]);
    // notify plugins
    if (plugins())
        emit(plugins()->interface()->onEditTask(_id, _taskData));

    return true;
}


/**
 * @fn editUserPrivate
 */
QueuedResult<bool>
QueuedCorePrivateHelper::editUserPrivate(const long long _id,
                                         const QVariantHash &_userData)
{
    qCDebug(LOG_LIB) << "Edit user with ID" << _id;

    auto userObj = users()->user(_id);
    if (!userObj) {
        qCWarning(LOG_LIB) << "Could not find user with ID" << _id;
        return QueuedError("User does not exist",
                           QueuedEnums::ReturnStatus::InvalidArgument);
    };

    // modify record in database first
    bool status = database()->modify(QueuedDB::USERS_TABLE, _id, _userData);
    if (!status) {
        qCWarning(LOG_LIB) << "Could not modify user record" << _id
                           << "in database, do not edit it in memory";
        return QueuedError("", QueuedEnums::ReturnStatus::Error);
    }

    // modify values stored in memory
    for (auto &property : _userData.keys())
        userObj->setProperty(qPrintable(property), _userData[property]);
    // notify plugins
    if (plugins())
        emit(plugins()->interface()->onEditUser(_id, _userData));

    return true;
}


/**
 * @fn editUserPermissionPrivate
 */
QueuedResult<bool> QueuedCorePrivateHelper::editUserPermissionPrivate(
    const long long _id, const QueuedEnums::Permission &_permission,
    const bool _add)
{
    qCDebug(LOG_LIB) << "Edit permissions" << static_cast<int>(_permission)
                     << "for user" << _id << "add" << _add;

    auto userObj = users()->user(_id);
    if (!userObj) {
        qCWarning(LOG_LIB) << "Could not find user with ID" << _id;
        return QueuedError("User does not exist",
                           QueuedEnums::ReturnStatus::InvalidArgument);
    }

    // edit runtime permissions to get value
    auto perms = _add ? userObj->addPermission(_permission)
                      : userObj->removePermission(_permission);
    auto permissions = static_cast<uint>(perms);
    qCInfo(LOG_LIB) << "New user permissions" << perms;

    // modify in database now
    QVariantHash payload = {{"permissions", permissions}};
    bool status = database()->modify(QueuedDB::USERS_TABLE, _id, payload);
    if (!status) {
        qCWarning(LOG_LIB) << "Could not modify user record" << _id
                           << "in database, do not edit it in memory";
        // rollback in-memory values
        if (_add)
            userObj->removePermission(_permission);
        else
            userObj->addPermission(_permission);
        return QueuedError("", QueuedEnums::ReturnStatus::Error);
    }

    return true;
}
