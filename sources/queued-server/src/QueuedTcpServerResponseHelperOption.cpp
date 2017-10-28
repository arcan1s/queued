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

    auto res = QueuedCoreAdaptor::getOption(_option);

    QVariantHash output;
    res.match(
        [&output](const QVariant &val) {
            output = {{"code", 200}, {"token", val}};
        },
        [&output](const QueuedError &) {
            output = {{"code", 404}, {"message", "Option not found"}};
        });

    return output;
}


QVariantHash QueuedTcpServerResponseHelperOption::setOption(
    const QString &_option, const QVariantHash &_value, const QString &_token)
{
    qCDebug(LOG_SERV) << "Set option" << _option << "to" << _value;

    if (!_value.contains("value"))
        return {{"code", 400}, {"message", "No required fields found"}};

    auto res
        = QueuedCoreAdaptor::sendOptionEdit(_option, _value["value"], _token);

    QVariantHash output;
    res.match(
        [&output](const QVariant &) {
            output = {{"code", 200}};
        },
        [&output](const QueuedError &err) {
            output = {{"code", 500}, {"message", err.message().c_str()}};
        });

    return output;
}
