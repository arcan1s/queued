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
 * @file QueuedStaticConfig.h
 * Header of Queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDCONFIGURATION_H
#define QUEUEDCONFIGURATION_H

#include <QHash>
#include <QString>
#include <QVariant>

#include "QueuedConfig.h"


/**
 * @addtogroup QueuedConfig
 * @brief Queued configuration related types
 */
namespace QueuedConfig
{
/**
 * @struct QueuedAdminSetup
 * @brief structure to define administrator user
 * @var QueuedAdminSetup::name
 * administrator user name
 * @var QueuedAdminSetup::password
 * administrator user password
 * @var QueuedAdminSetup::salt
 * user passwords salt
 */
typedef struct {
    QString name;
    QString password;
    QString salt;
} QueuedAdminSetup;
/**
 * @struct QueuedDBSetup
 * @brief structure to define database setup
 * @var QueuedDBSetup::driver
 * driver name
 * @var QueuedDBSetup::hostname
 * hostname to connect
 * @var QueuedDBSetup::password
 * password to connect if any
 * @var QueuedDBSetup::path
 * path to database
 * @var QueuedDBSetup::port
 * port to connect
 * @var QueuedDBSetup::username
 * username to connect if any
 */
typedef struct {
    QString driver;
    QString hostname;
    QString password;
    QString path;
    int port;
    QString username;
} QueuedDBSetup;
/**
 * @enum QueuedSettings
 * @brief settings keys enum
 * @var QueuedSettings::Invalid
 * unknown key
 * @var QueuedSettings::DatabaseInterval
 * database actions interval in msecs
 * @var QueuedSettings::DatabaseVersion
 * internal field to control current database version
 * @var QueuedSettings::DefaultLimits
 * default limits value
 * @var QueuedSettings::KeepTasks
 * keep ended tasks in msecs
 * @var QueuedSettings::KeepUsers
 * keep users last logged in msecs
 * @var QueuedSettings::OnExitAction
 * on queued exit action enum
 * @var QueuedSettings::Plugins
 * plugin list
 * @var QueuedSettings::ProcessCommandLine
 * control process command line
 * @var QueuedSettings::ServerAddress
 * queued server bind address
 * @var QueuedSettings::ServerMaxConnections
 * queued server max connections
 * @var QueuedSettings::ServerPort
 * queued server bind port
 * @var QueuedSettings::ServerTimeout
 * server thread timeout
 * @var QueuedSettings::TokenExpiration
 * token expiration value in days
 */
enum class QueuedSettings {
    Invalid,
    DatabaseInterval,
    DatabaseVersion,
    DefaultLimits,
    KeepTasks,
    KeepUsers,
    OnExitAction,
    Plugins,
    ProcessCommandLine,
    ServerAddress,
    ServerMaxConnections,
    ServerPort,
    ServerTimeout,
    TokenExpiration
};
/**
 * @struct QueuedSettingsField
 * @brief structure to define advanced settings field
 * @var QueuedSettingsField::id
 * interval field ID
 * @var QueuedSettingsField::defaultValue
 * settings default value
 */
typedef struct {
    QueuedSettings id;
    QVariant defaultValue;
} QueuedSettingsField;
/**
 * @typedef QueuedSettingsDefaultMap
 * map of settings indices to related values
 */
typedef QHash<QString, QueuedSettingsField> QueuedSettingsDefaultMap;
/**
 * @brief default settings map
 */
static const QueuedSettingsDefaultMap QueuedSettingsDefaults = {
    {"", {QueuedSettings::Invalid, QVariant()}},
    {"DatabaseInterval", {QueuedSettings::DatabaseInterval, 86400000}},
    {"DatabaseVersion",
     {QueuedSettings::DatabaseVersion, QueuedConfig::DATABASE_VERSION}},
    {"DefaultLimits", {QueuedSettings::DefaultLimits, "0\n0\n0\n0\n0"}},
    {"KeepTasks", {QueuedSettings::KeepTasks, 0}},
    {"KeepUsers", {QueuedSettings::KeepUsers, 0}},
    {"OnExitAction", {QueuedSettings::OnExitAction, 2}},
    {"Plugins", {QueuedSettings::Plugins, ""}},
    {"ProcessCommandLine",
     {QueuedSettings::ProcessCommandLine,
      "systemd-run\n--scope\n--unit={name}\n--uid={uid}\n--gid={gid}"
      "\n-p\nCPUQuota={cpu}%\n-p\nMemoryHigh={memory}\n{application}"}},
    {"ServerAddress", {QueuedSettings::ServerAddress, ""}},
    {"ServerMaxConnections", {QueuedSettings::ServerMaxConnections, 30}},
    {"ServerPort", {QueuedSettings::ServerPort, 8080}},
    {"ServerTimeout", {QueuedSettings::ServerTimeout, -1}},
    {"TokenExpiration", {QueuedSettings::TokenExpiration, 30}},
};
};

#endif /* QUEUEDCONFIGURATION_H */
