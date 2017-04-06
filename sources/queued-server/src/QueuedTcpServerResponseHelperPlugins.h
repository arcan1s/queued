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


#ifndef QUEUEDTCPSERVERRESPONSEHELPLUGINS_H
#define QUEUEDTCPSERVERRESPONSEHELPLUGINS_H

#include <QVariant>


namespace QueuedTcpServerResponseHelperPlugins
{
QVariantHash addPlugin(const QString &_name, const QString &_token);
QVariantHash listPlugins();
QVariantHash removePlugin(const QString &_name, const QString &_token);
};


#endif /* QUEUEDTCPSERVERRESPONSEHELPLUGINS_H */
