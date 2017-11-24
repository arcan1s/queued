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
 * @file QueuedUserManager.cpp
 * Source code of queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>
#include <queued/QueuedUser.h>


/**
 * @class QueuedUserManager
 */
/**
 * @fn QueuedUserManager
 */
QueuedUserManager::QueuedUserManager(QObject *parent)
    : QObject(parent)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    m_tokens = new QueuedTokenManager(this);
}


/**
 * @fn ~QueuedUserManager
 */
QueuedUserManager::~QueuedUserManager()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    delete m_tokens;
    m_users.clear();
}


/**
 * @fn add
 */
QueuedUser *QueuedUserManager::add(const QVariantHash &_properties,
                                   const long long _id)
{
    qCDebug(LOG_LIB) << "Add user" << _properties << "with ID" << _id;

    QueuedUser::QueuedUserDefinitions defs;
    defs.name = _properties["name"].toString();
    defs.email = _properties["email"].toString();
    defs.password = _properties["password"].toString();
    defs.permissions = _properties["permissions"].toUInt();
    defs.priority = _properties["priority"].toUInt();
    defs.limits = _properties["limits"].toString();

    return add(defs, _id);
}


/**
 * @fn add
 */
QueuedUser *
QueuedUserManager::add(const QueuedUser::QueuedUserDefinitions &_definitions,
                       const long long _id)
{
    qCDebug(LOG_LIB) << "Add user" << _definitions.name << "with ID" << _id;

    if (m_users.contains(_definitions.name)) {
        qCWarning(LOG_LIB) << "User" << _definitions.name << "already exists";
        return nullptr;
    }

    auto user = new QueuedUser(this, _definitions, _id);
    m_users[user->name()] = user;

    return user;
}


/**
 * @fn authorize
 */
QString QueuedUserManager::authorize(const QString &_user)
{
    qCDebug(LOG_LIB) << "Authorize user manually" << _user;

    auto time = QDateTime::currentDateTimeUtc();
    time = time.addDays(9999);

    return m_tokens->registerToken(_user, time);
}


/**
 * @fn authorize
 */
QString QueuedUserManager::authorize(const QString &_user,
                                     const QString &_password)
{
    qCDebug(LOG_LIB) << "Authorize user" << _user;

    auto userObj = user(_user, false);
    if (!userObj) {
        qCInfo(LOG_LIB) << "No user found" << _user;
        return "";
    }

    bool status = userObj->isPasswordValid(_password, salt());
    if (!status) {
        qCInfo(LOG_LIB) << "User password invalid for" << _user;
        return "";
    }

    auto time = QDateTime::currentDateTimeUtc();
    QDateTime expiry = time.addDays(tokenExpiration());
    emit(userLoggedIn(userObj->index(), time));
    return m_tokens->registerToken(_user, expiry);
}


/**
 * @fn authorize
 */
bool QueuedUserManager::authorize(const QString &_token,
                                  const QueuedEnums::Permission _service)
{
    qCDebug(LOG_LIB) << "Authorize user for" << static_cast<int>(_service);

    QString username = m_tokens->isTokenValid(_token);
    if (username.isEmpty()) {
        qCInfo(LOG_LIB) << "Token" << _token << "invalid";
        return false;
    }

    auto userObj = user(username, false);
    if (!userObj) {
        qCInfo(LOG_LIB) << "No user found" << username;
        return false;
    }

    return userObj->hasPermission(_service);
}


/**
 * @fn checkToken
 */
QDateTime QueuedUserManager::checkToken(const QString &_token,
                                        bool *_valid) const
{
    if (_valid) {
        QString user = m_tokens->isTokenValid(_token);
        *_valid = !user.isEmpty();
    }

    return m_tokens->tokenExpiration(_token);
}


/**
 * @fn ids
 */
QPair<uint, uint> QueuedUserManager::ids(const long long _id)
{
    qCDebug(LOG_LIB) << "Get ids for user" << _id;

    auto userObj = user(_id);
    if (!userObj) {
        qCWarning(LOG_LIB) << "No user found for ID" << _id;
        return {1, 1};
    }

    return userObj->ids();
}


/**
 * @fn loadTokens
 */
void QueuedUserManager::loadTokens(const QList<QVariantHash> &_tokens)
{
    qCDebug(LOG_LIB) << "Set tokens from" << _tokens;

    m_tokens->loadTokens(_tokens);
}


/**
 * @fn loadUsers
 */
void QueuedUserManager::loadUsers(const QList<QVariantHash> &_users)
{
    qCDebug(LOG_LIB) << "Set users from" << _users;

    // load now
    for (auto &userData : _users)
        add(userData, userData["_id"].toLongLong());
}


/**
 * @fn user
 */
QueuedUser *QueuedUserManager::user(const long long _id)
{
    qCDebug(LOG_LIB) << "Look for user" << _id;

    for (auto &userObj : m_users.values()) {
        if (userObj->index() != _id)
            continue;
        return userObj;
    }

    return nullptr;
}


/**
 * @fn user
 */
QueuedUser *QueuedUserManager::user(const QString &_name, const bool _isToken)
{
    qCDebug(LOG_LIB) << "Look for user" << _name << "is token" << _isToken;

    if (_isToken)
        return m_users.value(m_tokens->isTokenValid(_name), nullptr);
    else
        return m_users.value(_name, nullptr);
}


/**
 * @salt
 */
QString QueuedUserManager::salt() const
{
    return m_salt;
}


/*
 * @fn tokenExpiration
 */
long long QueuedUserManager::tokenExpiration() const
{
    return m_tokenExpiration;
}


/**
 * @fn setSalt
 */
void QueuedUserManager::setSalt(const QString &_salt)
{
    m_salt = _salt;
}


/**
 * @fn setTokenExpiration
 */
void QueuedUserManager::setTokenExpiration(const long long &_expiry)
{
    qCDebug(LOG_LIB) << "Set token expiration to" << _expiry;
    if (_expiry <= 0)
        return;

    m_tokenExpiration = _expiry;
}
