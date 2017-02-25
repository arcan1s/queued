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
     * @brief deinit subclasses
     */
    void deinit();
    /**
     * @brief init subclasses
     * @param _configuration path to configuration file
     */
    void init(const QString &_configuration);

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
};


#endif /* QUEUEDCORE_H */
