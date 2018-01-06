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

#include <QDBusConnection>
#include <QDBusReply>

#include "QueuedProcess.h"
#include "QueuedUser.h"


namespace QueuedPluginSpecification
{
struct Plugin;
};

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
QueuedResult<bool> auth(const QString &_token);
/**
 * @brief send auth method
 * @param _name
 * user name
 * @param _password
 * user password
 * @return generated token ID or empty string in case of invalid password
 */
QueuedResult<QString> auth(const QString &_name, const QString &_password);
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
QueuedResult<bool> sendOptionEdit(const QString &_key, const QVariant &_value,
                                  const QString &_token);
/**
 * @brief send PasswordHash
 * @param _password
 * user password
 * @return hashed password with applied salt
 */
QueuedResult<QString> sendPasswordHash(const QString &_password);
/**
 * @brief send PluginAdd
 * @param _plugin
 * plugin name
 * @param _token
 * auth user token
 * @return true on successful plugin addition
 */
QueuedResult<bool> sendPluginAdd(const QString &_plugin, const QString &_token);
/**
 * @brief send PluginRemove
 * @param _plugin
 * plugin name
 * @param _token
 * auth user token
 * @return true on successful plugin removal
 */
QueuedResult<bool> sendPluginRemove(const QString &_plugin, const QString &_token);
/**
 * @brief send TaskAdd
 * @param _definitions
 * process definitions
 * @param _token
 * auth user token
 * @return task ID or {0, -1} if no task added
 */
QueuedResult<long long> sendTaskAdd(const QueuedProcess::QueuedProcessDefinitions &_definitions,
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
QueuedResult<bool> sendTaskEdit(const long long _id,
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
QueuedResult<bool> sendTaskStart(const long long _id, const QString &_token);
/**
 * @brief send TaskStop
 * @param _id
 * task ID
 * @param _token
 * auth user token
 * @return true on successful task stop
 */
QueuedResult<bool> sendTaskStop(const long long _id, const QString &_token);
/**
 * @brief send UserAdd
 * @param _definitions
 * user definitions
 * @param _token
 * auth user token
 * @return user ID or -1 if no user added
 */
QueuedResult<long long> sendUserAdd(const QueuedUser::QueuedUserDefinitions &_definitions,
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
QueuedResult<bool> sendUserEdit(const long long _id,
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
QueuedResult<bool> sendUserPermissionAdd(const long long _id,
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
QueuedResult<bool> sendUserPermissionRemove(const long long _id,
                                            const QueuedEnums::Permission _permission,
                                            const QString &_token);
// specific methods for properties
/**
 * @brief get plugin
 * @param _plugin
 * plugin name
 * @param _token
 * user auth token
 * @return plugin specification body
 */
QueuedResult<QueuedPluginSpecification::Plugin> getPlugin(const QString &_plugin,
                                                          const QString &_token);
/**
 * @brief get plugin options
 * @param _plugin
 * plugin name
 * @param _token
 * user auth token
 * @return plugin options dictionary
 */
QueuedResult<QVariantHash> getPluginOptions(const QString &_plugin, const QString &_token);
/**
 * @brief get option
 * @param _property
 * option name
 * @param _token
 * user auth token
 * @return option value
 */
QueuedResult<QVariant> getOption(const QString &_property, const QString &_token);
/**
 * @brief get option
 * @param _property
 * option name
 * @param _token
 * user auth token
 * @return option value
 */
QueuedResult<QVariant> getOption(const QueuedConfig::QueuedSettings _property,
                                 const QString &_token);
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
QueuedResult<QList<QVariantHash>> getPerformance(const QDateTime &_from, const QDateTime &_to,
                                                 const QString &_token);
/**
 * @brief server status
 * @return server status information
 */
QueuedResult<QueuedStatusMap> getStatus();
/**
 * @brief get all task properties
 * @param _id
 * task ID
 * @param _token
 * user auth token
 * @return task properties
 */
QueuedResult<QVariantHash> getTask(const long long _id, const QString &_token);
/**
 * @brief get task property
 * @param _id
 * task id
 * @param _property
 * task property name
 * @param _token
 * user auth token
 * @return task property value
 */
QueuedResult<QVariant> getTask(const long long _id, const QString &_property,
                               const QString &_token);
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
QueuedResult<QList<QVariantHash>> getTasks(const long long _user, const QDateTime &_from,
                                           const QDateTime &_to, const QString &_token);
/**
 * @brief get user properties
 * @param _id
 * user id
 * @param _token
 * user auth token
 * @return user properties
 */
QueuedResult<QVariantHash> getUser(const long long _id, const QString &_token);
/**
 * @brief get user property
 * @param _id
 * user id
 * @param _property
 * user property name
 * @param _token
 * user auth token
 * @return user property value
 */
QueuedResult<QVariant> getUser(const long long _id, const QString &_property,
                               const QString &_token);
/**
 * @brief get user ID
 * @param _name
 * user name
 * @param _token
 * user auth token
 * @return user ID or {0, -1} if no user found. If _name is numeric value it
 * returns converted one
 */
QueuedResult<long long> getUserId(const QString &_name, const QString &_token);
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
QueuedResult<QList<QVariantHash>> getUsers(const QDateTime &_lastLogged,
                                           const QueuedEnums::Permission _permission,
                                           const QString &_token);
// common methods
/**
 * @brief additional method to avoid conversion from QueuedResult to
 * QDBusVariant
 * @tparam T
 * QueuedResult payload class
 * @param _data
 * input data
 * @return converted data to QDBusVariant
 */
template <class T> QDBusVariant toDBusVariant(const QueuedResult<T> &_data)
{
    return QDBusVariant(QVariant::fromValue<QueuedResult<T>>(_data));
};
/**
 * @brief additional method to avoid conversion from QVariant to
 * QueuedResult
 * @tparam T
 * QueuedResult payload class
 * @param _data
 * input data
 * @return converted data to QueuedResult
 */
template <class T> QueuedResult<T> toResult(const QVariant &_data)
{
    return qdbus_cast<QueuedResult<T>>(_data.value<QDBusArgument>());
};
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
template <typename T>
QueuedResult<T> sendRequest(const QString &_service, const QString &_path,
                            const QString &_interface, const QString &_cmd,
                            const QVariantList &_args)
{
    QDBusConnection bus = QDBusConnection::systemBus();
    QDBusMessage request = QDBusMessage::createMethodCall(_service, _path, _interface, _cmd);
    if (!_args.isEmpty())
        request.setArguments(_args);

    QDBusReply<QDBusVariant> dbusResponse = bus.call(request, QDBus::BlockWithGui);

    if (dbusResponse.isValid()) {
        auto response = dbusResponse.value();
        return toResult<T>(response.variant());
    } else {
        return QueuedError(dbusResponse.error().message().toStdString());
    }
};
}


#endif /* QUEUEDCOREADAPTOR_H */
