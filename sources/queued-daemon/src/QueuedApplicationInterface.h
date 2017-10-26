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


#ifndef QUEUEDAPPLICATIONINTERFACE_H
#define QUEUEDAPPLICATIONINTERFACE_H

#include <QDBusAbstractAdaptor>
#include <QDBusVariant>

#include "QueuedConfig.h"


class QueuedApplication;

class QueuedApplicationInterface : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", DBUS_SERVICE_NAME)

public:
    explicit QueuedApplicationInterface(QueuedApplication *parent);
    virtual ~QueuedApplicationInterface();

public slots:
    QDBusVariant Active() const;
    QDBusVariant UIDs() const;

private:
    QueuedApplication *m_application = nullptr;
};


#endif /* QUEUEDAPPLICATIONINTERFACE_H */
