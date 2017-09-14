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


#include "QueuedApplication.h"

#include <QDBusConnection>
#include <QDBusMessage>

#include <queued/Queued.h>

#include "QueuedApplicationInterface.h"


QueuedApplication::QueuedApplication(QObject *parent, const QVariantHash &args)
    : QObject(parent)
    , m_configuration(args)
{
    qCDebug(LOG_APP) << __PRETTY_FUNCTION__;

    m_core = new QueuedCore(this);

    init();
    initDBus();
}


QueuedApplication::~QueuedApplication()
{
    qCDebug(LOG_APP) << __PRETTY_FUNCTION__;

    QDBusConnection::sessionBus().unregisterObject(
        QueuedConfig::DBUS_APPLICATION_PATH);
    m_core->deleteLater();
}


void QueuedApplication::init()
{
    m_core->init(m_configuration["config"].toString());
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
