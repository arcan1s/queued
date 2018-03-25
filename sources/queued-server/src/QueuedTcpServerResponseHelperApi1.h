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


#ifndef QUEUEDTCPSERVERRESPONSEHELPERAPI1_H
#define QUEUEDTCPSERVERRESPONSEHELPERAPI1_H

#include <QVariant>

#include "QueuedTcpServerResponseHelper.h"


namespace QueuedTcpServerResponseHelperApi1
{
QVariantHash getData(const QueuedTcpServerResponseHelper::RequestPath _request, const QString &_arg,
                     const QString &_type, const QVariantHash &_data, const QString &_token);
QVariantHash getStatus();
}; // namespace QueuedTcpServerResponseHelperApi1


#endif /* QUEUEDTCPSERVERRESPONSEHELPERAPI1_H */
