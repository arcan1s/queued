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


#include "QueuedTcpServerResponseHelperApi1.h"

#include <queued/Queued.h>

#include "QueuedTcpServerResponseHelperAuth.h"
#include "QueuedTcpServerResponseHelperOption.h"
#include "QueuedTcpServerResponseHelperPermissions.h"
#include "QueuedTcpServerResponseHelperPlugins.h"


QVariantHash QueuedTcpServerResponseHelperApi1::getData(
    const QueuedTcpServerResponseHelper::RequestPath _request,
    const QString &_arg, const QString &_type, const QVariantHash &_data,
    const QString &_token)
{
    qCDebug(LOG_SERV) << "Get data for" << static_cast<int>(_request)
                      << "with arg" << _arg << "with data" << _data;

    QVariantHash output;
    if ((_request != QueuedTcpServerResponseHelper::RequestPath::Auth)
        && !QueuedTcpServerResponseHelperAuth::tryAuth(_token)) {
        output = {{"code", 401}};
        return output;
    }

    switch (_request) {
    case QueuedTcpServerResponseHelper::RequestPath::Auth:
        if (_type == "POST")
            output = QueuedTcpServerResponseHelperAuth::auth(_data);
        else
            output = {{"code", 405}};
        break;
    case QueuedTcpServerResponseHelper::RequestPath::Option:
        if (_type == "GET")
            output = QueuedTcpServerResponseHelperOption::getOption(_arg);
        else if (_type == "POST")
            output = QueuedTcpServerResponseHelperOption::setOption(_arg, _data,
                                                                    _token);
        else
            output = {{"code", 405}};
        break;
    case QueuedTcpServerResponseHelper::RequestPath::Permissions:
        if (_type == "DELETE")
            output = QueuedTcpServerResponseHelperPermissions::removePermission(
                _arg.toLongLong(), _data, _token);
        else if (_type == "POST")
            output = QueuedTcpServerResponseHelperPermissions::addPermission(
                _arg.toLongLong(), _data, _token);
        else
            output = {{"code", 405}};
        break;
    case QueuedTcpServerResponseHelper::RequestPath::Plugins:
        if (_type == "DELETE")
            output = QueuedTcpServerResponseHelperPlugins::removePlugin(_arg,
                                                                        _token);
        else if (_type == "GET")
            output = QueuedTcpServerResponseHelperPlugins::listPlugins();
        else if (_type == "POST")
            output
                = QueuedTcpServerResponseHelperPlugins::addPlugin(_arg, _token);
        else
            output = {{"code", 405}};
        break;
    }

    return output;
}
