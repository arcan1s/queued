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


#include "QueuedTcpServerResponseHelperUser.h"

#include <queued/Queued.h>
#include <queued/QueuedUser.h>


QVariantHash QueuedTcpServerResponseHelperUser::addOrEditUser(
    const QString &_user, const QVariantHash &_data, const QString &_token)
{
    qCDebug(LOG_SERV) << "Add user" << _user << "with data" << _data;

    // try define if user exists first
    auto userIdRes = QueuedCoreAdaptor::getUserId(_user);
    long long userId = -1;
    userIdRes.match([&userId](const long long val) { userId = val; },
                    [&userId](const QueuedError &) {});

    auto defs = getDefinitions(_data);
    defs.name = _user;

    QVariantHash output;
    if (userId > 0) {
        // edit existing user
        auto res = QueuedCoreAdaptor::sendUserEdit(userId, defs, _token);
        res.match(
            [&output](const bool val) {
                output = {{"code", val ? 200 : 500}};
            },
            [&output](const QueuedError &err) {
                output = {{"code", 500}, {"message", err.message().c_str()}};
            });
    } else {
        // add new user
        auto res = QueuedCoreAdaptor::sendUserAdd(defs, _token);
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


QueuedUser::QueuedUserDefinitions
QueuedTcpServerResponseHelperUser::getDefinitions(const QVariantHash &_data)
{
    qCDebug(LOG_SERV) << "Generate definitions from" << _data;

    QueuedUser::QueuedUserDefinitions defs;
    defs.email = _data["email"].toString();
    auto res
        = QueuedCoreAdaptor::sendPasswordHash(_data["password"].toString());
    res.match([&defs](const QString &val) { defs.password = val; },
              [](const QueuedError &) {});
    defs.permissions = _data["permissions"].toUInt();
    defs.priority = _data["priority"].toUInt();
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
QueuedTcpServerResponseHelperUser::getReport(const QVariantHash &_data,
                                             const QString &_token)
{
    qCDebug(LOG_SERV) << "Get report using payload" << _data;

    QDateTime stop
        = QDateTime::fromString(_data["stop"].toString(), Qt::ISODateWithMs);
    QDateTime start
        = QDateTime::fromString(_data["start"].toString(), Qt::ISODateWithMs);

    QVariantHash output = {{"code", 200}};
    // some conversion magic
    QVariantList outputReport;
    auto res = QueuedCoreAdaptor::getPerformance(start, stop, _token);
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


QVariantHash
QueuedTcpServerResponseHelperUser::getUser(const QString &_user,
                                           const QVariantHash &_data)
{
    qCDebug(LOG_SERV) << "Get user data for" << _user << _data;

    auto userIdRes = QueuedCoreAdaptor::getUserId(_user);
    long long userId = -1;
    userIdRes.match([&userId](const long long val) { userId = val; },
                    [](const QueuedError &) {});
    if (userId == -1)
        return {{"code", 500}};

    auto property = _data["property"].toString();

    QVariantHash output = {{"code", 200}};
    if (property.isEmpty()) {
        auto res = QueuedCoreAdaptor::getUser(userId);
        res.match(
            [&output](const QVariantHash &val) { output["properties"] = val; },
            [&output](const QueuedError &err) {
                output = {{"code", 500}, {"message", err.message().c_str()}};
            });
    } else {
        auto res = QueuedCoreAdaptor::getUser(userId, property);
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


QVariantHash
QueuedTcpServerResponseHelperUser::getUsers(const QVariantHash &_data,
                                            const QString &_token)
{
    qCDebug(LOG_SERV) << "Get users" << _data;

    QDateTime lastLogin = QDateTime::fromString(_data["lastLogged"].toString(),
                                                Qt::ISODateWithMs);
    auto permission
        = QueuedEnums::stringToPermission(_data["permission"].toString());

    QVariantHash output = {{"code", 200}};
    // some conversion magic
    QVariantList outputReport;
    auto res = QueuedCoreAdaptor::getUsers(lastLogin, permission, _token);
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
