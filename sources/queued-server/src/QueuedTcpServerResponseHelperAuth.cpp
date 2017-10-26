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


#include "QueuedTcpServerResponseHelperAuth.h"

#include <queued/Queued.h>


QVariantHash QueuedTcpServerResponseHelperAuth::auth(const QVariantHash &_data)
{
    qCDebug(LOG_SERV) << "Auth with data" << _data;

    QVariantHash output;
    if (_data.contains("user") && _data.contains("password")) {
        auto res = QueuedCoreAdaptor::auth(_data["user"].toString(),
                                           _data["password"].toString());

        Result::match(
            res,
            [&output](const QString &val) {
                output = {{"code", 200}, {"token", val}};
            },
            [&output](const QueuedError &err) {
                output = {{"code", 401}, {"message", err.message().c_str()}};
            });
    } else {
        output = {{"code", 400}, {"message", "No required fields found"}};
    }

    return output;
}


bool QueuedTcpServerResponseHelperAuth::tryAuth(const QString &_token)
{
    qCDebug(LOG_APP) << "Try auth with" << _token;

    auto res = QueuedCoreAdaptor::auth(_token);

    bool ret = true;
    Result::match(res, [&ret](const bool val) { ret = val; },
                  [&ret](const QueuedError &err) { ret = false; });

    return ret;
}
