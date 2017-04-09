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


#ifndef QUEUEDTCPSERVERRESPONSEHELUSER_H
#define QUEUEDTCPSERVERRESPONSEHELUSER_H

#include <QVariant>

#include <queued/QueuedUser.h>


namespace QueuedTcpServerResponseHelperUser
{
QVariantHash addOrEditUser(const QString &_user, const QVariantHash &_data,
                           const QString &_token);
QueuedUser::QueuedUserDefinitions getDefinitions(const QVariantHash &_data);
QVariantHash getReport(const QVariantHash &_data, const QString &_token);
QVariantHash getUser(const QString &_user, const QVariantHash &_data);
QVariantHash getUsers(const QVariantHash &_data, const QString &_token);
};


#endif /* QUEUEDTCPSERVERRESPONSEHELUSER_H */