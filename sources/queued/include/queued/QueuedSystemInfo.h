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
 * @file QueuedSystemInfo.h
 * Header of Queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDSYSTEMINFO_H
#define QUEUEDSYSTEMINFO_H


/**
 * @addtogroup QueuedSystemInfo
 * @brief system information methods
 */
namespace QueuedSystemInfo
{
/**
 * @brief system CPU count
 * @return system CPU count
 */
long long cpuCount();
/**
 * @brief weight of specified CPU count
 * @param _cpu
 * CPU count to get weight
 * @return weight as proportion
 */
double cpuWeight(const long long _cpu);
/**
 * @brief system memory count
 * @return system memory count in bytes
 */
long long memoryCount();
/**
 * @brief weight of specified memory count
 * @param _memory
 * memory in bytes to get weight
 * @return weight as proportion
 */
double memoryWeight(const long long _memory);
}


#endif /* QUEUEDSYSTEMINFO_H */
