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
 * @file QueuedDebug.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include "queued/Queued.h"

#include "version.h"


Q_LOGGING_CATEGORY(LOG_APP, "org.queued.application", QtMsgType::QtWarningMsg)
Q_LOGGING_CATEGORY(LOG_CTL, "org.queued.control", QtMsgType::QtWarningMsg)
Q_LOGGING_CATEGORY(LOG_DBUS, "org.queued.dbus", QtMsgType::QtWarningMsg)
Q_LOGGING_CATEGORY(LOG_LIB, "org.queued.library", QtMsgType::QtWarningMsg)
Q_LOGGING_CATEGORY(LOG_PL, "org.queued.plugin", QtMsgType::QtWarningMsg)
Q_LOGGING_CATEGORY(LOG_SERV, "org.queued.server", QtMsgType::QtWarningMsg)


/**
 * @fn enableDebug
 */
void QueuedDebug::enableDebug()
{
    QLoggingCategory::setFilterRules(QString("org.queued.*=true"));
}


/**
 * @fn getBuildData
 */
QStringList QueuedDebug::getBuildData()
{
    QStringList metadata;

    // general information
    metadata += "Build details";
    metadata += QString("\tVERSION: %1").arg(VERSION);
    metadata += QString("\tCOMMIT_SHA: %1").arg(COMMIT_SHA);
    metadata += QString("\tBUILD_DATE: %1").arg(BUILD_DATE);

    // api
    metadata += "API";
    metadata += QString("\tDATABASE_VERSION: %1")
                    .arg(QueuedConfig::DATABASE_VERSION);
    metadata += QString("\tPLUGIN_INTERFACE: %1")
                    .arg(QueuedConfig::PLUGIN_INTERFACE);

    // paths
    metadata += "Paths";
    metadata
        += QString("\tBIN_INSTALL_DIR: %1").arg(QueuedConfig::BIN_INSTALL_DIR);
    metadata += QString("\tDATA_INSTALL_DIR: %1")
                    .arg(QueuedConfig::DATA_INSTALL_DIR);
    metadata += QString("\tINCLUDE_INSTALL_DIR: %1")
                    .arg(QueuedConfig::INCLUDE_INSTALL_DIR);
    metadata
        += QString("\tLIB_INSTALL_DIR: %1").arg(QueuedConfig::LIB_INSTALL_DIR);
    metadata += QString("\tROOT_INSTALL_DIR: %1")
                    .arg(QueuedConfig::ROOT_INSTALL_DIR);
    metadata += QString("\tHOME_PATH: %1").arg(QueuedConfig::HOME_PATH);
    metadata += QString("\tPLUGIN_PATH: %1").arg(QueuedConfig::PLUGIN_PATH);

    // dbus
    metadata += "DBus";
    metadata += QString("\tDBUS_SERVICE: %1").arg(QueuedConfig::DBUS_SERVICE);
    metadata += QString("\tDBUS_APPLICATION_PATH: %1")
                    .arg(QueuedConfig::DBUS_APPLICATION_PATH);
    metadata += QString("\tDBUS_OBJECT_PATH: %1")
                    .arg(QueuedConfig::DBUS_OBJECT_PATH);
    metadata += QString("\tDBUS_PROPERTY_PATH: %1")
                    .arg(QueuedConfig::DBUS_PROPERTY_PATH);

    // cmake build information
    metadata += "Cmake properties";
    metadata += QString("\tCMAKE_BUILD_TYPE: %1").arg(CMAKE_BUILD_TYPE);
    metadata += QString("\tCMAKE_CXX_COMPILER: %1").arg(CMAKE_CXX_COMPILER);
    metadata += QString("\tCMAKE_CXX_FLAGS: %1").arg(CMAKE_CXX_FLAGS);
    metadata
        += QString("\tCMAKE_CXX_FLAGS_DEBUG: %1").arg(CMAKE_CXX_FLAGS_DEBUG);
    metadata += QString("\tCMAKE_CXX_FLAGS_RELEASE: %1")
                    .arg(CMAKE_CXX_FLAGS_RELEASE);
    metadata += QString("\tCMAKE_CXX_FLAGS_OPTIMIZATION: %1")
                    .arg(CMAKE_CXX_FLAGS_OPTIMIZATION);
    metadata += QString("\tCMAKE_DEFINITIONS: %1").arg(CMAKE_DEFINITIONS);
    metadata += QString("\tCMAKE_INSTALL_PREFIX: %1").arg(CMAKE_INSTALL_PREFIX);
    metadata += QString("\tCMAKE_MODULE_LINKER_FLAGS: %1")
                    .arg(CMAKE_MODULE_LINKER_FLAGS);
    metadata += QString("\tCMAKE_SHARED_LINKER_FLAGS: %1")
                    .arg(CMAKE_SHARED_LINKER_FLAGS);

    // components
    metadata += "Components";
    metadata += QString("\tBUILD_DEB_PACKAGE: %1").arg(BUILD_DEB_PACKAGE);
    metadata += QString("\tBUILD_RPM_PACKAGE: %1").arg(BUILD_RPM_PACKAGE);
    metadata
        += QString("\tCLANGFORMAT_EXECUTABLE: %1").arg(CLANGFORMAT_EXECUTABLE);
    metadata += QString("\tCOVERITY_COMMENT: %1").arg(COVERITY_COMMENT);
    metadata += QString("\tCOVERITY_DIRECTORY: %1").arg(COVERITY_DIRECTORY);
    metadata += QString("\tCOVERITY_EMAIL: %1").arg(COVERITY_EMAIL);
    metadata += QString("\tCOVERITY_EXECUTABLE: %1").arg(COVERITY_EXECUTABLE);
    metadata += QString("\tCOVERITY_URL: %1").arg(COVERITY_URL);
    metadata += QString("\tCPPCHECK_EXECUTABLE: %1").arg(CPPCHECK_EXECUTABLE);

    // additional properties
    metadata += "Additional properties";
    metadata += QString("\tPROP_DOCS: %1").arg(PROP_DOCS);
    metadata += QString("\tPROP_FUTURE: %1").arg(PROP_FUTURE);
    metadata += QString("\tPROP_LOAD: %1").arg(PROP_LOAD);
    metadata += QString("\tPROP_TEST: %1").arg(PROP_TEST);

    return metadata;
}
