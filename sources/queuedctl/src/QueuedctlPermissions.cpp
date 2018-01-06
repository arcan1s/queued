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


#include "QueuedctlPermissions.h"
#include "QueuedctlCommon.h"

#include <queued/Queued.h>


QueuedctlCommon::QueuedctlResult QueuedctlPermissions::addPermission(const long long _id,
                                                                     const QString &_permission,
                                                                     const QString &_token)
{
    qCDebug(LOG_APP) << "Add permission" << _permission << "to" << _id;

    auto permission = QueuedEnums::stringToPermission(_permission);
    QueuedctlCommon::QueuedctlResult output;

    if (permission == QueuedEnums::Permission::Invalid) {
        output.output = "Invalid permission";
    } else {
        auto res = QueuedCoreAdaptor::sendUserPermissionAdd(_id, permission, _token);
        res.match([&output](const bool val) { output.status = val; },
                  [&output](const QueuedError &err) { output.output = err.message().c_str(); });
    }

    return output;
}


QueuedctlCommon::QueuedctlResult QueuedctlPermissions::removePermission(const long long _id,
                                                                        const QString &_permission,
                                                                        const QString &_token)
{
    qCDebug(LOG_APP) << "Remove permission" << _permission << "to" << _id;

    auto permission = QueuedEnums::stringToPermission(_permission);
    QueuedctlCommon::QueuedctlResult output;

    if (permission == QueuedEnums::Permission::Invalid) {
        output.output = "Invalid permission";
    } else {
        auto res = QueuedCoreAdaptor::sendUserPermissionRemove(_id, permission, _token);
        res.match([&output](const bool val) { output.status = val; },
                  [&output](const QueuedError &err) { output.output = err.message().c_str(); });
    }

    return output;
}


void QueuedctlPermissions::parser(QCommandLineParser &_parser)
{
    _parser.addPositionalArgument("user", "User ID.", "<user>");
    _parser.addPositionalArgument("permission", "Permission name.", "<permission>");
}
