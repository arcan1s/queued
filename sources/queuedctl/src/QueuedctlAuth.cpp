/*
 * Copyright (c) 2016 Evgeniy Alekseev
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

extern "C" {
#include <termios.h>
#include <unistd.h>
}


QString QueuedctlAuth::auth(const QString &_user)
{
    qCDebug(LOG_APP) << "Auth as user" << _user;

    // read password
    // do not show input characters
    struct termios tty;
    ::tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);

    qInfo() << "Password for" << _user;
    QTextStream stream(stdin);
    QString password;
    stream >> password;

    return QueuedCoreAdaptor::auth(_user, password);
}


QString QueuedctlAuth::getToken(const QString &_cache, const QString &_user)
{
    qCDebug(LOG_APP) << "Get token using cache" << _cache << "and user"
                     << _user;

    QString tokenId = token(_cache);
    if (tryAuth(tokenId)) {
        return tokenId;
    } else {
        tokenId = auth(_user);
        setToken(tokenId, _cache);
        return getToken(_cache, _user);
    }
}


void QueuedctlAuth::parser(QCommandLineParser &_parser)
{
    _parser.clearPositionalArguments();
}


void QueuedctlAuth::setToken(const QString &_token, const QString &_cache)
{
    qCDebug(LOG_APP) << "Save token to" << _cache;

    QSettings settings(_cache, QSettings::IniFormat);
    settings.beginGroup("queuedctl");
    settings.setValue("Token", _token);
    settings.endGroup();

    settings.sync();
}


QString QueuedctlAuth::token(const QString &_cache)
{
    qCDebug(LOG_APP) << "Load token from" << _cache;

    QString token;

    QSettings settings(_cache, QSettings::IniFormat);
    settings.beginGroup("queuedctl");
    token = settings.value("Token").toString();
    settings.endGroup();

    return token;
}


bool QueuedctlAuth::tryAuth(const QString &_token)
{
    qCDebug(LOG_APP) << "Try auth with" << _token;

    return QueuedCoreAdaptor::auth(_token);
}
