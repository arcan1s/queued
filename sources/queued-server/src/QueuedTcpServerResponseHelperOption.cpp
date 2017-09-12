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


#include "QueuedTcpServerResponseHelperOption.h"

#include <queued/Queued.h>


QVariantHash
QueuedTcpServerResponseHelperOption::getOption(const QString &_option)
{
    qCDebug(LOG_SERV) << "Get option" << _option;

    return {{"code", 200}, {"value", QueuedCoreAdaptor::getOption(_option)}};
}


QVariantHash QueuedTcpServerResponseHelperOption::setOption(
    const QString &_option, const QVariantHash &_value, const QString &_token)
{
    qCDebug(LOG_SERV) << "Set option" << _option << "to" << _value;

    if (!_value.contains("value"))
        return {{"code", 400}, {"message", "No required fields found"}};

    return {{"code",
             QueuedCoreAdaptor::sendOptionEdit(_option, _value["value"], _token)
                 ? 200
                 : 400}};
}
