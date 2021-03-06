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
 * @file QueuedSettings.cpp
 * Source code of queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>

#include <QSettings>
#include <QStandardPaths>
#include <queued/QueuedStaticConfig.h>


/**
 * @class QueuedSettings
 */
/**
 * @fn QueuedSettings
 */
QueuedSettings::QueuedSettings(QObject *_parent, const QString _path)
    : QObject(_parent)
    , m_path(_path)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    qRegisterMetaType<QueuedConfig::QueuedAdminSetup>("QueuedConfig::QueuedAdminSetup");
    qRegisterMetaType<QueuedConfig::QueuedDBSetup>("QueuedConfig::QueuedDBSetup");
}


/**
 * @fn ~QueuedSettings
 */
QueuedSettings::~QueuedSettings()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;
}


/**
 * @fn admin
 */
QueuedConfig::QueuedAdminSetup QueuedSettings::admin() const
{
    return m_cfgAdmin;
}


/**
 * @fn db
 */
QueuedConfig::QueuedDBSetup QueuedSettings::db() const
{
    return m_cfgDB;
}


/**
 * @fn defaultPath
 */
QString QueuedSettings::defaultPath()
{
    QString fileName = QStandardPaths::locate(QStandardPaths::ConfigLocation, "queued.ini");
    qCInfo(LOG_LIB) << "Found configuration file" << fileName;

    return fileName;
}


/**
 * @fn defaultTokenPath
 */
QString QueuedSettings::defaultTokenPath()
{
    QString fileName
        = QString("%1/queued")
              .arg(QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation));
    qCInfo(LOG_LIB) << "Cache file location" << fileName;

    return fileName;
}


/**
 * @fn path
 */
QString QueuedSettings::path() const
{
    return m_path;
}


/**
 * @fn readConfiguration
 */
void QueuedSettings::readConfiguration()
{
    QSettings settings(m_path, QSettings::IniFormat);
    qCInfo(LOG_LIB) << "Read configuration from" << settings.fileName();

    // administrator related settings
    settings.beginGroup("Administrator");
    m_cfgAdmin.name = settings.value("Username", "root").toString();
    m_cfgAdmin.password = settings.value("Password").toString();
    m_cfgAdmin.salt = settings.value("Salt").toString();
    settings.endGroup();

    // database related settings
    settings.beginGroup("Database");
    m_cfgDB.driver = settings.value("Driver", "QSQLITE").toString();
    m_cfgDB.hostname = settings.value("Hostname").toString();
    m_cfgDB.password = settings.value("Password").toString();
    // get standard path for temporary files
    QString defaultDB = QString("%1/queued.db")
                            .arg(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
    m_cfgDB.path = settings.value("Path", defaultDB).toString();
    m_cfgDB.port = settings.value("Port").toInt();
    m_cfgDB.username = settings.value("Username").toString();
    settings.endGroup();
}
