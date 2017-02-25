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
 * @file QueuedCore.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include "queued/Queued.h"

#include "queued/QueuedDatabaseSchema.h"


/**
 * @class QueuedCore
 */
/**
 * @fn QueuedCore
 */
QueuedCore::QueuedCore(QObject *parent)
    : QObject(parent)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;
}


/**
 * @fn ~QueuedCore
 */
QueuedCore::~QueuedCore()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    deinit();
}


/**
 * @fn deinit
 */
void QueuedCore::deinit()
{
}


/**
 * @fn init
 */
void QueuedCore::init(const QString &_configuration)
{
    qCDebug(LOG_LIB) << "Load configuration from" << _configuration;

    // deinit objects if any
    deinit();

    // read configuration first
    m_settings = new QueuedSettings(this, _configuration);
    // init database now
    auto dbSetup = m_settings->db();
    m_database = new QueuedDatabase(this, dbSetup.path, dbSetup.driver);
    m_database->open(dbSetup.hostname, dbSetup.port, dbSetup.username,
                     dbSetup.password);
    auto dbAdmin = m_settings->admin();
    m_database->createAdministrator(dbAdmin.name, dbAdmin.password);

    // and load advanced settings
    m_advancedSettings = new QueuedAdvancedSettings(this);
    m_advancedSettings->set(m_database->get(QueuedDB::SETTINGS_TABLE));

    // report manager
    m_reports = new QueuedReportManager(this, m_database);

    // load users and tokens
    m_users = new QueuedUserManager(this);
    auto expiry
        = m_advancedSettings->get(QString("TokenExpiration")).toLongLong();
    m_users->setTokenExpiration(expiry);
    m_users->loadTokens(m_database->get(QueuedDB::TOKENS_TABLE));
    m_users->loadUsers(m_database->get(QueuedDB::USERS_TABLE));

    // and processes finally
    auto onExitAction = static_cast<QueuedProcessManager::OnExitAction>(
        m_advancedSettings->get(QString("OnExitAction")).toInt());
    m_processes = new QueuedProcessManager(this, onExitAction);
    m_processes->add(m_database->get(QueuedDB::TASKS_TABLE));
}
