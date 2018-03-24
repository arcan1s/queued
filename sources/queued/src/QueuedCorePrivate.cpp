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

#include <queued/QueuedDatabaseSchema.h>

#include <queued/private/QueuedCorePrivateHelper.h>


/**
 * @class QueuedCorePrivate
 */
/**
 * @fn addPlugin
 */
QueuedResult<bool> QueuedCorePrivate::addPlugin(const QString &_plugin, const QString &_token)
{
    qCDebug(LOG_LIB) << "Add plugin" << _plugin;

    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (!isAdmin) {
        qCInfo(LOG_LIB) << "User" << _token << "not allowed to add plugin";
        return QueuedError("Not allowed", QueuedEnums::ReturnStatus::InsufficientPermissions);
    }

    return m_helper->editPluginPrivate(_plugin, true);
}


/**
 * @addTask
 */
QueuedResult<long long> QueuedCorePrivate::addTask(const QString &_command,
                                                   const QStringList &_arguments,
                                                   const QString &_workingDirectory,
                                                   const long long _userId, const uint _nice,
                                                   const QueuedLimits::Limits &_limits,
                                                   const QString &_token)
{
    qCDebug(LOG_LIB) << "Add task" << _command << "with arguments" << _arguments << "from user"
                     << _userId;

    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return QueuedError("Invalid token", QueuedEnums::ReturnStatus::InvalidToken);
    }
    long long userAuthId = authUser->index();
    long long actualUserId = (_userId == -1) ? userAuthId : _userId;

    // check permissions
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    bool isUser = m_users->authorize(_token, QueuedEnums::Permission::Job);
    if (userAuthId == actualUserId) {
        // it means that user places task as own one
        if (!isUser) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to add task";
            return QueuedError("Not allowed", QueuedEnums::ReturnStatus::InsufficientPermissions);
        }
    } else {
        // user tries to place task as another one
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to add task";
            return QueuedError("Not allowed", QueuedEnums::ReturnStatus::InsufficientPermissions);
        }
    }

    return m_helper->addTaskPrivate(_command, _arguments, _workingDirectory, _userId, _nice,
                                    _limits);
}


/**
 * @fn addUser
 */
QueuedResult<long long> QueuedCorePrivate::addUser(const QString &_name, const QString &_email,
                                                   const QString &_password,
                                                   const uint _permissions, const uint _priority,
                                                   const QueuedLimits::Limits &_limits,
                                                   const QString &_token)
{
    qCDebug(LOG_LIB) << "Add user" << _name << "with email" << _email << "and permissions"
                     << _permissions;

    // check permissions
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (!isAdmin) {
        qCInfo(LOG_LIB) << "User" << _token << "not allowed to add user";
        return QueuedError("Not allowed", QueuedEnums::ReturnStatus::InsufficientPermissions);
    }

    // check if already exists
    auto userObj = user(_name, _token);
    if (userObj) {
        qCWarning(LOG_LIB) << "User" << _name << "already exists";
        return QueuedError("User already exists", QueuedEnums::ReturnStatus::InvalidArgument);
    }

    return m_helper->addUserPrivate(_name, _email, _password, _permissions, _priority, _limits);
}


/**
 * @fn authorization
 */
QueuedResult<bool> QueuedCorePrivate::authorization(const QString &_token)
{
    bool status = false;
    m_users->checkToken(_token, &status);

    return status;
}


/**
 * @fn authorization
 */
QueuedResult<QString> QueuedCorePrivate::authorization(const QString &_name,
                                                       const QString &_password)
{
    qCDebug(LOG_LIB) << "Authorize user" << _name;

    QString token = m_users->authorize(_name, _password);
    if (token.isEmpty()) {
        return QueuedError("Invalid username or password",
                           QueuedEnums::ReturnStatus::InvalidPassword);
    } else {
        QVariantHash payload
            = {{"token", token},
               {"user", _name},
               {"validUntil", m_users->checkToken(token).toString(Qt::ISODateWithMs)}};
        m_database->add(QueuedDB::TOKENS_TABLE, payload);
        return token;
    }
}


/**
 * @fn editOption
 */
QueuedResult<bool> QueuedCorePrivate::editOption(const QString &_key, const QVariant &_value,
                                                 const QString &_token)
{
    qCDebug(LOG_LIB) << "Set key" << _key << "to" << _value;

    // check permissions
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (!isAdmin) {
        qCInfo(LOG_LIB) << "User" << _token << "not allowed to edit options";
        return QueuedError("Not allowed", QueuedEnums::ReturnStatus::InsufficientPermissions);
    }

    return m_helper->editOptionPrivate(_key, _value);
}


/**
 * @fn editTask
 */
QueuedResult<bool> QueuedCorePrivate::editTask(const long long _id, const QVariantHash &_taskData,
                                               const QString &_token)
{
    qCDebug(LOG_LIB) << "Edit task with ID" << _id;

    auto task = m_helper->tryGetTask(_id);
    if (!task)
        return QueuedError("Task does not exist", QueuedEnums::ReturnStatus::InvalidArgument);

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return QueuedError("Invalid token", QueuedEnums::ReturnStatus::InvalidToken);
    }
    long long userAuthId = authUser->index();
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    bool isUser = m_users->authorize(_token, QueuedEnums::Permission::Job);
    if (userAuthId == task->user()) {
        // it means that user edits own task
        if (!isUser) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to edit task";
            return QueuedError("Not allowed", QueuedEnums::ReturnStatus::InsufficientPermissions);
        }
    } else {
        // user tries to edit random task
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to edit task";
            return QueuedError("Not allowed", QueuedEnums::ReturnStatus::InsufficientPermissions);
        }
    }
    // only admin can edit run/stopped task
    if (!task->startTime().isNull()) {
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to edit run/exited task";
            return QueuedError("Not allowed", QueuedEnums::ReturnStatus::InsufficientPermissions);
        }
    }

    // drop admin fields
    QVariantHash payload
        = isAdmin ? _taskData : m_helper->dropAdminFields(QueuedDB::TASKS_TABLE, _taskData);
    if (payload.contains("nice"))
        payload["nice"] = std::min(payload["nice"].toUInt(), authUser->priority());

    return m_helper->editTaskPrivate(task, payload);
}


/**
 * @fn editUser
 */
QueuedResult<bool> QueuedCorePrivate::editUser(const long long _id, const QVariantHash &_userData,
                                               const QString &_token)
{
    qCDebug(LOG_LIB) << "Edit user with ID" << _id;

    auto userObj = m_helper->tryGetUser(_id);
    if (!userObj)
        return QueuedError("User does not exist", QueuedEnums::ReturnStatus::InvalidArgument);

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return QueuedError("Invalid token", QueuedEnums::ReturnStatus::InvalidToken);
    }
    long long userAuthId = authUser->index();
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (userAuthId != _id) {
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to edit user";
            return QueuedError("Not allowed", QueuedEnums::ReturnStatus::InsufficientPermissions);
        }
    }

    // drop admin fields
    QVariantHash payload
        = isAdmin ? _userData : m_helper->dropAdminFields(QueuedDB::USERS_TABLE, _userData);

    return m_helper->editUserPrivate(userObj, payload);
}


/**
 * @fn editUserPermission
 */
QueuedResult<bool> QueuedCorePrivate::editUserPermission(const long long _id,
                                                         const QueuedEnums::Permission &_permission,
                                                         const bool _add, const QString &_token)
{
    qCDebug(LOG_LIB) << "Edit permissions" << static_cast<int>(_permission) << "for user" << _id
                     << "add" << _add;

    // check permissions
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (!isAdmin) {
        qCInfo(LOG_LIB) << "User" << _token << "not allowed to edit permissions";
        return QueuedError("Not allowed", QueuedEnums::ReturnStatus::InsufficientPermissions);
    }

    return m_helper->editUserPermissionPrivate(_id, _permission, _add);
}


/**
 * @fn hashFromPassword
 */
QueuedResult<QString> QueuedCorePrivate::hashFromPassword(const QString &_password)
{
    return _password.isEmpty() ? ""
                               : QueuedUser::hashFromPassword(_password, m_settings->admin().salt);
}


/**
 * @fn option
 */
QueuedResult<QVariant> QueuedCorePrivate::option(const QString &_key, const QString &_token)
{
    qCDebug(LOG_LIB) << "Look for option" << _key;

    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);

    if ((isAdmin) || (!m_advancedSettings->isAdmin(_key)))
        return m_advancedSettings->get(_key);
    else
        return QueuedError("Not allowed", QueuedEnums::ReturnStatus::InsufficientPermissions);
}


/**
 * @fn performanceReport
 */
QueuedResult<QList<QVariantHash>> QueuedCorePrivate::performanceReport(const QDateTime &_from,
                                                                       const QDateTime &_to,
                                                                       const QString &_token) const
{
    qCDebug(LOG_LIB) << "Get performance report for" << _from << _to;

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return QueuedError("Invalid token", QueuedEnums::ReturnStatus::InvalidToken);
    }
    long long userAuthId = authUser->index();
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Reports);

    if (isAdmin) {
        return m_reports->performance(static_cast<QueuedCore *>(parent()), _from, _to);
    } else {
        auto data = m_reports->performance(static_cast<QueuedCore *>(parent()), _from, _to);
        QList<QVariantHash> output;
        for (auto &userData : data) {
            if (userData["_id"].toLongLong() != userAuthId)
                continue;
            output.append(userData);
        }
        return output;
    }
}


/**
 * @fn plugin
 */
QueuedResult<QueuedPluginSpecification::Plugin> QueuedCorePrivate::plugin(const QString &_plugin,
                                                                          const QString &_token)
{
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (!isAdmin)
        return QueuedError("Not allowed", QueuedEnums::ReturnStatus::InsufficientPermissions);

    return m_plugins->loadSpecification(_plugin);
}


/**
 * @fn pluginSettings
 */
QueuedResult<QVariantHash> QueuedCorePrivate::pluginSettings(const QString &_plugin,
                                                             const QString &_token)
{
    qCDebug(LOG_LIB) << "Get plugin settings for" << _plugin;

    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (!isAdmin)
        return QueuedError("Not allowed", QueuedEnums::ReturnStatus::InsufficientPermissions);

    auto dbSettings = m_database->get(QueuedDB::SETTINGS_TABLE,
                                      {{"key", QString("Plugin.%1.%").arg(_plugin), "LIKE"}});
    QVariantHash settings;
    std::for_each(dbSettings.cbegin(), dbSettings.cend(),
                  [&settings, &_plugin](const QVariantHash &value) {
                      auto key = value["key"].toString();
                      key.remove(QString("Plugin.%1.").arg(_plugin));
                      settings[key] = value["value"];
                  });

    // append default
    auto spec = m_plugins->loadSpecification(_plugin);
    QVariantHash defaultOpts;
    for (auto &opt : spec.options) {
        if (settings.contains(opt.name))
            continue;
        settings[opt.name] = opt.defaultValue;
    }

    return settings;
}


/**
 * @fn removePlugin
 */
QueuedResult<bool> QueuedCorePrivate::removePlugin(const QString &_plugin, const QString &_token)
{
    qCDebug(LOG_LIB) << "Remove plugin" << _plugin;

    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (!isAdmin) {
        qCInfo(LOG_LIB) << "User" << _token << "not allowed to remove plugin";
        return QueuedError("Not allowed", QueuedEnums::ReturnStatus::InsufficientPermissions);
    }

    return m_helper->editPluginPrivate(_plugin, false);
}


/**
 * @fn startTask
 */
QueuedResult<bool> QueuedCorePrivate::startTask(const long long _id, const QString &_token)
{
    qCDebug(LOG_LIB) << "Force start task with ID" << _id;

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return QueuedError("Invalid token", QueuedEnums::ReturnStatus::InvalidToken);
    }
    long long userAuthId = authUser->index();
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (userAuthId != _id) {
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to start tasks";
            return QueuedError("Not allowed", QueuedEnums::ReturnStatus::InsufficientPermissions);
        }
    }

    m_processes->start(_id);

    return true;
}


/**
 * @fn stopTask
 */
QueuedResult<bool> QueuedCorePrivate::stopTask(const long long _id, const QString &_token)
{
    qCDebug(LOG_LIB) << "Force stop task with ID" << _id;

    auto task = m_processes->process(_id);
    if (!task) {
        qCWarning(LOG_LIB) << "Could not find task with ID" << _id;
        return QueuedError("Task does not exist", QueuedEnums::ReturnStatus::InvalidArgument);
    }

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return QueuedError("Invalid token", QueuedEnums::ReturnStatus::InvalidToken);
    }
    long long userAuthId = authUser->index();
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    bool isUser = m_users->authorize(_token, QueuedEnums::Permission::Job);
    if (userAuthId == task->user()) {
        // it means that user edits own task
        if (!isUser) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to stop task";
            return QueuedError("Not allowed", QueuedEnums::ReturnStatus::InsufficientPermissions);
        }
    } else {
        // user tries to edit random task
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to stop task";
            return QueuedError("Not allowed", QueuedEnums::ReturnStatus::InsufficientPermissions);
        }
    }

    m_processes->stop(_id);

    return true;
}


/**
 * @fn task
 */
const QueuedProcess *QueuedCorePrivate::task(const long long _id, const QString &_token) const
{
    qCDebug(LOG_LIB) << "Get task by ID" << _id;

    auto task = m_helper->tryGetTask(_id);
    if (!task)
        return nullptr;

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return nullptr;
    }
    long long userAuthId = authUser->index();
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);

    if (isAdmin) {
        return task;
    } else if (userAuthId == task->user()) {
        return task;
    } else {
        qCInfo(LOG_LIB) << "User" << _token << "not allowed to get task" << _id;
        return nullptr;
    }
}


/**
 * @fn taskReport
 */
QueuedResult<QList<QVariantHash>> QueuedCorePrivate::taskReport(const long long _user,
                                                                const QDateTime &_from,
                                                                const QDateTime &_to,
                                                                const QString &_token) const
{
    qCDebug(LOG_LIB) << "Get tasks table by" << _user << _from << _to;

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return QueuedError("Invalid token", QueuedEnums::ReturnStatus::InvalidToken);
    }
    long long userAuthId = authUser->index();
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Reports);
    long long effectiveUserId = _user;
    if (_user == -1) {
        effectiveUserId = userAuthId;
    } else if (userAuthId != _user) {
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to get task report";
            return QueuedError("Not allowed", QueuedEnums::ReturnStatus::InsufficientPermissions);
        }
    }

    return m_reports->tasks(effectiveUserId, _from, _to);
}


/**
 * @fn user
 */
const QueuedUser *QueuedCorePrivate::user(const long long _id, const QString &_token) const
{
    qCDebug(LOG_LIB) << "Get user by ID" << _id;

    auto user = m_helper->tryGetUser(_id);
    if (!user)
        return nullptr;

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return nullptr;
    }
    long long userAuthId = authUser->index();
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);

    if (isAdmin) {
        return user;
    } else if (userAuthId == user->index()) {
        return user;
    } else {
        qCInfo(LOG_LIB) << "User" << _token << "not allowed to get user" << _id;
        return nullptr;
    }
}


/**
 * @fn user
 */
const QueuedUser *QueuedCorePrivate::user(const QString &_name, const QString &_token) const
{
    qCDebug(LOG_LIB) << "Get user by name" << _name;

    auto userObj = m_users->user(_name, false);

    return userObj ? user(userObj->index(), _token) : nullptr;
}


/**
 * @fn userReport
 */
QueuedResult<QList<QVariantHash>>
QueuedCorePrivate::userReport(const QDateTime &_lastLogged,
                              const QueuedEnums::Permission _permission,
                              const QString &_token) const
{
    qCDebug(LOG_LIB) << "Get users table by" << _lastLogged << static_cast<int>(_permission);

    // check permissions
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Reports);
    if (!isAdmin) {
        qCInfo(LOG_LIB) << "User" << _token << "not allowed to get user report";
        return QueuedError("Not allowed", QueuedEnums::ReturnStatus::InsufficientPermissions);
    }

    return m_reports->users(_lastLogged, _permission);
}
