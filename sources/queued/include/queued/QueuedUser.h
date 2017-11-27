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
 * @file QueuedUser.h
 * Header of Queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDUSER_H
#define QUEUEDUSER_H

#include <QProcess>
#include <QVariant>

#include "QueuedEnums.h"


namespace QueuedLimits
{
struct Limits;
};

/**
 * @brief representation of user in queued
 */
class QueuedUser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(long long index READ index)
    // generic properties
    Q_PROPERTY(QString email READ email WRITE setEmail)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString password READ password WRITE setPassword)
    Q_PROPERTY(uint permissions READ permissions WRITE setPermissions)
    Q_PROPERTY(uint priority READ priority WRITE setPriority)
    // limits
    Q_PROPERTY(QString limits READ limits WRITE setLimits)

public:
    /**
     * @struct QueuedUserDefinition
     * @brief structure to define user
     * @var name
     * user name
     * @var email
     * user email
     * @var password
     * password hash, may be empty
     * @var permissions
     * user permissions
     * @var limits
     * user defined limits
     */
    struct QueuedUserDefinitions {
        QString name;
        QString email;
        QString password;
        uint permissions = 0;
        uint priority = 0;
        QString limits;
    };

    /**
     * @brief QueuedUser class constructor
     * @param _parent
     * pointer to parent item
     * @param definitions
     * definitions of user
     * @param index
     * index of process
     */
    explicit QueuedUser(QObject *_parent,
                        const QueuedUserDefinitions &definitions,
                        const long long index);
    /**
     * @brief QueuedUser class destructor
     */
    virtual ~QueuedUser();
    // methods
    /**
     * @brief add permissions to user
     * @param _permission
     * new user permission
     * @return current user permissions
     */
    QueuedEnums::Permissions
    addPermission(const QueuedEnums::Permission _permission);
    /**
     * @brief generates SHA512 hash from given password
     * @param _password
     * password as string
     * @param _salt
     * password salt if any
     * @return SHA512 of password
     */
    static QString hashFromPassword(const QString &_password,
                                    const QString &_salt);
    /**
     * @brief test user permissions
     * @param _permission
     * permission to test
     * @return true if user has permission otherwise return false
     */
    bool hasPermission(const QueuedEnums::Permission _permission);
    /**
     * @brief get UID and GID from user ID
     * @return pair of {uid, gid}
     */
    QPair<uint, uint> ids();
    /**
     * @brief check if password is valid
     * @param _password
     * password as string
     * @param _salt
     * password salt if any
     * @return true if password matches stored hash
     */
    bool isPasswordValid(const QString &_password, const QString &_salt) const;
    /**
     * @brief remove permissions from user
     * @param _permission
     * permission to remove
     * @return current user permissions
     */
    QueuedEnums::Permissions
    removePermission(const QueuedEnums::Permission _permission);
    // main properties
    /**
     * @brief user email
     * @return assigned user email
     */
    QString email() const;
    /**
     * @brief index of user
     * @return assigned index of user
     */
    long long index() const;
    /**
     * @brief username
     * @return name of user associated with system one
     */
    QString name() const;
    /**
     * @brief user limits
     * @return user limits in native format
     */
    QueuedLimits::Limits nativeLimits() const;
    /**
     * @brief user password
     * @return SHA512 of user password
     */
    QString password() const;
    /**
     * @brief user permissions
     * @return sum of user permissions from QueuedUser::Permissions
     */
    uint permissions() const;
    /**
     * @brief user max priority
     * @return user maximal priority value
     */
    uint priority() const;
    // permissions
    /**
     * @brief user limits
     * @return user limits
     */
    QString limits() const;
    // main properties
    /**
     * @brief set user email
     * @param _email
     * new user email
     */
    void setEmail(const QString _email);
    /**
     * @brief set username
     * @param _name
     * new user name
     */
    void setName(const QString _name);
    /**
     * @brief set user password
     * @param _password
     * new user password
     */
    void setPassword(const QString _password);
    /**
     * @brief set user permissions
     * @param _permissions
     * new user permissions
     */
    void setPermissions(const uint _permissions);
    /**
     * @brief set user priority
     * @param _priority
     * new user priority value
     */
    void setPriority(const uint _priority);
    // permissions
    /**
     * @brief set limits
     * @param _limits
     * new user limits
     */
    void setLimits(const QString &_limits);
    /**
     * @brief equal operator implementation
     * @param _other
     * other object
     * @return true if objects are equal
     */
    bool operator==(const QueuedUser &_other);

private:
    /**
     * @brief process definitions
     */
    QueuedUserDefinitions m_definitions;
    /**
     * @brief index of user
     */
    long long m_index = -1;
};

#endif /* QUEUEDUSER_H */
