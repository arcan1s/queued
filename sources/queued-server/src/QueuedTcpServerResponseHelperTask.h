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


#ifndef QUEUEDTCPSERVERRESPONSEHELTASK_H
#define QUEUEDTCPSERVERRESPONSEHELTASK_H

#include <QVariant>

#include <queued/QueuedProcess.h>


namespace QueuedTcpServerResponseHelperTask
{
QVariantHash addOrEditTask(const long long _id, const QVariantHash &_data, const QString &_token);
QueuedProcess::QueuedProcessDefinitions getDefinitions(const QVariantHash &_data);
QVariantHash getTask(const long long _id, const QVariantHash &_data, const QString &_token);
QVariantHash getTasks(const QVariantHash &_data, const QString &_token);
QVariantHash startOrStopTask(const long long _id, const QString &_token);
QVariantHash startTask(const long long _id, const QString &_token);
QVariantHash stopTask(const long long _id, const QString &_token);
}; // namespace QueuedTcpServerResponseHelperTask


#endif /* QUEUEDTCPSERVERRESPONSEHELTASK_H */
