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
 * @file QueuedUser.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include "queued/Queued.h"

#include <QCryptographicHash>

extern "C" {
#include <pwd.h>
}


/**
 * @class QueuedUser
 */
/**
 * @fn QueuedUser
 */
QueuedUser::QueuedUser(QObject *parent,
                       const QueuedUserDefinitions &definitions,
                       const long long index)
    : QObject(parent)
    , m_definitions(definitions)
    , m_index(index)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    qRegisterMetaType<QueuedLimits::Limits>("QueuedLimits::Limits");
}


/**
 * @fn ~QueuedUser
 */
QueuedUser::~QueuedUser()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;
}


/**
 * @fn addPermissions
 */
QueuedEnums::Permissions
QueuedUser::addPermission(const QueuedEnums::Permission _permission)
{
    qCDebug(LOG_LIB) << "Add user permission" << static_cast<uint>(_permission);

    setPermissions(
        static_cast<QueuedEnums::Permissions>(m_definitions.permissions)
        | _permission);

    return static_cast<QueuedEnums::Permissions>(m_definitions.permissions);
}


/**
 * @fn hashFromPassword
 */
QString QueuedUser::hashFromPassword(const QString &_password)
{
    return QCryptographicHash::hash(_password.toUtf8(),
                                    QCryptographicHash::Sha512)
        .toHex();
}


/**
 * @fn hasPermission
 */
bool QueuedUser::hasPermission(const QueuedEnums::Permission _permission)
{
    qCDebug(LOG_LIB) << "Check permissions" << static_cast<int>(_permission);

    if (static_cast<QueuedEnums::Permissions>(m_definitions.permissions)
            .testFlag(QueuedEnums::Permission::SuperAdmin))
        return true;
    else
        return static_cast<QueuedEnums::Permissions>(m_definitions.permissions)
            .testFlag(_permission);
}


/**
 * @fn ids
 */
QPair<uint, uint> QueuedUser::ids()
{
    QPair<uint, uint> system = {1, 1};

    auto pwd = ::getpwnam(qPrintable(name()));
    if (!pwd) {
        qCWarning(LOG_LIB) << "No user found by name" << name();
        return system;
    }
    system = {pwd->pw_uid, pwd->pw_gid};

    return system;
}


/**
 * @fn isPasswordValid
 */
bool QueuedUser::isPasswordValid(const QString &_password) const
{
    return (m_definitions.password.toUtf8() == hashFromPassword(_password));
}


QueuedEnums::Permissions
QueuedUser::removePermission(const QueuedEnums::Permission _permission)
{
    qCDebug(LOG_LIB) << "Remove user permission"
                     << static_cast<uint>(_permission);

    if (hasPermission(_permission))
        setPermissions(
            static_cast<QueuedEnums::Permissions>(m_definitions.permissions)
            ^ _permission);

    return static_cast<QueuedEnums::Permissions>(m_definitions.permissions);
}


/**
 * @fn email
 */
QString QueuedUser::email() const
{
    return m_definitions.email;
}


/**
 * @fn index
 */
long long QueuedUser::index() const
{
    return m_index;
}


/**
 * @fn name
 */
QString QueuedUser::name() const
{
    return m_definitions.name;
}


/**
 * @fn nativeLimits
 */
QueuedLimits::Limits QueuedUser::nativeLimits() const
{
    return QueuedLimits::Limits(limits());
}


/**
 * @fn password
 */
QString QueuedUser::password() const
{
    return m_definitions.password;
}


/**
 * @fn permissions
 */
uint QueuedUser::permissions() const
{
    return m_definitions.permissions;
}


/**
 * @fn limits
 */
QString QueuedUser::limits() const
{
    return m_definitions.limits;
}


/**
 * @fn setEmail
 */
void QueuedUser::setEmail(const QString _email)
{
    qCDebug(LOG_LIB) << "New user email" << _email;

    m_definitions.email = _email;
}


/**
 * @fn setName
 */
void QueuedUser::setName(const QString _name)
{
    qCDebug(LOG_LIB) << "New user name" << _name;

    m_definitions.name = _name;
}


/**
 * @fn setPassword
 */
void QueuedUser::setPassword(const QString _password)
{
    qCDebug(LOG_LIB) << "New user passoword SHA" << _password;

    m_definitions.password = _password;
}


/**
 * @fn setPermissions
 */
void QueuedUser::setPermissions(const uint _permissions)
{
    qCDebug(LOG_LIB) << "New user permissions" << _permissions;

    m_definitions.permissions = _permissions;
}


/**
 * @fn setLimits
 */
void QueuedUser::setLimits(const QString &_limits)
{
    qCDebug(LOG_LIB) << "New user limits" << _limits;

    m_definitions.limits = _limits;
}


/**
 * @fn operator==
 */
bool QueuedUser::operator==(const QueuedUser &_other)
{
    return index() == _other.index();
}
