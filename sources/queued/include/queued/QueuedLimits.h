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
 * @file QueuedLimits.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDLIMITS_H
#define QUEUEDLIMITS_H

#include <QString>


/**
 * @defgroup QueuedLimits
 * @brief Queued limits
 */
namespace QueuedLimits
{
/**
 * @ingroup QueuedLimits
 * @struct Limits
 * @var cpu
 * limit by CPU
 * @var gpu
 * limit by GPU
 * @var memory
 * limit by memory
 * @var gpumemory
 * limit by GPU memory
 * @var storage
 * limit by storage
 * @var valid
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
        return QString("%1\x01%2\x01%3\x01%4\x01%5")
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
    Limits(const QString &_stringLimits)
    {
        QStringList limits = _stringLimits.split(QChar('\x01'));
        while (limits.count() < 4)
            limits.append(QString("0"));

        cpu = limits.at(0).toLongLong();
        gpu = limits.at(1).toLongLong();
        memory = limits.at(2).toLongLong();
        gpumemory = limits.at(3).toLongLong();
        storage = limits.at(4).toLongLong();
        valid = true;
    };
};
/**
 * @ingroup QueuedLimits
 * @brief convert QString memory value to integer
 * @param _value
 * value to convert
 * @param _status
 * conversion status
 * @return converted integer
 */
long long convertMemory(QString _value, bool *_status);
/**
 * @ingroup QueuedLimits
 * @brief compare two limits
 * @param _first
 * first limit
 * @param _second
 * second limit
 * @return true if first limit is less than second
 */
bool limitCompare(const long long _first, const long long _second);
/**
 * @ingroup QueuedLimits
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
