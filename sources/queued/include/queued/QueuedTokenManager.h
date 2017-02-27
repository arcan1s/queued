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
     * @return true if token is valid otherwise return false
     */
    bool isTokenValid(const QString &_token);
    /**
    * @brief upload tokens from database
    * @param _value
     * tokens from database
    */
    void loadTokens(const QList<QVariantHash> &_values);
    /**
     * @brief register new token
     * @param _validUntil
     * token valid until
     * @return new generated token
     */
    QString registerToken(const QDateTime _validUntil);

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
    /**
     * @brief signal which will be emitted on newly created token
     * @param _token
     * token ID
     * @param _validUntil
     * token valid until
     */
    void tokenRegistered(const QString &_token, const QDateTime &_validUntil);

private:
    /**
     * @brief token storage
     */
    QHash<QString, QDateTime> m_tokens;
};


#endif /* QUEUEDTOKENMANAGER_H */
