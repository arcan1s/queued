/*
 * Copyright (c) 2017 Evgeniy Alekseev
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
 * @file QueuedLimits.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>


/**
 * @fn convertMemory
 */
long long QueuedLimits::convertMemory(QString _value, bool *_status)
{
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

    return intValue;
}


/**
 * @fn limitCompare
 */
bool QueuedLimits::limitCompare(const long long _first, const long long _second)
{
    if (_first == 0)
        return false;
    else if (_second == 0)
        return true;
    else
        return _first < _second;
}


/**
 * @fn minimalLimits
 */
QueuedLimits::Limits
QueuedLimits::minimalLimits(const QueuedLimits::Limits &_task,
                            const QueuedLimits::Limits &_user,
                            const QueuedLimits::Limits &_default)
{
    QueuedLimits::Limits limits;
    limits.cpu = std::min({_task.cpu, _user.cpu, _default.cpu}, &limitCompare);
    limits.gpu = std::min({_task.gpu, _user.gpu, _default.gpu}, &limitCompare);
    limits.memory = std::min({_task.memory, _user.memory, _default.memory},
                             &limitCompare);
    limits.gpumemory = std::min(
        {_task.gpumemory, _user.gpumemory, _default.gpumemory}, &limitCompare);
    limits.storage = std::min({_task.storage, _user.storage, _default.storage},
                              &limitCompare);

    return limits;
}
