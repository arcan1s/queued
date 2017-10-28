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
 * @file QueuedDatabaseManager.cpp
 * Source code of queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>


/**
 * @class QueuedDatabaseManager
 */
/**
 * @fn QueuedDatabaseManager
 */
QueuedDatabaseManager::QueuedDatabaseManager(QObject *_parent,
                                             QueuedDatabase *_database)
    : QObject(_parent)
    , m_database(_database)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    m_timer.setSingleShot(false);
    // connections
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(cleanup()));

    startWorker();
}


/**
 * @fn ~QueuedDatabaseManager
 */
QueuedDatabaseManager::~QueuedDatabaseManager()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    disconnect(&m_timer, SIGNAL(timeout()), this, SLOT(cleanup()));

    m_timer.stop();
}


/**
 * @fn startWorker
 */
void QueuedDatabaseManager::startWorker()
{
    // stop timer first
    if (m_timer.isActive())
        m_timer.stop();

    // update interval
    m_timer.setInterval(std::chrono::milliseconds(interval()));
    // start timer
    m_timer.start();
}


/**
 * @fn interval
 */
long long QueuedDatabaseManager::interval() const
{
    return m_interval;
}


/**
 * @fn keepTasks
 */
long long QueuedDatabaseManager::keepTasks() const
{
    return m_keepTasks;
}


/**
 * @fn keepUsers
 */
long long QueuedDatabaseManager::keepUsers() const
{
    return m_keepUsers;
}


/**
 * @fn setInterval
 */
void QueuedDatabaseManager::setInterval(const long long _interval)
{
    qCDebug(LOG_LIB) << "Set interval to" << _interval;

    m_interval = _interval;
    // update timer now
    startWorker();
}


/**
 * @fn setKeepTasks
 */
void QueuedDatabaseManager::setKeepTasks(const long long _keepInterval)
{
    qCDebug(LOG_LIB) << "Set keep tasks to" << _keepInterval;

    m_keepTasks = _keepInterval;
}


/**
 * @fn setKeepUsers
 */
void QueuedDatabaseManager::setKeepUsers(const long long _keepInterval)
{
    qCDebug(LOG_LIB) << "Set keep users to" << _keepInterval;

    m_keepUsers = _keepInterval;
}


/**
 * @fn cleanup
 */
void QueuedDatabaseManager::cleanup()
{
    // tasks
    if (keepTasks() > 0) {
        QDateTime time = QDateTime::currentDateTimeUtc().addMSecs(-keepTasks());
        m_database->removeTasks(time);
    }
    // tokens
    m_database->removeTokens();
    // users
    if (keepUsers() > 0) {
        QDateTime time = QDateTime::currentDateTimeUtc().addMSecs(-keepUsers());
        m_database->removeUsers(time);
    }
}
