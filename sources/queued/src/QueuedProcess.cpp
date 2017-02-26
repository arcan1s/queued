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

#include <queued/Queued.h>

extern "C" {
#include <unistd.h>
}


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
