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
 * @file QueuedUser.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDUSER_H
#define QUEUEDUSER_H

#include <QProcess>
#include <QVariant>

#include "QueuedEnums.h"


/**
 * @struct QueuedUserDefinition
 * @brief structure to define user
 * @var name
 * user name
 * @var email
 * user email
 * @var passwordSHA512
 * password hash, may be empty
 * @var permissions
 * user permissions
 * @var cpuLimit
 * user limit by CPU
 * @var gpuLimit
 * user limit by GPU
 * @var memoryLimit
 * user limit by memory
 * @var gpumemoryLimit
 * user limit by GPU memory
 * @var storageLimit
 * user limit by storage
 */
typedef struct {
    QString name;
    QString email;
    QString passwordSHA512;
    unsigned int permissions;
    long long cpuLimit;
    long long gpuLimit;
    long long memoryLimit;
    long long gpumemoryLimit;
    long long storageLimit;
} QueuedUserDefinitions;


/**
 * @brief representation of user in queued
 */
class QueuedUser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(long long index READ index)
    // generic properties
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY userUpdated)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY userUpdated)
    Q_PROPERTY(
        QString password READ password WRITE setPassword NOTIFY userUpdated)
    Q_PROPERTY(unsigned int permissions READ permissions WRITE setPermissions
                   NOTIFY userUpdated)
    // limits
    Q_PROPERTY(
        long long cpuLimit READ cpuLimit WRITE setCpuLimit NOTIFY userUpdated)
    Q_PROPERTY(
        long long gpuLimit READ gpuLimit WRITE setGpuLimit NOTIFY userUpdated)
    Q_PROPERTY(long long memoryLimit READ memoryLimit WRITE setMemoryLimit
                   NOTIFY userUpdated)
    Q_PROPERTY(long long gpumemoryLimit READ gpumemoryLimit WRITE
                   setGpumemoryLimit NOTIFY userUpdated)
    Q_PROPERTY(long long storageLimit READ storageLimit WRITE setStorageLimit
                   NOTIFY userUpdated)

public:
    /**
     * @brief QueuedUser class constructor
     * @param parent         pointer to parent item
     * @param definitions    definitions of user
     * @param index          index of process
     */
    explicit QueuedUser(QObject *parent,
                        const QueuedUserDefinitions &definitions,
                        const long long index);
    /**
     * @brief QueuedUser class destructor
     */
    virtual ~QueuedUser();
    // methods
    /**
     * @brief add permissions to user
     * @param _permissions   new user permissions
     * @return current user permissions
     */
    QueuedEnums::Permissions
    addPermissions(const QueuedEnums::Permissions _permissions);
    /**
     * @brief generates SHA512 hash from given password
     * @param _password      password as string
     * @return SHA512 of password
     */
    static QString hashFromPassword(const QString &_password);
    /**
     * @brief test user permissions
     * @param _permission    permission to test
     * @return true if user has permission otherwise return false
     */
    bool hasPermission(const QueuedEnums::Permission _permission);
    /**
     * @brief check if password is valid
     * @param _password     password as string
     * @return true if password matches stored hash
     */
    bool isPasswordValid(const QString &_password) const;
    /**
     * @brief remove permissions from user
     * @param _permissions   permissions to remove
     * @return current user permissions
     */
    QueuedEnums::Permissions
    removePermissions(const QueuedEnums::Permissions _permissions);
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
     * @brief user password
     * @return SHA512 of user password
     */
    QString password() const;
    /**
     * @brief user permissions
     * @return sum of user permissions from QueuedUser::Permissions
     */
    unsigned int permissions() const;
    // permissions
    /**
     * @brief cpu limit
     * @return cpu limit in cores
     */
    long long cpuLimit() const;
    /**
     * @brief gpu limit
     * @return gpu limit in cores
     */
    long long gpuLimit() const;
    /**
     * @brief memory limit
     * @return memory limit in bytes
     */
    long long memoryLimit() const;
    /**
     * @brief GPU memory limit
     * @return GPU memory limit in bytes
     */
    long long gpumemoryLimit() const;
    /**
     * @brief storage limit
     * @return storage limit in bytes
     */
    long long storageLimit() const;
    // main properties
    /**
     * @brief set user email
     * @param _email         new user email
     */
    void setEmail(const QString _email);
    /**
     * @brief set username
     * @param _name          new user name
     */
    void setName(const QString _name);
    /**
     * @brief set user password
     * @param _password      new user password
     */
    void setPassword(const QString _password);
    /**
     * @brief set user permissions
     * @param _permissions   new user permissions
     */
    void setPermissions(const unsigned int _permissions);
    // permissions
    /**
     * @brief set cpu limit
     * @param _cpuLimit      new cpu limit in cores
     */
    void setCpuLimit(const long long _cpuLimit);
    /**
     * @brief set gpu limit
     * @param _gpuLimit      new gpu limit in cores
     */
    void setGpuLimit(const long long _gpuLimit);
    /**
     * @brief set memory limit
     * @param _memoryLimit   new memory limit in bytes
     */
    void setMemoryLimit(const long long _memoryLimit);
    /**
     * @brief set GPU memory limit
     * @param _gpumemoryLimit new GPU memory limit in bytes
     */
    void setGpumemoryLimit(const long long _gpumemoryLimit);
    /**
     * @brief set storage limit
     * @param _storageLimit  new storage limit in bytes
     */
    void setStorageLimit(const long long _storageLimit);
    /**
     * @brief equal operator implementation
     * @param _other         other object
     * @return true if objects are equal
     */
    bool operator==(const QueuedUser &_other);

signals:
    /**
     * @brief signal which emits on any property set action
     */
    void userUpdated();

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
