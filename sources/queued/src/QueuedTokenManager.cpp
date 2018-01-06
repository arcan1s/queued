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
/**
 * @file QueuedTokenManager.cpp
 * Source code of queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>

#include <QUuid>


/**
 * @class QueuedTokenManager
 */
/**
 * @fn QueuedTokenManager
 */
QueuedTokenManager::QueuedTokenManager(QObject *_parent)
    : QObject(_parent)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;
}


/**
 * @fn ~QueuedTokenManager
 */
QueuedTokenManager::~QueuedTokenManager()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;
}


/**
 * @fn isTokenValid
 */
QString QueuedTokenManager::isTokenValid(const QString &_token) const
{
    qCDebug(LOG_LIB) << "Check token on validity" << _token;

    if (m_tokens.contains(_token) && (tokenExpiration(_token) > QDateTime::currentDateTimeUtc()))
        return m_tokens[_token].user;
    else
        return "";
}


/**
 * @brief loadToken
 */
void QueuedTokenManager::loadToken(const QueuedTokenManager::QueuedTokenDefinitions &_definitions)
{
    qCDebug(LOG_LIB) << "Add token for user" << _definitions.user << "valid until"
                     << _definitions.validUntil;

    m_tokens[_definitions.token] = _definitions;

    // register expiry timer
    std::chrono::milliseconds duration(_definitions.validUntil.toMSecsSinceEpoch()
                                       - QDateTime::currentDateTimeUtc().toMSecsSinceEpoch());
    QTimer timer;
    timer.setSingleShot(true);
    timer.setInterval(duration);
    connect(&timer, &QTimer::timeout,
            [this, _definitions]() { return expireToken(_definitions.token); });
}


/**
 * @fn loadTokens
 */
void QueuedTokenManager::loadTokens(const QList<QVariantHash> &_values)
{
    qCDebug(LOG_LIB) << "Set values from" << _values;

    for (auto &token : _values) {
        QDateTime validUntil
            = QDateTime::fromString(token["validUntil"].toString(), Qt::ISODateWithMs);
        loadToken({token["token"].toString(), token["user"].toString(), validUntil});
    }
}


/**
 * @fn registerToken
 */
QString QueuedTokenManager::registerToken(const QString &_user, const QDateTime &_validUntil)
{
    // generate from uuid
    QString token = QUuid::createUuid().toString().remove('{').remove('}');
    qCInfo(LOG_LIB) << "Registered token" << token << "valid until" << _validUntil;

    // append to internal storage
    loadToken({token, _user, _validUntil});

    // and return requester
    return token;
}


/**
 * @fn tokenExpiration
 */
QDateTime QueuedTokenManager::tokenExpiration(const QString &_token) const
{
    return m_tokens[_token].validUntil;
}


/**
 * @fn expireToken
 */
void QueuedTokenManager::expireToken(const QString &_token)
{
    qCDebug(LOG_LIB) << "Expire token" << _token;

    if (!m_tokens.contains(_token))
        return;
    m_tokens.remove(_token);
    emit(tokenExpired(_token));
}
