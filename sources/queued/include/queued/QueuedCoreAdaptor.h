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
 * @file QueuedCoreAdaptor.h
 * Header of Queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDCOREADAPTOR_H
#define QUEUEDCOREADAPTOR_H

#include <QDBusArgument>
#include <QVariant>

#include "QueuedProcess.h"
#include "QueuedStaticConfig.h"
#include "QueuedUser.h"


/**
 * @addtogroup QueuedCoreAdaptor
 * @brief adaptor to DBus methods
 */
namespace QueuedCoreAdaptor
{
// specific methods for control interface
/**
 * @brief send TryAuth
 * @param _token
 * token ID
 * @return true if token is valid
 */
bool auth(const QString &_token);
/**
 * @brief send auth method
 * @param _name
 * user name
 * @param _password
 * user password
 * @return generated token ID or empty string in case of invalid password
 */
QString auth(const QString &_name, const QString &_password);
/**
 * @brief send OptionEdit
 * @param _key
 * option key to edit
 * @param _value
 * option value to edit
 * @param _token
 * auth user token
 * @return true on successful option edition
 */
bool sendOptionEdit(const QString &_key, const QVariant &_value,
                    const QString &_token);
/**
 * @brief send PluginAdd
 * @param _plugin
 * plugin name
 * @param _token
 * auth user token
 * @return true on successful plugin addition
 */
bool sendPluginAdd(const QString &_plugin, const QString &_token);
/**
 * @brief send PluginRemove
 * @param _plugin
 * plugin name
 * @param _token
 * auth user token
 * @return true on successful plugin removal
 */
bool sendPluginRemove(const QString &_plugin, const QString &_token);
/**
 * @brief send TaskAdd
 * @param _definitions
 * process definitions
 * @param _token
 * auth user token
 * @return task ID or {0, -1} if no task added
 */
long long
sendTaskAdd(const QueuedProcess::QueuedProcessDefinitions &_definitions,
            const QString &_token);
/**
 * @brief send TaskEdit
 * @param _id
 * task ID to edit
 * @param _definitions
 * process definitions
 * @param _token
 * auth user token
 * @return true on successful task edition
 */
bool sendTaskEdit(const long long _id,
                  const QueuedProcess::QueuedProcessDefinitions &_definitions,
                  const QString &_token);
/**
 * @brief send TaskStart
 * @param _id
 * task ID
 * @param _token
 * auth user token
 * @return true on successful task start
 */
bool sendTaskStart(const long long _id, const QString &_token);
/**
 * @brief send TaskStop
 * @param _id
 * task ID
 * @param _token
 * auth user token
 * @return true on successful task stop
 */
bool sendTaskStop(const long long _id, const QString &_token);
/**
 * @brief send UserAdd
 * @param _definitions
 * user definitions
 * @param _token
 * auth user token
 * @return user ID or -1 if no user added
 */
long long sendUserAdd(const QueuedUser::QueuedUserDefinitions &_definitions,
                      const QString &_token);
/**
 * @brief send UserEdit
 * @param _id
 * user ID to edit
 * @param _definitions
 * user definitions
 * @param _token
 * auth user token
 * @return true on successful user edition
 */
bool sendUserEdit(const long long _id,
                  const QueuedUser::QueuedUserDefinitions &_definitions,
                  const QString &_token);
/**
 * @brief send UserPermissionsAdd
 * @param _id
 * user ID
 * @param _permission
 * permission to add
 * @param _token
 * auth user token
 * @return true on successful permission addition
 */
bool sendUserPermissionAdd(const long long _id,
                           const QueuedEnums::Permission _permission,
                           const QString &_token);
/**
 * @brief send sendUserPermissionRemove
 * @param _id
 * user ID
 * @param _permission
 * permission to remove
 * @param _token
 * auth user token
 * @return true on successful permission removal
 */
bool sendUserPermissionRemove(const long long _id,
                              const QueuedEnums::Permission _permission,
                              const QString &_token);
// specific methods for properties
/**
 * @brief get option
 * @param _property
 * option name
 * @return option value
 */
QVariant getOption(const QString &_property);
/**
 * @brief get option
 * @param _property
 * option name
 * @return option value
 */
QVariant getOption(const QueuedConfig::QueuedSettings _property);
/**
 * @brief performance report
 * @param _from
 * minimal start task time
 * @param _to
 * maximal stop task time
 * @param _token
 * user auth token
 * @return list of user with used resources
 */
QList<QVariantHash> getPerformance(const QDateTime &_from, const QDateTime &_to,
                                   const QString &_token);
/**
 * @brief server status
 * @return server status information
 */
QHash<QString, QHash<QString, QString>> getStatus();
/**
 * @brief get all task properties
 * @param _id
 * task ID
 * @return task properties
 */
QVariantHash getTask(const long long _id);
/**
 * @return server status inforamtion
 */
QHash<QString, QHash<QString, QString>> getStatus();
/**
 * @brief get task property
 * @param _id
 * task id
 * @param _property
 * task property name
 * @return task property value
 */
QVariant getTask(const long long _id, const QString &_property);
/**
 * @brief get tasks list
 * @param _user
 * task user ID
 * @param _from
 * minimal start time
 * @param _to
 * maximal end time
 * @param _token
 * user auth token
 * @return list of task in database representation
 */
QList<QVariantHash> getTasks(const long long _user, const QDateTime &_from,
                             const QDateTime &_to, const QString &_token);
/**
 * @brief get user properties
 * @param _id
 * user id
 * @return user properties
 */
QVariantHash getUser(const long long _id);
/**
 * @brief get user property
 * @param _id
 * user id
 * @param _property
 * user property name
 * @return user property value
 */
QVariant getUser(const long long _id, const QString &_property);
/**
 * @brief get user ID
 * @param _name
 * user name
 * @return user ID or {0, -1} if no user found. If _name is numeric value it
 * returns converted one
 */
long long getUserId(const QString &_name);
/**
 * @brief get users list
 * @param _lastLogged
 * minimal last logged in time
 * @param _permission
 * permission to search
 * @param _token
 * user auth token
 * @return list of users in database representation
 */
QList<QVariantHash> getUsers(const QDateTime &_lastLogged,
                             const QueuedEnums::Permission _permission,
                             const QString &_token);
// common methods
/**
 * @brief common DBus request
 * @param _service
 * DBus service name
 * @param _path
 * DBus object path
 * @param _interface
 * DBus interface name
 * @param _cmd
 * command which will be sent to DBus
 * @param _args
 * command arguments
 * @return reply object from DBus request
 */
QVariantList sendRequest(const QString &_service, const QString &_path,
                         const QString &_interface, const QString &_cmd,
                         const QVariantList &_args);
/**
 * @brief additional method to avoid conversion from DBus type to native ones
 * @param _data
 * source data
 * @return converted value
 */
QVariant toNativeType(const QVariant &_data);
}


#endif /* QUEUEDCOREADAPTOR_H */
