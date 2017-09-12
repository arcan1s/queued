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


#include "QueuedTcpServerResponseHelperPlugins.h"

#include <queued/Queued.h>


QVariantHash
QueuedTcpServerResponseHelperPlugins::addPlugin(const QString &_name,
                                                const QString &_token)
{
    qCDebug(LOG_SERV) << "Add plugin" << _name;

    return {
        {"code", QueuedCoreAdaptor::sendPluginAdd(_name, _token) ? 200 : 400}};
}


QVariantHash QueuedTcpServerResponseHelperPlugins::listPlugins()
{
    return {{"code", 200},
            {"plugins", QueuedCoreAdaptor::getOption(
                            QueuedConfig::QueuedSettings::Plugins)}};
}


QVariantHash
QueuedTcpServerResponseHelperPlugins::removePlugin(const QString &_name,
                                                   const QString &_token)
{
    qCDebug(LOG_SERV) << "Remove plugin" << _name;

    return {{"code",
             QueuedCoreAdaptor::sendPluginRemove(_name, _token) ? 200 : 400}};
}
