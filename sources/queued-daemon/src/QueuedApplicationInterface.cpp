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


#include "QueuedApplicationInterface.h"

#include <QCoreApplication>
#include <QDBusMetaType>

#include <queued/Queued.h>

#include "QueuedApplication.h"

extern "C" {
#include <unistd.h>
}


QueuedApplicationInterface::QueuedApplicationInterface(QueuedApplication *parent)
    : QDBusAbstractAdaptor(parent)
    , m_application(parent)
{
    qCDebug(LOG_DBUS) << __PRETTY_FUNCTION__;

    qRegisterMetaType<QueuedResult<QStringList>>("QueuedResult<QStringList>");
    qDBusRegisterMetaType<QueuedResult<QStringList>>();
}


QueuedApplicationInterface::~QueuedApplicationInterface()
{
    qCDebug(LOG_DBUS) << __PRETTY_FUNCTION__;
}


QDBusVariant QueuedApplicationInterface::Active() const
{
    return QueuedCoreAdaptor::toDBusVariant(QueuedResult<bool>(true));
}


QDBusVariant QueuedApplicationInterface::UIDs() const
{
    QStringList uids;
    uids.append(QString::number(::getuid()));
    uids.append(QString::number(::geteuid()));

    return QueuedCoreAdaptor::toDBusVariant(QueuedResult<QStringList>(uids));
}
