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

    auto res = QueuedCoreAdaptor::sendPluginAdd(_name, _token);

    QVariantHash output;
    res.match(
        [&output](const bool) {
            output = {{"code", 200}};
        },
        [&output](const QueuedError &err) {
            output = {{"code", 500}, {"message", err.message().c_str()}};
        });

    return output;
}


QVariantHash QueuedTcpServerResponseHelperPlugins::listPlugins()
{
    auto res
        = QueuedCoreAdaptor::getOption(QueuedConfig::QueuedSettings::Plugins);

    QVariantHash output;
    res.match(
        [&output](const QVariant &val) {
            output = {{"code", 200}, {"plugins", val.toStringList()}};
        },
        [&output](const QueuedError &err) {
            output = {{"code", 500}, {"message", err.message().c_str()}};
        });

    return output;
}


QVariantHash
QueuedTcpServerResponseHelperPlugins::removePlugin(const QString &_name,
                                                   const QString &_token)
{
    qCDebug(LOG_SERV) << "Remove plugin" << _name;

    auto res = QueuedCoreAdaptor::sendPluginRemove(_name, _token);

    QVariantHash output;
    res.match(
        [&output](const bool) {
            output = {{"code", 200}};
        },
        [&output](const QueuedError &err) {
            output = {{"code", 500}, {"message", err.message().c_str()}};
        });

    return output;
}
