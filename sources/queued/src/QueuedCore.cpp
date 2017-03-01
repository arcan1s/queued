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
 * @file QueuedCore.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include "queued/Queued.h"
#include <queued/Queued.h>
#include <queued/QueuedDatabaseSchema.h>

#include "queued/QueuedDatabaseSchema.h"


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
 * @addTask
 */
bool QueuedCore::addTask(
    const QString &_command, const QStringList &_arguments,
    const QString &_workingDirectory, const unsigned int _nice,
    const long long _userId, const QueuedLimits::Limits &_limits,
    const QueuedUserManager::QueuedUserAuthorization &_auth)
{
    qCDebug(LOG_LIB) << "Add task" << _command << "with arguments" << _arguments
                     << "from user" << _userId;

    long long userAuthId = m_users->user(_auth.user)->index();
    long long actualUserId = (_userId == -1) ? userAuthId : _userId;

    // check permissions
    bool isAdmin = m_users->authorize(_auth, QueuedEnums::Permission::Admin);
    bool isUser = m_users->authorize(_auth, QueuedEnums::Permission::JobOwner);
    if (userAuthId == actualUserId) {
        // it means that user places task as own one
        if (!isUser) {
            qCInfo(LOG_LIB) << "User" << _auth.user
                            << "not allowed to add task";
            return false;
        }
    } else {
        // user tries to place task as another one
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _auth.user
                            << "not allowed to add task";
            return false;
        }
    }

    // add to database
    auto ids = m_users->ids(_userId);
    auto taskLimits = QueuedLimits::minimalLimits(
        _limits, m_users->user(_userId)->limits(),
        QueuedLimits::Limits(
            m_advancedSettings->get(QString("DefaultLimits")).toString()));
    QVariantHash properties = {{"user", _userId},
                               {"command", _command},
                               {"commandArguments", _arguments},
                               {"workDirectory", _workingDirectory},
                               {"nice", _nice},
                               {"uid", ids.first},
                               {"gid", ids.second},
                               {"limits", taskLimits.toString()}};
    auto id = m_database->add(QueuedDB::TASKS_TABLE, properties);
    if (id == -1) {
        qCWarning(LOG_LIB) << "Could not add task" << _command;
        return false;
    }

    // add to child object
    m_processes->add(properties, id);
    return true;
}


/**
 * @fn addUser
 */
bool QueuedCore::addUser(
    const QString &_name, const QString &_email, const QString &_password,
    const unsigned int _permissions, const QueuedLimits::Limits &_limits,
    const QueuedUserManager::QueuedUserAuthorization &_auth)
{
    qCDebug(LOG_LIB) << "Add user" << _name << "with email" << _email
                     << "and permissions" << _permissions;

    // check permissions
    bool isAdmin = m_users->authorize(_auth, QueuedEnums::Permission::Admin);
    if (!isAdmin) {
        qCInfo(LOG_LIB) << "User" << _auth.user << "not allowed to add user";
        return false;
    }

    // add to dababase
    QVariantHash properties
        = {{"name", _name},
           {"password", QueuedUser::hashFromPassword(_password)},
           {"email", _email},
           {"permissions", _permissions},
           {"limits", _limits.toString()}};
    auto id = m_database->add(QueuedDB::USERS_TABLE, properties);
    if (id == -1) {
        qCWarning(LOG_LIB) << "Could not add user" << _name;
        return false;
    }

    // add to child object
    m_users->add(properties, id);
    return true;
}


QueuedUserManager::QueuedUserAuthorization
QueuedCore::authorization(const QString &_name, const QString &_password)
{
    qCDebug(LOG_LIB) << "Authorize user" << _name;

    QString token = m_users->authorize(_name, _password);
    QueuedUserManager::QueuedUserAuthorization auth;
    auth.user = _name;

    if (!token.isEmpty()) {
        QVariantHash payload = {
            {"token", token},
            {"validUntil", m_users->checkToken(token).toString(Qt::ISODate)}};
        m_database->add(QueuedDB::TOKENS_TABLE, payload);
    }

    return auth;
}


/**
 * @fn editOption
 */
bool QueuedCore::editOption(
    const QString &_key, const QVariant &_value,
    const QueuedUserManager::QueuedUserAuthorization &_auth)
{
    qCDebug(LOG_LIB) << "Set key" << _key << "to" << _value;

    // check permissions
    bool isAdmin = m_users->authorize(_auth, QueuedEnums::Permission::Admin);
    if (!isAdmin) {
        qCInfo(LOG_LIB) << "User" << _auth.user
                        << "not allowed to edit options";
        return false;
    }

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

    // add to child objectm
    if (status)
        m_advancedSettings->set(_key, _value);
    return status;
}


/**
 * @fn editTask
 */
bool QueuedCore::editTask(
    const long long _id, const QVariantHash &_taskData,
    const QueuedUserManager::QueuedUserAuthorization &_auth)
{
    qCDebug(LOG_LIB) << "Edit task with ID" << _id;

    auto task = m_processes->process(_id);
    if (!task) {
        qCWarning(LOG_LIB) << "Could not find task with ID" << _id;
        return false;
    }

    // check permissions
    long long userAuthId = m_users->user(_auth.user)->index();
    bool isAdmin = m_users->authorize(_auth, QueuedEnums::Permission::Admin);
    bool isUser = m_users->authorize(_auth, QueuedEnums::Permission::JobOwner);
    if (userAuthId == task->user()) {
        // it means that user edits own task
        if (!isUser) {
            qCInfo(LOG_LIB) << "User" << _auth.user
                            << "not allowed to edit task";
            return false;
        }
    } else {
        // user tries to edit random task
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _auth.user
                            << "not allowed to edit task";
            return false;
        }
    }
    // only admin can edit run/stopped task
    if (task->pstate() != QueuedEnums::ProcessState::NotRunning) {
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _auth.user
                            << "not allowed to edit run/exited task";
            return false;
        }
    }

    // drop admin fields
    QVariantHash payload
        = isAdmin ? _taskData
                  : dropAdminFields(QueuedDB::TASKS_TABLE, _taskData);

    // modify record in database first
    bool status = m_database->modify(QueuedDB::TASKS_TABLE, _id, payload);
    if (!status) {
        qCWarning(LOG_LIB) << "Could not modify task record" << _id
                           << "in database, do not edit it in memory";
        return false;
    }

    // modify values stored in memory
    for (auto &property : payload.keys())
        task->setProperty(property.toLocal8Bit().constData(),
                          payload[property]);

    return true;
}


/**
 * @fn editUser
 */
bool QueuedCore::editUser(
    const long long _id, const QVariantHash &_userData,
    const QueuedUserManager::QueuedUserAuthorization &_auth)
{
    qCDebug(LOG_LIB) << "Edit user with ID" << _id;

    auto user = m_users->user(_id);
    if (!user) {
        qCWarning(LOG_LIB) << "Could not find user with ID" << _id;
        return false;
    }

    // check permissions
    bool isAdmin = m_users->authorize(_auth, QueuedEnums::Permission::Admin);
    long long userAuthId = m_users->user(_auth.user)->index();
    if (userAuthId != _id) {
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _auth.user
                            << "not allowed to edit user";
            return false;
        }
    }

    // drop admin fields
    QVariantHash payload
        = isAdmin ? _userData
                  : dropAdminFields(QueuedDB::USERS_TABLE, _userData);

    // modify record in database first
    bool status = m_database->modify(QueuedDB::USERS_TABLE, _id, payload);
    if (!status) {
        qCWarning(LOG_LIB) << "Could not modify user record" << _id
                           << "in database, do not edit it in memory";
        return false;
    }

    // modify values stored in memory
    for (auto &property : payload.keys())
        user->setProperty(property.toLocal8Bit().constData(),
                          payload[property]);

    return true;
}


/**
 * @fn editUserPermission
 */
bool QueuedCore::editUserPermission(
    const long long _id, const QueuedEnums::Permission &_permission,
    const bool _add, const QueuedUserManager::QueuedUserAuthorization &_auth)
{
    qCDebug(LOG_LIB) << "Edit permissions" << static_cast<int>(_permission)
                     << "for user" << _id << "add" << _add;

    auto user = m_users->user(_id);
    if (!user) {
        qCWarning(LOG_LIB) << "Could not find user with ID" << _id;
        return false;
    }

    // check permissions
    bool isAdmin = m_users->authorize(_auth, QueuedEnums::Permission::Admin);
    long long userAuthId = m_users->user(_auth.user)->index();
    if (userAuthId != _id) {
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _auth.user
                            << "not allowed to edit permissions";
            return false;
        }
    }

    // edit runtime permissions to get value
    if (_add)
        user->addPermissions(_permission);
    else
        user->removePermissions(_permission);
    unsigned int permissions = user->permissions();
    qCInfo(LOG_LIB) << "New user permissions";

    // modify in database now
    QVariantHash payload = {{"permissions", permissions}};
    bool status = m_database->modify(QueuedDB::USERS_TABLE, _id, payload);
    if (!status) {
        qCWarning(LOG_LIB) << "Could not modify user record" << _id
                           << "in database, do not edit it in memory";
        // rollback in-memory values
        if (_add)
            user->removePermissions(_permission);
        else
            user->addPermissions(_permission);
        return false;
    }

    return true;
}


/**
 * @fn startTask
 */
bool QueuedCore::startTask(
    const long long _id,
    const QueuedUserManager::QueuedUserAuthorization &_auth)
{
    qCDebug(LOG_LIB) << "Force start task with ID" << _id;

    // check permissions
    bool isAdmin = m_users->authorize(_auth, QueuedEnums::Permission::Admin);
    long long userAuthId = m_users->user(_auth.user)->index();
    if (userAuthId != _id) {
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _auth.user
                            << "not allowed to start tasks";
            return false;
        }
    }

    m_processes->start(_id);

    return true;
}


/**
 * @fn stopTask
 */
bool QueuedCore::stopTask(
    const long long _id,
    const QueuedUserManager::QueuedUserAuthorization &_auth)
{
    qCDebug(LOG_LIB) << "Force stop task with ID" << _id;

    auto task = m_processes->process(_id);
    if (!task) {
        qCWarning(LOG_LIB) << "Could not find task with ID" << _id;
        return false;
    }

    // check permissions
    long long userAuthId = m_users->user(_auth.user)->index();
    bool isAdmin = m_users->authorize(_auth, QueuedEnums::Permission::Admin);
    bool isUser = m_users->authorize(_auth, QueuedEnums::Permission::JobOwner);
    if (userAuthId == task->user()) {
        // it means that user edits own task
        if (!isUser) {
            qCInfo(LOG_LIB) << "User" << _auth.user
                            << "not allowed to stop task";
            return false;
        }
    } else {
        // user tries to edit random task
        if (!isAdmin) {
            qCInfo(LOG_LIB) << "User" << _auth.user
                            << "not allowed to stop task";
            return false;
        }
    }

    m_processes->stop(_id);

    return true;
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

    // delete objects now
    if (m_reports)
        delete m_reports;
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
    initUsers();
    initProcesses();

    // settings update notifier
    m_connections += connect(
        m_advancedSettings,
        SIGNAL(valueUpdated(const QString &, const QVariant &)), this,
        SLOT(updateSettings(const QString &, const QVariant &)));
}


/**
 * @fn updateSettings
 */
void QueuedCore::updateSettings(const QString &_key, const QVariant &_value)
{
    qCDebug(LOG_LIB) << "Received update for" << _key << "with value" << _value;

    // FIXME propbably there is a better way to change settings
    QString key = _key.toLower();
    if (key == QString("defaultlimits"))
        ;
    else if (key == QString("tokenexpiration"))
        m_users->setTokenExpiration(_value.toLongLong());
    else if (key == QString("onexitaction"))
        m_processes->setOnExitAction(
            static_cast<QueuedProcessManager::OnExitAction>(_value.toInt()));
    else
        qCInfo(LOG_LIB) << "Unused key" << _key;
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
        record[QString("startTime")] = _startTime.toString(Qt::ISODate);
    if (_endTime.isValid())
        record[QString("endTime")] = _endTime.toString(Qt::ISODate);

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

    QVariantHash record = {{"lastLogin", _time.toString(Qt::ISODate)}};

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
 * @fn initProcesses
 */
void QueuedCore::initProcesses()
{
    // init processes
    auto onExitAction = static_cast<QueuedProcessManager::OnExitAction>(
        m_advancedSettings->get(QString("OnExitAction")).toInt());

    m_processes = new QueuedProcessManager(this, onExitAction);
    auto dbProcesses = m_database->get(
        QueuedDB::TASKS_TABLE,
        QString("WHERE state != %1")
            .arg(static_cast<int>(QueuedEnums::ProcessState::Exited)));
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
    if (!status)
        throw QueuedDatabaseException("Could not open database");

    // create administrator if required
    auto dbAdmin = m_settings->admin();
    m_database->createAdministrator(dbAdmin.name, dbAdmin.password);

    // and load advanced settings
    m_advancedSettings = new QueuedAdvancedSettings(this);
    m_advancedSettings->set(m_database->get(QueuedDB::SETTINGS_TABLE));

    // report manager
    m_reports = new QueuedReportManager(this, m_database);
}


/**
 * @fn initUsers
 */
void QueuedCore::initUsers()
{
    // load users and tokens
    auto expiry
        = m_advancedSettings->get(QString("TokenExpiration")).toLongLong();

    m_users = new QueuedUserManager(this);
    m_users->setTokenExpiration(expiry);
    QString now = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    auto dbTokens = m_database->get(
        QueuedDB::TOKENS_TABLE,
        QString("WHERE datetime(validUntil) > datetime(%2)").arg(now));
    m_users->loadTokens(dbTokens);
    auto dbUsers = m_database->get(QueuedDB::USERS_TABLE);
    m_users->loadUsers(dbUsers);

    m_connections += connect(
        m_users, SIGNAL(userLoggedIn(const long long, const QDateTime &)), this,
        SLOT(updateUserLoginTime(const long long, const QDateTime &)));
}
