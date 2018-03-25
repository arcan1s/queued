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


#ifndef QUEUEDTCPSERVERRESPONSEHELPERPERMISSIONS_H
#define QUEUEDTCPSERVERRESPONSEHELPERPERMISSIONS_H

#include <QVariant>


namespace QueuedTcpServerResponseHelperPermissions
{
QVariantHash addPermission(const long long _id, const QVariantHash &_value, const QString &_token);
QVariantHash removePermission(const long long _id, const QVariantHash &_value,
                              const QString &_token);
}; // namespace QueuedTcpServerResponseHelperPermissions


#endif /* QUEUEDTCPSERVERRESPONSEHELPERPERMISSIONS_H */
