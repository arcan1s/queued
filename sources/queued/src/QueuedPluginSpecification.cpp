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
 * @file QueuedPluginSpecification.cpp
 * Source code of queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>

#include <QFile>
#include <QJsonParseError>


/**
 * @fn dumpSpecification
 */
QVariantHash QueuedPluginSpecification::dumpSpecification(const Plugin &_plugin)
{
    QVariantHash output = {{"author", _plugin.author},
                           {"description", _plugin.description},
                           {"homepage", _plugin.homepage},
                           {"license", _plugin.license}};

    QVariantHash options;
    for (auto opt : _plugin.options)
        options[opt.name] = QVariantHash({{"default", opt.defaultValue},
                                          {"description", opt.description},
                                          {"type", opt.type}});
    output["options"] = options;

    return output;
}


/**
 * @fn readSpecification
 */
QueuedPluginSpecification::Plugin
QueuedPluginSpecification::readSpecification(const QString &_path)
{
    qCDebug(LOG_PL) << "Read specification from" << _path;

    QFile jsonFile(_path);
    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCWarning(LOG_LIB) << "Could not open" << _path;
        return QueuedPluginSpecification::Plugin();
    }
    QString jsonText = jsonFile.readAll();
    jsonFile.close();

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonText.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError) {
        qCWarning(LOG_LIB) << "Parse error" << error.errorString();
        return QueuedPluginSpecification::Plugin();
    }
    auto map = jsonDoc.toVariant().toMap();

    // convert to hash
    QVariantHash hash;
    QVariantMap::const_iterator it;
    for (it = map.constBegin(); it != map.constEnd(); ++it)
        hash[it.key()] = it.value();

    return readSpecification(hash);
}


/**
 * @fn readSpecification
 */
QueuedPluginSpecification::Plugin
QueuedPluginSpecification::readSpecification(const QVariantHash &_map)
{
    qCDebug(LOG_PL) << "Read specification from" << _map;

    QueuedPluginSpecification::Plugin plugin;

    // easy part
    plugin.author = _map["author"].toString();
    plugin.description = _map["description"].toString();
    plugin.homepage = _map["homepage"].toString();
    plugin.license = _map["license"].toString();

    // now lets try to parse options
    auto options = _map["options"].toMap();
    for (auto &option : options.keys()) {
        auto optionMap = options[option].toMap();

        QueuedPluginSpecification::PluginOption optionObj;
        optionObj.name = option;
        optionObj.defaultValue = optionMap["default"];
        optionObj.description = optionMap["description"].toString();
        optionObj.type = optionMap["type"].toString();
        plugin.options.append(optionObj);
    }

    return plugin;
}
