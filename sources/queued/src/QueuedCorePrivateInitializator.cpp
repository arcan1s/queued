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
 * @file QueuedCorePrivateInitializator.cpp
 * Source code of queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>
#include <queued/private/QueuedCorePrivate.h>

#include "queued/QueuedDatabaseSchema.h"

#include <queued/private/QueuedCorePrivateHelper.h>


/**
 * @class QueuedCorePrivate
 */
/**
 * @fn QueuedCorePrivate
 */
QueuedCorePrivate::QueuedCorePrivate(QObject *_parent)
    : QObject(_parent)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    m_helper = new QueuedCorePrivateHelper(this);
}


/**
 * @fn ~QueuedCorePrivate
 */
QueuedCorePrivate::~QueuedCorePrivate()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    deinit();
}


/**
 * @fn deinit
 */
void QueuedCorePrivate::deinit()
{
    // clear connections first
    for (auto &connection : m_connections)
        disconnect(connection);
    m_connections.clear();
}


/**
 * @fn init
 */
void QueuedCorePrivate::init(const QString &_configuration)
{
    qCDebug(LOG_LIB) << "Load configuration from" << _configuration;

    // deinit objects if any
    deinit();

    // init parts
    initSettings(_configuration);
    initUsers();
    initPlugins();
    initProcesses();

    // settings update notifier
    m_connections
        += connect(m_advancedSettings,
                   SIGNAL(valueUpdated(const QueuedConfig::QueuedSettings,
                                       const QString &, const QVariant &)),
                   this,
                   SLOT(updateSettings(const QueuedConfig::QueuedSettings,
                                       const QString &, const QVariant &)));

    // run!
    m_processes->start();
}


/**
 * @fn initPlugins
 */
void QueuedCorePrivate::initPlugins()
{
    QStringList pluginList
        = m_advancedSettings->get(QueuedConfig::QueuedSettings::Plugins)
              .toString()
              .split('\n');
    QString token = m_users->authorize(m_settings->admin().name);

    m_plugins = m_helper->initObject(m_plugins, token);
    for (auto &plugin : pluginList)
        m_plugins->loadPlugin(plugin, pluginSettings(plugin));
}


/**
 * @fn initProcesses
 */
void QueuedCorePrivate::initProcesses()
{
    // init processes
    auto onExitAction = static_cast<QueuedEnums::ExitAction>(
        m_advancedSettings->get(QueuedConfig::QueuedSettings::OnExitAction)
            .toInt());
    auto processLine
        = m_advancedSettings
              ->get(QueuedConfig::QueuedSettings::ProcessCommandLine)
              .toString();

    m_processes = m_helper->initObject(m_processes);
    m_processes->setProcessLine(processLine);
    m_processes->setExitAction(onExitAction);
    auto dbProcesses
        = m_database->get(QueuedDB::TASKS_TABLE, "WHERE endTime IS NULL");
    m_processes->loadProcesses(dbProcesses);

    m_connections
        += connect(m_processes, &QueuedProcessManager::taskStartTimeReceived,
                   [this](const long long _index, const QDateTime &_time) {
                       return updateTaskTime(_index, _time, QDateTime());
                   });
    m_connections
        += connect(m_processes, &QueuedProcessManager::taskStopTimeReceived,
                   [this](const long long _index, const QDateTime &_time) {
                       return updateTaskTime(_index, QDateTime(), _time);
                   });
}


/**
 * @fn initSettings
 */
void QueuedCorePrivate::initSettings(const QString &_configuration)
{
    // read configuration first
    m_settings = m_helper->initObject(m_settings, _configuration);
    m_settings->readConfiguration();
    // init database now
    auto dbSetup = m_settings->db();
    m_database = m_helper->initObject(m_database, dbSetup.path, dbSetup.driver);
    m_database->close();
    bool status = m_database->open(dbSetup.hostname, dbSetup.port,
                                   dbSetup.username, dbSetup.password);
    if (!status) {
        QString message = "Could not open database";
        qCCritical(LOG_LIB) << message;
        throw QueuedDatabaseException(message);
    }

    // create administrator if required
    auto dbAdmin = m_settings->admin();
    m_database->createAdministrator(dbAdmin.name, dbAdmin.password);

    // and load advanced settings
    m_advancedSettings = m_helper->initObject(m_advancedSettings);
    m_advancedSettings->set(m_database->get(QueuedDB::SETTINGS_TABLE));
    if (!m_advancedSettings->checkDatabaseVersion()) {
        qCInfo(LOG_LIB) << "Bump database version to"
                        << QueuedConfig::DATABASE_VERSION;
        m_helper->editOptionPrivate(
            m_advancedSettings->internalId(
                QueuedConfig::QueuedSettings::DatabaseVersion),
            QueuedConfig::DATABASE_VERSION);
    }

    // report manager
    m_reports = m_helper->initObject(m_reports, m_database);
    // database manager
    m_databaseManager = m_helper->initObject(m_databaseManager, m_database);
}


/**
 * @fn initUsers
 */
void QueuedCorePrivate::initUsers()
{
    // load users and tokens
    auto expiry
        = m_advancedSettings->get(QueuedConfig::QueuedSettings::TokenExpiration)
              .toLongLong();

    m_users = m_helper->initObject(m_users);
    m_users->setSalt(m_settings->admin().salt);
    m_users->setTokenExpiration(expiry);
    QString now = QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs);
    auto dbTokens = m_database->get(
        QueuedDB::TOKENS_TABLE,
        QString("WHERE datetime(validUntil) > datetime('%1')").arg(now));
    m_users->loadTokens(dbTokens);
    auto dbUsers = m_database->get(QueuedDB::USERS_TABLE);
    m_users->loadUsers(dbUsers);

    m_connections += connect(
        m_users, SIGNAL(userLoggedIn(const long long, const QDateTime &)), this,
        SLOT(updateUserLoginTime(const long long, const QDateTime &)));
}
