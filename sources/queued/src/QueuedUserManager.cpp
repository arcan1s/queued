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
 * @file QueuedUserManager.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>

#include <QDateTime>


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
    defs.name = _properties[QString("name")].toString();
    defs.email = _properties[QString("email")].toString();
    defs.password = _properties[QString("password")].toString();
    defs.permissions = _properties[QString("permissions")].toUInt();
    defs.limits
        = QueuedLimits::Limits(_properties[QString("limits")].toString());

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
QString QueuedUserManager::authorize(const QString &_user,
                                     const QString &_password)
{
    qCDebug(LOG_LIB) << "Authorize user" << _user;

    auto userObj = user(_user);
    if (!userObj) {
        qCInfo(LOG_LIB) << "No user found" << _user;
        return QString();
    }

    bool status = userObj->isPasswordValid(_password);
    if (!status) {
        qCInfo(LOG_LIB) << "User password invalid for" << _user;
        return QString();
    }

    auto time = QDateTime::currentDateTimeUtc();
    QDateTime expiry = time.addDays(tokenExpiration());
    emit(userLoggedIn(userObj->index(), time));
    return m_tokens->registerToken(expiry);
}


/**
 * @fn authorize
 */
bool QueuedUserManager::authorize(const QString &_user, const QString &_token,
                                  const QueuedEnums::Permission _service)
{
    qCDebug(LOG_LIB) << "Authorize user" << _user << "for"
                     << static_cast<int>(_service);

    bool status = m_tokens->isTokenValid(_token);
    if (!status) {
        qCInfo(LOG_LIB) << "Token invalid for user" << _user;
        return false;
    }

    if (!m_users.contains(_user)) {
        qCInfo(LOG_LIB) << "No user found" << _user;
        return false;
    }

    return m_users[_user]->hasPermission(_service);
}


/**
 * @fn ids
 */
QPair<unsigned int, unsigned int> QueuedUserManager::ids(const long long _id)
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
        add(userData, userData[QString("_id")].toLongLong());
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
QueuedUser *QueuedUserManager::user(const QString &_name)
{
    qCDebug(LOG_LIB) << "Look for user" << _name;

    return m_users.value(_name, nullptr);
}


/*
 * @fn tokenExpiration
 */
long long QueuedUserManager::tokenExpiration() const
{
    return m_tokenExpiration;
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
