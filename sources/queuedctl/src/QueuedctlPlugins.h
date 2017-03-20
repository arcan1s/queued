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


#ifndef QUEUEDCTLPLUGINS_H
#define QUEUEDCTLPLUGINS_H

#include <QCommandLineParser>


namespace QueuedctlPlugins
{
bool addPlugin(const QString &_plugin, const QString &_token);
QStringList listPlugins();
bool removePlugin(const QString &_plugin, const QString &_token);
void parser(QCommandLineParser &_parser);
};


#endif /* QUEUEDCTLPLUGINS_H */