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
 * @file QueuedUser.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include "queued/Queued.h"

#include <QCryptographicHash>


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
QueuedUser::addPermissions(const QueuedEnums::Permissions _permissions)
{
    qCDebug(LOG_LIB) << "Add user permission" << _permissions;

    setPermissions(
        static_cast<QueuedEnums::Permissions>(m_definitions.permissions)
        & _permissions);

    return static_cast<QueuedEnums::Permissions>(m_definitions.permissions);
}


/**
 * @fn hashFromPassword
 */
QString QueuedUser::hashFromPassword(const QString &_password)
{
    return QCryptographicHash::hash(_password.toUtf8(),
                                    QCryptographicHash::Sha512);
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
 * @fn isPasswordValid
 */
bool QueuedUser::isPasswordValid(const QString &_password) const
{
    return (m_definitions.passwordSHA512.toUtf8()
            == QCryptographicHash::hash(_password.toUtf8(),
                                        QCryptographicHash::Sha512));
}


QueuedEnums::Permissions
QueuedUser::removePermissions(const QueuedEnums::Permissions _permissions)
{
    qCDebug(LOG_LIB) << "Remove user permission" << _permissions;

    setPermissions(
        static_cast<QueuedEnums::Permissions>(m_definitions.permissions)
        & ~_permissions);

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
 * @fn password
 */
QString QueuedUser::password() const
{
    return m_definitions.passwordSHA512;
}


/**
 * @fn permissions
 */
unsigned int QueuedUser::permissions() const
{
    return m_definitions.permissions;
}


/**
 * @fn cpuLimit
 */
long long QueuedUser::cpuLimit() const
{
    return m_definitions.cpuLimit;
}


/**
 * @fn gpuLimit
 */
long long QueuedUser::gpuLimit() const
{
    return m_definitions.gpuLimit;
}


/**
 * @fn memoryLimit
 */
long long QueuedUser::memoryLimit() const
{
    return m_definitions.memoryLimit;
}


/**
 * @fn gpumemoryLimit
 */
long long QueuedUser::gpumemoryLimit() const
{
    return m_definitions.gpumemoryLimit;
}


/**
 * @fn storageLimit
 */
long long QueuedUser::storageLimit() const
{
    return m_definitions.storageLimit;
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

    m_definitions.passwordSHA512 = _password;
}


/**
 * @fn setPermissions
 */
void QueuedUser::setPermissions(const unsigned int _permissions)
{
    qCDebug(LOG_LIB) << "New user permissions" << _permissions;

    m_definitions.permissions = _permissions;
}


/**
 * @fn setCpuLimit
 */
void QueuedUser::setCpuLimit(const long long _cpuLimit)
{
    qCDebug(LOG_LIB) << "New user cpu limit" << _cpuLimit;

    m_definitions.cpuLimit = _cpuLimit;
}


/**
 * @fn setGpuLimit
 */
void QueuedUser::setGpuLimit(const long long _gpuLimit)
{
    qCDebug(LOG_LIB) << "New user gpu limit" << _gpuLimit;

    m_definitions.gpuLimit = _gpuLimit;
}


/**
 * @fn setMemoryLimit
 */
void QueuedUser::setMemoryLimit(const long long _memoryLimit)
{
    qCDebug(LOG_LIB) << "New user memory limit" << _memoryLimit;

    m_definitions.memoryLimit = _memoryLimit;
}


/**
 * @fn setGpumemoryLimit
 */
void QueuedUser::setGpumemoryLimit(const long long _gpumemoryLimit)
{
    qCDebug(LOG_LIB) << "New user gpu memory limit" << _gpumemoryLimit;

    m_definitions.gpumemoryLimit = _gpumemoryLimit;
}


/**
 * @fn setStorageLimit
 */
void QueuedUser::setStorageLimit(const long long _storageLimit)
{
    qCDebug(LOG_LIB) << "New user storage limit" << _storageLimit;

    m_definitions.storageLimit = _storageLimit;
}


/**
 * @fn operator==
 */
bool QueuedUser::operator==(const QueuedUser &_other)
{
    return index() == _other.index();
}
