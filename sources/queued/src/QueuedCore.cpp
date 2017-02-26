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
bool QueuedCore::addTask(const QString &_command, const QStringList &_arguments,
                         const QString &_workingDirectory,
                         const unsigned int _nice, const long long _userId,
                         const QueuedLimits::Limits &_limits)
{
    qCDebug(LOG_LIB) << "Add task" << _command << "with arguments" << _arguments
                     << "from user" << _userId;

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

    m_processes->add(properties, id);
    return true;
}


/**
 * @fn addUser
 */
bool QueuedCore::addUser(const QString &_name, const QString &_email,
                         const QString &_password,
                         const unsigned int _permissions,
                         const QueuedLimits::Limits &_limits)
{
    qCDebug(LOG_LIB) << "Add user" << _name << "with email" << _email
                     << "and permissions" << _permissions;

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

    m_users->add(properties, id);
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
 * @fn editOption
 */
bool QueuedCore::editOption(const QString &_key, const QVariant &_value)
{
    qCDebug(LOG_LIB) << "Set key" << _key << "to" << _value;

    long long id = m_advancedSettings->id(_key);
    QVariantHash payload = {
        {"key", _key}, {"value", _value},
    };

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

    if (status)
        m_advancedSettings->set(_key, _value);
    return status;
}


/**
 * @fn editTask
 */
bool QueuedCore::editTask(const long long _id, const QVariantHash &_taskData)
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
        task->setProperty(property.toLocal8Bit().constData(),
                          _taskData[property]);

    return true;
}


/**
 * @fn editUser
 */
bool QueuedCore::editUser(const long long _id, const QVariantHash &_userData)
{
    qCDebug(LOG_LIB) << "Edit user with ID" << _id;

    auto user = m_users->user(_id);
    if (!user) {
        qCWarning(LOG_LIB) << "Could not find user with ID" << _id;
        return false;
    }

    // modify record in database first
    bool status = m_database->modify(QueuedDB::USERS_TABLE, _id, _userData);
    if (!status) {
        qCWarning(LOG_LIB) << "Could not modify user record" << _id
                           << "in database, do not edit it in memory";
        return false;
    }

    // modify values stored in memory
    for (auto &property : _userData.keys())
        user->setProperty(property.toLocal8Bit().constData(),
                          _userData[property]);

    return true;
}


/**
 * @fn editUserPermission
 */
bool QueuedCore::editUserPermission(const long long _id,
                                    const QueuedEnums::Permission &_permission,
                                    const bool _add)
{
    qCDebug(LOG_LIB) << "Edit permissions" << static_cast<int>(_permission)
                     << "for user" << _id << "add" << _add;

    auto user = m_users->user(_id);
    if (!user) {
        qCWarning(LOG_LIB) << "Could not find user with ID" << _id;
        return false;
    }

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
 * @fn init
 */
void QueuedCore::init(const QString &_configuration)
{
    qCDebug(LOG_LIB) << "Load configuration from" << _configuration;

    // deinit objects if any
    deinit();

    // read configuration first
    m_settings = new QueuedSettings(this, _configuration);
    // init database now
    auto dbSetup = m_settings->db();
    m_database = new QueuedDatabase(this, dbSetup.path, dbSetup.driver);
    m_database->open(dbSetup.hostname, dbSetup.port, dbSetup.username,
                     dbSetup.password);
    auto dbAdmin = m_settings->admin();
    m_database->createAdministrator(dbAdmin.name, dbAdmin.password);

    // and load advanced settings
    m_advancedSettings = new QueuedAdvancedSettings(this);
    m_advancedSettings->set(m_database->get(QueuedDB::SETTINGS_TABLE));

    // report manager
    m_reports = new QueuedReportManager(this, m_database);

    // load users and tokens
    m_users = new QueuedUserManager(this);
    auto expiry
        = m_advancedSettings->get(QString("TokenExpiration")).toLongLong();
    m_users->setTokenExpiration(expiry);
    m_users->loadTokens(m_database->get(QueuedDB::TOKENS_TABLE));
    m_users->loadUsers(m_database->get(QueuedDB::USERS_TABLE));
    m_connections += connect(
        m_users, SIGNAL(userLoggedIn(const long long, const QDateTime &)), this,
        SLOT(updateUserLoginTime(const long long, const QDateTime &)));

    // and processes finally
    auto onExitAction = static_cast<QueuedProcessManager::OnExitAction>(
        m_advancedSettings->get(QString("OnExitAction")).toInt());
    m_processes = new QueuedProcessManager(this, onExitAction);
    m_processes->loadProcesses(m_database->get(QueuedDB::TASKS_TABLE));
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
