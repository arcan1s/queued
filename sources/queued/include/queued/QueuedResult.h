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
 * @file QueuedResult.h
 * Header of Queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDRESULT_H
#define QUEUEDRESULT_H

#include <QDBusArgument>

#include <result/result.hpp>

#include "QueuedPluginSpecification.h"


namespace QueuedEnums
{
enum class ReturnStatus;
};


/**
 * @typedef QueuedError
 * custom result error implementation
 */
typedef Result::Error<QueuedEnums::ReturnStatus> QueuedError;
/**
 * @brief DBus marshalling method
 * @param _argument
 * output DBus argument
 * @param _arg
 * input error class
 * @return appended argument body
 */
inline QDBusArgument &operator<<(QDBusArgument &_argument, const QueuedError &_arg)
{
    _argument.beginStructure();
    _argument << QString(_arg.message().c_str());
    _argument << static_cast<long long>(_arg.code());
    _argument.endStructure();

    return _argument;
};
/**
 * @brief DBus un-marshalling method
 * @param _argument
 * input DBus argument
 * @param _arg
 * output object
 * @return source DBus argument
 */
inline const QDBusArgument &operator>>(const QDBusArgument &_argument, QueuedError &_arg)
{
    QString message;
    long long code;

    _argument.beginStructure();
    _argument >> message;
    _argument >> code;
    _argument.endStructure();

    _arg = QueuedError(message.toStdString(), static_cast<QueuedEnums::ReturnStatus>(code));
    return _argument;
};
/**
 * @typedef QueuedStatusMap
 * status report representation
 */
typedef QHash<QString, QHash<QString, QString>> QueuedStatusMap;
/**
 * @typedef QueuedResult
 * @tparam T
 * result value payload class
 * custom Result<T, E> implementation
 */
template <class T> using QueuedResult = Result::Result<T, QueuedEnums::ReturnStatus>;
Q_DECLARE_METATYPE(QueuedResult<bool>)
Q_DECLARE_METATYPE(QueuedResult<long long>)
Q_DECLARE_METATYPE(QueuedResult<QString>)
Q_DECLARE_METATYPE(QueuedResult<QStringList>)
Q_DECLARE_METATYPE(QueuedResult<QVariant>)
Q_DECLARE_METATYPE(QueuedResult<QList<QVariantHash>>)
Q_DECLARE_METATYPE(QueuedResult<QVariantHash>)
Q_DECLARE_METATYPE(QueuedResult<QueuedPluginSpecification::Plugin>)
Q_DECLARE_METATYPE(QueuedResult<QueuedPluginSpecification::PluginOption>)
Q_DECLARE_METATYPE(QueuedResult<QueuedStatusMap>)
Q_DECLARE_METATYPE(QueuedPluginSpecification::Plugin)
Q_DECLARE_METATYPE(QueuedPluginSpecification::PluginOption)
/**
 * @brief DBus marshalling method
 * @param _argument
 * output DBus argument
 * @param _arg
 * input variant object
 * @return appended argument body
 */
inline QDBusArgument &operator<<(QDBusArgument &_argument, const QVariant &_arg)
{
    return _argument << QDBusVariant(_arg.isValid() ? _arg : "");
};
/**
 * @brief DBus un-marshalling method
 * @param _argument
 * input DBus argument
 * @param _arg
 * output variant object
 * @return source DBus argument
 */
inline const QDBusArgument &operator>>(const QDBusArgument &_argument, QVariant &_arg)
{
    QDBusVariant variant;

    _argument >> variant;
    _arg = variant.variant();

    return _argument;
};
/**
 * @brief DBus marshalling method
 * @tparam T
 * result value payload class
 * @param _argument
 * output DBus argument
 * @param _arg
 * input result object
 * @return appended argument body
 */
template <class T>
inline QDBusArgument &operator<<(QDBusArgument &_argument, const QueuedResult<T> &_arg)
{
    // HACK we are using explicit cast to QString here to make sure of valid
    // marshalling
    _argument.beginStructure();
    switch (_arg.type()) {
    case Result::Content::Value:
        _argument << QString("v");
        _argument << _arg.get();
        _argument << QueuedError();
        break;
    case Result::Content::Error:
        _argument << QString("e");
        _argument << T();
        _argument << _arg.error();
        break;
    case Result::Content::Empty:
        _argument << QString("n");
        _argument << T();
        _argument << QueuedError();
        break;
    }
    _argument.endStructure();

    return _argument;
};
/**
 * @brief DBus un-marshalling method
 * @tparam T
 * result value payload class
 * @param _argument
 * input DBus argument
 * @param _arg
 * output result object
 * @return source DBus argument
 */
template <class T>
inline const QDBusArgument &operator>>(const QDBusArgument &_argument, QueuedResult<T> &_arg)
{
    QString type;
    T value;
    QueuedError error;

    _argument.beginStructure();
    _argument >> type;
    _argument >> value;
    _argument >> error;
    _argument.endStructure();

    if (type == "v")
        _arg = value;
    else if (type == "e")
        _arg = error;
    else if (type == "n")
        _arg = QueuedResult<T>();
    return _argument;
};


#endif /* QUEUEDRESULT_H */
