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


#ifndef QUEUEDCTLTASK_H
#define QUEUEDCTLTASK_H

#include <QCommandLineParser>

#include <queued/QueuedProcess.h>

#include "QueuedctlCommon.h"


namespace QueuedctlTask
{
QueuedctlCommon::QueuedctlResult
addTask(const QueuedProcess::QueuedProcessDefinitions &_definitions,
        const QString &_token);
QueuedProcess::QueuedProcessDefinitions
getDefinitions(const QCommandLineParser &_parser, const bool _expandAll,
               const QString &_token);
QueuedctlCommon::QueuedctlResult
getTask(const long long _id, const QString &_property, const QString &_token);
QueuedctlCommon::QueuedctlResult getTasks(const QCommandLineParser &_parser,
                                          const QString &_token);
void parserAdd(QCommandLineParser &_parser);
void parserGet(QCommandLineParser &_parser);
void parserList(QCommandLineParser &_parser);
void parserSet(QCommandLineParser &_parser);
void parserStart(QCommandLineParser &_parser);
QueuedctlCommon::QueuedctlResult
setTask(const long long _id,
        const QueuedProcess::QueuedProcessDefinitions &_definitions,
        const QString &_token);
QueuedctlCommon::QueuedctlResult startTask(const long long _id,
                                           const QString &_token);
QueuedctlCommon::QueuedctlResult stopTask(const long long _id,
                                          const QString &_token);
};


#endif /* QUEUEDCTLTASK_H */
