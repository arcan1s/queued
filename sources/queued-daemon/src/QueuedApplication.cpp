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


#include "QueuedApplication.h"

#include <QDBusConnection>
#include <QDBusMessage>

#include "queued/Queued.h"

#include "QueuedApplicationInterface.h"


QueuedApplication::QueuedApplication(QObject *parent, const QVariantHash &args)
    : QObject(parent)
    , m_configuration(args)
{
    qSetMessagePattern(QueuedDebug::LOG_FORMAT);
    qCDebug(LOG_APP) << __PRETTY_FUNCTION__;
    for (auto &metadata : QueuedDebug::getBuildData())
        qCDebug(LOG_APP) << metadata;

    init();
    initDBus();
}


QueuedApplication::~QueuedApplication()
{
    qCDebug(LOG_APP) << __PRETTY_FUNCTION__;

    QDBusConnection::sessionBus().unregisterObject(
        QueuedConfig::DBUS_APPLICATION_PATH);
    deinit();
}


void QueuedApplication::deinit()
{
    if (m_core)
        delete m_core;
}


void QueuedApplication::init()
{
    deinit();

    initCore();
}


void QueuedApplication::initCore()
{
    m_core = new QueuedCore(this);
    // init objects
    m_core->init(m_configuration[QString("config")].toString());
}


void QueuedApplication::initDBus()
{
    QDBusConnection bus = QDBusConnection::systemBus();

    if (!bus.registerObject(QueuedConfig::DBUS_APPLICATION_PATH,
                            new QueuedApplicationInterface(this),
                            QDBusConnection::ExportAllContents)) {
        QString message = QString("Could not register application object %1")
                              .arg(bus.lastError().message());
        qCCritical(LOG_DBUS) << message;
        throw QueuedDBusException(message);
    }
}
