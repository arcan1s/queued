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


#ifndef QUEUEDCTLTASK_H
#define QUEUEDCTLTASK_H

#include <QCommandLineParser>

#include <queued/QueuedProcess.h>


namespace QueuedctlTask
{
QueuedProcess::QueuedProcessDefinitions
getDefinitions(const QCommandLineParser &_parser);
QVariant getTask(const long long _id, const QString &_property);
void parserAdd(QCommandLineParser &_parser);
void parserGet(QCommandLineParser &_parser);
void parserSet(QCommandLineParser &_parser);
bool setTask(const long long _id,
             const QueuedProcess::QueuedProcessDefinitions &_definitions,
             const QString &_token);
};


#endif /* QUEUEDCTLTASK_H */
