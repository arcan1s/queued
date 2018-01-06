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


#include "QueuedTcpServerResponseHelper.h"

#include <QRegularExpression>

#include <queued/Queued.h>

#include "QueuedTcpServerResponseHelperApi1.h"


QVariantHash QueuedTcpServerResponseHelper::getData(const Request &_request,
                                                    const QVariantHash &_data,
                                                    const QString &_token)
{
    qCDebug(LOG_SERV) << "Get data for request" << static_cast<int>(_request.path)
                      << _request.apiVersion << _request.arg << "with data" << _data;

    QVariantHash output;
    if (_request.apiVersion == 1)
        output = QueuedTcpServerResponseHelperApi1::getData(_request.path, _request.arg,
                                                            _request.type, _data, _token);

    return output;
}


QueuedTcpServerResponseHelper::Request
QueuedTcpServerResponseHelper::parsePath(const QString &_path)
{
    qCDebug(LOG_SERV) << "Parse path" << _path;

    // /api/v1/request/arg or /api/v1/request
    QRegularExpression regex("^\\/api\\/v(?<version>\\d+)\\/"
                             "(?<path>[\\d\\w]*)(\\/(?<arg>[\\d\\w]+))?$");
    regex.setPatternOptions(QRegularExpression::DotMatchesEverythingOption);

    Request request;
    request.valid = false;

    QRegularExpressionMatchIterator it = regex.globalMatch(_path);
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();

        request.apiVersion = match.captured("version").toUInt();
        request.arg = match.captured("arg");
        request.path = pathToEnum(match.captured("path"));

        // check if request is valid
        request.valid = (request.path != RequestPath::Unknown)
                        && (std::find(std::begin(QueuedConfig::WEBAPI_VERSIONS),
                                      std::end(QueuedConfig::WEBAPI_VERSIONS), request.apiVersion)
                            != std::end(QueuedConfig::WEBAPI_VERSIONS));
    }

    return request;
}


QueuedTcpServerResponseHelper::RequestPath
QueuedTcpServerResponseHelper::pathToEnum(const QString &_path)
{
    qCDebug(LOG_SERV) << "Convert path" << _path;

    if (_path == "auth")
        return RequestPath::Auth;
    else if (_path == "option")
        return RequestPath::Option;
    else if (_path == "permissions")
        return RequestPath::Permissions;
    else if (_path == "plugin")
        return RequestPath::Plugin;
    else if (_path == "plugins")
        return RequestPath::Plugins;
    else if (_path == "reports")
        return RequestPath::Reports;
    else if (_path == "status")
        return RequestPath::Status;
    else if (_path == "task")
        return RequestPath::Task;
    else if (_path == "tasks")
        return RequestPath::Tasks;
    else if (_path == "user")
        return RequestPath::User;
    else if (_path == "users")
        return RequestPath::Users;

    return RequestPath::Unknown;
}
