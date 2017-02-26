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
/**
 * @file QueuedTokenManager.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include "queued/Queued.h"

#include <QTimer>
#include <QUuid>


/**
 * @class QueuedTokenManager
 */
/**
 * @fn QueuedTokenManager
 */
QueuedTokenManager::QueuedTokenManager(QObject *parent)
    : QObject(parent)
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
bool QueuedTokenManager::isTokenValid(const QString &_token)
{
    qCDebug(LOG_LIB) << "Check token on validity" << _token;

    return m_tokens.contains(_token);
}


/**
 * @fn loadTokens
 */
void QueuedTokenManager::loadTokens(const QList<QVariantHash> &_values)
{
    qCDebug(LOG_LIB) << "Set values from" << _values;

    QDateTime now = QDateTime::currentDateTimeUtc();
    for (auto &token : _values) {
        QDateTime validUntil = QDateTime::fromString(
            token[QString("validUntil")].toString(), Qt::ISODate);
        if (validUntil <= now)
            continue;
        QString tokenId = token[QString("token")].toString();
        m_tokens[tokenId] = validUntil;
        QTimer::singleShot(validUntil.toMSecsSinceEpoch()
                               - now.toMSecsSinceEpoch(),
                           Qt::VeryCoarseTimer,
                           [this, tokenId]() { return expireToken(tokenId); });
    }
}


/**
 * @fn registerToken
 */
QString QueuedTokenManager::registerToken(const QDateTime _validUntil)
{
    // generate from uuid
    QString token
        = QUuid::createUuid().toString().remove(QChar('{')).remove(QChar('}'));
    qCInfo(LOG_LIB) << "Registered token" << token << "valid until"
                    << _validUntil;

    // append to internal storage
    m_tokens[token] = _validUntil;
    emit(tokenRegistered(token, _validUntil));

    // and return requester
    return token;
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
