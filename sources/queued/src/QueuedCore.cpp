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
 * @file QueuedCore.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>

#include <QDBusConnection>
#include <QDBusMessage>

#include <queued/QueuedDatabaseSchema.h>


/**
 * @class QueuedCore
 */
/**
 * @fn QueuedCore
 */
QueuedCore::QueuedCore(QObject *parent)
    : QObject(parent)
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
bool QueuedCore::addPlugin(const QString &_plugin, const QString &_token)
{
    qCDebug(LOG_LIB) << "Add plugin" << _plugin;

    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (!isAdmin) {
        qCInfo(LOG_LIB) << "User" << _token << "not allowed to add plugin";
        return false;
    }

    return editPluginPrivate(_plugin, true);
}


/**
 * @addTask
 */
long long
QueuedCore::addTask(const QString &_command, const QStringList &_arguments,
                    const QString &_workingDirectory, const long long _userId,
                    const QueuedLimits::Limits &_limits, const QString &_token)
{
    qCDebug(LOG_LIB) << "Add task" << _command << "with arguments" << _arguments
                     << "from user" << _userId;

    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return false;
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
            return -1;
        }
    } else {
        // user tries to place task as another one
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to add task";
            return -1;
        }
    }

    return addTaskPrivate(_command, _arguments, _workingDirectory, _userId,
                          _limits);
}


/**
 * @fn addUser
 */
long long QueuedCore::addUser(const QString &_name, const QString &_email,
                              const QString &_password, const uint _permissions,
                              const QueuedLimits::Limits &_limits,
                              const QString &_token)
{
    qCDebug(LOG_LIB) << "Add user" << _name << "with email" << _email
                     << "and permissions" << _permissions;

    // check permissions
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (!isAdmin) {
        qCInfo(LOG_LIB) << "User" << _token << "not allowed to add user";
        return -1;
    }

    // check if already exists
    auto userObj = user(_name);
    if (userObj) {
        qCWarning(LOG_LIB) << "User" << _name << "already exists";
        return -1;
    }

    return addUserPrivate(_name, _email, _password, _permissions, _limits);
}


/**
 * @fn authorization
 */
bool QueuedCore::authorization(const QString &_token)
{
    bool status = false;
    m_users->checkToken(_token, &status);

    return status;
}


/**
 * @fn authorization
 */
QString QueuedCore::authorization(const QString &_name,
                                  const QString &_password)
{
    qCDebug(LOG_LIB) << "Authorize user" << _name;

    QString token = m_users->authorize(_name, _password);
    if (!token.isEmpty()) {
        QVariantHash payload
            = {{"token", token},
               {"user", _name},
               {"validUntil",
                m_users->checkToken(token).toString(Qt::ISODateWithMs)}};
        m_database->add(QueuedDB::TOKENS_TABLE, payload);
    }

    return token;
}


/**
 * @fn editOption
 */
bool QueuedCore::editOption(const QString &_key, const QVariant &_value,
                            const QString &_token)
{
    qCDebug(LOG_LIB) << "Set key" << _key << "to" << _value;

    // check permissions
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (!isAdmin) {
        qCInfo(LOG_LIB) << "User" << _token << "not allowed to edit options";
        return false;
    }

    return editOptionPrivate(_key, _value);
}


/**
 * @fn editTask
 */
bool QueuedCore::editTask(const long long _id, const QVariantHash &_taskData,
                          const QString &_token)
{
    qCDebug(LOG_LIB) << "Edit task with ID" << _id;

    auto task = m_processes->process(_id);
    if (!task) {
        qCWarning(LOG_LIB) << "Could not find task with ID" << _id;
        return false;
    }

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return false;
    }
    long long userAuthId = authUser->index();
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    bool isUser = m_users->authorize(_token, QueuedEnums::Permission::Job);
    if (userAuthId == task->user()) {
        // it means that user edits own task
        if (!isUser) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to edit task";
            return false;
        }
    } else {
        // user tries to edit random task
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to edit task";
            return false;
        }
    }
    // only admin can edit run/stopped task
    if (!task->startTime().isNull()) {
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _token
                            << "not allowed to edit run/exited task";
            return false;
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
bool QueuedCore::editUser(const long long _id, const QVariantHash &_userData,
                          const QString &_token)
{
    qCDebug(LOG_LIB) << "Edit user with ID" << _id;

    auto userObj = user(_id);
    if (!userObj) {
        qCWarning(LOG_LIB) << "Could not find user with ID" << _id;
        return false;
    }

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return false;
    }
    long long userAuthId = authUser->index();
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (userAuthId != _id) {
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to edit user";
            return false;
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
bool QueuedCore::editUserPermission(const long long _id,
                                    const QueuedEnums::Permission &_permission,
                                    const bool _add, const QString &_token)
{
    qCDebug(LOG_LIB) << "Edit permissions" << static_cast<int>(_permission)
                     << "for user" << _id << "add" << _add;

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return false;
    }
    long long userAuthId = authUser->index();
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (userAuthId != _id) {
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _token
                            << "not allowed to edit permissions";
            return false;
        }
    }

    return editUserPermissionPrivate(_id, _permission, _add);
}


/**
 * @fn option
 */
QVariant QueuedCore::option(const QString &_key)
{
    qCDebug(LOG_LIB) << "Look for option" << _key;

    return m_advancedSettings->get(_key);
}


/**
 * @fn performanceReport
 */
QList<QVariantHash> QueuedCore::performanceReport(const QDateTime &_from,
                                                  const QDateTime &_to,
                                                  const QString &_token) const
{
    qCDebug(LOG_LIB) << "Get performance report for" << _from << _to;

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return QList<QVariantHash>();
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
bool QueuedCore::removePlugin(const QString &_plugin, const QString &_token)
{
    qCDebug(LOG_LIB) << "Remove plugin" << _plugin;

    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (!isAdmin) {
        qCInfo(LOG_LIB) << "User" << _token << "not allowed to remove plugin";
        return false;
    }

    return editPluginPrivate(_plugin, false);
}


/**
 * @fn startTask
 */
bool QueuedCore::startTask(const long long _id, const QString &_token)
{
    qCDebug(LOG_LIB) << "Force start task with ID" << _id;

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return false;
    }
    long long userAuthId = authUser->index();
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    if (userAuthId != _id) {
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to start tasks";
            return false;
        }
    }

    m_processes->start(_id);

    return true;
}


/**
 * @fn stopTask
 */
bool QueuedCore::stopTask(const long long _id, const QString &_token)
{
    qCDebug(LOG_LIB) << "Force stop task with ID" << _id;

    auto task = m_processes->process(_id);
    if (!task) {
        qCWarning(LOG_LIB) << "Could not find task with ID" << _id;
        return false;
    }

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return false;
    }
    long long userAuthId = authUser->index();
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Admin);
    bool isUser = m_users->authorize(_token, QueuedEnums::Permission::Job);
    if (userAuthId == task->user()) {
        // it means that user edits own task
        if (!isUser) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to stop task";
            return false;
        }
    } else {
        // user tries to edit random task
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _token << "not allowed to stop task";
            return false;
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
QList<QVariantHash> QueuedCore::taskReport(const long long _user,
                                           const QDateTime &_from,
                                           const QDateTime &_to,
                                           const QString &_token) const
{
    qCDebug(LOG_LIB) << "Get tasks table by" << _user << _from << _to;

    // check permissions
    auto authUser = m_users->user(_token, true);
    if (!authUser) {
        qCWarning(LOG_LIB) << "Could not find auth user" << _token;
        return QList<QVariantHash>();
    }
    long long userAuthId = authUser->index();
    bool isAdmin = m_users->authorize(_token, QueuedEnums::Permission::Reports);
    long long effectiveUserId = _user;
    if (_user == -1) {
        effectiveUserId = userAuthId;
    } else if (userAuthId != _user) {
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _token
                            << "not allowed to get task report";
            return QList<QVariantHash>();
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
QList<QVariantHash>
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
        return QList<QVariantHash>();
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
    QDBusConnection::sessionBus().unregisterObject(
        QueuedConfig::DBUS_OBJECT_PATH);
    QDBusConnection::sessionBus().unregisterObject(
        QueuedConfig::DBUS_PROPERTY_PATH);
    QDBusConnection::sessionBus().unregisterObject(
        QueuedConfig::DBUS_REPORTS_PATH);
    QDBusConnection::sessionBus().unregisterService(QueuedConfig::DBUS_SERVICE);

    // delete objects now
    if (m_databaseManager)
        delete m_databaseManager;
    if (m_reports)
        delete m_reports;
    if (m_plugins)
        delete m_plugins;
    if (m_processes)
        delete m_processes;
    if (m_users)
        delete m_users;
    if (m_database)
        delete m_database;
    if (m_settings)
        delete m_settings;
    if (m_advancedSettings)
        delete m_advancedSettings;
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
    initPlugins();
    initUsers();
    initProcesses();

    // settings update notifier
    m_connections += connect(
        m_advancedSettings,
        SIGNAL(valueUpdated(const QueuedConfig::QueuedSettings, const QString &,
                            const QVariant &)),
        this, SLOT(updateSettings(const QueuedConfig::QueuedSettings,
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

    // FIXME propbably there is a better way to change settings
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
    if (_startTime.isValid())
        record[QString("startTime")] = _startTime.toString(Qt::ISODateWithMs);
    if (_endTime.isValid())
        record[QString("endTime")] = _endTime.toString(Qt::ISODateWithMs);

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

    m_plugins = new QueuedPluginManager(this);
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

    m_processes = new QueuedProcessManager(this, processLine, onExitAction);
    auto dbProcesses = m_database->get(QueuedDB::TASKS_TABLE,
                                       QString("WHERE endTime IS NULL"));
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
    m_settings = new QueuedSettings(this, _configuration);
    // init database now
    auto dbSetup = m_settings->db();
    m_database = new QueuedDatabase(this, dbSetup.path, dbSetup.driver);
    bool status = m_database->open(dbSetup.hostname, dbSetup.port,
                                   dbSetup.username, dbSetup.password);
    if (!status) {
        QString message = QString("Could not open database");
        qCCritical(LOG_LIB) << message;
        throw QueuedDatabaseException(message);
    }

    // create administrator if required
    auto dbAdmin = m_settings->admin();
    m_database->createAdministrator(dbAdmin.name, dbAdmin.password);

    // and load advanced settings
    m_advancedSettings = new QueuedAdvancedSettings(this);
    m_advancedSettings->set(m_database->get(QueuedDB::SETTINGS_TABLE));
    if (!m_advancedSettings->checkDatabaseVersion()) {
        qCInfo(LOG_LIB) << "Bump database version to"
                        << QueuedConfig::DATABASE_VERSION;
        editOptionPrivate(m_advancedSettings->internalId(
                              QueuedConfig::QueuedSettings::DatabaseVersion),
                          QueuedConfig::DATABASE_VERSION);
    }

    // report manager
    m_reports = new QueuedReportManager(this, m_database);
    // database manager
    m_databaseManager = new QueuedDatabaseManager(this, m_database);
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

    m_users = new QueuedUserManager(this);
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
long long QueuedCore::addTaskPrivate(const QString &_command,
                                     const QStringList &_arguments,
                                     const QString &_workingDirectory,
                                     const long long _userId,
                                     const QueuedLimits::Limits &_limits)
{
    qCDebug(LOG_LIB) << "Add task" << _command << "with arguments" << _arguments
                     << "from user" << _userId;

    // add to database
    auto ids = m_users->ids(_userId);
    auto userObj = user(_userId);
    if (!userObj) {
        qCWarning(LOG_LIB) << "Could not find task user" << _userId;
        return false;
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
        return id;
    }

    // add to child object
    m_processes->add(properties, id);
    return id;
}


/**
 * @fn addUserPrivate
 */
long long QueuedCore::addUserPrivate(const QString &_name,
                                     const QString &_email,
                                     const QString &_password,
                                     const uint _permissions,
                                     const QueuedLimits::Limits &_limits)
{
    qCDebug(LOG_LIB) << "Add user" << _name << "with email" << _email
                     << "and permissions" << _permissions;
    // add to database
    QVariantHash properties
        = {{"name", _name},
           {"password", QueuedUser::hashFromPassword(_password)},
           {"email", _email},
           {"permissions", _permissions},
           {"limits", _limits.toString()}};
    auto id = m_database->add(QueuedDB::USERS_TABLE, properties);
    if (id == -1) {
        qCWarning(LOG_LIB) << "Could not add user" << _name;
        return id;
    }

    // add to child object
    m_users->add(properties, id);
    return id;
}


/**
 * @fn editOptionPrivate
 */
bool QueuedCore::editOptionPrivate(const QString &_key, const QVariant &_value)
{
    qCDebug(LOG_LIB) << "Set key" << _key << "to" << _value;

    // add to database
    long long id = m_advancedSettings->id(_key);
    QVariantHash payload = {{"key", _key}, {"value", _value}};

    bool status = false;
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
        // notify plugin if required
    }
    return status;
}


/**
 * @fn editPluginPrivate
 */
bool QueuedCore::editPluginPrivate(const QString &_plugin, const bool _add)
{
    qCDebug(LOG_LIB) << "Edit plugin" << _plugin << "add" << _add;

    QStringList pluginList
        = m_advancedSettings->get(QueuedConfig::QueuedSettings::Plugins)
              .toString()
              .split('\n');

    bool status = false;
    if (_add && !pluginList.contains(_plugin))
        status = m_plugins->loadPlugin(_plugin, pluginSettings(_plugin));
    else if (!_add && pluginList.contains(_plugin))
        status = m_plugins->unloadPlugin(_plugin);
    else
        qCDebug(LOG_LIB) << "Plugin" << _plugin
                         << "not loaded or already loaded";

    return status;
}


/**
 * @fn editTaskPrivate
 */
bool QueuedCore::editTaskPrivate(const long long _id,
                                 const QVariantHash &_taskData)
{
    qCDebug(LOG_LIB) << "Edit task with ID" << _id;

    auto task = m_processes->process(_id);
    if (!task) {
        qCWarning(LOG_LIB) << "Could not find task with ID" << _id;
        return false;
    }

    // modify record in database first
    bool status = m_database->modify(QueuedDB::TASKS_TABLE, _id, _taskData);
    if (!status) {
        qCWarning(LOG_LIB) << "Could not modify task record" << _id
                           << "in database, do not edit it in memory";
        return false;
    }

    // modify values stored in memory
    for (auto &property : _taskData.keys())
        task->setProperty(qPrintable(property), _taskData[property]);

    return true;
}


/**
 * @fn editUserPrivate
 */
bool QueuedCore::editUserPrivate(const long long _id,
                                 const QVariantHash &_userData)
{
    qCDebug(LOG_LIB) << "Edit user with ID" << _id;

    auto userObj = m_users->user(_id);
    if (!userObj) {
        qCWarning(LOG_LIB) << "Could not find user with ID" << _id;
        return false;
    };

    // modify record in database first
    bool status = m_database->modify(QueuedDB::USERS_TABLE, _id, _userData);
    if (!status) {
        qCWarning(LOG_LIB) << "Could not modify user record" << _id
                           << "in database, do not edit it in memory";
        return false;
    }

    // modify values stored in memory
    for (auto &property : _userData.keys())
        userObj->setProperty(qPrintable(property), _userData[property]);

    return true;
}


/**
 * @fn editUserPermissionPrivate
 */
bool QueuedCore::editUserPermissionPrivate(
    const long long _id, const QueuedEnums::Permission &_permission,
    const bool _add)
{
    qCDebug(LOG_LIB) << "Edit permissions" << static_cast<int>(_permission)
                     << "for user" << _id << "add" << _add;

    auto userObj = m_users->user(_id);
    if (!userObj) {
        qCWarning(LOG_LIB) << "Could not find user with ID" << _id;
        return false;
    }

    // edit runtime permissions to get value
    auto perms = _add ? userObj->addPermission(_permission)
                      : userObj->removePermission(_permission);
    uint permissions = static_cast<uint>(perms);
    qCInfo(LOG_LIB) << "New user permissions" << perms << permissions;

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
        return false;
    }

    return true;
}
