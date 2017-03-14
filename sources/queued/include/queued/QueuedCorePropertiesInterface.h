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
 * @file QueuedCorePropertiesInterface.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDCOREPROPERTIESINTERFACE_H
#define QUEUEDCOREPROPERTIESINTERFACE_H

#include <QDBusAbstractAdaptor>
#include <QDBusVariant>

#include "QueuedConfig.h"


class QueuedCore;

/**
 * @brief DBus interface for QueuedCore class
 */
class QueuedCorePropertiesInterface : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", DBUS_SERVICE_NAME)

public:
    /**
     * @brief QueuedCorePropertiesInterface class constructor
     * @param parent
     * pointer to QueuedCore object
     */
    explicit QueuedCorePropertiesInterface(QueuedCore *parent);
    /**
     * @brief QueuedCorePropertiesInterface class destructor
     */
    virtual ~QueuedCorePropertiesInterface();

public slots:
    /**
     * @brief get advanced option
     * @param property
     * property name
     * @return property value or empty if property not found
     */
    QDBusVariant Option(const QString &property);
    /**
     * @brief get task property
     * @param id
     * task ID
     * @param property
     * property name
     * @return property value or empty if task or property not found
     */
    QDBusVariant Task(const long long id, const QString &property);
    /**
     * @brief get user property
     * @param id
     * user ID
     * @param property
     * property name
     * @return property value or empty if user or property not found
     */
    QDBusVariant User(const long long id, const QString &property);

private:
    /**
     * @brief pointer to database object
     */
    QueuedCore *m_core = nullptr;
};


#endif /* QUEUEDCOREPROPERTIESINTERFACE_H */
