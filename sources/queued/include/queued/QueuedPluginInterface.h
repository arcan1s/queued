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
 * @file QueuedPluginInterface.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDPLUGININTERFACE_H
#define QUEUEDPLUGININTERFACE_H

#include <QtPlugin>

#include "QueuedConfig.h"


class QueuedPluginManagerInterface;

/**
 * @brief plugin interface for queued
 */
class QueuedPluginInterface
{
public:
    /**
     * @brief QueuedPluginInterface class destructor
     */
    virtual ~QueuedPluginInterface(){};
    /**
     * @brief will be emitted to map interface signals to plugin slots
     * @param _manager
     * pointer to manager interface object
     */
    virtual void connect(const QueuedPluginManagerInterface *_manager) = 0;
    /**
     * @brief plugin initialization method
     * @param _settings
     * plugin settings structure from database
     * @remark plugin settings will be stored as "plugin.name.Key"
     */
    virtual void init(const QVariantHash &_settings) = 0;
    /**
     * @brief set plugin token
     * @remark this method may be safety ignored if plugin does not use methods
     * require auth
     * @param _token
     * new token ID
     */
    virtual void setToken(const QString &_token) = 0;
    /**
     * @brief method which will be called on option update
     * @param _key
     * option key
     * @param _value
     * option value
     */
    virtual void updateSettings(const QString &_key, const QVariant &_value)
        = 0;
};

Q_DECLARE_INTERFACE(QueuedPluginInterface, PLUGIN_INTERFACE_NAME)


#endif /* QUEUEDPLUGININTERFACE_H */
