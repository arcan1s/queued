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


#include "QueuedctlPlugins.h"

#include <queued/Queued.h>


bool QueuedctlPlugins::addPlugin(const QString &_plugin, const QString &_token)
{
    qCDebug(LOG_APP) << "Add plugin" << _plugin;

    return QueuedCoreAdaptor::sendPluginAdd(_plugin, _token);
}


QStringList QueuedctlPlugins::listPlugins()
{
    return QueuedCoreAdaptor::getOption(
               QueuedAdvancedSettings::internalId(
                   QueuedConfig::QueuedSettings::Plugins))
        .toString()
        .split('\n');
}


bool QueuedctlPlugins::removePlugin(const QString &_plugin,
                                    const QString &_token)
{
    qCDebug(LOG_APP) << "Remove plugin" << _plugin;

    return QueuedCoreAdaptor::sendPluginRemove(_plugin, _token);
}


void QueuedctlPlugins::parser(QCommandLineParser &_parser)
{
    _parser.addPositionalArgument("plugin", "Plugin name.", "<plugin>");
}
