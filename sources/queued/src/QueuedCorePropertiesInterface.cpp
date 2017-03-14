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
 * @file QueuedCorePropertiesInterface.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include "queued/Queued.h"


/**
 * @class QueuedCorePropertiesInterface
 */
/**
 * @fn QueuedCorePropertiesInterface
 */
QueuedCorePropertiesInterface::QueuedCorePropertiesInterface(QueuedCore *parent)
    : QDBusAbstractAdaptor(parent)
    , m_core(parent)
{
    qCDebug(LOG_DBUS) << __PRETTY_FUNCTION__;
}


/**
 * @fn ~QueuedCorePropertiesInterface
 */
QueuedCorePropertiesInterface::~QueuedCorePropertiesInterface()
{
    qCDebug(LOG_DBUS) << __PRETTY_FUNCTION__;
}


/**
 * @fn Option
 */
QDBusVariant QueuedCorePropertiesInterface::Option(const QString &property)
{
    qCDebug(LOG_DBUS) << "Get property" << property;

    return QDBusVariant(m_core->option(property));
}


/**
 * @fn Task
 */
QDBusVariant QueuedCorePropertiesInterface::Task(const long long id,
                                                 const QString &property)
{
    qCDebug(LOG_DBUS) << "Get property" << property << "from task" << id;

    auto task = m_core->task(id);
    if (!task) {
        qCWarning(LOG_DBUS) << "Could not find task" << id;
        return QDBusVariant();
    }

    return QDBusVariant(task->property(property.toLocal8Bit().constData()));
}


/**
 * @fn User
 */
QDBusVariant QueuedCorePropertiesInterface::User(const long long id,
                                                 const QString &property)
{
    qCDebug(LOG_DBUS) << "Get property" << property << "from user" << id;

    auto user = m_core->user(id);
    if (!user) {
        qCWarning(LOG_DBUS) << "Could not find user" << id;
        return QDBusVariant();
    }

    return QDBusVariant(user->property(property.toLocal8Bit().constData()));
}
