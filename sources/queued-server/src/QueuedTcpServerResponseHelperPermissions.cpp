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


#include "QueuedTcpServerResponseHelperPermissions.h"

#include <queued/Queued.h>


QVariantHash QueuedTcpServerResponseHelperPermissions::addPermission(
    const long long _id, const QVariantHash &_value, const QString &_token)
{
    qCDebug(LOG_SERV) << "Add permission for" << _id << _value;

    if (!_value.contains("permission"))
        return {{"code", 400}, {"message", "No required fields found"}};

    auto permission
        = QueuedEnums::stringToPermission(_value["permission"].toString());
    if (permission == QueuedEnums::Permission::Invalid)
        return {{"code", 400}, {"message", "Invalid permission"}};

    return {{"code",
             QueuedCoreAdaptor::sendUserPermissionAdd(_id, permission, _token)
                 ? 200
                 : 400}};
}


QVariantHash QueuedTcpServerResponseHelperPermissions::removePermission(
    const long long _id, const QVariantHash &_value, const QString &_token)
{
    qCDebug(LOG_SERV) << "Remove permission for" << _id << _value;

    if (!_value.contains("permission"))
        return {{"code", 400}, {"message", "No required fields found"}};

    auto permission
        = QueuedEnums::stringToPermission(_value["permission"].toString());
    if (permission == QueuedEnums::Permission::Invalid)
        return {{"code", 400}, {"message", "Invalid permission"}};

    return {{"code", QueuedCoreAdaptor::sendUserPermissionRemove(
                         _id, permission, _token)
                         ? 200
                         : 400}};
}
