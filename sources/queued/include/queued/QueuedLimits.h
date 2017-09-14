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
 * @file QueuedLimits.h
 * Header of Queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDLIMITS_H
#define QUEUEDLIMITS_H

#include <QString>


/**
 * @addtogroup QueuedLimits
 * @brief Queued limits
 */
namespace QueuedLimits
{
/**
 * @struct Limits
 * @var Limits::cpu
 * limit by CPU
 * @var Limits::gpu
 * limit by GPU
 * @var Limits::memory
 * limit by memory
 * @var Limits::gpumemory
 * limit by GPU memory
 * @var Limits::storage
 * limit by storage
 * @var Limits::valid
 * is this permissions default generated or not
 */
struct Limits {
    long long cpu;
    long long gpu;
    long long memory;
    long long gpumemory;
    long long storage;
    bool valid;
    // structure methods
    /**
     * @brief limits to string conversion
     * @return string representation of limits
     */
    QString toString() const
    {
        return QString("%1\n%2\n%3\n%4\n%5")
            .arg(cpu)
            .arg(gpu)
            .arg(memory)
            .arg(gpumemory)
            .arg(storage);
    };
    /**
     * @brief default structure constructor
     */
    Limits()
        : cpu(0)
        , gpu(0)
        , memory(0)
        , gpumemory(0)
        , storage(0)
        , valid(false){};
    /**
     * @brief structure constructor from string representation
     * @param _stringLimits
     * limits string representation
     */
    explicit Limits(const QString &_stringLimits)
        : Limits()
    {
        QStringList limits = _stringLimits.split(QChar('\n'));
        while (limits.count() < 5)
            limits.append(QString("0"));

        cpu = limits.at(0).toLongLong();
        gpu = limits.at(1).toLongLong();
        memory = limits.at(2).toLongLong();
        gpumemory = limits.at(3).toLongLong();
        storage = limits.at(4).toLongLong();
        valid = true;
    };
    /**
     * @brief structure constructor from fields
     * @param _cpu
     * limit by CPU cores
     * @param _gpu
     * limit by GPU cores
     * @param _memory
     * limit by memory
     * @param _gpumemory
     * limit by GPU memory
     * @param _storage
     * limit by storage
     */
    Limits(const long long _cpu, const long long _gpu, const long long _memory,
           const long long _gpumemory, const long long _storage)
        : cpu(_cpu)
        , gpu(_gpu)
        , memory(_memory)
        , gpumemory(_gpumemory)
        , storage(_storage)
        , valid(true){};
    /**
     * @brief *= operator overload
     * @param _count
     * multiplier
     * @return multiplied values
     */
    Limits &operator*=(const long long _count)
    {
        cpu *= _count;
        gpu *= _count;
        memory *= _count;
        gpumemory *= _count;

        return *this;
    };
};
/**
 * @brief convert QString memory value to integer
 * @param _value
 * value to convert
 * @param _status
 * conversion status
 * @return converted integer
 */
long long convertMemory(QString _value, bool *_status = nullptr);
/**
 * @brief compare two limits
 * @param _first
 * first limit
 * @param _second
 * second limit
 * @return true if first limit is less than second
 */
bool limitCompare(const long long _first, const long long _second);
/**
 * @brief get minimal limits from given
 * @param _task
 * task defined limits
 * @param _user
 * user defined limit
 * @param _default
 * default limits if anu
 * @return minimal limits from given
 */
Limits minimalLimits(const Limits &_task, const Limits &_user,
                     const Limits &_default);
};


#endif /* QUEUEDLIMITS_H */
