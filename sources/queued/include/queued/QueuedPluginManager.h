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
 * @file QueuedPluginManager.h
 * Header of Queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDPLUGINMANAGER_H
#define QUEUEDPLUGINMANAGER_H

#include <QHash>
#include <QObject>
#include <QPair>


class QueuedPluginInterface;
class QueuedPluginManagerInterface;

/**
 * @brief report manager for queued
 */
class QueuedPluginManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @typedef QueuedPluginMap
     * map of names to QueuedPluginInterface pointers
     */
    typedef QHash<QString, QueuedPluginInterface *> QueuedPluginMap;

    /**
     * @brief QueuedPluginManager class constructor
     * @param parent
     * pointer to parent item
     * @param token
     * plugin auth token
     */
    explicit QueuedPluginManager(QObject *parent, const QString &token);
    /**
     * @brief QueuedPluginManager class destructor
     */
    virtual ~QueuedPluginManager();
    /**
     * @brief get option name from database format
     * @param _key
     * option key
     * @return plugin name and option key in plugin format
     */
    static QPair<QString, QString> convertOptionName(const QString &_key);
    /**
     * @brief plugin manager interface
     * @return pointer to plugin manager interface
     */
    QueuedPluginManagerInterface *interface();
    /**
     * @brief load plugin
     * @param _name
     * plugin name
     * @param _settings
     * plugin settings
     * @return true on success load otherwise return false
     */
    bool loadPlugin(const QString &_name, const QVariantHash &_settings);
    /**
     * @brief path to plugin location
     * @return full path to plugin location
     */
    QStringList pluginLocations() const;
    /**
     * @brief unload plugin
     * @param _name
     * plugin name
     * @return true on success unload otherwise return false
     */
    bool unloadPlugin(const QString &_name);

public slots:
    /**
     * @brief notifies plugin about option changes
     * @param _key
     * database option key
     * @param _value
     * option value
     */
    void optionChanged(const QString &_key, const QVariant &_value);

private:
    /**
     * @brief pointer to database object
     */
    QueuedPluginManagerInterface *m_interface = nullptr;
    /**
     * @brief loaded plugins
     */
    QueuedPluginMap m_plugins;
    /**
     * @brief plugin auth token
     */
    QString m_token;
};


#endif /* QUEUEDPLUGINMANAGER_H */
