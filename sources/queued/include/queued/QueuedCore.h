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
 * @file QueuedCore.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDCORE_H
#define QUEUEDCORE_H

#include <QObject>

#include "QueuedEnums.h"
#include "QueuedLimits.h"
#include "QueuedUserManager.h"


class QueuedAdvancedSettings;
class QueuedDatabase;
class QueuedProcessManager;
class QueuedReportManager;
class QueuedSettings;
class QueuedTokenManager;

/**
 * @brief aggregator of queued classes
 */
class QueuedCore : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief QueuedCore class constructor
     * @param parent         pointer to parent item
     */
    explicit QueuedCore(QObject *parent);
    /**
     * @brief QueuedCore class destructor
     */
    virtual ~QueuedCore();
    /**
     * @brief add new task
     * @param _command
     * command line
     * @param _arguments
     * command arguments
     * @param _workingDirectory
     * working directory
     * @param _nice
     * nice level
     * @param _userId
     * task owner user ID
     * @param _limits
     * task defined limits
     * @param _auth
     * user auth structure
     * @return true on successfully addition
     */
    bool addTask(const QString &_command, const QStringList &_arguments,
                 const QString &_workingDirectory, const uint _nice,
                 const long long _userId, const QueuedLimits::Limits &_limits,
                 const QueuedUserManager::QueuedUserAuthorization &_auth);
    /**
     * @brief add new user
     * @param _name
     * user name
     * @param _email
     * user email
     * @param _password
     * user password
     * @param _permissions
     * user permissions
     * @param _limits
     * user limits
     * @param _auth
     * user auth structure
     * @return true on successfully addition
     */
    bool addUser(const QString &_name, const QString &_email,
                 const QString &_password, const uint _permissions,
                 const QueuedLimits::Limits &_limits,
                 const QueuedUserManager::QueuedUserAuthorization &_auth);
    /**
     * @brief authorize and create new token for user
     * @param _name
     * user name
     * @param _password
     * user password
     * @return authorization structure. Token field will be empty in case if no
     * authorization occurs
     */
    QueuedUserManager::QueuedUserAuthorization
    authorization(const QString &_name, const QString &_password);
    /**
     * @brief edit advanced settings
     * @param _key
     * advanced settings key
     * @param _value
     * advanced settings value
     * @param _auth
     * user auth structure
     * @return true on successful option edition
     */
    bool editOption(const QString &_key, const QVariant &_value,
                    const QueuedUserManager::QueuedUserAuthorization &_auth);
    /**
     * @brief edit task
     * @param _id
     * task ID to edit
     * @param _taskData
     * task data to edit
     * @param _auth
     * user auth structure
     * @remark _taskData should contain only fields defined in schema, any other
     * fields will be ignored. No need to pass all properties here
     * @return true on successful task edition
     */
    bool editTask(const long long _id, const QVariantHash &_taskData,
                  const QueuedUserManager::QueuedUserAuthorization &_auth);
    /**
     * @brief edit user
     * @param _id
     * user ID to edit
     * @param _userData
     * user data to edit
     * @param _auth
     * user auth structure
     * @remark _userData should contain only fields defined in schema, any other
     * fields will be ignored. No need to pass all properties here
     * @return true on successful user edition
     */
    bool editUser(const long long _id, const QVariantHash &_userData,
                  const QueuedUserManager::QueuedUserAuthorization &_auth);
    /**
     * @brief edit user permissions
     * @param _id
     * user ID to edit
     * @param _permission
     * permission to add or remove
     * @param _add
     * indicates whether it should be added or removed
     * @param _auth
     * user auth structure
     * @return true on successful user permission edition
     */
    bool
    editUserPermission(const long long _id,
                       const QueuedEnums::Permission &_permission,
                       const bool _add,
                       const QueuedUserManager::QueuedUserAuthorization &_auth);
    /**
     * @brief force start task
     * @param _id
     * task ID
     * @param _auth
     * user auth structure
     * @return true on successful task start
     */
    bool startTask(const long long _id,
                   const QueuedUserManager::QueuedUserAuthorization &_auth);
    /**
     * @brief force stop task
     * @param _id
     * task ID
     * @param _auth
     * user auth structure
     * @return true on successful task stop
     */
    bool stopTask(const long long _id,
                  const QueuedUserManager::QueuedUserAuthorization &_auth);
    // control methods
    /**
     * @brief deinit subclasses
     */
    void deinit();
    /**
     * @brief init subclasses
     * @param _configuration
     * path to configuration file
     * @throw QueuedDatabaseException
     * @throw QueuedDBusException
     */
    void init(const QString &_configuration);

private slots:
    /**
     * @brief notify clients about settings update
     * @param _key
     * updated key
     * @param _value
     * new value
     */
    void updateSettings(const QString &_key, const QVariant &_value);
    /**
     * @brief update process time
     * @param _id
     * task id
     * @param _startTime
     * task start time or empty
     * @param _endTime
     * task end time or empty
     */
    void updateTaskTime(const long long _id, const QDateTime &_startTime,
                        const QDateTime &_endTime);
    /**
     * @brief update user login time
     * @param _id
     * user ID
     * @param _time
     * user login time
     */
    void updateUserLoginTime(const long long _id, const QDateTime &_time);

private:
    /**
     * @brief pointer to advanced settings object
     */
    QueuedAdvancedSettings *m_advancedSettings = nullptr;
    /**
     * @brief pointer to database object
     */
    QueuedDatabase *m_database = nullptr;
    /**
     * @brief pointer to process manager
     */
    QueuedProcessManager *m_processes = nullptr;
    /**
     * @brief pointer to report manager
     */
    QueuedReportManager *m_reports = nullptr;
    /**
     * @brief pointer to settings object
     */
    QueuedSettings *m_settings = nullptr;
    /**
     * @brief pointer to user manager
     */
    QueuedUserManager *m_users = nullptr;
    // additional properties
    /**
     * @brief connection list
     */
    QList<QMetaObject::Connection> m_connections;
    /**
     * @brief drop non-admin fields from database payload
     * @param _table
     * table name
     * @param _payload
     * initial database payload
     * @return payload with dropped keys
     */
    QVariantHash dropAdminFields(const QString &_table,
                                 const QVariantHash &_payload);
    /**
     * @brief init DBus interface
     * @throw QueuedDBusException
     */
    void initDBus();
    /**
     * @brief init processes
     */
    void initProcesses();
    /**
     * @brief init settings and database
     * @param _configuration
     * path to configuration file
     * @throw QueuedDatabaseException
     */
    void initSettings(const QString &_configuration);
    /**
     * @brief init users
     */
    void initUsers();
};


#endif /* QUEUEDCORE_H */
