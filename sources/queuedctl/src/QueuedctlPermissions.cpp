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


#include "QueuedctlPermissions.h"

#include <queued/Queued.h>


bool QueuedctlPermissions::addPermission(const long long _id,
                                         const QString &_permission,
                                         const QString &_token)
{
    qCDebug(LOG_APP) << "Add permission" << _permission << "to" << _id;

    auto permission = QueuedEnums::stringToPermission(_permission);

    if (permission != QueuedEnums::Permission::Invalid)
        return QueuedCoreAdaptor::sendUserPermissionAdd(_id, permission,
                                                        _token);
    else
        return false;
}


bool QueuedctlPermissions::removePermission(const long long _id,
                                            const QString &_permission,
                                            const QString &_token)
{
    qCDebug(LOG_APP) << "Remove permission" << _permission << "to" << _id;

    auto permission = QueuedEnums::stringToPermission(_permission);

    if (permission != QueuedEnums::Permission::Invalid)
        return QueuedCoreAdaptor::sendUserPermissionRemove(_id, permission,
                                                           _token);
    else
        return false;
}


void QueuedctlPermissions::parser(QCommandLineParser &_parser)
{
    _parser.addPositionalArgument("user", "User ID.", "<user>");
    _parser.addPositionalArgument("permission", "Permission name.",
                                  "<permission>");
}
