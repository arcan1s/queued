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
 * @file QueuedCorePrivate.h
 * Header of Queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDCOREPRIVATE_H
#define QUEUEDCOREPRIVATE_H

#include <QObject>

#include "queued/QueuedEnums.h"
#include "queued/QueuedLimits.h"
#include "queued/QueuedResult.h"
#include "queued/QueuedStaticConfig.h"


class QueuedAdvancedSettings;
class QueuedCorePrivateHelper;
class QueuedDatabase;
class QueuedDatabaseManager;
class QueuedPluginManager;
class QueuedProcess;
class QueuedProcessManager;
class QueuedReportManager;
class QueuedSettings;
class QueuedUser;
class QueuedUserManager;

/**
 * @brief private aggregator of queued classes
 */
class QueuedCorePrivate : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief QueuedCorePrivate class constructor
     * @param _parent
     * pointer to parent item
     */
    explicit QueuedCorePrivate(QObject *_parent);
    /**
     * @brief QueuedCorePrivate class destructor
     */
    virtual ~QueuedCorePrivate();
    /**
     * @brief add plugin to autoload and load it now
     * @param _plugin
     * plugin name
     * @param _token
     * user auth token
     * @return true on successfully addition
     */
    QueuedResult<bool> addPlugin(const QString &_plugin, const QString &_token);
    /**
     * @brief add new task
     * @param _command
     * command line
     * @param _arguments
     * command arguments
     * @param _workingDirectory
     * working directory
     * @param _userId
     * task owner user ID
     * @param _nice
     * task nice level
     * @param _limits
     * task defined limits
     * @param _token
     * user auth token
     * @return task ID or -1 if no task added
     */
    QueuedResult<long long> addTask(const QString &_command,
                                    const QStringList &_arguments,
                                    const QString &_workingDirectory,
                                    const long long _userId, const uint _nice,
                                    const QueuedLimits::Limits &_limits,
                                    const QString &_token);
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
     * @param _priority
     * user priority
     * @param _limits
     * user limits
     * @param _token
     * user auth token
     * @return user ID or -1 if no user created
     */
    QueuedResult<long long> addUser(const QString &_name, const QString &_email,
                                    const QString &_password,
                                    const uint _permissions,
                                    const uint _priority,
                                    const QueuedLimits::Limits &_limits,
                                    const QString &_token);
    /**
     * @brief try to authorize by given token
     * @param _token
     * token ID
     * @return true if token is valid
     */
    QueuedResult<bool> authorization(const QString &_token);
    /**
     * @brief authorize and create new token for user
     * @param _name
     * user name
     * @param _password
     * user password
     * @return token. It will be empty if authorization error occurs
     */
    QueuedResult<QString> authorization(const QString &_name,
                                        const QString &_password);
    /**
     * @brief edit advanced settings
     * @param _key
     * advanced settings key
     * @param _value
     * advanced settings value
     * @param _token
     * user auth token
     * @return true on successful option edition
     */
    QueuedResult<bool> editOption(const QString &_key, const QVariant &_value,
                                  const QString &_token);
    /**
     * @brief edit task
     * @param _id
     * task ID to edit
     * @param _taskData
     * task data to edit
     * @param _token
     * user auth token
     * @remark _taskData should contain only fields defined in schema, any other
     * fields will be ignored. No need to pass all properties here
     * @return true on successful task edition
     */
    QueuedResult<bool> editTask(const long long _id,
                                const QVariantHash &_taskData,
                                const QString &_token);
    /**
     * @brief edit user
     * @param _id
     * user ID to edit
     * @param _userData
     * user data to edit
     * @param _token
     * user auth token
     * @remark _userData should contain only fields defined in schema, any other
     * fields will be ignored. No need to pass all properties here
     * @return true on successful user edition
     */
    QueuedResult<bool> editUser(const long long _id,
                                const QVariantHash &_userData,
                                const QString &_token);
    /**
     * @brief edit user permissions
     * @param _id
     * user ID to edit
     * @param _permission
     * permission to add or remove
     * @param _add
     * indicates whether it should be added or removed
     * @param _token
     * user auth token
     * @return true on successful user permission edition
     */
    QueuedResult<bool>
    editUserPermission(const long long _id,
                       const QueuedEnums::Permission &_permission,
                       const bool _add, const QString &_token);
    /**
     * @brief hash password
     * @param _password
     * user password as plain text
     * @return hashed password with applied salt
     */
    QueuedResult<QString> hashFromPassword(const QString &_password);
    /**
     * @brief get value from advanced settings
     * @param _key
     * key string
     * @param _token
     * user auth token
     * @return option value or empty QVariant
     */
    QueuedResult<QVariant> option(const QString &_key, const QString &_token);
    /**
     * @brief usage report
     * @param _from
     * start report date
     * @param _to
     * stop report date
     * @param _token
     * user auth token
     * @return performance table
     */
    QueuedResult<QList<QVariantHash>>
    performanceReport(const QDateTime &_from, const QDateTime &_to,
                      const QString &_token) const;
    /**
     * @brief get plugin description
     * @param _plugin
     * plugin name
     * @param _token
     * user auth token
     * @return dictionary of PluginSpecification representation
     */
    QueuedResult<QVariantHash> plugin(const QString &_plugin,
                                      const QString &_token);
    /**
     * @brief get plugin settings
     * @param _plugin
     * plugin name
     * @param _token
     * user auth token
     * @return hash of plugin settings
     */
    QueuedResult<QVariantHash> pluginSettings(const QString &_plugin,
                                              const QString &_token);
    /**
     * @brief remove plugin from autoload and unload it now
     * @param _plugin
     * plugin name
     * @param _token
     * user auth token
     * @return true on successful plugin removal
     */
    QueuedResult<bool> removePlugin(const QString &_plugin,
                                    const QString &_token);
    /**
     * @brief force start task
     * @param _id
     * task ID
     * @param _token
     * user auth token
     * @return true on successful task start
     */
    QueuedResult<bool> startTask(const long long _id, const QString &_token);
    /**
     * @brief force stop task
     * @param _id
     * task ID
     * @param _token
     * user auth token
     * @return true on successful task stop
     */
    QueuedResult<bool> stopTask(const long long _id, const QString &_token);
    /**
     * @brief get task by ID
     * @param _id
     * task ID
     * @param _token
     * user auth token
     * @return task object or nullptr if no task found
     */
    const QueuedProcess *task(const long long _id, const QString &_token) const;
    /**
     * list of tasks which match criteria
     * @param _user
     * task user ID filter
     * @param _from
     * minimal start time
     * @param _to
     * maximal end time
     * @param _token
     * user auth token
     * @return list of tasks in database format
     */
    QueuedResult<QList<QVariantHash>> taskReport(const long long _user,
                                                 const QDateTime &_from,
                                                 const QDateTime &_to,
                                                 const QString &_token) const;
    /**
     * @brief get user by ID
     * @param _id
     * user ID
     * @param _token
     * user auth token
     * @return user object or nullptr if no user found
     */
    const QueuedUser *user(const long long _id, const QString &_token) const;
    /**
     * @brief get user by name
     * @param _name
     * user name
     * @param _token
     * user auth token
     * @return user object or nullptr if no user found
     */
    const QueuedUser *user(const QString &_name, const QString &_token) const;
    /**
     * list of users which match criteria
     * @param _lastLogged
     * last logged minimal date
     * @param _permission
     * user permission filter
     * @param _token
     * user auth token
     * @return list of users in database format
     */
    QueuedResult<QList<QVariantHash>>
    userReport(const QDateTime &_lastLogged,
               const QueuedEnums::Permission _permission,
               const QString &_token) const;
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
     * @param _id
     * updated key id
     * @param _key
     * updated key
     * @param _value
     * new value
     */
    void updateSettings(const QueuedConfig::QueuedSettings _id,
                        const QString &_key, const QVariant &_value);
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
     * @brief admin token for internal services
     */
    QString m_adminToken;
    /**
     * @brief private helper pointer
     */
    friend class QueuedCorePrivateHelper;
    QueuedCorePrivateHelper *m_helper = nullptr;
    /**
     * @brief pointer to advanced settings object
     */
    QueuedAdvancedSettings *m_advancedSettings = nullptr;
    /**
     * @brief pointer to database object
     */
    QueuedDatabase *m_database = nullptr;
    /**
     * @brief pointer to database manager object
     */
    QueuedDatabaseManager *m_databaseManager = nullptr;
    /**
     * @brief pointer to plugin manager
     */
    QueuedPluginManager *m_plugins = nullptr;
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
     * @brief init plugins
     */
    void initPlugins();
    /**
     * @brief init processes
     */
    void initProcesses();
    /**
     * @brief init reports
     */
    void initReports();
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


#endif /* QUEUEDCOREPRIVATE_H */
