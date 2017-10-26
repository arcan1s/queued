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


#ifndef QUEUEDCTLAUTH_H
#define QUEUEDCTLAUTH_H

#include <QCommandLineParser>

#include "QueuedctlCommon.h"


namespace QueuedctlAuth
{
QueuedctlCommon::QueuedctlResult auth(const QString &_user,
                                      const QString &_cache);
QString getToken(const QString &_cache, const QString &_user);
void parser(QCommandLineParser &_parser);
void setToken(const QString &_token, const QString &_user,
              const QString &_cache);
QString token(const QString &_user, const QString &_cache);
bool tryAuth(const QString &_token);
};


#endif /* QUEUEDCTLAUTH_H */
