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
 * @file QueuedPropertyInterface.cpp
 * Source code of queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>

#include <QDBusMetaType>
#include <QMetaProperty>


/**
 * @class QueuedPropertyInterface
 */
/**
 * @fn QueuedPropertyInterface
 */
QueuedPropertyInterface::QueuedPropertyInterface(QueuedCore *parent)
    : QDBusAbstractAdaptor(parent)
    , m_core(parent)
{
    qCDebug(LOG_DBUS) << __PRETTY_FUNCTION__;

    qRegisterMetaType<QueuedResult<QVariant>>("QueuedResult<QVariant>");
    qDBusRegisterMetaType<QueuedResult<QVariant>>();
}


/**
 * @fn ~QueuedPropertyInterface
 */
QueuedPropertyInterface::~QueuedPropertyInterface()
{
    qCDebug(LOG_DBUS) << __PRETTY_FUNCTION__;
}


/**
 * @fn Option
 */
QDBusVariant QueuedPropertyInterface::Option(const QString &property)
{
    qCDebug(LOG_DBUS) << "Get property" << property;

    return QueuedCoreAdaptor::toDBusVariant(m_core->option(property));
}


/**
 * @fn Task
 */
QDBusVariant QueuedPropertyInterface::Task(const long long id,
                                           const QString &property)
{
    qCDebug(LOG_DBUS) << "Get property" << property << "from task" << id;

    auto task = m_core->task(id);
    if (!task) {
        qCWarning(LOG_DBUS) << "Could not find task" << id;
        return QueuedCoreAdaptor::toDBusVariant(QueuedResult<QVariant>(
            QueuedError("Task does not exist",
                        QueuedEnums::ReturnStatus::InvalidArgument)));
    }

    if (property.isEmpty()) {
        auto response = QVariant::fromValue<QVariantHash>(getProperties(task));
        return QueuedCoreAdaptor::toDBusVariant(
            QueuedResult<QVariant>(response));
    } else {
        auto response = task->property(qPrintable(property));
        if (response.type() == QVariant::DateTime)
            response = response.toDateTime().toString(Qt::ISODateWithMs);
        return QueuedCoreAdaptor::toDBusVariant(
            QueuedResult<QVariant>(response));
    }
}


/**
 * @fn User
 */
QDBusVariant QueuedPropertyInterface::User(const long long id,
                                           const QString &property)
{
    qCDebug(LOG_DBUS) << "Get property" << property << "from user" << id;

    auto user = m_core->user(id);
    if (!user) {
        qCWarning(LOG_DBUS) << "Could not find user" << id;
        return QueuedCoreAdaptor::toDBusVariant(QueuedResult<QVariant>(
            QueuedError("User does not exist",
                        QueuedEnums::ReturnStatus::InvalidArgument)));
    }

    if (property.isEmpty()) {
        auto response = QVariant::fromValue<QVariantHash>(getProperties(user));
        return QueuedCoreAdaptor::toDBusVariant(
            QueuedResult<QVariant>(response));
    } else {
        auto response = user->property(qPrintable(property));
        if (response.type() == QVariant::DateTime)
            response = response.toDateTime().toString(Qt::ISODateWithMs);
        return QueuedCoreAdaptor::toDBusVariant(
            QueuedResult<QVariant>(response));
    }
}


/**
 * @fn UserIdByName
 */
QDBusVariant QueuedPropertyInterface::UserIdByName(const QString &name)
{
    qCDebug(LOG_DBUS) << "Look for user ID" << name;

    auto user = m_core->user(name);
    if (!user) {
        qCWarning(LOG_DBUS) << "Could not find user" << name;
        return QueuedCoreAdaptor::toDBusVariant(QueuedResult<long long>(
            QueuedError("User does not exist",
                        QueuedEnums::ReturnStatus::InvalidArgument)));
    }

    return QueuedCoreAdaptor::toDBusVariant(
        QueuedResult<long long>(user->index()));
}


/**
 * @fn getProperties
 */
QVariantHash QueuedPropertyInterface::getProperties(const QObject *_object)
{
    qCDebug(LOG_DBUS) << "Get all properties from" << _object->objectName();

    QVariantHash result;

    auto meta = _object->metaObject();
    int count = meta->propertyCount();
    for (int i = 0; i < count; i++) {
        QMetaProperty prop = meta->property(i);
        auto name = prop.name();
        if (QString(name) == "objectName")
            continue;
        result[name] = _object->property(name);
        if (result[name].type() == QVariant::DateTime)
            result[name]
                = result[name].toDateTime().toString(Qt::ISODateWithMs);
    }

    return result;
}
