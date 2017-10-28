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
 * @file QueuedCoreAdaptor.cpp
 * Source code of queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>


/**
 * @fn auth
 */
QueuedResult<bool> QueuedCoreAdaptor::auth(const QString &_token)
{
    QVariantList args = {_token};
    return sendRequest<bool>(QueuedConfig::DBUS_SERVICE,
                             QueuedConfig::DBUS_OBJECT_PATH,
                             QueuedConfig::DBUS_SERVICE, "TryAuth", args);
}


/**
 * @fn auth
 */
QueuedResult<QString> QueuedCoreAdaptor::auth(const QString &_name,
                                              const QString &_password)
{
    qCDebug(LOG_DBUS) << "Auth user" << _name;

    QVariantList args = {_name, _password};
    return sendRequest<QString>(QueuedConfig::DBUS_SERVICE,
                                QueuedConfig::DBUS_OBJECT_PATH,
                                QueuedConfig::DBUS_SERVICE, "Auth", args);
}


/**
 * @fn sendOptionEdit
 */
QueuedResult<bool> QueuedCoreAdaptor::sendOptionEdit(const QString &_key,
                                                     const QVariant &_value,
                                                     const QString &_token)
{
    qCDebug(LOG_DBUS) << "Edit option" << _key << "to" << _value;

    QVariantList args
        = {_key, QVariant::fromValue(QDBusVariant(_value)), _token};
    return sendRequest<bool>(QueuedConfig::DBUS_SERVICE,
                             QueuedConfig::DBUS_OBJECT_PATH,
                             QueuedConfig::DBUS_SERVICE, "OptionEdit", args);
}


/**
 * @fn sendPasswordHash
 */
QueuedResult<QString>
QueuedCoreAdaptor::sendPasswordHash(const QString &_password)
{
    QVariantList args = {_password};
    return sendRequest<QString>(
        QueuedConfig::DBUS_SERVICE, QueuedConfig::DBUS_OBJECT_PATH,
        QueuedConfig::DBUS_SERVICE, "PasswordHash", args);
}


/**
 * @fn sendPluginAdd
 */
QueuedResult<bool> QueuedCoreAdaptor::sendPluginAdd(const QString &_plugin,
                                                    const QString &_token)
{
    qCDebug(LOG_DBUS) << "Add plugin" << _plugin;

    QVariantList args = {_plugin, _token};
    return sendRequest<bool>(QueuedConfig::DBUS_SERVICE,
                             QueuedConfig::DBUS_OBJECT_PATH,
                             QueuedConfig::DBUS_SERVICE, "PluginAdd", args);
}


/**
 * @fn sendPluginRemove
 */
QueuedResult<bool> QueuedCoreAdaptor::sendPluginRemove(const QString &_plugin,
                                                       const QString &_token)
{
    qCDebug(LOG_DBUS) << "Remove plugin" << _plugin;

    QVariantList args = {_plugin, _token};
    return sendRequest<bool>(QueuedConfig::DBUS_SERVICE,
                             QueuedConfig::DBUS_OBJECT_PATH,
                             QueuedConfig::DBUS_SERVICE, "PluginRemove", args);
}


/**
 * @fn sendTaskAdd
 */
QueuedResult<long long> QueuedCoreAdaptor::sendTaskAdd(
    const QueuedProcess::QueuedProcessDefinitions &_definitions,
    const QString &_token)
{
    qCDebug(LOG_DBUS) << "Add task" << _definitions.command;

    auto limits = QueuedLimits::Limits(_definitions.limits);
    QVariantList args = {_definitions.command,
                         _definitions.arguments,
                         _definitions.workingDirectory,
                         _definitions.user,
                         limits.cpu,
                         limits.gpu,
                         limits.memory,
                         limits.gpumemory,
                         limits.storage,
                         _token};
    return sendRequest<long long>(QueuedConfig::DBUS_SERVICE,
                                  QueuedConfig::DBUS_OBJECT_PATH,
                                  QueuedConfig::DBUS_SERVICE, "TaskAdd", args);
}


/**
 * @fn sendTaskEdit
 */
QueuedResult<bool> QueuedCoreAdaptor::sendTaskEdit(
    const long long _id,
    const QueuedProcess::QueuedProcessDefinitions &_definitions,
    const QString &_token)
{
    qCDebug(LOG_DBUS) << "Edit task" << _id;

    auto limits = QueuedLimits::Limits(_definitions.limits);
    QVariantList args = {_id,
                         _definitions.command,
                         _definitions.arguments,
                         _definitions.workingDirectory,
                         _definitions.nice,
                         _definitions.uid,
                         _definitions.gid,
                         _definitions.user,
                         limits.cpu,
                         limits.gpu,
                         limits.memory,
                         limits.gpumemory,
                         limits.storage,
                         _token};
    return sendRequest<bool>(QueuedConfig::DBUS_SERVICE,
                             QueuedConfig::DBUS_OBJECT_PATH,
                             QueuedConfig::DBUS_SERVICE, "TaskEdit", args);
}


/**
 * @fn sendTaskStart
 */
QueuedResult<bool> QueuedCoreAdaptor::sendTaskStart(const long long _id,
                                                    const QString &_token)
{
    qCDebug(LOG_DBUS) << "Start task" << _id;

    QVariantList args = {_id, _token};
    return sendRequest<bool>(QueuedConfig::DBUS_SERVICE,
                             QueuedConfig::DBUS_OBJECT_PATH,
                             QueuedConfig::DBUS_SERVICE, "TaskStart", args);
}


/**
 * @fn sendTaskStop
 */
QueuedResult<bool> QueuedCoreAdaptor::sendTaskStop(const long long _id,
                                                   const QString &_token)
{
    qCDebug(LOG_DBUS) << "Stop task" << _id;

    QVariantList args = {_id, _token};
    return sendRequest<bool>(QueuedConfig::DBUS_SERVICE,
                             QueuedConfig::DBUS_OBJECT_PATH,
                             QueuedConfig::DBUS_SERVICE, "TaskStop", args);
}


/**
 * @fn sendUserAdd
 */
QueuedResult<long long> QueuedCoreAdaptor::sendUserAdd(
    const QueuedUser::QueuedUserDefinitions &_definitions,
    const QString &_token)
{
    qCDebug(LOG_DBUS) << "Add user" << _definitions.name;

    auto limits = QueuedLimits::Limits(_definitions.limits);
    QVariantList args = {_definitions.name,
                         _definitions.email,
                         _definitions.password,
                         _definitions.permissions,
                         limits.cpu,
                         limits.gpu,
                         limits.memory,
                         limits.gpumemory,
                         limits.storage,
                         _token};
    return sendRequest<long long>(QueuedConfig::DBUS_SERVICE,
                                  QueuedConfig::DBUS_OBJECT_PATH,
                                  QueuedConfig::DBUS_SERVICE, "UserAdd", args);
}


/**
 * @fn sendUserEdit
 */
QueuedResult<bool> QueuedCoreAdaptor::sendUserEdit(
    const long long _id, const QueuedUser::QueuedUserDefinitions &_definitions,
    const QString &_token)
{
    qCDebug(LOG_DBUS) << "Edit user" << _id;

    auto limits = QueuedLimits::Limits(_definitions.limits);
    QVariantList args = {_id,
                         _definitions.name,
                         _definitions.password,
                         _definitions.email,
                         limits.cpu,
                         limits.gpu,
                         limits.memory,
                         limits.gpumemory,
                         limits.storage,
                         _token};
    return sendRequest<bool>(QueuedConfig::DBUS_SERVICE,
                             QueuedConfig::DBUS_OBJECT_PATH,
                             QueuedConfig::DBUS_SERVICE, "UserEdit", args);
}


/**
 * @fn sendUserPermissionAdd
 */
QueuedResult<bool> QueuedCoreAdaptor::sendUserPermissionAdd(
    const long long _id, const QueuedEnums::Permission _permission,
    const QString &_token)
{
    qCDebug(LOG_DBUS) << "Add permission" << static_cast<int>(_permission)
                      << "to" << _id;

    QVariantList args = {_id, static_cast<uint>(_permission), _token};
    return sendRequest<bool>(
        QueuedConfig::DBUS_SERVICE, QueuedConfig::DBUS_OBJECT_PATH,
        QueuedConfig::DBUS_SERVICE, "UserPermissionAdd", args);
}


/**
 * @fn sendUserPermissionRemove
 */
QueuedResult<bool> QueuedCoreAdaptor::sendUserPermissionRemove(
    const long long _id, const QueuedEnums::Permission _permission,
    const QString &_token)
{
    qCDebug(LOG_DBUS) << "Remove permission" << static_cast<int>(_permission)
                      << "from" << _id;

    QVariantList args = {_id, static_cast<uint>(_permission), _token};
    return sendRequest<bool>(
        QueuedConfig::DBUS_SERVICE, QueuedConfig::DBUS_OBJECT_PATH,
        QueuedConfig::DBUS_SERVICE, "UserPermissionRemove", args);
}


/**
 * @fn getOption
 */
QueuedResult<QVariant> QueuedCoreAdaptor::getOption(const QString &_property)
{
    qCDebug(LOG_DBUS) << "Get option" << _property;

    QVariantList args = {_property};
    return sendRequest<QVariant>(QueuedConfig::DBUS_SERVICE,
                                 QueuedConfig::DBUS_PROPERTY_PATH,
                                 QueuedConfig::DBUS_SERVICE, "Option", args);
}


/**
 * @fn getOption
 */
QueuedResult<QVariant>
QueuedCoreAdaptor::getOption(const QueuedConfig::QueuedSettings _property)
{
    qCDebug(LOG_DBUS) << "Get option" << static_cast<int>(_property);

    return getOption(QueuedAdvancedSettings::internalId(_property));
}


/**
 * @fn getPerformance
 */
QueuedResult<QList<QVariantHash>>
QueuedCoreAdaptor::getPerformance(const QDateTime &_from, const QDateTime &_to,
                                  const QString &_token)
{
    qCDebug(LOG_DBUS) << "Get performance report for" << _from << _to;

    QVariantList args = {_from.toString(Qt::ISODateWithMs),
                         _to.toString(Qt::ISODateWithMs), _token};
    return sendRequest<QList<QVariantHash>>(
        QueuedConfig::DBUS_SERVICE, QueuedConfig::DBUS_REPORTS_PATH,
        QueuedConfig::DBUS_SERVICE, "Performance", args);
}


/**
 * @fn getStatus
 */
QueuedResult<QueuedStatusMap> QueuedCoreAdaptor::getStatus()
{
    return sendRequest<QueuedStatusMap>(
        QueuedConfig::DBUS_SERVICE, QueuedConfig::DBUS_REPORTS_PATH,
        QueuedConfig::DBUS_SERVICE, "Status", {});
}


/**
 * @fn getTask
 */
QueuedResult<QVariantHash> QueuedCoreAdaptor::getTask(const long long _id)
{
    qCDebug(LOG_DBUS) << "Get task properties" << _id;

    auto res = getTask(_id, "");

    QueuedResult<QVariantHash> output;
    res.match(
        [&output](const QVariant &val) {
            output = toResult<QVariantHash>(val);
        },
        [&output](const QueuedError &err) { output = err; });

    return output;
}


/**
 * @fn getTask
 */
QueuedResult<QVariant> QueuedCoreAdaptor::getTask(const long long _id,
                                                  const QString &_property)
{
    qCDebug(LOG_DBUS) << "Get task property" << _id << _property;

    QVariantList args = {_id, _property};
    return sendRequest<QVariant>(QueuedConfig::DBUS_SERVICE,
                                 QueuedConfig::DBUS_PROPERTY_PATH,
                                 QueuedConfig::DBUS_SERVICE, "Task", args);
}


/**
 * @fn getTasks
 */
QueuedResult<QList<QVariantHash>>
QueuedCoreAdaptor::getTasks(const long long _user, const QDateTime &_from,
                            const QDateTime &_to, const QString &_token)
{
    qCDebug(LOG_DBUS) << "Get tasks list for" << _user << _from << _to;

    QVariantList args = {_user, _from.toString(Qt::ISODateWithMs),
                         _to.toString(Qt::ISODateWithMs), _token};
    return sendRequest<QList<QVariantHash>>(
        QueuedConfig::DBUS_SERVICE, QueuedConfig::DBUS_REPORTS_PATH,
        QueuedConfig::DBUS_SERVICE, "Tasks", args);
}


/**
 * @fn getUser
 */
QueuedResult<QVariantHash> QueuedCoreAdaptor::getUser(const long long _id)
{
    qCDebug(LOG_DBUS) << "Get user property" << _id;

    auto res = getUser(_id, "");

    QueuedResult<QVariantHash> output;
    res.match(
        [&output](const QVariant &val) {
            output = toResult<QVariantHash>(val);
        },
        [&output](const QueuedError &err) { output = err; });

    return output;
}


/**
 * @fn getUser
 */
QueuedResult<QVariant> QueuedCoreAdaptor::getUser(const long long _id,
                                                  const QString &_property)
{
    qCDebug(LOG_DBUS) << "Get user property" << _id << _property;

    QVariantList args = {_id, _property};
    return sendRequest<QVariant>(QueuedConfig::DBUS_SERVICE,
                                 QueuedConfig::DBUS_PROPERTY_PATH,
                                 QueuedConfig::DBUS_SERVICE, "User", args);
}


/**
 * @fn getUserId
 */
QueuedResult<long long> QueuedCoreAdaptor::getUserId(const QString &_name)
{
    qCDebug(LOG_DBUS) << "Get user ID for" << _name;

    bool status = false;
    long long stringToLong = _name.toLongLong(&status);
    if (status)
        return stringToLong;

    QVariantList args = {_name};
    return sendRequest<long long>(
        QueuedConfig::DBUS_SERVICE, QueuedConfig::DBUS_PROPERTY_PATH,
        QueuedConfig::DBUS_SERVICE, "UserIdByName", args);
}


/**
 * @fn getUsers
 */
QueuedResult<QList<QVariantHash>>
QueuedCoreAdaptor::getUsers(const QDateTime &_lastLogged,
                            const QueuedEnums::Permission _permission,
                            const QString &_token)
{
    qCDebug(LOG_DBUS) << "Get users list for" << _lastLogged
                      << static_cast<int>(_permission);

    QVariantList args = {_lastLogged.toString(Qt::ISODateWithMs),
                         static_cast<uint>(_permission), _token};
    return sendRequest<QList<QVariantHash>>(
        QueuedConfig::DBUS_SERVICE, QueuedConfig::DBUS_REPORTS_PATH,
        QueuedConfig::DBUS_SERVICE, "Users", args);
}
