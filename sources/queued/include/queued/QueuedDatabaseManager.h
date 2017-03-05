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
 * @file QueuedDatabaseManager.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDDATABASEMANAGER_H
#define QUEUEDDATABASEMANAGER_H

#include <QObject>
#include <QTimer>


class QueuedDatabase;

/**
 * @brief additional methods over database manager
 */
class QueuedDatabaseManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(long long interval READ interval WRITE setInterval)
    Q_PROPERTY(long long keepTasks READ keepTasks WRITE setKeepTasks)
    Q_PROPERTY(long long keepUsers READ keepUsers WRITE setKeepUsers)

public:
    /**
     * @brief additional constant to store msecs in day count
     */
    const long long MSEC_IN_DAY = 86400000;

    /**
     * @brief QueuedDatabaseManager class constructor
     * @param parent
     * pointer to parent item
     * @param database
     * pointer to database object
     */
    explicit QueuedDatabaseManager(QObject *parent, QueuedDatabase *database);
    /**
     * @brief QueuedDatabaseManager class destructor
     */
    virtual ~QueuedDatabaseManager();
    /**
     * @brief start timer
     */
    void startWorker();
    // properties
    /**
     * @breef database actions interval
     * @return interval in milliseconds
     */
    long long interval() const;
    /**
     * @brief keep ended tasks
     * @return interval for keeping tasks in milliseconds
     */
    long long keepTasks() const;
    /**
     * @brief keep users last logged in
     * @return interval for keeping users in milliseconds
     */
    long long keepUsers() const;
    /**
     * @brief set database actions interval
     * @param _interval
     * interval in milliseconds
     */
    void setInterval(const long long _interval);
    /**
     * @brief keep interval for tasks
     * @param _keepInterval
     * interval in milliseconds
     */
    void setKeepTasks(const long long _keepInterval);
    /**
     * @brief keep interval for users
     * @param _keepInterval
     * interval in milliseconds
     */
    void setKeepUsers(const long long _keepInterval);

public slots:
    /**
     * @brief cleanup database
     */
    void cleanup();

private:
    /**
     * @brief pointer to database object
     */
    QueuedDatabase *m_database = nullptr;
    /**
     * @brief database actions interval
     */
    long long m_interval = MSEC_IN_DAY;
    /**
     * @brief ended task interval
     */
    long long m_keepTasks = 0;
    /**
     * @brief user last logged in interval
     */
    long long m_keepUsers = 0;
    /**
     * @brief timer object
     */
    QTimer m_timer;
};


#endif /* QUEUEDDATABASEMANAGER_H */
