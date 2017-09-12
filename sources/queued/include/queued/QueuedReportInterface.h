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
 * @file QueuedReportInterface.h
 * Header of Queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDREPORTINTERFACE_H
#define QUEUEDREPORTINTERFACE_H

#include <QDBusAbstractAdaptor>
#include <QDBusVariant>

#include "QueuedConfig.h"


class QueuedCore;

/**
 * @brief DBus interface for QueuedReportManager class
 */
class QueuedReportInterface : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", DBUS_SERVICE_NAME)

public:
    /**
     * @brief QueuedReportInterface class constructor
     * @param parent
     * pointer to QueuedCore object
     */
    explicit QueuedReportInterface(QueuedCore *parent);
    /**
     * @brief QueuedReportInterface class destructor
     */
    virtual ~QueuedReportInterface();

public slots:
    /**
     * @brief performance report
     * @param from
     * start report date
     * @param to
     * stop report date
     * @param token
     * user auth token
     * @return
     */
    QDBusVariant Performance(const QString &from, const QString &to,
                             const QString &token);
    /**
     * @brief server status
     * @return server status
     */
    QDBusVariant Status();
    /**
     * @brief tasks list
     * @param user
     * user ID to select
     * @param from
     * minimal task start time
     * @param to
     * maximal task end time
     * @param token
     * user auth token
     * @return list of tasks match to query
     */
    QDBusVariant Tasks(const qlonglong user, const QString &from,
                       const QString &to, const QString &token);
    /**
     * @brief users list
     * @param lastLogged
     * minimal last logged
     * @param permission
     * permission to search
     * @param token
     * user auth token
     * @return list of users match to query
     */
    QDBusVariant Users(const QString &lastLogged, const uint permission,
                       const QString &token);

private:
    /**
     * @brief pointer to database object
     */
    QueuedCore *m_core = nullptr;
};


#endif /* QUEUEDREPORTINTERFACE_H */
