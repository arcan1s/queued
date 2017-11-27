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
 * @file QueuedPluginManager.cpp
 * Source code of queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>

#include <QDir>
#include <QPluginLoader>
#include <QStandardPaths>


/**
 * @class QueuedPluginManager
 */
/**
 * @fn QueuedPluginManager
 */
QueuedPluginManager::QueuedPluginManager(QObject *parent, const QString &token)
    : QObject(parent)
    , m_token(token)
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    m_interface = new QueuedPluginManagerInterface();
}


/**
 * @fn ~QueuedPluginManager
 */
QueuedPluginManager::~QueuedPluginManager()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    QStringList plugins = m_plugins.keys();
    for (auto &plugin : plugins)
        unloadPlugin(plugin);

    m_interface->deleteLater();
}


/**
 * @fn convertOptionName
 */
QPair<QString, QString>
QueuedPluginManager::convertOptionName(const QString &_key)
{
    qCDebug(LOG_PL) << "Convert option name" << _key;

    QStringList fields = _key.split('.');
    if (fields.count() < 3) {
        qCWarning(LOG_PL) << "Invalid option name" << _key;
        return {"", ""};
    }
    // Plugin.
    fields.takeFirst();
    // plugin name
    QString plugin = fields.takeFirst();
    QString option = fields.join('.');

    return {plugin, option};
}


/**
 * @fn interface
 */
QueuedPluginManagerInterface *QueuedPluginManager::interface()
{
    return m_interface;
}


/**
 * @fn loadPlugin
 */
bool QueuedPluginManager::loadPlugin(const QString &_name,
                                     const QVariantHash &_settings)
{
    qCDebug(LOG_PL) << "Load plugin" << _name << "with settings" << _settings;

    // check if it was loaded already then call QueuedPluginInterface::init()
    if (m_plugins.contains(_name)) {
        m_plugins[_name]->init(_settings);
        m_plugins[_name]->setToken(m_token);
        return true;
    }

    // normal load
    QString libraryName = QString("lib%2.so").arg(_name);
    // init plugin settings with valid keys
    QVariantHash pluginSettings;
    for (auto &key : _settings.keys())
        pluginSettings[convertOptionName(key).first] = _settings[key];

    for (auto &dir : pluginLocations()) {
        if (!QDir(dir).entryList(QDir::Files).contains(libraryName))
            continue;
        // build plugin loader
        QPluginLoader loader(QString("%1/%2").arg(dir).arg(libraryName), this);
        qCInfo(LOG_PL) << "Loading" << loader.fileName();
        // load plugin
        QObject *plugin = loader.instance();
        QueuedPluginInterface *item = nullptr;
        if (loader.isLoaded())
            item = qobject_cast<QueuedPluginInterface *>(plugin);
        else
            qCCritical(LOG_PL) << "Could not load the library for" << _name
                               << "error" << loader.errorString();
        if (item) {
            m_plugins[_name] = item;
            item->init(pluginSettings);
            item->setToken(m_token);
            item->setup(interface());
        } else {
            qCCritical(LOG_PL) << "Could not cast plugin" << _name;
        }
        // do not try to load it from other paths
        break;
    }

    return m_plugins.contains(_name);
}


QueuedPluginSpecification::Plugin
QueuedPluginManager::loadSpecification(const QString &_name)
{
    qCDebug(LOG_PL) << "Load specification for" << _name;

    QString jsonName = QString("%1.json").arg(_name);
    QString path;
    for (auto &dir : pluginLocations()) {
        if (!QDir(dir).entryList(QDir::Files).contains(jsonName))
            continue;
        path = QString("%1/%2").arg(dir).arg(jsonName);
        break;
    }

    if (path.isEmpty()) {
        qCWarning(LOG_PL) << "Could not find" << jsonName;
        return QueuedPluginSpecification::Plugin();
    } else {
        return QueuedPluginSpecification::readSpecification(path);
    }
}


/**
 * @fn pluginLocations
 */
QStringList QueuedPluginManager::pluginLocations()
{
    QStringList locations = QStandardPaths::standardLocations(
        QStandardPaths::GenericDataLocation);
    for (auto &loc : locations)
        loc = QString("%1/%2/%3")
                  .arg(loc)
                  .arg(QueuedConfig::HOME_PATH)
                  .arg(QueuedConfig::PLUGIN_PATH);

    return locations;
}


/**
 * @fn unloadPlugin
 */
bool QueuedPluginManager::unloadPlugin(const QString &_name)
{
    qCDebug(LOG_PL) << "Unload plugin" << _name;

    if (!m_plugins.contains(_name)) {
        qCWarning(LOG_LIB) << "Could not find plugin" << _name;
        return false;
    }

    auto plugin = m_plugins.take(_name);
    delete plugin;

    return true;
}


/**
 * @fn optionChanged
 */
void QueuedPluginManager::optionChanged(const QString &_key,
                                        const QVariant &_value)
{
    qCDebug(LOG_PL) << "Option" << _key << "changed to" << _value;

    auto option = convertOptionName(_key);
    if (!m_plugins.contains(option.first)) {
        qCWarning(LOG_PL) << "Plugin" << option.first << "not found for"
                          << _key;
        return;
    }

    return m_plugins[option.first]->updateSettings(option.second, _value);
}
