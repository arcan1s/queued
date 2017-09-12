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
        output["token"] = QueuedCoreAdaptor::auth(_data["user"].toString(),
                                                  _data["password"].toString());
        output["code"] = output["token"].toString().isEmpty() ? 401 : 200;
    } else {
        output = {{"code", 400}, {"message", "No required fields found"}};
    }

    return output;
}


bool QueuedTcpServerResponseHelperAuth::tryAuth(const QString &_token)
{
    qCDebug(LOG_APP) << "Try auth with" << _token;

    return QueuedCoreAdaptor::auth(_token);
}
