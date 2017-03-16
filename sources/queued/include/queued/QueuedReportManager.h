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
 * @file QueuedReportManager.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDREPORTMANAGER_H
#define QUEUEDREPORTMANAGER_H

#include <QDateTime>
#include <QObject>

#include <queued/QueuedEnums.h>


class QueuedCore;
class QueuedDatabase;

/**
 * @brief report manager for queued
 */
class QueuedReportManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief QueuedReportManager class constructor
     * @param parent
     * pointer to parent item
     * @param database
     * pointer to database object
     */
    explicit QueuedReportManager(QObject *parent, QueuedDatabase *database);
    /**
     * @brief QueuedReportManager class destructor
     */
    virtual ~QueuedReportManager();
    /**
     * @brief usage report
     * @param _from
     * start report date
     * @param _to
     * stop report date
     * @param _core
     * pointer to core object
     * @return performance table
     */
    QList<QVariantHash> performance(const QueuedCore *_core,
                                    const QDateTime &_from = QDateTime(),
                                    const QDateTime &_to = QDateTime()) const;
    /**
     * list of tasks which match criteria
     * @param _user
     * task user ID filter
     * @param _from
     * minimal start time
     * @param _to
     * maximal end time
     * @return list of tasks in database format
     */
    QList<QVariantHash> tasks(const long long _user = -1,
                              const QDateTime &_from = QDateTime(),
                              const QDateTime &_to = QDateTime()) const;
    /**
     * list of users which match criteria
     * @param _lastLogged
     * last logged minimal date
     * @param _permission
     * user permission filter
     * @return list of users in database format
     */
    QList<QVariantHash> users(const QDateTime &_lastLogged = QDateTime(),
                              const QueuedEnums::Permission _permission
                              = QueuedEnums::Permission::Invalid) const;

private:
    /**
     * @brief pointer to database object
     */
    QueuedDatabase *m_database = nullptr;
};


#endif /* QUEUEDREPORTMANAGER_H */
