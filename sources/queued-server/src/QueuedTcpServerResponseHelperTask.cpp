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


#include "QueuedTcpServerResponseHelperTask.h"

#include <queued/Queued.h>


QVariantHash QueuedTcpServerResponseHelperTask::addOrEditTask(
    const long long _id, const QVariantHash &_data, const QString &_token)
{
    qCDebug(LOG_SERV) << "Add or edit task" << _id << "with data" << _data;

    auto defs = getDefinitions(_data);
    if (_id > 0) {
        // edit existing task
        bool status = QueuedCoreAdaptor::sendTaskEdit(_id, defs, _token);
        return {{"code", status ? 200 : 400}};
    } else {
        // add new task
        auto id = QueuedCoreAdaptor::sendTaskAdd(defs, _token);
        return {{"code", id > 0 ? 200 : 400}, {"id", id}};
    }
}


QueuedProcess::QueuedProcessDefinitions
QueuedTcpServerResponseHelperTask::getDefinitions(const QVariantHash &_data)
{
    qCDebug(LOG_SERV) << "Get definitions from" << _data;

    QueuedProcess::QueuedProcessDefinitions defs;
    auto args = _data["arguments"].toList();
    for (auto &arg : args)
        defs.arguments.append(arg.toString());
    defs.command = _data["command"].toString();
    defs.endTime
        = QDateTime::fromString(_data["end"].toString(), Qt::ISODateWithMs);
    defs.gid = _data["gid"].toUInt();
    defs.nice = _data["nice"].toUInt();
    defs.startTime
        = QDateTime::fromString(_data["start"].toString(), Qt::ISODateWithMs);
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


QVariantHash
QueuedTcpServerResponseHelperTask::getTask(const long long _id,
                                           const QVariantHash &_data)
{
    qCDebug(LOG_SERV) << "Get task" << _id << _data;

    auto property = _data["property"].toString();

    QVariantHash output = {{"code", 200}};
    // some conversion magic
    auto value = QueuedCoreAdaptor::getTask(_id, property);
    if (property.isEmpty())
        output["properties"] = qdbus_cast<QVariantHash>(value);
    else
        output["properties"] = QVariantHash({{property, value}});

    return output;
}


QVariantHash
QueuedTcpServerResponseHelperTask::getTasks(const QVariantHash &_data,
                                            const QString &_token)
{
    qCDebug(LOG_SERV) << "Get tasks" << _data;

    long long user = _data.value("userId").toLongLong();
    QDateTime start
        = QDateTime::fromString(_data["start"].toString(), Qt::ISODateWithMs);
    QDateTime stop
        = QDateTime::fromString(_data["stop"].toString(), Qt::ISODateWithMs);

    QVariantHash output = {{"code", 200}};
    // some conversion magic
    QVariantList outputReport;
    auto report = QueuedCoreAdaptor::getTasks(user, start, stop, _token);
    for (auto &user : report)
        outputReport.append(user);
    output["report"] = outputReport;

    return output;
}
