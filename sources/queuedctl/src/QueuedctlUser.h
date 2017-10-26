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


#ifndef QUEUEDCTLUSER_H
#define QUEUEDCTLUSER_H

#include <QCommandLineParser>

#include <queued/QueuedUser.h>

#include "QueuedctlCommon.h"


namespace QueuedctlUser
{
QueuedctlCommon::QueuedctlResult
addUser(const QueuedUser::QueuedUserDefinitions &_definitions,
        const QString &_token);
QueuedctlCommon::QueuedctlResult getReport(const QCommandLineParser &_parser,
                                           const QString &_token);
QueuedUser::QueuedUserDefinitions
getDefinitions(const QCommandLineParser &_parser, const bool _expandAll);
QString getPassword();
QueuedctlCommon::QueuedctlResult getUser(const long long _id,
                                         const QString &_property);
QueuedctlCommon::QueuedctlResult getUsers(const QCommandLineParser &_parser,
                                          const QString &_token);
void parserAdd(QCommandLineParser &_parser);
void parserGet(QCommandLineParser &_parser);
void parserList(QCommandLineParser &_parser);
void parserReport(QCommandLineParser &_parser);
void parserSet(QCommandLineParser &_parser);
QueuedctlCommon::QueuedctlResult
setUser(const long long _id,
        const QueuedUser::QueuedUserDefinitions &_definitions,
        const QString &_token);
};


#endif /* QUEUEDCTLUSER_H */
