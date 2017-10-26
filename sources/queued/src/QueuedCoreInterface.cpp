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
 * @file QueuedCoreInterface.cpp
 * Source code of queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>

#include <QDBusMetaType>


/**
 * @class QueuedCoreInterface
 */
/**
 * @fn QueuedCoreInterface
 */
QueuedCoreInterface::QueuedCoreInterface(QueuedCore *parent)
    : QDBusAbstractAdaptor(parent)
    , m_core(parent)
{
    qCDebug(LOG_DBUS) << __PRETTY_FUNCTION__;

    qRegisterMetaType<QueuedResult<bool>>("QueuedResult<bool>");
    qDBusRegisterMetaType<QueuedResult<bool>>();

    qRegisterMetaType<QueuedResult<long long>>("QueuedResult<long long>");
    qDBusRegisterMetaType<QueuedResult<long long>>();

    qRegisterMetaType<QueuedResult<QString>>("QueuedResult<QString>");
    qDBusRegisterMetaType<QueuedResult<QString>>();
}


/**
 * @fn ~QueuedCoreInterface
 */
QueuedCoreInterface::~QueuedCoreInterface()
{
    qCDebug(LOG_DBUS) << __PRETTY_FUNCTION__;
}


/**
 * @fn Auth
 */
QDBusVariant QueuedCoreInterface::Auth(const QString &name,
                                       const QString &password)
{
    qCDebug(LOG_DBUS) << "Authorize user" << name;

    return QueuedCoreAdaptor::toDBusVariant(
        m_core->authorization(name, password));
}


/**
 * @fn OptionEdit
 */
QDBusVariant QueuedCoreInterface::OptionEdit(const QString &key,
                                             const QDBusVariant &value,
                                             const QString &token)
{
    qCDebug(LOG_DBUS) << "Edit option" << key << value.variant();

    return QueuedCoreAdaptor::toDBusVariant(
        m_core->editOption(key, value.variant(), token));
}


/**
 * @fn PasswordHash
 */
QDBusVariant QueuedCoreInterface::PasswordHash(const QString &password)
{
    return QueuedCoreAdaptor::toDBusVariant(m_core->hashFromPassword(password));
}


/**
 * @fn PluginAdd
 */
QDBusVariant QueuedCoreInterface::PluginAdd(const QString &plugin,
                                            const QString &token)
{
    qCDebug(LOG_DBUS) << "Add plugin" << plugin;

    return QueuedCoreAdaptor::toDBusVariant(m_core->addPlugin(plugin, token));
}


/**
 * @fn PluginRemove
 */
QDBusVariant QueuedCoreInterface::PluginRemove(const QString &plugin,
                                               const QString &token)
{
    qCDebug(LOG_DBUS) << "Remove plugin" << plugin;

    return QueuedCoreAdaptor::toDBusVariant(
        m_core->removePlugin(plugin, token));
}


/**
 * @fn TaskAdd
 */
QDBusVariant QueuedCoreInterface::TaskAdd(
    const QString &command, const QStringList &arguments,
    const QString &workingDirectory, const qlonglong user, const qlonglong cpu,
    const qlonglong gpu, const qlonglong memory, const qlonglong gpumemory,
    const qlonglong storage, const QString &token)
{
    qCDebug(LOG_DBUS) << "Add new task with parameters" << command << arguments
                      << workingDirectory << "from user" << user;

    return QueuedCoreAdaptor::toDBusVariant(m_core->addTask(
        command, arguments, workingDirectory, user,
        QueuedLimits::Limits(cpu, gpu, memory, gpumemory, storage), token));
}


/**
 * @fn TaskEdit
 */
QDBusVariant QueuedCoreInterface::TaskEdit(
    const qlonglong id, const QString &command, const QStringList &arguments,
    const QString &directory, const uint nice, const uint uid, const uint gid,
    const qlonglong user, const qlonglong cpu, const qlonglong gpu,
    const qlonglong memory, const qlonglong gpumemory, const qlonglong storage,
    const QString &token)
{
    qCDebug(LOG_DBUS) << "Edit task" << id << command << arguments << directory
                      << nice << uid << gid << cpu << gpu << memory << gpumemory
                      << storage;

    auto task = m_core->task(id);
    if (!task) {
        qCWarning(LOG_DBUS) << "Could not find task" << id;
        return QueuedCoreAdaptor::toDBusVariant(QueuedResult<bool>(
            QueuedError("Task does not exist",
                        QueuedEnums::ReturnStatus::InvalidArgument)));
    }

    // build payload
    QVariantHash data;
    if (!command.isEmpty())
        data["command"] = command;
    if (!arguments.isEmpty())
        data["arguments"] = arguments;
    if (!directory.isEmpty())
        data["directory"] = directory;
    if (nice > 0)
        data["nice"] = nice;
    if (uid > 0)
        data["uid"] = uid;
    if (gid > 0)
        data["gid"] = gid;
    if (user > 0)
        data["user"] = user;
    // append limits now
    auto limits = task->nativeLimits();
    if (cpu > -1)
        limits.cpu = cpu;
    if (gpu > -1)
        limits.gpu = gpu;
    if (memory > -1)
        limits.memory = memory;
    if (gpumemory > -1)
        limits.gpumemory = gpumemory;
    if (storage > -1)
        limits.storage = storage;
    data["limits"] = limits.toString();

    return QueuedCoreAdaptor::toDBusVariant(m_core->editTask(id, data, token));
}


/**
 * @fn TaskStart
 */
QDBusVariant QueuedCoreInterface::TaskStart(const qlonglong id,
                                            const QString &token)
{
    qCDebug(LOG_DBUS) << "Force start task" << id;

    return QueuedCoreAdaptor::toDBusVariant(m_core->startTask(id, token));
}


/**
 * @fn TaskStop
 */
QDBusVariant QueuedCoreInterface::TaskStop(const qlonglong id,
                                           const QString &token)
{
    qCDebug(LOG_DBUS) << "Force stop task" << id;

    return QueuedCoreAdaptor::toDBusVariant(m_core->stopTask(id, token));
}


/**
 * @fn TryAuth
 */
QDBusVariant QueuedCoreInterface::TryAuth(const QString &token)
{
    return QueuedCoreAdaptor::toDBusVariant(m_core->authorization(token));
}


/**
 * @fn UserAdd
 */
QDBusVariant
QueuedCoreInterface::UserAdd(const QString &name, const QString &email,
                             const QString &password, const uint permissions,
                             const qlonglong cpu, const qlonglong gpu,
                             const qlonglong memory, const qlonglong gpumemory,
                             const qlonglong storage, const QString &token)
{
    qCDebug(LOG_DBUS) << "Add new user with paramaters" << name << email
                      << permissions;

    return QueuedCoreAdaptor::toDBusVariant(m_core->addUser(
        name, email, password, permissions,
        QueuedLimits::Limits(cpu, gpu, memory, gpumemory, storage), token));
}


/**
 * @fn UserEdit
 */
QDBusVariant
QueuedCoreInterface::UserEdit(const qlonglong id, const QString &name,
                              const QString &password, const QString &email,
                              const qlonglong cpu, const qlonglong gpu,
                              const qlonglong memory, const qlonglong gpumemory,
                              const qlonglong storage, const QString &token)
{
    qCDebug(LOG_DBUS) << "Edit user" << id << name << email << cpu << gpu
                      << memory << gpumemory << storage;

    // get user object first to match limits
    auto user = m_core->user(id);
    if (!user) {
        qCWarning(LOG_DBUS) << "Could not find user" << id;
        return QueuedCoreAdaptor::toDBusVariant(QueuedResult<bool>(
            QueuedError("User does not exist",
                        QueuedEnums::ReturnStatus::InvalidArgument)));
    }

    // build payload
    QVariantHash data;
    if (!name.isEmpty())
        data["name"] = name;
    if (!password.isEmpty())
        data["password"] = password;
    if (!email.isEmpty())
        data["email"] = email;
    // append limits now
    auto limits = user->nativeLimits();
    if (cpu > -1)
        limits.cpu = cpu;
    if (gpu > -1)
        limits.gpu = gpu;
    if (memory > -1)
        limits.memory = memory;
    if (gpumemory > -1)
        limits.gpumemory = gpumemory;
    if (storage > -1)
        limits.storage = storage;
    data["limits"] = limits.toString();

    return QueuedCoreAdaptor::toDBusVariant(m_core->editUser(id, data, token));
}


/**
 * @fn UserPermissionAdd
 */
QDBusVariant QueuedCoreInterface::UserPermissionAdd(const qlonglong id,
                                                    const uint permission,
                                                    const QString &token)
{
    qCDebug(LOG_DBUS) << "Add permission" << permission << "to user" << id;

    return QueuedCoreAdaptor::toDBusVariant(m_core->editUserPermission(
        id, static_cast<QueuedEnums::Permission>(permission), true, token));
}


/**
 * @fn UserPermissionRemove
 */
QDBusVariant QueuedCoreInterface::UserPermissionRemove(const qlonglong id,
                                                       const uint permission,
                                                       const QString &token)
{
    qCDebug(LOG_DBUS) << "Remove permission" << permission << "from user" << id;

    return QueuedCoreAdaptor::toDBusVariant(m_core->editUserPermission(
        id, static_cast<QueuedEnums::Permission>(permission), false, token));
}
