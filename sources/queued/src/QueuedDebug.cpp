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
 * @file QueuedDebug.cpp
 * Source code of queued library
 * @author Queued team
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>

#include "version.h"


Q_LOGGING_CATEGORY(LOG_APP, "org.queued.application", QtMsgType::QtWarningMsg)
Q_LOGGING_CATEGORY(LOG_CTL, "org.queued.control", QtMsgType::QtWarningMsg)
Q_LOGGING_CATEGORY(LOG_DBUS, "org.queued.dbus", QtMsgType::QtWarningMsg)
Q_LOGGING_CATEGORY(LOG_LIB, "org.queued.library", QtMsgType::QtWarningMsg)
Q_LOGGING_CATEGORY(LOG_PL, "org.queued.plugin", QtMsgType::QtWarningMsg)
Q_LOGGING_CATEGORY(LOG_SERV, "org.queued.server", QtMsgType::QtWarningMsg)


/**
 * @fn applyLogFormat
 */
void QueuedDebug::applyLogFormat()
{
    qSetMessagePattern(QueuedDebug::LOG_FORMAT);
}


/**
 * @fn enableDebug
 */
void QueuedDebug::enableDebug()
{
    QLoggingCategory::setFilterRules("org.queued.*=true");
}


/**
 * @fn getBuildData
 */
QStringList QueuedDebug::getBuildData()
{
    QStringList metadata;

    auto data = getBuildMetaData();
    auto sections = data.keys();
    sections.sort();
    for (auto &section : sections) {
        metadata += section;
        auto keys = data[section].keys();
        keys.sort();
        for (auto &key : keys)
            metadata += QString("\t%1: %2").arg(key).arg(data[section][key]);
    }

    return metadata;
}


/**
 * @fn getBuildMetaData
 */
QHash<QString, QHash<QString, QString>> QueuedDebug::getBuildMetaData()
{
    return {// general information
            {"Build details",
             {
                 {"VERSION", VERSION},
                 {"COMMIT_SHA", COMMIT_SHA},
                 {"BUILD_DATE", BUILD_DATE},
                 {"QT_VERSION", qVersion()},
             }},
            // api
            {"API",
             {
                 {"DATABASE_VERSION",
                  QString::number(QueuedConfig::DATABASE_VERSION)},
                 {"PLUGIN_INTERFACE", QueuedConfig::PLUGIN_INTERFACE},
                 {"WEBAPI_TOKEN_HEADER", QueuedConfig::WEBAPI_TOKEN_HEADER},
                 {"WEBAPI_VERSIONS",
                  std::accumulate(
                      std::next(std::begin(QueuedConfig::WEBAPI_VERSIONS)),
                      std::end(QueuedConfig::WEBAPI_VERSIONS),
                      QString::number(QueuedConfig::WEBAPI_VERSIONS[0]),
                      [](const QString str, const int version) {
                          return QString("%1,%2").arg(str).arg(version);
                      })},
             }},
            // paths
            {"Paths",
             {
                 {"BIN_INSTALL_DIR", QueuedConfig::BIN_INSTALL_DIR},
                 {"DATA_INSTALL_DIR", QueuedConfig::DATA_INSTALL_DIR},
                 {"INCLUDE_INSTALL_DIR", QueuedConfig::INCLUDE_INSTALL_DIR},
                 {"LIB_INSTALL_DIR", QueuedConfig::LIB_INSTALL_DIR},
                 {"ROOT_INSTALL_DIR", QueuedConfig::ROOT_INSTALL_DIR},
                 {"HOME_PATH", QueuedConfig::HOME_PATH},
                 {"PLUGIN_PATH", QueuedConfig::PLUGIN_PATH},
             }},
            // dbus
            {"DBus",
             {
                 {"DBUS_SERVICE", QueuedConfig::DBUS_SERVICE},
                 {"DBUS_APPLICATION_PATH", QueuedConfig::DBUS_APPLICATION_PATH},
                 {"DBUS_OBJECT_PATH", QueuedConfig::DBUS_OBJECT_PATH},
                 {"DBUS_PROPERTY_PATH", QueuedConfig::DBUS_PROPERTY_PATH},
             }},
            // cmake build information
            {"Cmake properties",
             {
                 {"CMAKE_BUILD_TYPE", CMAKE_BUILD_TYPE},
                 {"CMAKE_CXX_COMPILER", CMAKE_CXX_COMPILER},
                 {"CMAKE_CXX_FLAGS", CMAKE_CXX_FLAGS},
                 {"CMAKE_CXX_FLAGS_DEBUG", CMAKE_CXX_FLAGS_DEBUG},
                 {"CMAKE_CXX_FLAGS_RELEASE", CMAKE_CXX_FLAGS_RELEASE},
                 {"CMAKE_CXX_FLAGS_OPTIMIZATION", CMAKE_CXX_FLAGS_OPTIMIZATION},
                 {"CMAKE_DEFINITIONS", CMAKE_DEFINITIONS},
                 {"CMAKE_INSTALL_PREFIX", CMAKE_INSTALL_PREFIX},
                 {"CMAKE_MODULE_LINKER_FLAGS", CMAKE_MODULE_LINKER_FLAGS},
                 {"CMAKE_SHARED_LINKER_FLAGS", CMAKE_SHARED_LINKER_FLAGS},
             }},
            // components
            {"Components",
             {
                 {"BUILD_DEB_PACKAGE", BUILD_DEB_PACKAGE},
                 {"BUILD_RPM_PACKAGE", BUILD_RPM_PACKAGE},
                 {"CLANGFORMAT_EXECUTABLE", CLANGFORMAT_EXECUTABLE},
                 {"COVERITY_COMMENT", COVERITY_COMMENT},
                 {"COVERITY_DIRECTORY", COVERITY_DIRECTORY},
                 {"COVERITY_EMAIL", COVERITY_EMAIL},
                 {"COVERITY_EXECUTABLE", COVERITY_EXECUTABLE},
                 {"COVERITY_URL", COVERITY_URL},
                 {"CPPCHECK_EXECUTABLE", CPPCHECK_EXECUTABLE},
             }},
            // additional properties
            {"Additional properties",
             {
                 {"PROP_DOCS", PROP_DOCS},
                 {"PROP_FUTURE", PROP_FUTURE},
                 {"PROP_LOAD", PROP_LOAD},
                 {"PROP_TEST", PROP_TEST},
             }}};
}
