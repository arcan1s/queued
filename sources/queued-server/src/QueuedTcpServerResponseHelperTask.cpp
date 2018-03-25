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


#include "QueuedTcpServerResponseHelperTask.h"

#include <queued/Queued.h>


QVariantHash QueuedTcpServerResponseHelperTask::addOrEditTask(const long long _id,
                                                              const QVariantHash &_data,
                                                              const QString &_token)
{
    qCDebug(LOG_SERV) << "Add or edit task" << _id << "with data" << _data;

    auto defs = getDefinitions(_data);

    QVariantHash output;
    if (_id > 0) {
        // edit existing task
        auto res = QueuedCoreAdaptor::sendTaskEdit(_id, defs, _token);
        res.match(
            [&output](const bool val) {
                output = {{"code", val ? 200 : 500}};
            },
            [&output](const QueuedError &err) {
                output = {{"code", 500}, {"message", err.message().c_str()}};
            });
    } else {
        // add new task
        auto res = QueuedCoreAdaptor::sendTaskAdd(defs, _token);
        res.match(
            [&output](const long long val) {
                output = {{"code", val ? 200 : 500}, {"id", val}};
            },
            [&output](const QueuedError &err) {
                output = {{"code", 500}, {"message", err.message().c_str()}};
            });
    }

    return output;
}


QueuedProcess::QueuedProcessDefinitions
QueuedTcpServerResponseHelperTask::getDefinitions(const QVariantHash &_data)
{
    qCDebug(LOG_SERV) << "Get definitions from" << _data;

    auto defs = QueuedProcess::QueuedProcessDefinitions();
    auto args = _data["arguments"].toList();
    for (auto &arg : args)
        defs.arguments.append(arg.toString());
    defs.command = _data["command"].toString();
    defs.endTime = QDateTime::fromString(_data["end"].toString(), Qt::ISODateWithMs);
    defs.gid = _data["gid"].toUInt();
    defs.nice = _data["nice"].toUInt();
    defs.startTime = QDateTime::fromString(_data["start"].toString(), Qt::ISODateWithMs);
    defs.uid = _data["uid"].toUInt();
    defs.user = _data["user"].toLongLong();
    defs.workingDirectory = _data["workingDirectory"].toString();
    // limits
    QueuedLimits::Limits limits;
    limits.cpu = _data["limitCpu"].toLongLong();
    limits.gpu = _data["limitGpu"].toLongLong();
    limits.memory = _data["limitMemory"].toLongLong();
    limits.gpumemory = _data["limitGpumemory"].toLongLong();
    limits.storage = _data["limitStorage"].toLongLong();
    defs.limits = limits.toString();

    return defs;
}


QVariantHash QueuedTcpServerResponseHelperTask::getTask(const long long _id,
                                                        const QVariantHash &_data,
                                                        const QString &_token)
{
    qCDebug(LOG_SERV) << "Get task" << _id << _data;

    auto property = _data["property"].toString();

    QVariantHash output = {{"code", 200}};
    if (property.isEmpty()) {
        auto res = QueuedCoreAdaptor::getTask(_id, _token);
        res.match([&output](const QVariantHash &val) { output["properties"] = val; },
                  [&output](const QueuedError &err) {
                      output = {{"code", 500}, {"message", err.message().c_str()}};
                  });
    } else {
        auto res = QueuedCoreAdaptor::getTask(_id, property, _token);
        res.match(
            [&output, &property](const QVariant &val) {
                output["properties"] = {{property, val}};
            },
            [&output](const QueuedError &err) {
                output = {{"code", 500}, {"message", err.message().c_str()}};
            });
    }

    return output;
}


QVariantHash QueuedTcpServerResponseHelperTask::getTasks(const QVariantHash &_data,
                                                         const QString &_token)
{
    qCDebug(LOG_SERV) << "Get tasks" << _data;

    long long userId = _data.value("userId").toLongLong();
    QDateTime start = QDateTime::fromString(_data["start"].toString(), Qt::ISODateWithMs);
    QDateTime stop = QDateTime::fromString(_data["stop"].toString(), Qt::ISODateWithMs);

    QVariantHash output;
    // some conversion magic
    QVariantList outputReport;
    auto res = QueuedCoreAdaptor::getTasks(userId, start, stop, _token);
    res.match(
        [&output, &outputReport](const QList<QVariantHash> &val) {
            for (auto &user : val)
                outputReport += user;
            output = {{"code", 200}, {"report", outputReport}};
        },
        [&output](const QueuedError &err) {
            output = {{"code", 500}, {"message", err.message().c_str()}};
        });

    return output;
}


QVariantHash QueuedTcpServerResponseHelperTask::startOrStopTask(const long long _id,
                                                                const QString &_token)
{
    qCDebug(LOG_SERV) << "Change task state" << _id;

    auto res = QueuedCoreAdaptor::getTask(_id, _token);

    QVariantHash output;
    res.match(
        [&output, &_id, &_token](const QVariantHash &val) {
            if (val["startTime"].toString().isEmpty() || !val["endTime"].toString().isEmpty())
                output = startTask(_id, _token);
            else
                output = stopTask(_id, _token);
        },
        [&output](const QueuedError &err) {
            output = {{"code", 400}, {"message", err.message().c_str()}};
        });

    return output;
}


QVariantHash QueuedTcpServerResponseHelperTask::startTask(const long long _id,
                                                          const QString &_token)
{
    qCDebug(LOG_SERV) << "Start task" << _id;

    auto res = QueuedCoreAdaptor::sendTaskStart(_id, _token);

    QVariantHash output;
    res.match(
        [&output](const bool val) {
            output = {{"code", val ? 200 : 500}};
        },
        [&output](const QueuedError &err) {
            output = {{"code", 500}, {"message", err.message().c_str()}};
        });

    return output;
}


QVariantHash QueuedTcpServerResponseHelperTask::stopTask(const long long _id, const QString &_token)
{
    qCDebug(LOG_SERV) << "Stop task" << _id;

    auto res = QueuedCoreAdaptor::sendTaskStop(_id, _token);

    QVariantHash output;
    res.match(
        [&output](const bool val) {
            output = {{"code", val ? 200 : 500}};
        },
        [&output](const QueuedError &err) {
            output = {{"code", 500}, {"message", err.message().c_str()}};
        });

    return output;
}
