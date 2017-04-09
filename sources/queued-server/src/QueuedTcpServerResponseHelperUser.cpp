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


#include "QueuedTcpServerResponseHelperUser.h"

#include <queued/Queued.h>


QVariantHash QueuedTcpServerResponseHelperUser::addOrEditUser(
    const QString &_user, const QVariantHash &_data, const QString &_token)
{
    qCDebug(LOG_SERV) << "Add user" << _user << "with data" << _data;

    // try define if user exists first
    auto userId = QueuedCoreAdaptor::getUserId(_user);

    auto defs = getDefinitions(_data);
    defs.name = _user;
    if (userId > 0) {
        // edit existing user
        bool status = QueuedCoreAdaptor::sendUserEdit(userId, defs, _token);
        return {{"code", status ? 200 : 400}};
    } else {
        // add new user
        auto id = QueuedCoreAdaptor::sendUserAdd(defs, _token);
        return {{"code", id > 0 ? 200 : 400}, {"id", id}};
    }
}


QueuedUser::QueuedUserDefinitions
QueuedTcpServerResponseHelperUser::getDefinitions(const QVariantHash &_data)
{
    qCDebug(LOG_SERV) << "Generate definitions from" << _data;

    QueuedUser::QueuedUserDefinitions defs;
    defs.email = _data["email"].toString();
    defs.password = QueuedUser::hashFromPassword(_data["password"].toString());
    defs.permissions = _data["permissions"].toUInt();
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
    auto report = QueuedCoreAdaptor::getPerformance(start, stop, _token);
    for (auto &user : report)
        outputReport.append(user);
    output["report"] = outputReport;

    return output;
}


QVariantHash
QueuedTcpServerResponseHelperUser::getUser(const QString &_user,
                                           const QVariantHash &_data)
{
    qCDebug(LOG_SERV) << "Get user data for" << _user << _data;

    auto userId = QueuedCoreAdaptor::getUserId(_user);
    auto property = _data["property"].toString();

    QVariantHash output = {{"code", 200}};
    if (property.isEmpty())
        output["properties"] = QueuedCoreAdaptor::getUser(userId);
    else
        output["properties"] = QVariantHash(
            {{property, QueuedCoreAdaptor::getUser(userId, property)}});

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
    auto report = QueuedCoreAdaptor::getUsers(lastLogin, permission, _token);
    for (auto &user : report)
        outputReport.append(user);
    output["report"] = outputReport;

    return output;
}
