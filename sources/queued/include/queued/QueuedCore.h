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
 * @file QueuedCore.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDCORE_H
#define QUEUEDCORE_H

#include <QObject>

#include "QueuedLimits.h"


class QueuedAdvancedSettings;
class QueuedDatabase;
class QueuedProcessManager;
class QueuedReportManager;
class QueuedSettings;
class QueuedTokenManager;
class QueuedUserManager;

/**
 * @brief aggregator of queued classes
 */
class QueuedCore : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief QueuedCore class constructor
     * @param parent         pointer to parent item
     */
    explicit QueuedCore(QObject *parent);
    /**
     * @brief QueuedCore class destructor
     */
    virtual ~QueuedCore();
    /**
     * @brief add new task
     * @param _command       command line
     * @param _arguments     command arguments
     * @param _workingDirectory working directory
     * @param _nice          nice level
     * @param _userId        task owner user ID
     * @param _limits        task defined limits
     * @return true on successfully addition
     */
    bool addTask(const QString &_command, const QStringList &_arguments,
                 const QString &_workingDirectory, const unsigned int _nice,
                 const long long _userId, const QueuedLimits::Limits &_limits);
    /**
     * @brief add new user
     * @param _name          user name
     * @param _email         user email
     * @param _password      user password
     * @param _permissions   user permissions
     * @param _limits        user limits
     * @return true on successfully addition
     */
    bool addUser(const QString &_name, const QString &_email,
                 const QString &_password, const unsigned int _permissions,
                 const QueuedLimits::Limits &_limits);
    /**
     * @brief deinit subclasses
     */
    void deinit();
    /**
     * @brief edit user
     * @param _id            user ID to edit
     * @param _userData      user data to edit
     * @remark ref
     * @return
     */
    bool editUser(const long long _id, const QVariantHash &_userData);
    /**
     * @brief init subclasses
     * @param _configuration path to configuration file
     */
    void init(const QString &_configuration);

private slots:
    /**
     * @brief update process time
     * @param _id            task id
     * @param _startTime     task start time or empty
     * @param _endTime       task end time or empty
     */
    void updateTaskTime(const long long _id, const QDateTime &_startTime,
                        const QDateTime &_endTime);
    /**
     * @brief update user login time
     * @param _id            user ID
     * @param _time          user login time
     */
    void updateUserLoginTime(const long long _id, const QDateTime &_time);

private:
    /**
     * @brief pointer to advanced settings object
     */
    QueuedAdvancedSettings *m_advancedSettings = nullptr;
    /**
     * @brief pointer to database object
     */
    QueuedDatabase *m_database = nullptr;
    /**
     * @brief pointer to process manager
     */
    QueuedProcessManager *m_processes = nullptr;
    /**
     * @brief pointer to report manager
     */
    QueuedReportManager *m_reports = nullptr;
    /**
     * @brief pointer to settings object
     */
    QueuedSettings *m_settings = nullptr;
    /**
     * @brief pointer to user manager
     */
    QueuedUserManager *m_users = nullptr;
    // additional properties
    /**
     * @brief connection list
     */
    QList<QMetaObject::Connection> m_connections;
};


#endif /* QUEUEDCORE_H */
