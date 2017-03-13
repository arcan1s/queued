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
 * @file QueuedCoreInterface.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include "queued/Queued.h"


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
QString QueuedCoreInterface::Auth(const QString &name, const QString &password)
{
    qCDebug(LOG_DBUS) << "Authorize user" << name;

    return m_core->authorization(name, password);
}


/**
 * @fn OptionEdit
 */
bool QueuedCoreInterface::OptionEdit(const QString &key,
                                     const QDBusVariant &value,
                                     const QString &token)
{
    qCDebug(LOG_DBUS) << "Edit option" << key << value.variant();

    return m_core->editOption(key, value.variant(), token);
}


/**
 * @fn PluginAdd
 */
bool QueuedCoreInterface::PluginAdd(const QString &plugin, const QString &token)
{
    qCDebug(LOG_DBUS) << "Add plugin" << plugin;

    return m_core->addPlugin(plugin, token);
}


/**
 * @fn PluginRemove
 */
bool QueuedCoreInterface::PluginRemove(const QString &plugin,
                                       const QString &token)
{
    qCDebug(LOG_DBUS) << "Remove plugin" << plugin;

    return m_core->removePlugin(plugin, token);
}


/**
 * @fn TaskAdd
 */
qlonglong QueuedCoreInterface::TaskAdd(
    const QString &command, const QStringList &arguments,
    const QString &workingDirectory, const qlonglong user, const qlonglong cpu,
    const qlonglong gpu, const QString &memory, const QString &gpumemory,
    const QString &storage, const QString &token)
{
    qCDebug(LOG_DBUS) << "Add new task with parameters" << command << arguments
                      << workingDirectory << "from user" << user;

    return m_core->addTask(
        command, arguments, workingDirectory, user,
        QueuedLimits::Limits(cpu, gpu, QueuedLimits::convertMemory(memory),
                             QueuedLimits::convertMemory(gpumemory),
                             QueuedLimits::convertMemory(storage)),
        token);
}


/**
 * @fn TaskEdit
 */
bool QueuedCoreInterface::TaskEdit(const qlonglong id, const QString &command,
                                   const QStringList &arguments,
                                   const QString &directory, const uint nice,
                                   const uint uid, const uint gid,
                                   const qlonglong user, const qlonglong cpu,
                                   const qlonglong gpu, const QString &memory,
                                   const QString &gpumemory,
                                   const QString &storage, const QString &token)
{
    qCDebug(LOG_DBUS) << "Edit task" << id << command << arguments << directory
                      << nice << uid << gid << cpu << gpu << memory << gpumemory
                      << storage;

    auto task = m_core->task(id);
    if (!task) {
        qCWarning(LOG_DBUS) << "Could not find task" << id;
        return false;
    }

    // build payload
    QVariantHash data;
    if (!command.isEmpty())
        data[QString("command")] = command;
    if (!arguments.isEmpty())
        data[QString("arguments")] = arguments;
    if (!directory.isEmpty())
        data[QString("directory")] = directory;
    if (nice > 0)
        data[QString("nice")] = nice;
    if (uid > 0)
        data[QString("uid")] = uid;
    if (gid > 0)
        data[QString("gid")] = gid;
    if (user > 0)
        data[QString("user")] = user;
    // append limits now
    auto limits = task->nativeLimits();
    if (cpu > -1)
        limits.cpu = cpu;
    if (gpu > -1)
        limits.gpu = gpu;
    if (memory > -1)
        limits.memory = QueuedLimits::convertMemory(memory);
    if (gpumemory > -1)
        limits.gpumemory = QueuedLimits::convertMemory(gpumemory);
    if (storage > -1)
        limits.storage = QueuedLimits::convertMemory(storage);
    data[QString("limits")] = limits.toString();

    return m_core->editTask(id, data, token);
}


/**
 * @fn TaskStart
 */
bool QueuedCoreInterface::TaskStart(const qlonglong id, const QString &token)
{
    qCDebug(LOG_DBUS) << "Force start task" << id;

    return m_core->startTask(id, token);
}


/**
 * @fn TaskStop
 */
bool QueuedCoreInterface::TaskStop(const qlonglong id, const QString &token)
{
    qCDebug(LOG_DBUS) << "Force stop task" << id;

    return m_core->stopTask(id, token);
}


/**
 * @fn TryAuth
 */
bool QueuedCoreInterface::TryAuth(const QString &token)
{
    return m_core->authorization(token);
}


/**
 * @fn UserAdd
 */
qlonglong
QueuedCoreInterface::UserAdd(const QString &name, const QString &email,
                             const QString &password, const uint permissions,
                             const qlonglong cpu, const qlonglong gpu,
                             const QString &memory, const QString &gpumemory,
                             const QString &storage, const QString &token)
{
    qCDebug(LOG_DBUS) << "Add new user with paramaters" << name << email
                      << permissions;

    return m_core->addUser(
        name, email, password, permissions,
        QueuedLimits::Limits(cpu, gpu, QueuedLimits::convertMemory(memory),
                             QueuedLimits::convertMemory(gpumemory),
                             QueuedLimits::convertMemory(storage)),
        token);
}


/**
 * @fn UserEdit
 */
bool QueuedCoreInterface::UserEdit(const qlonglong id, const QString &name,
                                   const QString &password,
                                   const QString &email, const qlonglong cpu,
                                   const qlonglong gpu, const QString &memory,
                                   const QString &gpumemory,
                                   const QString &storage, const QString &token)
{
    qCDebug(LOG_DBUS) << "Edit user" << id << name << email << cpu << gpu
                      << memory << gpumemory << storage;

    // get user object first to match limits
    auto user = m_core->user(id);
    if (!user) {
        qCWarning(LOG_DBUS) << "Could not find user" << id;
        return false;
    }

    // build payload
    QVariantHash data;
    if (!name.isEmpty())
        data[QString("name")] = name;
    if (!password.isEmpty())
        data[QString("password")] = password;
    if (!email.isEmpty())
        data[QString("email")] = email;
    // append limits now
    auto limits = user->nativeLimits();
    if (cpu > -1)
        limits.cpu = cpu;
    if (gpu > -1)
        limits.gpu = gpu;
    if (memory > -1)
        limits.memory = QueuedLimits::convertMemory(memory);
    if (gpumemory > -1)
        limits.gpumemory = QueuedLimits::convertMemory(gpumemory);
    if (storage > -1)
        limits.storage = QueuedLimits::convertMemory(storage);
    data[QString("limits")] = limits.toString();

    return m_core->editUser(id, data, token);
}


/**
 * @fn UserPermissionAdd
 */
bool QueuedCoreInterface::UserPermissionAdd(const qlonglong id,
                                            const uint permission,
                                            const QString &token)
{
    qCDebug(LOG_DBUS) << "Add permission" << permission << "to user" << id;

    return m_core->editUserPermission(
        id, static_cast<QueuedEnums::Permission>(permission), true, token);
}


/**
 * @fn UserPermissionRemove
 */
bool QueuedCoreInterface::UserPermissionRemove(const qlonglong id,
                                               const uint permission,
                                               const QString &token)
{
    qCDebug(LOG_DBUS) << "Remove permission" << permission << "from user" << id;

    return m_core->editUserPermission(
        id, static_cast<QueuedEnums::Permission>(permission), false, token);
}
