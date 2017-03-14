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
/**
 * @file QueuedTokenManager.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDTOKENMANAGER_H
#define QUEUEDTOKENMANAGER_H

#include <QDateTime>
#include <QObject>


/**
 * @brief token management system
 */
class QueuedTokenManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @struct QueuedTokenDefinitions
     * @brief structure to define token
     * @var QueuedTokenDefinitions::token
     * token ID
     * @var QueuedTokenDefinitions::user
     * token owner user
     * @var QueuedTokenDefinitions::validUntil
     * valid until
     */
    typedef struct {
        QString token;
        QString user;
        QDateTime validUntil;
    } QueuedTokenDefinitions;

    /**
     * @brief QueuedTokenManager class constructor
     * @param parent
     * pointer to parent item
     */
    explicit QueuedTokenManager(QObject *parent);
    /**
     * @brief QueuedTokenManager class destructor
     */
    virtual ~QueuedTokenManager();
    /**
     * @brief check if token is valid
     * @param _token
     * token ID
     * @return token user if token is valid otherwise return false
     */
    QString isTokenValid(const QString &_token) const;
    /**
     * @brief load token
     * @param _definitions
     * token definitions
     */
    void
    loadToken(const QueuedTokenManager::QueuedTokenDefinitions &_definitions);
    /**
     * @brief upload tokens from database
     * @param _values
     * tokens from database
     */
    void loadTokens(const QList<QVariantHash> &_values);
    /**
     * @brief register new token
     * @param _user
     * token user
     * @param _validUntil
     * token valid until
     * @return new generated token
     */
    QString registerToken(const QString &_user, const QDateTime &_validUntil);
    /**
     * @brief token expiration
     * @param _token
     * token ID
     * @return token expiration if token found, otherwise return default
     * QDateTime constructor
     */
    QDateTime tokenExpiration(const QString &_token) const;

public slots:
    /**
     * @brief method which will be called on token expiring
     * @param _token
     * expired token ID
     */
    void expireToken(const QString &_token);

signals:
    /**
     * @brief signal which will be emitted on token expiration
     * @param _token
     * token ID
     */
    void tokenExpired(const QString &_token);

private:
    /**
     * @brief token storage
     */
    QHash<QString, QueuedTokenDefinitions> m_tokens;
};


#endif /* QUEUEDTOKENMANAGER_H */
