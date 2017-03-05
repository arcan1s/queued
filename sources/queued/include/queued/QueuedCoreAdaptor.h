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
 * @file QueuedCoreAdaptor.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDCOREADAPTOR_H
#define QUEUEDCOREADAPTOR_H

#include <QDBusArgument>
#include <QVariant>


/**
 * @defgroup QueuedCoreAdaptor
 * @brief adaptor to DBus methods
 */
namespace QueuedCoreAdaptor
{
/**
 * @ingroup QueuedCoreAdaptor
 * @brief common DBus request
 * @param _service
 * DBus service name
 * @param _path
 * DBus object path
 * @param _interface
 * DBus interface name
 * @param _cmd
 * command which will be sent to DBus
 * @param _args
 * command arguments
 * @return reply object from DBus request
 */
QVariantList sendRequest(const QString &_service, const QString &_path,
                         const QString &_interface, const QString &_cmd,
                         const QVariantList &_args);
/**
 * @ingroup QueuedCoreAdaptor
 * @brief additional method to avoid conversion from DBus type to native ones
 * @tparam T
 * type to which DBus data should be converted
 * @param _data
 * source data
 * @return converted value
 */
template <class T> T toNativeType(const QVariant &_data)
{
    return qdbus_cast<T>(
        _data.value<QDBusVariant>().variant().value<QDBusArgument>());
};
}


#endif /* QUEUEDCOREADAPTOR_H */
