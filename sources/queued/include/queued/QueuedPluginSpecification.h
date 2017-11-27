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
 * @file QueuedPluginSpecification.h
 * Header of Queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDPLUGINSPECIFICATION_H
#define QUEUEDPLUGINSPECIFICATION_H

#include <QString>


/**
 * @addtogroup QueuedPluginSpecification
 * @brief plugin specification representation
 */
namespace QueuedPluginSpecification
{
/**
 * @struct PluginOption
 * @brief plugin option structure
 * @var PluginOption::defaultValue
 * option default value
 * @var PluginOption::description
 * option description
 * @var PluginOption::name
 * option name
 * @var PluginOption::type
 * option type
 */
struct PluginOption {
    QVariant defaultValue;
    QString description;
    QString name;
    QString type;
};
/**
 * @struct Plugin
 * @brief plugin specification structure
 * @var Plugin::author
 * plugin author
 * @var Plugin::description
 * plugin description
 * @var Plugin::homepage
 * plugin license
 * @var Plugin::license
 * plugin license
 * @param Plugin::options
 * plugin options
 */
struct Plugin {
    QString author;
    QString description;
    QString homepage;
    QString license;
    QList<PluginOption> options;
};
/**
 * @brief dump specification to map
 * @param _plugin
 * plugin object
 * @return specification dump
 */
QVariantHash dumpSpecification(const Plugin &_plugin);
/**
 * @brief init specification from file
 * @param _path
 * path to specification file
 * @return initialized object
 */
Plugin readSpecification(const QString &_path);
/**
 * @brief init specification from map
 * @param _map
 * specification map
 * @return initialized object
 */
Plugin readSpecification(const QVariantHash &_map);
};


#endif /* QUEUEDPLUGINSPECIFICATION_H */
