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


#ifndef QUEUEDCTLCOMMON_H
#define QUEUEDCTLCOMMON_H

#include <QCommandLineParser>


namespace QueuedctlCommon
{
typedef struct {
    bool status = false;
    QString output;
} QueuedctlResult;
void checkArgs(const QStringList &_args, const int _count,
               QCommandLineParser &_parser);
void preprocess(const QStringList &_args, QCommandLineParser &_parser);
void print(QueuedctlResult &_result);
QueuedctlResult process(QCommandLineParser &_parser, const QString &_cache,
                        const QString &_user);
};


#endif /* QUEUEDCTLCOMMON_H */
