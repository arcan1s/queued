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
 * @file QueuedCoreAdaptor.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include "queued/Queued.h"

#include <QDBusConnection>
#include <QDBusMessage>


/**
 * @fn auth
 */
bool QueuedCoreAdaptor::auth(const QString &_token)
{
    QVariantList args = {_token};
    return sendRequest(QueuedConfig::DBUS_SERVICE,
                       QueuedConfig::DBUS_OBJECT_PATH,
                       QueuedConfig::DBUS_SERVICE, "TryAuth", args)
        .at(0)
        .toBool();
}


/**
 * @fn auth
 */
QString QueuedCoreAdaptor::auth(const QString &_name, const QString &_password)
{
    qCDebug(LOG_DBUS) << "Auth user" << _name;

    QVariantList args = {_name, _password};
    return sendRequest(QueuedConfig::DBUS_SERVICE,
                       QueuedConfig::DBUS_OBJECT_PATH,
                       QueuedConfig::DBUS_SERVICE, "Auth", args)
        .at(0)
        .toString();
}


/**
 * @fn sendOptionEdit
 */
bool QueuedCoreAdaptor::sendOptionEdit(const QString &_key,
                                       const QVariant &_value,
                                       const QString &_token)
{
    qCDebug(LOG_DBUS) << "Edit option" << _key << "to" << _value;

    QVariantList args
        = {_key, QVariant::fromValue(QDBusVariant(_value)), _token};
    return sendRequest(QueuedConfig::DBUS_SERVICE,
                       QueuedConfig::DBUS_OBJECT_PATH,
                       QueuedConfig::DBUS_SERVICE, "OptionEdit", args)
        .at(0)
        .toBool();
}


/**
 * @fn sendPluginAdd
 */
bool QueuedCoreAdaptor::sendPluginAdd(const QString &_plugin,
                                      const QString &_token)
{
    qCDebug(LOG_DBUS) << "Add plugin" << _plugin;

    QVariantList args = {_plugin, _token};
    return sendRequest(QueuedConfig::DBUS_SERVICE,
                       QueuedConfig::DBUS_OBJECT_PATH,
                       QueuedConfig::DBUS_SERVICE, "PluginAdd", args)
        .at(0)
        .toBool();
}


/**
 * @fn sendPluginRemove
 */
bool QueuedCoreAdaptor::sendPluginRemove(const QString &_plugin,
                                         const QString &_token)
{
    qCDebug(LOG_DBUS) << "Remove plugin" << _plugin;

    QVariantList args = {_plugin, _token};
    return sendRequest(QueuedConfig::DBUS_SERVICE,
                       QueuedConfig::DBUS_OBJECT_PATH,
                       QueuedConfig::DBUS_SERVICE, "PluginRemove", args)
        .at(0)
        .toBool();
}


/**
 * @fn sendTaskAdd
 */
long long QueuedCoreAdaptor::sendTaskAdd(
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
    return sendRequest(QueuedConfig::DBUS_SERVICE,
                       QueuedConfig::DBUS_OBJECT_PATH,
                       QueuedConfig::DBUS_SERVICE, "TaskAdd", args)
        .at(0)
        .toLongLong();
}


/**
 * @fn sendTaskEdit
 */
bool QueuedCoreAdaptor::sendTaskEdit(
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
    return sendRequest(QueuedConfig::DBUS_SERVICE,
                       QueuedConfig::DBUS_OBJECT_PATH,
                       QueuedConfig::DBUS_SERVICE, "TaskEdit", args)
        .at(0)
        .toBool();
}


/**
 * @fn sendTaskStart
 */
bool QueuedCoreAdaptor::sendTaskStart(const long long _id,
                                      const QString &_token)
{
    qCDebug(LOG_DBUS) << "Start task" << _id;

    QVariantList args = {_id, _token};
    return sendRequest(QueuedConfig::DBUS_SERVICE,
                       QueuedConfig::DBUS_OBJECT_PATH,
                       QueuedConfig::DBUS_SERVICE, "TaskStart", args)
        .at(0)
        .toBool();
}


/**
 * @fn sendTaskStop
 */
bool QueuedCoreAdaptor::sendTaskStop(const long long _id, const QString &_token)
{
    qCDebug(LOG_DBUS) << "Stop task" << _id;

    QVariantList args = {_id, _token};
    return sendRequest(QueuedConfig::DBUS_SERVICE,
                       QueuedConfig::DBUS_OBJECT_PATH,
                       QueuedConfig::DBUS_SERVICE, "TaskStop", args)
        .at(0)
        .toBool();
}


/**
 * @fn sendUserAdd
 */
long long QueuedCoreAdaptor::sendUserAdd(
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
    return sendRequest(QueuedConfig::DBUS_SERVICE,
                       QueuedConfig::DBUS_OBJECT_PATH,
                       QueuedConfig::DBUS_SERVICE, "UserAdd", args)
        .at(0)
        .toLongLong();
}


/**
 * @fn sendUserEdit
 */
bool QueuedCoreAdaptor::sendUserEdit(
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
    return sendRequest(QueuedConfig::DBUS_SERVICE,
                       QueuedConfig::DBUS_OBJECT_PATH,
                       QueuedConfig::DBUS_SERVICE, "UserEdit", args)
        .at(0)
        .toBool();
}


/**
 * sendUserPermissionAdd
 */
bool QueuedCoreAdaptor::sendUserPermissionAdd(
    const long long _id, const QueuedEnums::Permission _permission,
    const QString &_token)
{
    qCDebug(LOG_DBUS) << "Add permission" << static_cast<int>(_permission)
                      << "to" << _id;

    QVariantList args = {_id, static_cast<uint>(_permission), _token};
    return sendRequest(QueuedConfig::DBUS_SERVICE,
                       QueuedConfig::DBUS_OBJECT_PATH,
                       QueuedConfig::DBUS_SERVICE, "UserPermissionAdd", args)
        .at(0)
        .toBool();
}


/**
 * sendUserPermissionRemove
 */
bool QueuedCoreAdaptor::sendUserPermissionRemove(
    const long long _id, const QueuedEnums::Permission _permission,
    const QString &_token)
{
    qCDebug(LOG_DBUS) << "Remove permission" << static_cast<int>(_permission)
                      << "from" << _id;

    QVariantList args = {_id, static_cast<uint>(_permission), _token};
    return sendRequest(QueuedConfig::DBUS_SERVICE,
                       QueuedConfig::DBUS_OBJECT_PATH,
                       QueuedConfig::DBUS_SERVICE, "UserPermissionRemove", args)
        .at(0)
        .toBool();
}


/**
 * @fn getOption
 */
QVariant QueuedCoreAdaptor::getOption(const QString &_property)
{
    qCDebug(LOG_DBUS) << "Get option" << _property;

    QVariantList args = {_property};
    return toNativeType(sendRequest(QueuedConfig::DBUS_SERVICE,
                                    QueuedConfig::DBUS_PROPERTY_PATH,
                                    QueuedConfig::DBUS_SERVICE, "Option", args)
                            .at(0));
}


/**
 * @fn getTask
 */
QVariant QueuedCoreAdaptor::getTask(const long long _id,
                                    const QString &_property)
{
    qCDebug(LOG_DBUS) << "Get task property" << _id << _property;

    QVariantList args = {_id, _property};
    return toNativeType(sendRequest(QueuedConfig::DBUS_SERVICE,
                                    QueuedConfig::DBUS_PROPERTY_PATH,
                                    QueuedConfig::DBUS_SERVICE, "Task", args)
                            .at(0));
}


/**
 * @fn getUser
 */
QVariant QueuedCoreAdaptor::getUser(const long long _id,
                                    const QString &_property)
{
    qCDebug(LOG_DBUS) << "Get user property" << _id << _property;

    QVariantList args = {_id, _property};
    return toNativeType(sendRequest(QueuedConfig::DBUS_SERVICE,
                                    QueuedConfig::DBUS_PROPERTY_PATH,
                                    QueuedConfig::DBUS_SERVICE, "User", args)
                            .at(0));
}


/**
 * @fn sendRequest
 */
QVariantList QueuedCoreAdaptor::sendRequest(const QString &_service,
                                            const QString &_path,
                                            const QString &_interface,
                                            const QString &_cmd,
                                            const QVariantList &_args)
{
    qCDebug(LOG_DBUS) << "Send request to service" << _service << "by interface"
                      << _interface << "to" << _path << "command" << _cmd
                      << "with args" << _args;

    QDBusConnection bus = QDBusConnection::systemBus();
    QDBusMessage request
        = QDBusMessage::createMethodCall(_service, _path, _interface, _cmd);
    if (!_args.isEmpty())
        request.setArguments(_args);

    QDBusMessage response = bus.call(request, QDBus::BlockWithGui);
    QVariantList arguments = response.arguments();

    QString error = response.errorMessage();
    if (!error.isEmpty())
        qCWarning(LOG_DBUS) << "Error message" << error;

    return arguments;
}


/**
 * @fn toNativeType
 */
QVariant QueuedCoreAdaptor::toNativeType(const QVariant &_data)
{
    return _data.value<QDBusVariant>().variant();
}
