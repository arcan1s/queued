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
 * @file QueuedUserManager.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDUSERMANAGER_H
#define QUEUEDUSERMANAGER_H

#include <QHash>
#include <QObject>

#include "QueuedEnums.h"
#include "QueuedUser.h"


class QueuedTokenManager;

/**
 * @brief user manager for queued
 */
class QueuedUserManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(
        long long tokenExpiration READ tokenExpiration WRITE setTokenExpiration)

public:
    /**
     * @struct UserAuthorization
     * user authorization structure
     * @remark only
     * @var token
     * authorization token
     * @var user
     * username, token owner
     */
    typedef struct {
        QString token;
        QString user;
    } UserAuthorization;

    /**
     * @brief QueuedUserManager class constructor
     * @param parent
     * pointer to parent item
     */
    explicit QueuedUserManager(QObject *parent);
    /**
     * @brief QueuedUserManager class destructor
     */
    virtual ~QueuedUserManager();
    /**
     * @brief add user
     * @param _properties
     * user properties from database
     * @param _id
     * user ID
     * @return pointer to created user
     */
    QueuedUser *add(const QVariantHash &_properties, const long long _id);
    /**
     * @brief add user
     * @param _definitions
     * user definitions
     * @param _id
     * user ID
     * @return pointer to created user
     */
    QueuedUser *add(const QueuedUser::QueuedUserDefinitions &_definitions,
                    const long long _id);
    /**
     * @brief authorize user
     * @param _user
     * user name
     * @param _password
     * user password
     * @return generated tokens or empty string if it is not valid
     */
    QString authorize(const QString &_user, const QString &_password);
    /**
     * @brief authorize user for service
     * @param _auth
     * user authorization structure
     * @param _service
     * service to authorize
     * @return true if user allowed to do it otherwise return false
     */
    bool authorize(const UserAuthorization &_auth,
                   const QueuedEnums::Permission _service);
    /**
     * @brief get UID and GID from user ID
     * @param _id
     * user id
     * @return pair of {uid, gid}
     */
    QPair<unsigned int, unsigned int> ids(const long long _id);
    /**
     * @brief load tokens
     * @param _tokens
     * tokens list from database
     */
    void loadTokens(const QList<QVariantHash> &_tokens);
    /**
     * @brief load users
     * @param _users
     * users list from database
     */
    void loadUsers(const QList<QVariantHash> &_users);
    /**
     * @brief user by ID
     * @param _id
     * user id for search
     * @return user by id or nullptr if no user found
     */
    QueuedUser *user(const long long _id);
    /**
     * @brief user by name
     * @param _name
     * user name for search
     * @return user by name or nullptr if no user found
     */
    QueuedUser *user(const QString &_name);
    // properties
    /**
     * @brief token expiration
     * @return token expiration in days
     */
    long long tokenExpiration() const;
    /**
     * @brief set token expiration
     * @param _expiry
     * token expiration in days
     */
    void setTokenExpiration(const long long &_expiry);

signals:
    /**
     * @brief signal which emits on each user successfully login
     * @param _id
     * user ID
     * @param _time
     * user login time
     */
    void userLoggedIn(const long long _id, const QDateTime &_time);

private:
    /**
     * @brief token expiration in days
     */
    long long m_tokenExpiration = 30;
    /**
     * @brief pointer to token manager
     */
    QueuedTokenManager *m_tokens = nullptr;
    /**
     * @brief list of users
     */
    QHash<QString, QueuedUser *> m_users;
};


#endif /* QUEUEDUSERMANAGER_H */
