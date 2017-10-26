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
 * @file QueuedCore.cpp
 * Source code of queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>

#include <QDBusConnection>
#include <QDBusMessage>
#include <variant>

#include "../../3rdparty/result/result.hpp"
#include <queued/QueuedDatabaseSchema.h>
#include <queued/QueuedStaticConfig.h>


/**
 * @class QueuedCore
 */
/**
 * @fn QueuedCore
 */
QueuedCore::QueuedCore(QObject *_parent)
    : QObject(_parent)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;
}


/**
 * @fn ~QueuedCore
 */
QueuedCore::~QueuedCore()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    deinit();
}


/**
 * @fn addPlugin
 */
QueuedResult<bool> QueuedCore::addPlugin(const QString &_plugin,
                                         const QString &_token)
{
    qCDebug(LOG_LIB) << "Add plugin" << _plugin;

    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (!isAdmin) {
        qCInfo(LOG_LIB) << "User" << _token << "not allowed to add plugin";
        return QueuedError("Not allowed",
                           QueuedEnums::ReturnStatus::InsufficientPermissions);
    }

    return editPluginPrivate(_plugin, true);
}


/**
 * @addTask
 */
QueuedResult<long long>
QueuedCore::addTask(const QString &_command, const QStringList &_arguments,
                    const QString &_workingDirectory, const long long _userId,
                    const QueuedLimits::Limits &_limits, const QString &_token)
{
    qCDebug(LOG_LIB) << "Add task" << _command << "with arguments" << _arguments
                     << "from user" << _userId;

    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return QueuedError("Invalid token",
                           QueuedEnums::ReturnStatus::InvalidToken);
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
            return QueuedError(
                "Not allowed",
                QueuedEnums::ReturnStatus::InsufficientPermissions);
        }
    } else {
        // user tries to place task as another one
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to add task";
            return QueuedError(
                "Not allowed",
                QueuedEnums::ReturnStatus::InsufficientPermissions);
        }
    }

    return addTaskPrivate(_command, _arguments, _workingDirectory, _userId,
                          _limits);
}


/**
 * @fn addUser
 */
QueuedResult<long long>
QueuedCore::addUser(const QString &_name, const QString &_email,
                    const QString &_password, const uint _permissions,
                    const QueuedLimits::Limits &_limits, const QString &_token)
{
    qCDebug(LOG_LIB) << "Add user" << _name << "with email" << _email
                     << "and permissions" << _permissions;

    // check permissions
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (!isAdmin) {
        qCInfo(LOG_LIB) << "User" << _token << "not allowed to add user";
        return QueuedError("Not allowed",
                           QueuedEnums::ReturnStatus::InsufficientPermissions);
    }

    // check if already exists
    auto userObj = user(_name);
    if (userObj) {
        qCWarning(LOG_LIB) << "User" << _name << "already exists";
        return QueuedError("User already exists",
                           QueuedEnums::ReturnStatus::InvalidArgument);
    }

    return addUserPrivate(_name, _email, _password, _permissions, _limits);
}


/**
 * @fn authorization
 */
QueuedResult<bool> QueuedCore::authorization(const QString &_token)
{
    bool status = false;
    m_users->checkToken(_token, &status);

    return status;
}


/**
 * @fn authorization
 */
QueuedResult<QString> QueuedCore::authorization(const QString &_name,
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
               {"validUntil",
                m_users->checkToken(token).toString(Qt::ISODateWithMs)}};
        m_database->add(QueuedDB::TOKENS_TABLE, payload);
        return token;
    }
}


/**
 * @fn editOption
 */
QueuedResult<bool> QueuedCore::editOption(const QString &_key,
                                          const QVariant &_value,
                                          const QString &_token)
{
    qCDebug(LOG_LIB) << "Set key" << _key << "to" << _value;

    // check permissions
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (!isAdmin) {
        qCInfo(LOG_LIB) << "User" << _token << "not allowed to edit options";
        return QueuedError("Not allowed",
                           QueuedEnums::ReturnStatus::InsufficientPermissions);
    }

    return editOptionPrivate(_key, _value);
}


/**
 * @fn editTask
 */
QueuedResult<bool> QueuedCore::editTask(const long long _id,
                                        const QVariantHash &_taskData,
                                        const QString &_token)
{
    qCDebug(LOG_LIB) << "Edit task with ID" << _id;

    auto task = m_processes->process(_id);
    if (!task) {
        qCWarning(LOG_LIB) << "Could not find task with ID" << _id;
        return QueuedError("Task does not exist",
                           QueuedEnums::ReturnStatus::InvalidArgument);
    }

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return QueuedError("Invalid token",
                           QueuedEnums::ReturnStatus::InvalidToken);
    }
    long long userAuthId = authUser->index();
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    bool isUser = m_users->authorize(_token, QueuedEnums::Permission::Job);
    if (userAuthId == task->user()) {
        // it means that user edits own task
        if (!isUser) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to edit task";
            return QueuedError(
                "Not allowed",
                QueuedEnums::ReturnStatus::InsufficientPermissions);
        }
    } else {
        // user tries to edit random task
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to edit task";
            return QueuedError(
                "Not allowed",
                QueuedEnums::ReturnStatus::InsufficientPermissions);
        }
    }
    // only admin can edit run/stopped task
    if (!task->startTime().isNull()) {
        if (!isAdmin) {
            qCInfo(LOG_LIB)
                << "User" << _token << "not allowed to edit run/exited task";
            return QueuedError(
                "Not allowed",
                QueuedEnums::ReturnStatus::InsufficientPermissions);
        }
    }

    // drop admin fields
    QVariantHash payload
        = isAdmin ? _taskData
                  : dropAdminFields(QueuedDB::TASKS_TABLE, _taskData);

    return editTaskPrivate(_id, payload);
}


/**
 * @fn editUser
 */
QueuedResult<bool> QueuedCore::editUser(const long long _id,
                                        const QVariantHash &_userData,
                                        const QString &_token)
{
    qCDebug(LOG_LIB) << "Edit user with ID" << _id;

    auto userObj = user(_id);
    if (!userObj) {
        qCWarning(LOG_LIB) << "Could not find user with ID" << _id;
        return QueuedError("User does not exist",
                           QueuedEnums::ReturnStatus::InvalidArgument);
    }

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return QueuedError("Invalid token",
                           QueuedEnums::ReturnStatus::InvalidToken);
    }
    long long userAuthId = authUser->index();
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (userAuthId != _id) {
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to edit user";
            return QueuedError(
                "Not allowed",
                QueuedEnums::ReturnStatus::InsufficientPermissions);
        }
    }

    // drop admin fields
    QVariantHash payload
        = isAdmin ? _userData
                  : dropAdminFields(QueuedDB::USERS_TABLE, _userData);

    return editUserPrivate(_id, payload);
}


/**
 * @fn editUserPermission
 */
QueuedResult<bool>
QueuedCore::editUserPermission(const long long _id,
                               const QueuedEnums::Permission &_permission,
                               const bool _add, const QString &_token)
{
    qCDebug(LOG_LIB) << "Edit permissions" << static_cast<int>(_permission)
                     << "for user" << _id << "add" << _add;

    // check permissions
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (!isAdmin) {
        qCInfo(LOG_LIB) << "User" << _token
                        << "not allowed to edit permissions";
        return QueuedError("Not allowed",
                           QueuedEnums::ReturnStatus::InsufficientPermissions);
    }

    return editUserPermissionPrivate(_id, _permission, _add);
}


/**
 * @fn hashFromPassword
 */
QueuedResult<QString> QueuedCore::hashFromPassword(const QString &_password)
{
    return _password.isEmpty() ? ""
                               : QueuedUser::hashFromPassword(
                                     _password, m_settings->admin().salt);
}


/**
 * @fn option
 */
QueuedResult<QVariant> QueuedCore::option(const QString &_key)
{
    qCDebug(LOG_LIB) << "Look for option" << _key;

    return m_advancedSettings->get(_key);
}


/**
 * @fn performanceReport
 */
QueuedResult<QList<QVariantHash>>
QueuedCore::performanceReport(const QDateTime &_from, const QDateTime &_to,
                              const QString &_token) const
{
    qCDebug(LOG_LIB) << "Get performance report for" << _from << _to;

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return QueuedError("Invalid token",
                           QueuedEnums::ReturnStatus::InvalidToken);
    }
    long long userAuthId = authUser->index();
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Reports);

    if (isAdmin) {
        return m_reports->performance(this, _from, _to);
    } else {
        auto data = m_reports->performance(this, _from, _to);
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
 * @fn pluginSettings
 */
QVariantHash QueuedCore::pluginSettings(const QString &_plugin)
{
    qCDebug(LOG_LIB) << "Get plugin settings for" << _plugin;

    auto dbSettings
        = m_database->get(QueuedDB::SETTINGS_TABLE,
                          QString("WHERE key LIKE 'Plugin.%1.%'").arg(_plugin));
    QVariantHash settings;
    std::for_each(dbSettings.cbegin(), dbSettings.cend(),
                  [&settings](const QVariantHash &value) {
                      settings[value["key"].toString()] = value["value"];
                  });

    return settings;
}


/**
 * @fn removePlugin
 */
QueuedResult<bool> QueuedCore::removePlugin(const QString &_plugin,
                                            const QString &_token)
{
    qCDebug(LOG_LIB) << "Remove plugin" << _plugin;

    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (!isAdmin) {
        qCInfo(LOG_LIB) << "User" << _token << "not allowed to remove plugin";
        return QueuedError("Not allowed",
                           QueuedEnums::ReturnStatus::InsufficientPermissions);
    }

    return editPluginPrivate(_plugin, false);
}


/**
 * @fn startTask
 */
QueuedResult<bool> QueuedCore::startTask(const long long _id,
                                         const QString &_token)
{
    qCDebug(LOG_LIB) << "Force start task with ID" << _id;

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return QueuedError("Invalid token",
                           QueuedEnums::ReturnStatus::InvalidToken);
    }
    long long userAuthId = authUser->index();
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (userAuthId != _id) {
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to start tasks";
            return QueuedError(
                "Not allowed",
                QueuedEnums::ReturnStatus::InsufficientPermissions);
        }
    }

    m_processes->start(_id);

    return true;
}


/**
 * @fn stopTask
 */
QueuedResult<bool> QueuedCore::stopTask(const long long _id,
                                        const QString &_token)
{
    qCDebug(LOG_LIB) << "Force stop task with ID" << _id;

    auto task = m_processes->process(_id);
    if (!task) {
        qCWarning(LOG_LIB) << "Could not find task with ID" << _id;
        return QueuedError("Task does not exist",
                           QueuedEnums::ReturnStatus::InvalidArgument);
    }

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return QueuedError("Invalid token",
                           QueuedEnums::ReturnStatus::InvalidToken);
    }
    long long userAuthId = authUser->index();
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    bool isUser = m_users->authorize(_token, QueuedEnums::Permission::Job);
    if (userAuthId == task->user()) {
        // it means that user edits own task
        if (!isUser) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to stop task";
            return QueuedError(
                "Not allowed",
                QueuedEnums::ReturnStatus::InsufficientPermissions);
        }
    } else {
        // user tries to edit random task
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to stop task";
            return QueuedError(
                "Not allowed",
                QueuedEnums::ReturnStatus::InsufficientPermissions);
        }
    }

    m_processes->stop(_id);

    return true;
}


/**
 * @fn task
 */
const QueuedProcess *QueuedCore::task(const long long _id) const
{
    qCDebug(LOG_LIB) << "Get task by ID" << _id;

    return m_processes->process(_id);
}


/**
 * @fn taskReport
 */
QueuedResult<QList<QVariantHash>>
QueuedCore::taskReport(const long long _user, const QDateTime &_from,
                       const QDateTime &_to, const QString &_token) const
{
    qCDebug(LOG_LIB) << "Get tasks table by" << _user << _from << _to;

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return QueuedError("Invalid token",
                           QueuedEnums::ReturnStatus::InvalidToken);
    }
    long long userAuthId = authUser->index();
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Reports);
    long long effectiveUserId = _user;
    if (_user == -1) {
        effectiveUserId = userAuthId;
    } else if (userAuthId != _user) {
        if (!isAdmin) {
            qCInfo(LOG_LIB)
                << "User" << _token << "not allowed to get task report";
            return QueuedError(
                "Not allowed",
                QueuedEnums::ReturnStatus::InsufficientPermissions);
        }
    }

    return m_reports->tasks(effectiveUserId, _from, _to);
}


/**
 * @fn user
 */
const QueuedUser *QueuedCore::user(const long long _id) const
{
    qCDebug(LOG_LIB) << "Get user by ID" << _id;

    return m_users->user(_id);
}


/**
 * @fn user
 */
const QueuedUser *QueuedCore::user(const QString &_name) const
{
    qCDebug(LOG_LIB) << "Get user by name" << _name;

    return m_users->user(_name, false);
}


/**
 * @fn userReport
 */
QueuedResult<QList<QVariantHash>>
QueuedCore::userReport(const QDateTime &_lastLogged,
                       const QueuedEnums::Permission _permission,
                       const QString &_token) const
{
    qCDebug(LOG_LIB) << "Get users table by" << _lastLogged
                     << static_cast<int>(_permission);

    // check permissions
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Reports);
    if (!isAdmin) {
        qCInfo(LOG_LIB) << "User" << _token << "not allowed to get user report";
        return QueuedError("Not allowed",
                           QueuedEnums::ReturnStatus::InsufficientPermissions);
    }

    return m_reports->users(_lastLogged, _permission);
}


/**
 * @fn deinit
 */
void QueuedCore::deinit()
{
    // clear connections first
    for (auto &connection : m_connections)
        disconnect(connection);
    m_connections.clear();

    // dbus cleanup
    QDBusConnection::systemBus().unregisterObject(
        QueuedConfig::DBUS_OBJECT_PATH);
    QDBusConnection::systemBus().unregisterObject(
        QueuedConfig::DBUS_PROPERTY_PATH);
    QDBusConnection::systemBus().unregisterObject(
        QueuedConfig::DBUS_REPORTS_PATH);
    QDBusConnection::systemBus().unregisterService(QueuedConfig::DBUS_SERVICE);
}


/**
 * @fn init
 */
void QueuedCore::init(const QString &_configuration)
{
    qCDebug(LOG_LIB) << "Load configuration from" << _configuration;

    // deinit objects if any
    deinit();

    // init parts
    initSettings(_configuration);
    initUsers();
    initPlugins();
    initProcesses();

    // settings update notifier
    m_connections
        += connect(m_advancedSettings,
                   SIGNAL(valueUpdated(const QueuedConfig::QueuedSettings,
                                       const QString &, const QVariant &)),
                   this,
                   SLOT(updateSettings(const QueuedConfig::QueuedSettings,
                                       const QString &, const QVariant &)));

    // dbus session
    initDBus();

    // run!
    m_processes->start();
}


/**
 * @fn updateSettings
 */
void QueuedCore::updateSettings(const QueuedConfig::QueuedSettings _id,
                                const QString &_key, const QVariant &_value)
{
    qCDebug(LOG_LIB) << "Received update for" << static_cast<int>(_id) << _key
                     << "with value" << _value;

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
        m_processes->setExitAction(
            static_cast<QueuedEnums::ExitAction>(_value.toInt()));
        break;
    case QueuedConfig::QueuedSettings::Plugins:
        // do nothing here
        break;
    case QueuedConfig::QueuedSettings::ProcessCommandLine:
        m_processes->setProcessLine(_value.toString());
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
void QueuedCore::updateTaskTime(const long long _id,
                                const QDateTime &_startTime,
                                const QDateTime &_endTime)
{
    qCDebug(LOG_LIB) << "Update task" << _id << "time to" << _startTime
                     << _endTime;

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
void QueuedCore::updateUserLoginTime(const long long _id,
                                     const QDateTime &_time)
{
    qCDebug(LOG_LIB) << "Update user" << _id << "with login time" << _time;

    QVariantHash record = {{"lastLogin", _time.toString(Qt::ISODateWithMs)}};

    bool status = m_database->modify(QueuedDB::USERS_TABLE, _id, record);
    if (!status)
        qCWarning(LOG_LIB) << "Could not modify user record" << _id;
}


/**
 * @fn dropAdminFields
 */
QVariantHash QueuedCore::dropAdminFields(const QString &_table,
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
 * @fn initDBus
 */
void QueuedCore::initDBus()
{
    QDBusConnection bus = QDBusConnection::systemBus();

    if (!bus.registerService(QueuedConfig::DBUS_SERVICE)) {
        QString message = QString("Could not register service %1")
                              .arg(bus.lastError().message());
        qCCritical(LOG_DBUS) << message;
        throw QueuedDBusException(message);
    }

    if (!bus.registerObject(QueuedConfig::DBUS_OBJECT_PATH,
                            new QueuedCoreInterface(this),
                            QDBusConnection::ExportAllContents)) {
        QString message = QString("Could not register core object %1")
                              .arg(bus.lastError().message());
        qCCritical(LOG_DBUS) << message;
        throw QueuedDBusException(message);
    }
    if (!bus.registerObject(QueuedConfig::DBUS_PROPERTY_PATH,
                            new QueuedPropertyInterface(this),
                            QDBusConnection::ExportAllContents)) {
        QString message = QString("Could not register properties object %1")
                              .arg(bus.lastError().message());
        qCCritical(LOG_DBUS) << message;
        throw QueuedDBusException(message);
    }
    if (!bus.registerObject(QueuedConfig::DBUS_REPORTS_PATH,
                            new QueuedReportInterface(this),
                            QDBusConnection::ExportAllContents)) {
        QString message = QString("Could not register reports object %1")
                              .arg(bus.lastError().message());
        qCCritical(LOG_DBUS) << message;
        throw QueuedDBusException(message);
    }
}


/**
 * @fn initPlugins
 */
void QueuedCore::initPlugins()
{
    QStringList pluginList
        = m_advancedSettings->get(QueuedConfig::QueuedSettings::Plugins)
              .toString()
              .split('\n');
    QString token = m_users->authorize(m_settings->admin().name);

    m_plugins = initObject(m_plugins, token);
    for (auto &plugin : pluginList)
        m_plugins->loadPlugin(plugin, pluginSettings(plugin));
}


/**
 * @fn initProcesses
 */
void QueuedCore::initProcesses()
{
    // init processes
    auto onExitAction = static_cast<QueuedEnums::ExitAction>(
        m_advancedSettings->get(QueuedConfig::QueuedSettings::OnExitAction)
            .toInt());
    auto processLine
        = m_advancedSettings
              ->get(QueuedConfig::QueuedSettings::ProcessCommandLine)
              .toString();

    m_processes = initObject(m_processes);
    m_processes->setProcessLine(processLine);
    m_processes->setExitAction(onExitAction);
    auto dbProcesses
        = m_database->get(QueuedDB::TASKS_TABLE, "WHERE endTime IS NULL");
    m_processes->loadProcesses(dbProcesses);

    m_connections
        += connect(m_processes, &QueuedProcessManager::taskStartTimeReceived,
                   [this](const long long _index, const QDateTime &_time) {
                       return updateTaskTime(_index, _time, QDateTime());
                   });
    m_connections
        += connect(m_processes, &QueuedProcessManager::taskStopTimeReceived,
                   [this](const long long _index, const QDateTime &_time) {
                       return updateTaskTime(_index, QDateTime(), _time);
                   });
}


/**
 * @fn initSettings
 */
void QueuedCore::initSettings(const QString &_configuration)
{
    // read configuration first
    m_settings = initObject(m_settings, _configuration);
    m_settings->readConfiguration();
    // init database now
    auto dbSetup = m_settings->db();
    m_database = initObject(m_database, dbSetup.path, dbSetup.driver);
    m_database->close();
    bool status = m_database->open(dbSetup.hostname, dbSetup.port,
                                   dbSetup.username, dbSetup.password);
    if (!status) {
        QString message = "Could not open database";
        qCCritical(LOG_LIB) << message;
        throw QueuedDatabaseException(message);
    }

    // create administrator if required
    auto dbAdmin = m_settings->admin();
    m_database->createAdministrator(dbAdmin.name, dbAdmin.password);

    // and load advanced settings
    m_advancedSettings = initObject(m_advancedSettings);
    m_advancedSettings->set(m_database->get(QueuedDB::SETTINGS_TABLE));
    if (!m_advancedSettings->checkDatabaseVersion()) {
        qCInfo(LOG_LIB) << "Bump database version to"
                        << QueuedConfig::DATABASE_VERSION;
        editOptionPrivate(m_advancedSettings->internalId(
                              QueuedConfig::QueuedSettings::DatabaseVersion),
                          QueuedConfig::DATABASE_VERSION);
    }

    // report manager
    m_reports = initObject(m_reports, m_database);
    // database manager
    m_databaseManager = initObject(m_databaseManager, m_database);
}


/**
 * @fn initUsers
 */
void QueuedCore::initUsers()
{
    // load users and tokens
    auto expiry
        = m_advancedSettings->get(QueuedConfig::QueuedSettings::TokenExpiration)
              .toLongLong();

    m_users = initObject(m_users);
    m_users->setSalt(m_settings->admin().salt);
    m_users->setTokenExpiration(expiry);
    QString now = QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs);
    auto dbTokens = m_database->get(
        QueuedDB::TOKENS_TABLE,
        QString("WHERE datetime(validUntil) > datetime('%1')").arg(now));
    m_users->loadTokens(dbTokens);
    auto dbUsers = m_database->get(QueuedDB::USERS_TABLE);
    m_users->loadUsers(dbUsers);

    m_connections += connect(
        m_users, SIGNAL(userLoggedIn(const long long, const QDateTime &)), this,
        SLOT(updateUserLoginTime(const long long, const QDateTime &)));
}


/**
 * @addTaskPrivate
 */
QueuedResult<long long> QueuedCore::addTaskPrivate(
    const QString &_command, const QStringList &_arguments,
    const QString &_workingDirectory, const long long _userId,
    const QueuedLimits::Limits &_limits)
{
    qCDebug(LOG_LIB) << "Add task" << _command << "with arguments" << _arguments
                     << "from user" << _userId;

    // add to database
    auto ids = m_users->ids(_userId);
    auto userObj = user(_userId);
    if (!userObj) {
        qCWarning(LOG_LIB) << "Could not find task user" << _userId;
        return QueuedError("Invalid token",
                           QueuedEnums::ReturnStatus::InvalidToken);
    }
    auto taskLimits = QueuedLimits::minimalLimits(
        _limits, userObj->nativeLimits(),
        QueuedLimits::Limits(
            m_advancedSettings->get(QueuedConfig::QueuedSettings::DefaultLimits)
                .toString()));
    QVariantHash properties = {{"user", _userId},
                               {"command", _command},
                               {"commandArguments", _arguments},
                               {"workDirectory", _workingDirectory},
                               {"nice", 0},
                               {"uid", ids.first},
                               {"gid", ids.second},
                               {"limits", taskLimits.toString()}};
    auto id = m_database->add(QueuedDB::TASKS_TABLE, properties);
    if (id == -1) {
        qCWarning(LOG_LIB) << "Could not add task" << _command;
        return QueuedError("", QueuedEnums::ReturnStatus::Error);
    }

    // add to child object
    m_processes->add(properties, id);
    // notify plugins
    if (m_plugins)
        emit(m_plugins->interface()->onAddTask(id));

    return id;
}


/**
 * @fn addUserPrivate
 */
QueuedResult<long long>
QueuedCore::addUserPrivate(const QString &_name, const QString &_email,
                           const QString &_password, const uint _permissions,
                           const QueuedLimits::Limits &_limits)
{
    qCDebug(LOG_LIB) << "Add user" << _name << "with email" << _email
                     << "and permissions" << _permissions;
    // add to database
    QVariantHash properties = {{"name", _name},
                               {"password", _password},
                               {"email", _email},
                               {"permissions", _permissions},
                               {"limits", _limits.toString()}};
    auto id = m_database->add(QueuedDB::USERS_TABLE, properties);
    if (id == -1) {
        qCWarning(LOG_LIB) << "Could not add user" << _name;
        return QueuedError("", QueuedEnums::ReturnStatus::Error);
    }

    // add to child object
    m_users->add(properties, id);
    // notify plugins
    if (m_plugins)
        emit(m_plugins->interface()->onAddUser(id));

    return id;
}


/**
 * @fn editOptionPrivate
 */
QueuedResult<bool> QueuedCore::editOptionPrivate(const QString &_key,
                                                 const QVariant &_value)
{
    qCDebug(LOG_LIB) << "Set key" << _key << "to" << _value;

    // add to database
    long long id = m_advancedSettings->id(_key);
    QVariantHash payload = {{"key", _key}, {"value", _value}};

    bool status;
    if (id == -1) {
        id = m_database->add(QueuedDB::SETTINGS_TABLE, payload);
        qCInfo(LOG_LIB) << "Added new key with ID" << id;
        status = (id != -1);
    } else {
        status = m_database->modify(QueuedDB::SETTINGS_TABLE, id, payload);
        qCInfo(LOG_LIB) << "Value for" << _key
                        << "has been modified with status" << status;
    }

    // add to child object
    if (status) {
        m_advancedSettings->set(_key, _value);
        // notify plugins if required
        if (m_plugins) {
            auto tryPluginOption = m_plugins->convertOptionName(_key);
            if ((!tryPluginOption.first.isEmpty())
                && (!tryPluginOption.second.isEmpty()))
                m_plugins->optionChanged(_key, _value);
            // notify plugins
            emit(m_plugins->interface()->onEditOption(_key, _value));
        }
    }

    return status;
}


/**
 * @fn editPluginPrivate
 */
QueuedResult<bool> QueuedCore::editPluginPrivate(const QString &_plugin,
                                                 const bool _add)
{
    qCDebug(LOG_LIB) << "Edit plugin" << _plugin << "add" << _add;

    QStringList pluginList
        = m_advancedSettings->get(QueuedConfig::QueuedSettings::Plugins)
              .toString()
              .split('\n');

    QueuedResult<bool> r;
    if (_add && !pluginList.contains(_plugin)) {
        if (m_plugins->loadPlugin(_plugin, pluginSettings(_plugin))) {
            pluginList.append(_plugin);
            r = true;
        }
    } else if (!_add && pluginList.contains(_plugin)) {
        if (m_plugins->unloadPlugin(_plugin)) {
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
        editOptionPrivate(m_advancedSettings->internalId(
                              QueuedConfig::QueuedSettings::Plugins),
                          pluginList.join('\n'));
        // notify plugins
        if (m_plugins) {
            if (_add)
                emit(m_plugins->interface()->onAddPlugin(_plugin));
            else
                emit(m_plugins->interface()->onRemovePlugin(_plugin));
        }
    }

    return r;
}


/**
 * @fn editTaskPrivate
 */
QueuedResult<bool> QueuedCore::editTaskPrivate(const long long _id,
                                               const QVariantHash &_taskData)
{
    qCDebug(LOG_LIB) << "Edit task with ID" << _id;

    auto task = m_processes->process(_id);
    if (!task) {
        qCWarning(LOG_LIB) << "Could not find task with ID" << _id;
        return QueuedError("Task does not exist",
                           QueuedEnums::ReturnStatus::InvalidArgument);
    }

    // modify record in database first
    bool status = m_database->modify(QueuedDB::TASKS_TABLE, _id, _taskData);
    if (!status) {
        qCWarning(LOG_LIB) << "Could not modify task record" << _id
                           << "in database, do not edit it in memory";
        return QueuedError("", QueuedEnums::ReturnStatus::Error);
    }

    // modify values stored in memory
    for (auto &property : _taskData.keys())
        task->setProperty(qPrintable(property), _taskData[property]);
    // notify plugins
    if (m_plugins)
        emit(m_plugins->interface()->onEditTask(_id, _taskData));

    return true;
}


/**
 * @fn editUserPrivate
 */
QueuedResult<bool> QueuedCore::editUserPrivate(const long long _id,
                                               const QVariantHash &_userData)
{
    qCDebug(LOG_LIB) << "Edit user with ID" << _id;

    auto userObj = m_users->user(_id);
    if (!userObj) {
        qCWarning(LOG_LIB) << "Could not find user with ID" << _id;
        return QueuedError("User does not exist",
                           QueuedEnums::ReturnStatus::InvalidArgument);
    };

    // modify record in database first
    bool status = m_database->modify(QueuedDB::USERS_TABLE, _id, _userData);
    if (!status) {
        qCWarning(LOG_LIB) << "Could not modify user record" << _id
                           << "in database, do not edit it in memory";
        return QueuedError("", QueuedEnums::ReturnStatus::Error);
    }

    // modify values stored in memory
    for (auto &property : _userData.keys())
        userObj->setProperty(qPrintable(property), _userData[property]);
    // notify plugins
    if (m_plugins)
        emit(m_plugins->interface()->onEditUser(_id, _userData));

    return true;
}


/**
 * @fn editUserPermissionPrivate
 */
QueuedResult<bool> QueuedCore::editUserPermissionPrivate(
    const long long _id, const QueuedEnums::Permission &_permission,
    const bool _add)
{
    qCDebug(LOG_LIB) << "Edit permissions" << static_cast<int>(_permission)
                     << "for user" << _id << "add" << _add;

    auto userObj = m_users->user(_id);
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
    bool status = m_database->modify(QueuedDB::USERS_TABLE, _id, payload);
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
