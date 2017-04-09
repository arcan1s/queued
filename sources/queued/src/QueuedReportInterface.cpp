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
 * @file QueuedReportInterface.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>

#include <QDBusMetaType>


/**
 * @class QueuedReportInterface
 */
/**
 * @fn QueuedReportInterface
 */
QueuedReportInterface::QueuedReportInterface(QueuedCore *parent)
    : QDBusAbstractAdaptor(parent)
    , m_core(parent)
{
    qCDebug(LOG_DBUS) << __PRETTY_FUNCTION__;

    // QList<QVariantHash>
    qRegisterMetaType<QList<QVariantHash>>("QList<QVariantHash>");
    qDBusRegisterMetaType<QList<QVariantHash>>();
    // QHash<QString, QString>
    qRegisterMetaType<QHash<QString, QString>>("QHash<QString, QString>");
    qDBusRegisterMetaType<QHash<QString, QString>>();
    // QHash<QString, QHash<QString, QString>>
    qRegisterMetaType<QHash<QString, QHash<QString, QString>>>(
        "QHash<QString, QHash<QString, QString>>");
    qDBusRegisterMetaType<QHash<QString, QHash<QString, QString>>>();
}


/**
 * @fn ~QueuedReportInterface
 */
QueuedReportInterface::~QueuedReportInterface()
{
    qCDebug(LOG_DBUS) << __PRETTY_FUNCTION__;
}


/**
 * @fn Performance
 */
QDBusVariant QueuedReportInterface::Performance(const QString &from,
                                                const QString &to,
                                                const QString &token)
{
    qCDebug(LOG_DBUS) << "Performance report for" << from << to;

    return QDBusVariant(
        QVariant::fromValue<QList<QVariantHash>>(m_core->performanceReport(
            QDateTime::fromString(from, Qt::ISODateWithMs),
            QDateTime::fromString(to, Qt::ISODateWithMs), token)));
}


/**
 * @fn Status
 */
QDBusVariant QueuedReportInterface::Status()
{
    auto metadata = QueuedDebug::getBuildMetaData();
    // append metadata here

    return QDBusVariant(
        QVariant::fromValue<QHash<QString, QHash<QString, QString>>>(metadata));
}


/**
 * @fn Tasks
 */
QDBusVariant QueuedReportInterface::Tasks(const qlonglong user,
                                          const QString &from,
                                          const QString &to,
                                          const QString &token)
{
    qCDebug(LOG_DBUS) << "Search for tasks" << user << from << to;

    return QDBusVariant(QVariant::fromValue<QList<QVariantHash>>(
        m_core->taskReport(user, QDateTime::fromString(from, Qt::ISODateWithMs),
                           QDateTime::fromString(to, Qt::ISODateWithMs),
                           token)));
}


/**
 * @fn Users
 */
QDBusVariant QueuedReportInterface::Users(const QString &lastLogged,
                                          const uint permission,
                                          const QString &token)
{
    qCDebug(LOG_DBUS) << "Search for users" << lastLogged << permission;

    return QDBusVariant(QVariant::fromValue<QList<QVariantHash>>(
        m_core->userReport(QDateTime::fromString(lastLogged, Qt::ISODateWithMs),
                           permission < 1 ? QueuedEnums::Permission::Invalid
                                          : QueuedEnums::Permission(permission),
                           token)));
}
