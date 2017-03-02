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
 * @file QueuedCoreInterface.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDCOREINTERFACE_H
#define QUEUEDCOREINTERFACE_H

#include <QDBusAbstractAdaptor>
#include <QDBusVariant>

#include "QueuedConfig.h"


class QueuedCore;

/**
 * @brief DBus interface for QueuedCore class
 */
class QueuedCoreInterface : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", DBUS_SERVICE_NAME)

public:
    /**
     * @brief QueuedCoreInterface class constructor
     * @param parent
     * pointer to QueuedCore object
     */
    explicit QueuedCoreInterface(QueuedCore *parent);
    /**
     * @brief QueuedCoreInterface class destructor
     */
    virtual ~QueuedCoreInterface();

public slots:
    /**
     * @brief auth user by password
     * @param name
     * user name
     * @param password
     * user password
     * @return generated token ID or empty string in case of invalid password
     */
    QString Auth(const QString &name, const QString &password);
    /**
     * @brief edit option
     * @param key
     * option key to edit
     * @param value
     * option value to edit
     * @param whoAmI
     * auth user name
     * @param token
     * auth user token
     * @return true on successful option edition
     */
    bool OptionEdit(const QString &key, const QDBusVariant &value,
                    const QString &whoAmI, const QString &token);
    /**
     * @brief add new task
     * @param command
     * command line
     * @param arguments
     * command line arguments
     * @param workingDirectory
     * working directory
     * @param nice
     * nice level
     * @param user
     * user ID
     * @param cpu
     * limit by CPU cores
     * @param gpu
     * limit by GPU cores
     * @param memory
     * limit by memory
     * @param gpumemory
     * limit by GPU memory
     * @param storage
     * limit by storage
     * @param whoAmI
     * auth user name
     * @param token
     * auth user token
     * @return true on successful task addition
     */
    bool TaskAdd(const QString &command, const QStringList &arguments,
                 const QString &workingDirectory, const uint nice,
                 const long long user, const long long cpu, const long long gpu,
                 const QString &memory, const QString &gpumemory,
                 const QString &storage, const QString &whoAmI,
                 const QString &token);
    /**
     * @brief edit task
     * @param id
     * task ID
     * @param data
     * new task data
     * @param whoAmI
     * auth user name
     * @param token
     * auth user token
     * @return true on successful task edition
     */
    bool TaskEdit(const qlonglong id, const QDBusVariant &data,
                  const QString &whoAmI, const QString &token);
    /**
     * @brief force start task
     * @param id
     * task ID
     * @param whoAmI
     * auth user name
     * @param token
     * auth user token
     * @return true on successful task start
     */
    bool TaskStart(const qlonglong id, const QString &whoAmI,
                   const QString &token);
    /**
     * @brief force stop task
     * @param id
     * task ID
     * @param whoAmI
     * auth user name
     * @param token
     * auth user token
     * @return true on successful task stop
     */
    bool TaskStop(const qlonglong id, const QString &whoAmI,
                  const QString &token);
    /**
     * @brief add new user
     * @param name
     * user name
     * @param email
     * user email
     * @param password
     * user password
     * @param permissions
     * user permissions
     * @param cpu
     * limit by CPU cores
     * @param gpu
     * limit by GPU cores
     * @param memory
     * limit by memory
     * @param gpumemory
     * limit by GPU memory
     * @param storage
     * limit by storage
     * @param whoAmI
     * auth user name
     * @param token
     * auth user token
     * @return true on successful task addition
     */
    bool UserAdd(const QString &name, const QString &email,
                 const QString &password, const uint permissions,
                 const long long cpu, const long long gpu,
                 const QString &memory, const QString &gpumemory,
                 const QString &storage, const QString &whoAmI,
                 const QString &token);
    /**
     * @brief edit user
     * @param id
     * user ID
     * @param data
     * new user data
     * @param whoAmI
     * auth user name
     * @param token
     * auth user token
     * @return true on successful user edition
     */
    bool UserEdit(const qlonglong id, const QDBusVariant &data,
                  const QString &whoAmI, const QString &token);
    /**
     * @brief add permission to user
     * @param id
     * user ID
     * @param permission
     * permission to add
     * @param whoAmI
     * auth user name
     * @param token
     * auth user token
     * @return true on successful permission addition
     */
    bool UserPermissionAdd(const qlonglong id, const uint permission,
                           const QString &whoAmI, const QString &token);
    /**
     * @brief remove permission from user
     * @param id
     * user ID
     * @param permission
     * permission to remove
     * @param whoAmI
     * auth user name
     * @param token
     * auth user token
     * @return true on successful permission removal
     */
    bool UserPermissionRemove(const qlonglong id, const uint permission,
                              const QString &whoAmI, const QString &token);

private:
    /**
     * @brief pointer to database object
     */
    QueuedCore *m_core = nullptr;
};


#endif /* QUEUEDCOREINTERFACE_H */
