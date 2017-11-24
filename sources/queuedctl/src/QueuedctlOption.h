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


#ifndef QUEUEDCTLOPTION_H
#define QUEUEDCTLOPTION_H

#include <QCommandLineParser>

#include "QueuedctlCommon.h"


namespace QueuedctlOption
{
QueuedctlCommon::QueuedctlResult editOption(const QString &_option,
                                            const QVariant &_value,
                                            const QString &_token);
QueuedctlCommon::QueuedctlResult getOption(const QString &_option,
                                           const QString &_token);
void parserGet(QCommandLineParser &_parser);
void parserSet(QCommandLineParser &_parser);
};


#endif /* QUEUEDCTLOPTION_H */
