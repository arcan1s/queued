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
 * @file QueuedCore.cpp
 * Source code of queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>

#include <queued/private/QueuedCorePrivate.h>


/**
 * @class QueuedCore
 */
/**
 * @fn QueuedCore
 */
QueuedCore::QueuedCore(QObject *_parent)
    : QObject(_parent)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    m_impl = new QueuedCorePrivate(this);
}


/**
 * @fn ~QueuedCore
 */
QueuedCore::~QueuedCore()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    deinit();
    m_impl->deleteLater();
}


/**
 * @fn addPlugin
 */
QueuedResult<bool> QueuedCore::addPlugin(const QString &_plugin,
                                         const QString &_token)
{
    qCDebug(LOG_LIB) << "Add plugin" << _plugin;

    return m_impl->addPlugin(_plugin, _token);
}


/**
 * @addTask
 */
QueuedResult<long long>
QueuedCore::addTask(const QString &_command, const QStringList &_arguments,
                    const QString &_workingDirectory, const long long _userId,
                    const uint _nice, const QueuedLimits::Limits &_limits,
                    const QString &_token)
{
    qCDebug(LOG_LIB) << "Add task" << _command << "with arguments" << _arguments
                     << "from user" << _userId;

    return m_impl->addTask(_command, _arguments, _workingDirectory, _userId,
                           _nice, _limits, _token);
}


/**
 * @fn addUser
 */
QueuedResult<long long>
QueuedCore::addUser(const QString &_name, const QString &_email,
                    const QString &_password, const uint _permissions,
                    const uint _priority, const QueuedLimits::Limits &_limits,
                    const QString &_token)
{
    qCDebug(LOG_LIB) << "Add user" << _name << "with email" << _email
                     << "and permissions" << _permissions;

    return m_impl->addUser(_name, _email, _password, _permissions, _priority,
                           _limits, _token);
}


/**
 * @fn authorization
 */
QueuedResult<bool> QueuedCore::authorization(const QString &_token)
{
    return m_impl->authorization(_token);
}


/**
 * @fn authorization
 */
QueuedResult<QString> QueuedCore::authorization(const QString &_name,
                                                const QString &_password)
{
    qCDebug(LOG_LIB) << "Authorize user" << _name;

    return m_impl->authorization(_name, _password);
}


/**
 * @fn editOption
 */
QueuedResult<bool> QueuedCore::editOption(const QString &_key,
                                          const QVariant &_value,
                                          const QString &_token)
{
    qCDebug(LOG_LIB) << "Set key" << _key << "to" << _value;

    return m_impl->editOption(_key, _value, _token);
}


/**
 * @fn editTask
 */
QueuedResult<bool> QueuedCore::editTask(const long long _id,
                                        const QVariantHash &_taskData,
                                        const QString &_token)
{
    qCDebug(LOG_LIB) << "Edit task with ID" << _id;

    return m_impl->editTask(_id, _taskData, _token);
}


/**
 * @fn editUser
 */
QueuedResult<bool> QueuedCore::editUser(const long long _id,
                                        const QVariantHash &_userData,
                                        const QString &_token)
{
    qCDebug(LOG_LIB) << "Edit user with ID" << _id;

    return m_impl->editUser(_id, _userData, _token);
}


/**
 * @fn editUserPermission
 */
QueuedResult<bool>
QueuedCore::editUserPermission(const long long _id,
                               const QueuedEnums::Permission &_permission,
                               const bool _add, const QString &_token)
{
    qCDebug(LOG_LIB) << "Edit permissions" << static_cast<int>(_permission)
                     << "for user" << _id << "add" << _add;

    return m_impl->editUserPermission(_id, _permission, _add, _token);
}


/**
 * @fn hashFromPassword
 */
QueuedResult<QString> QueuedCore::hashFromPassword(const QString &_password)
{
    return m_impl->hashFromPassword(_password);
}


/**
 * @fn option
 */
QueuedResult<QVariant> QueuedCore::option(const QString &_key,
                                          const QString &_token)
{
    qCDebug(LOG_LIB) << "Look for option" << _key;

    return m_impl->option(_key, _token);
}


/**
 * @fn performanceReport
 */
QueuedResult<QList<QVariantHash>>
QueuedCore::performanceReport(const QDateTime &_from, const QDateTime &_to,
                              const QString &_token) const
{
    qCDebug(LOG_LIB) << "Get performance report for" << _from << _to;

    return m_impl->performanceReport(_from, _to, _token);
}


/**
 * @fn plugin
 */
QueuedResult<QueuedPluginSpecification::Plugin>
QueuedCore::plugin(const QString &_plugin, const QString &_token)
{
    qCDebug(LOG_LIB) << "Get data for plugin" << _plugin;

    return m_impl->plugin(_plugin, _token);
}


/**
 * @fn pluginSettings
 */
QueuedResult<QVariantHash> QueuedCore::pluginSettings(const QString &_plugin,
                                                      const QString &_token)
{
    qCDebug(LOG_LIB) << "Get plugin settings for" << _plugin;

    return m_impl->pluginSettings(_plugin, _token);
}


/**
 * @fn removePlugin
 */
QueuedResult<bool> QueuedCore::removePlugin(const QString &_plugin,
                                            const QString &_token)
{
    qCDebug(LOG_LIB) << "Remove plugin" << _plugin;

    return m_impl->removePlugin(_plugin, _token);
}


/**
 * @fn startTask
 */
QueuedResult<bool> QueuedCore::startTask(const long long _id,
                                         const QString &_token)
{
    qCDebug(LOG_LIB) << "Force start task with ID" << _id;

    return m_impl->startTask(_id, _token);
}


/**
 * @fn stopTask
 */
QueuedResult<bool> QueuedCore::stopTask(const long long _id,
                                        const QString &_token)
{
    qCDebug(LOG_LIB) << "Force stop task with ID" << _id;

    return m_impl->stopTask(_id, _token);
}


/**
 * @fn task
 */
const QueuedProcess *QueuedCore::task(const long long _id,
                                      const QString &_token) const
{
    qCDebug(LOG_LIB) << "Get task by ID" << _id;

    return m_impl->task(_id, _token);
}


/**
 * @fn taskReport
 */
QueuedResult<QList<QVariantHash>>
QueuedCore::taskReport(const long long _user, const QDateTime &_from,
                       const QDateTime &_to, const QString &_token) const
{
    qCDebug(LOG_LIB) << "Get tasks table by" << _user << _from << _to;

    return m_impl->taskReport(_user, _from, _to, _token);
}


/**
 * @fn user
 */
const QueuedUser *QueuedCore::user(const long long _id,
                                   const QString &_token) const
{
    qCDebug(LOG_LIB) << "Get user by ID" << _id;

    return m_impl->user(_id, _token);
}


/**
 * @fn user
 */
const QueuedUser *QueuedCore::user(const QString &_name,
                                   const QString &_token) const
{
    qCDebug(LOG_LIB) << "Get user by name" << _name;

    return m_impl->user(_name, _token);
}


/**
 * @fn userReport
 */
QueuedResult<QList<QVariantHash>>
QueuedCore::userReport(const QDateTime &_lastLogged,
                       const QueuedEnums::Permission _permission,
                       const QString &_token) const
{
    qCDebug(LOG_LIB) << "Get users table by" << _lastLogged
                     << static_cast<int>(_permission);

    return m_impl->userReport(_lastLogged, _permission, _token);
}


/**
 * @fn deinit
 */
void QueuedCore::deinit()
{
    m_impl->deinit();

    // dbus cleanup
    QDBusConnection::systemBus().unregisterObject(
        QueuedConfig::DBUS_OBJECT_PATH);
    QDBusConnection::systemBus().unregisterObject(
        QueuedConfig::DBUS_PROPERTY_PATH);
    QDBusConnection::systemBus().unregisterObject(
        QueuedConfig::DBUS_REPORTS_PATH);
    QDBusConnection::systemBus().unregisterService(QueuedConfig::DBUS_SERVICE);
}


/**
 * @fn init
 */
void QueuedCore::init(const QString &_configuration)
{
    qCDebug(LOG_LIB) << "Load configuration from" << _configuration;

    m_impl->init(_configuration);

    // dbus session
    initDBus();
}


/**
 * @fn initDBus
 */
void QueuedCore::initDBus()
{
    QDBusConnection bus = QDBusConnection::systemBus();

    if (!bus.registerService(QueuedConfig::DBUS_SERVICE)) {
        QString message = QString("Could not register service %1")
                              .arg(bus.lastError().message());
        qCCritical(LOG_DBUS) << message;
        throw QueuedDBusException(message);
    }

    if (!bus.registerObject(QueuedConfig::DBUS_OBJECT_PATH,
                            new QueuedCoreInterface(this),
                            QDBusConnection::ExportAllContents)) {
        QString message = QString("Could not register core object %1")
                              .arg(bus.lastError().message());
        qCCritical(LOG_DBUS) << message;
        throw QueuedDBusException(message);
    }
    if (!bus.registerObject(QueuedConfig::DBUS_PROPERTY_PATH,
                            new QueuedPropertyInterface(this),
                            QDBusConnection::ExportAllContents)) {
        QString message = QString("Could not register properties object %1")
                              .arg(bus.lastError().message());
        qCCritical(LOG_DBUS) << message;
        throw QueuedDBusException(message);
    }
    if (!bus.registerObject(QueuedConfig::DBUS_REPORTS_PATH,
                            new QueuedReportInterface(this),
                            QDBusConnection::ExportAllContents)) {
        QString message = QString("Could not register reports object %1")
                              .arg(bus.lastError().message());
        qCCritical(LOG_DBUS) << message;
        throw QueuedDBusException(message);
    }
}
