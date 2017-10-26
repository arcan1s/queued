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


#ifndef QUEUEDCTLPERMISSIONS_H
#define QUEUEDCTLPERMISSIONS_H

#include <QCommandLineParser>

#include "QueuedctlCommon.h"


namespace QueuedctlPermissions
{
QueuedctlCommon::QueuedctlResult addPermission(const long long _id,
                                               const QString &_permission,
                                               const QString &_token);
QueuedctlCommon::QueuedctlResult removePermission(const long long _id,
                                                  const QString &_permission,
                                                  const QString &_token);
void parser(QCommandLineParser &_parser);
};


#endif /* QUEUEDCTLPERMISSIONS_H */
