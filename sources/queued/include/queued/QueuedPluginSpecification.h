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

#include <QDebug>
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
 * @brief DBus marshalling method
 * @param _argument
 * output DBus argument
 * @param _arg
 * input variant object
 * @return appended argument body
 */
inline QDBusArgument &operator<<(QDBusArgument &_argument, const PluginOption &_arg)
{
    _argument.beginStructure();
    _argument << QDBusVariant(_arg.defaultValue.isValid() ? _arg.defaultValue : "");
    _argument << _arg.description;
    _argument << _arg.name;
    _argument << _arg.type;
    _argument.endStructure();

    return _argument;
};
/**
 * @brief DBus un-marshalling method
 * @param _argument
 * input DBus argument
 * @param _arg
 * output variant object
 * @return source DBus argument
 */
inline const QDBusArgument &operator>>(const QDBusArgument &_argument, PluginOption &_arg)
{
    QDBusVariant variant;

    _argument.beginStructure();
    _argument >> variant;
    _arg.defaultValue = variant.variant();
    _argument >> _arg.description;
    _argument >> _arg.name;
    _argument >> _arg.type;
    _argument.endStructure();

    return _argument;
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
 * @brief DBus marshalling method
 * @param _argument
 * output DBus argument
 * @param _arg
 * input variant object
 * @return appended argument body
 */
inline QDBusArgument &operator<<(QDBusArgument &_argument, const Plugin &_arg)
{
    _argument.beginStructure();
    _argument << _arg.author;
    _argument << _arg.description;
    _argument << _arg.homepage;
    _argument << _arg.license;
    _argument << _arg.options;
    _argument.endStructure();

    return _argument;
};
/**
 * @brief DBus un-marshalling method
 * @param _argument
 * input DBus argument
 * @param _arg
 * output variant object
 * @return source DBus argument
 */
inline const QDBusArgument &operator>>(const QDBusArgument &_argument, Plugin &_arg)
{
    _argument.beginStructure();
    _argument >> _arg.author;
    _argument >> _arg.description;
    _argument >> _arg.homepage;
    _argument >> _arg.license;
    _argument >> _arg.options;
    _argument.endStructure();

    return _argument;
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
}; // namespace QueuedPluginSpecification


#endif /* QUEUEDPLUGINSPECIFICATION_H */
