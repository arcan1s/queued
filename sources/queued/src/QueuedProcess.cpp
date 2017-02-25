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
 * @file QueuedProcess.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include "queued/Queued.h"

#include <unistd.h>
#include <queued/Queued.h>


/**
 * @class QueuedProcess
 */
/**
 * @fn QueuedProcess
 */
QueuedProcess::QueuedProcess(QObject *parent,
                             const QueuedProcessDefinitions definitions,
                             const long long index)
    : QProcess(parent)
    , m_definitions(definitions)
    , m_index(index)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    setArguments(m_definitions.arguments);
    setProgram(m_definitions.command);
    setWorkingDirectory(m_definitions.workingDirectory);
}


/**
 * @fn ~QueuedProcess
 */
QueuedProcess::~QueuedProcess()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;
}


/**
 * @fn index
 */
long long QueuedProcess::index() const
{
    return m_index;
}


/**
 * @fn name
 */
QString QueuedProcess::name() const
{
    return QString("queued-%1").arg(index());
}


/**
 * @fn removeLimit
 */
void QueuedProcess::removeLimit(const QueuedEnums::LimitType _limitType)
{
    qCDebug(LOG_LIB) << "Remove limit" << static_cast<int>(_limitType);

    m_limits.remove(_limitType);
}


/**
 * @fn setLimit
 */
void QueuedProcess::setLimit(const QueuedEnums::LimitType _limitType,
                             const QVariant &_value)
{
    qCDebug(LOG_LIB) << "Set limit" << static_cast<int>(_limitType) << "to"
                     << _value;

    if (!_value.isValid())
        return removeLimit(_limitType);

    bool status = false;
    long long intValue = _value.type() == QVariant::String
                             ? convertMemory(_value.toString(), &status)
                             : _value.toLongLong(&status);

    if (!status)
        removeLimit(_limitType);
    else
        m_limits[_limitType] = intValue;
}


/**
 * @fn operator==
 */
bool QueuedProcess::operator==(const QueuedProcess &_other)
{
    return name() == _other.name();
}


/**
 * @fn setupChildProcess
 */
void QueuedProcess::setupChildProcess()
{
    ::setgid(m_definitions.gid);
    ::setuid(m_definitions.uid);
}


/**
 * @fn run
 */
void QueuedProcess::run()
{
    m_definitions.startTime = QDateTime::currentDateTimeUtc();
    return start();
}


/**
 * @fn setEndTime
 */
 void QueuedProcess::setEndTime(const QDateTime &_time)
{
    qCDebug(LOG_LIB) << "Set end time to" << _time;

    m_definitions.endTime = _time;
}


/**
 * @fn convertMemory
 */
long long QueuedProcess::convertMemory(QString _value, bool *_status) const
{
    qCDebug(LOG_LIB) << "Convert memory value" << _value;

    long long intValue;
    if (_value.endsWith(QString("K")))
        intValue = _value.remove(QString("K")).toLongLong(_status) * 1024;
    else if (_value.endsWith(QString("M")))
        intValue
            = _value.remove(QString("M")).toLongLong(_status) * 1024 * 1024;
    else if (_value.endsWith(QString("G")))
        intValue = _value.remove(QString("G")).toLongLong(_status) * 1024 * 1024
                   * 1024;
    else
        intValue = _value.toInt(_status);

    qCInfo(LOG_LIB) << "Converted value" << intValue;
    return intValue;
}
