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
 * @file QueuedSystemInfo.cpp
 * Source code of queued library
 * @author Queued team
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>

#include <QThread>

extern "C" {
#include <sys/sysinfo.h>
}


/**
 * @fn cpuCount
 */
long long QueuedSystemInfo::cpuCount()
{
    return QThread::idealThreadCount();
}


/**
 * @fn cpuWeight
 */
double QueuedSystemInfo::cpuWeight(const long long _cpu)
{
    qCDebug(LOG_LIB) << "Get CPU weight for" << _cpu;

    if ((_cpu > 0) && (_cpu < cpuCount()))
        return static_cast<double>(_cpu) / static_cast<double>(cpuCount());
    else
        return 1.0;
}


/**
 * @fn memoryCount
 */
long long QueuedSystemInfo::memoryCount()
{
    struct sysinfo info;
    if (::sysinfo(&info) != 0)
        throw QueuedConfigurationException("Count not get memory information");

    return info.totalram;
}


/**
 * @fn memoryWeight
 */
double QueuedSystemInfo::memoryWeight(const long long _memory)
{
    qCDebug(LOG_LIB) << "Get memory weight for" << _memory;

    if ((_memory > 0) && (_memory < memoryCount()))
        return static_cast<double>(_memory)
               / static_cast<double>(memoryCount());
    else
        return 1.0;
}
