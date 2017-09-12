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


#include "QueuedTcpServerResponseHelperApi1.h"

#include <queued/Queued.h>

#include "QueuedTcpServerResponseHelperAuth.h"
#include "QueuedTcpServerResponseHelperOption.h"
#include "QueuedTcpServerResponseHelperPermissions.h"
#include "QueuedTcpServerResponseHelperPlugins.h"
#include "QueuedTcpServerResponseHelperTask.h"
#include "QueuedTcpServerResponseHelperUser.h"


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
    case QueuedTcpServerResponseHelper::RequestPath::Reports:
        if (_type == "GET")
            output
                = QueuedTcpServerResponseHelperUser::getReport(_data, _token);
        else
            output = {{"code", 405}};
        break;
    case QueuedTcpServerResponseHelper::RequestPath::Status:
        if (_type == "GET")
            output = getStatus();
        else
            output = {{"code", 405}};
        break;
    case QueuedTcpServerResponseHelper::RequestPath::Task:
        if (_type == "GET")
            output = QueuedTcpServerResponseHelperTask::getTask(
                _arg.toLongLong(), _data);
        else if (_type == "POST")
            output = QueuedTcpServerResponseHelperTask::addOrEditTask(
                _arg.toLongLong(), _data, _token);
        else if (_type == "PUT")
            output = QueuedTcpServerResponseHelperTask::startOrStopTask(
                _arg.toLongLong(), _token);
        else
            output = {{"code", 405}};
        break;
    case QueuedTcpServerResponseHelper::RequestPath::Tasks:
        if (_type == "GET")
            output = QueuedTcpServerResponseHelperTask::getTasks(_data, _token);
        else
            output = {{"code", 405}};
        break;
        break;
    case QueuedTcpServerResponseHelper::RequestPath::User:
        if (_type == "GET")
            output = QueuedTcpServerResponseHelperUser::getUser(_arg, _data);
        else if (_type == "POST")
            output = QueuedTcpServerResponseHelperUser::addOrEditUser(
                _arg, _data, _token);
        else
            output = {{"code", 405}};
        break;
    case QueuedTcpServerResponseHelper::RequestPath::Users:
        if (_type == "GET")
            output = QueuedTcpServerResponseHelperUser::getUsers(_data, _token);
        else
            output = {{"code", 405}};
        break;
    case QueuedTcpServerResponseHelper::RequestPath::Unknown:
        output = {{"code", 404}};
        break;
    }

    return output;
}


QVariantHash QueuedTcpServerResponseHelperApi1::getStatus()
{
    QVariantHash output = {{"code", 200}};

    auto data = QueuedCoreAdaptor::getStatus();
    auto sections = data.keys();
    sections.sort();
    for (auto &section : sections) {
        QVariantHash sectionData;
        auto keys = data[section].keys();
        keys.sort();
        for (auto &key : keys)
            sectionData[key] = data[section][key];
        // append output
        output[section] = sectionData;
    }

    return output;
}
