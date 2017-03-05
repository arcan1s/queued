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
/**
 * @file QueuedCoreAdaptor.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include "queued/Queued.h"

#include <QDBusConnection>
#include <QDBusMessage>


/**
 * @fn sendRequest
 */
QVariantList QueuedCoreAdaptor::sendRequest(const QString &_service,
                                            const QString &_path,
                                            const QString &_interface,
                                            const QString &_cmd,
                                            const QVariantList &_args)
{
    qCDebug(LOG_DBUS) << "Send request to service" << _service << "by interface"
                      << _interface << "to" << _path << "command" << _cmd
                      << "with args" << _args;

    QDBusConnection bus = QDBusConnection::systemBus();
    QDBusMessage request
        = QDBusMessage::createMethodCall(_service, _path, _interface, _cmd);
    if (!_args.isEmpty())
        request.setArguments(_args);

    QDBusMessage response = bus.call(request, QDBus::BlockWithGui);
    QVariantList arguments = response.arguments();

    QString error = response.errorMessage();
    if (!error.isEmpty())
        qCWarning(LOG_DBUS) << "Error message" << error;

    return arguments;
}
