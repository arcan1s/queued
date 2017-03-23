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
 * @file QueuedPluginManagerInterface.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDPLUGINMANAGERINTERFACE_H
#define QUEUEDPLUGINMANAGERINTERFACE_H

#include <QObject>


/**
 * @brief signal interface for plugin manager
 */
class QueuedPluginManagerInterface : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief QueuedPluginManagerInterface class constructor
     */
    explicit QueuedPluginManagerInterface()
        : QObject(nullptr){};
    /**
     * @brief QueuedPluginManagerInterface class destructor
     */
    virtual ~QueuedPluginManagerInterface(){};

signals:
    /**
     * @brief signal which emits on plugin addition
     * @param _plugin
     * plugin name
     */
    void onAddPlugin(const QString &_plugin);
    /**
     * @brief signal which emits on task addition
     * @param _id
     * generated task ID
     */
    void onAddTask(const long long _id);
    /**
     * @brief signal which emits on user addition
     * @param _id
     * generated user ID
     */
    void onAddUser(const long long _id);
    /**
     * @brief signal which emits on user authorization
     * @param _user
     * user name
     */
    void onAuthorization(const QString &_user);
    /**
     * @brief signal which emits on option edition
     * @param _option
     * option name
     * @param _value
     * option value
     */
    void onEditOption(const QString &_option, const QVariant &_value);
    /**
     * @brief signal which emits on task edition
     * @param _id
     * task ID
     * @param _taskData
     * new task data
     */
    void onEditTask(const long long _id, const QVariantHash &_taskData);
    /**
     * @brief signal which emits on user edition
     * @param _id
     * user ID
     * @param _userData
     * new user data
     */
    void onEditUser(const long long _id, const QVariantHash &_userData);
    /**
     * @brief signal which emits on plugin removal
     * @param _plugin
     * plugin name
     */
    void onRemovePlugin(const QString &_plugin);
    /**
     * @brief signal which emits on task starting
     * @param _id
     * started task ID
     */
    void onStartTask(const long long _id);
    /**
     * @brief signal which emits on task stoping
     * @param _id
     * stopped task ID
     */
    void onStopTask(const long long _id);
};


#endif /* QUEUEDPLUGINMANAGERINTERFACE_H */
