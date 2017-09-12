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


namespace QueuedctlTask
{
long long addTask(const QueuedProcess::QueuedProcessDefinitions &_definitions,
                  const QString &_token);
QueuedProcess::QueuedProcessDefinitions
getDefinitions(const QCommandLineParser &_parser, const bool _expandAll);
QVariant getTask(const long long _id, const QString &_property);
QList<QVariantHash> getTasks(const QCommandLineParser &_parser,
                             const QString &_token);
void parserAdd(QCommandLineParser &_parser);
void parserGet(QCommandLineParser &_parser);
void parserList(QCommandLineParser &_parser);
void parserSet(QCommandLineParser &_parser);
void parserStart(QCommandLineParser &_parser);
bool setTask(const long long _id,
             const QueuedProcess::QueuedProcessDefinitions &_definitions,
             const QString &_token);
bool startTask(const long long _id, const QString &_token);
bool stopTask(const long long _id, const QString &_token);
};


#endif /* QUEUEDCTLTASK_H */
