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


#include "QueuedctlPlugins.h"
#include "QueuedctlCommon.h"

#include <queued/Queued.h>


QueuedctlCommon::QueuedctlResult
QueuedctlPlugins::addPlugin(const QString &_plugin, const QString &_token)
{
    qCDebug(LOG_APP) << "Add plugin" << _plugin;

    auto res = QueuedCoreAdaptor::sendPluginAdd(_plugin, _token);

    QueuedctlCommon::QueuedctlResult output;
    res.match([&output](const bool val) { output.status = val; },
              [&output](const QueuedError &err) {
                  output.output = err.message().c_str();
              });

    return output;
}


QueuedctlCommon::QueuedctlResult QueuedctlPlugins::listPlugins()
{
    auto res = QueuedCoreAdaptor::getOption(
        QueuedConfig::QueuedSettings::Plugins, "");

    QueuedctlCommon::QueuedctlResult output;
    res.match(
        [&output](const QVariant &val) {
            output.status = val.isValid();
            output.output = val.toString();
        },
        [&output](const QueuedError &err) {
            output.output = err.message().c_str();
        });

    return output;
}


QueuedctlCommon::QueuedctlResult
QueuedctlPlugins::removePlugin(const QString &_plugin, const QString &_token)
{
    qCDebug(LOG_APP) << "Remove plugin" << _plugin;

    auto res = QueuedCoreAdaptor::sendPluginRemove(_plugin, _token);

    QueuedctlCommon::QueuedctlResult output;
    res.match([&output](const bool val) { output.status = val; },
              [&output](const QueuedError &err) {
                  output.output = err.message().c_str();
              });

    return output;
}


void QueuedctlPlugins::parser(QCommandLineParser &_parser)
{
    _parser.addPositionalArgument("plugin", "Plugin name.", "<plugin>");
}
