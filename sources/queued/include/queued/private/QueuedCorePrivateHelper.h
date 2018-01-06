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
 * @file QueuedCorePrivateHelper.h
 * Header of Queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDCOREPRIVATEHELPER_H
#define QUEUEDCOREPRIVATEHELPER_H

#include <QObject>

#include "queued/QueuedEnums.h"
#include "queued/QueuedLimits.h"
#include "queued/QueuedResult.h"


class QueuedAdvancedSettings;
class QueuedCorePrivate;
class QueuedDatabase;
class QueuedPluginManager;
class QueuedProcess;
class QueuedProcessManager;
class QueuedUser;
class QueuedUserManager;

/**
 * @brief helper for private core class
 */
class QueuedCorePrivateHelper : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief QueuedCorePrivateHelper class constructor
     * @param _parent
     * pointer to parent item
     */
    explicit QueuedCorePrivateHelper(QObject *_parent);
    /**
     * @brief QueuedCorePrivateHelper class destructor
     */
    virtual ~QueuedCorePrivateHelper();

private:
    // interfaces
    friend class QueuedCorePrivate;
    QueuedCorePrivate *m_core = nullptr;
    /**
     * @brief pointer to advanced settings object
     * @return pointer to advanced settings object
     */
    QueuedAdvancedSettings *advancedSettings();
    /**
     * @brief pointer to database object
     * @return pointer to database object
     */
    QueuedDatabase *database();
    /**
     * @brief pointer to plugin manager
     * @return pointer to plugin manager
     */
    QueuedPluginManager *plugins();
    /**
     * @brief pointer to process manager
     * @return pointer to process manager
     */
    QueuedProcessManager *processes();
    /**
     * @brief pointer to user manager
     * @return pointer to user manager
     */
    QueuedUserManager *users();

    /**
     * @brief drop non-admin fields from database payload
     * @param _table
     * table name
     * @param _payload
     * initial database payload
     * @return payload with dropped keys
     */
    QVariantHash dropAdminFields(const QString &_table, const QVariantHash &_payload);
    /**
     * @brief method allows to init class if it was not created
     * @tparam T
     * class name
     * @tparam Args
     * class constructor arguments
     * @param _parent
     * pointer to parent object of destination object
     * @param _dest
     * pointer to destination
     * @param _args
     * class constructor arguments
     */
    template <class T, typename... Args> T *initObject(T *_dest, Args... _args)
    {
        return _dest ? _dest : new T(m_core, _args...);
    };
    // private interfaces
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
     * @return task ID or -1 if no task added
     */
    QueuedResult<long long> addTaskPrivate(const QString &_command, const QStringList &_arguments,
                                           const QString &_workingDirectory,
                                           const long long _userId, const uint _nice,
                                           const QueuedLimits::Limits &_limits);
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
     * @return user ID or -1 if no user found
     */
    QueuedResult<long long> addUserPrivate(const QString &_name, const QString &_email,
                                           const QString &_password, const uint _permissions,
                                           const uint _priority,
                                           const QueuedLimits::Limits &_limits);
    /**
     * @brief edit advanced settings
     * @param _key
     * advanced settings key
     * @param _value
     * advanced settings value
     * @return true on successful option edition
     */
    QueuedResult<bool> editOptionPrivate(const QString &_key, const QVariant &_value);
    /**
     * @brief edit plugin list
     * @param _plugin
     * plugin name
     * @param add
     * true if it requires add plugin
     * @return true on successful action
     */
    QueuedResult<bool> editPluginPrivate(const QString &_plugin, const bool _add);
    /**
     * @brief edit task
     * @param _process
     * task object
     * @param _taskData
     * task data to edit
     * @remark _taskData should contain only fields defined in schema, any other
     * fields will be ignored. No need to pass all properties here
     * @return true on successful task edition
     */
    QueuedResult<bool> editTaskPrivate(QueuedProcess *_process, const QVariantHash &_taskData);
    /**
     * @brief edit user
     * @param _id
     * user ID to edit
     * @param _userData
     * user data to edit
     * @remark _userData should contain only fields defined in schema, any other
     * fields will be ignored. No need to pass all properties here
     * @return true on successful user edition
     */
    QueuedResult<bool> editUserPrivate(QueuedUser *_user, const QVariantHash &_userData);
    /**
     * @brief edit user permissions
     * @param _id
     * user ID to edit
     * @param _permission
     * permission to add or remove
     * @param _add
     * indicates whether it should be added or removed
     * @return true on successful user permission edition
     */
    QueuedResult<bool> editUserPermissionPrivate(const long long _id,
                                                 const QueuedEnums::Permission &_permission,
                                                 const bool _add);
    /**
     * @brief try get task from storages
     * @param _id
     * task ID to search
     * @return pointer to found task if any
     */
    QueuedProcess *tryGetTask(const long long _id);
    /**
     * @brief try get user from storages
     * @param _id
     * user ID to search
     * @return pointer to found user if any
     */
    QueuedUser *tryGetUser(const long long _id);
};


#endif /* QUEUEDCOREPRIVATEHELPER_H */
