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


#include "QueuedctlAuth.h"

#include <QSettings>

#include <queued/Queued.h>

#include "QueuedctlUser.h"


QueuedctlCommon::QueuedctlResult QueuedctlAuth::auth(const QString &_user,
                                                     const QString &_cache)
{
    qCDebug(LOG_APP) << "Auth as user" << _user;

    QueuedctlCommon::QueuedctlResult output;

    auto res = QueuedCoreAdaptor::auth(_user, QueuedctlUser::getPassword());
    res.match(
        [&output, &_user, &_cache](const QString &val) {
            setToken(val, _user, _cache);
            output.status = true;
        },
        [&output](const QueuedError &err) {
            output.status = false;
            output.output = err.message().c_str();
        });

    return output;
}


QString QueuedctlAuth::getToken(const QString &_cache, const QString &_user)
{
    qCDebug(LOG_APP) << "Get token using cache" << _cache << "and user"
                     << _user;

    QString tokenId = token(_user, _cache);
    if (tryAuth(tokenId)) {
        return tokenId;
    } else {
        auth(_user, _cache);
        return getToken(_cache, _user);
    }
}


void QueuedctlAuth::parser(QCommandLineParser &_parser) {}


void QueuedctlAuth::setToken(const QString &_token, const QString &_user,
                             const QString &_cache)
{
    qCDebug(LOG_APP) << "Save token to" << _cache << "from user" << _user;

    QSettings settings(_cache, QSettings::IniFormat);
    settings.beginGroup(QString("queuedctl/%1").arg(_user));
    settings.setValue("Token", _token);
    settings.endGroup();

    settings.sync();
}


QString QueuedctlAuth::token(const QString &_user, const QString &_cache)
{
    qCDebug(LOG_APP) << "Load token from" << _cache << "for user" << _user;

    QString token;

    QSettings settings(_cache, QSettings::IniFormat);
    settings.beginGroup(QString("queuedctl/%1").arg(_user));
    token = settings.value("Token").toString();
    settings.endGroup();

    return token;
}


bool QueuedctlAuth::tryAuth(const QString &_token)
{
    qCDebug(LOG_APP) << "Try auth with" << _token;

    auto res = QueuedCoreAdaptor::auth(_token);

    return ((res.type() == Result::Content::Value) && res.get());
}
