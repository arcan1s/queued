/*
 * Copyright (c) 2016 Evgeniy Alekseev
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
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include "queued/Queued.h"

#include <QSettings>
#include <QStandardPaths>


/**
 * @class QueuedSettings
 */
/**
 * @fn QueuedSettings
 */
QueuedSettings::QueuedSettings(QObject *parent, const QString path)
    : QObject(parent)
    , m_path(path)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    readConfiguration();
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
QueuedCfg::QueuedAdminSetup QueuedSettings::admin() const
{
    return m_cfgAdmin;
}


/**
 * @fn db
 */
QueuedCfg::QueuedDBSetup QueuedSettings::db() const
{
    return m_cfgDB;
}


/**
 * @fn defaultPath
 */
QString QueuedSettings::defaultPath()
{
    QString fileName = QStandardPaths::locate(QStandardPaths::ConfigLocation,
                                              QString("queued.ini"));
    qCInfo(LOG_LIB) << "Found configuration file" << fileName;

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
    settings.beginGroup(QString("Administrator"));
    m_cfgAdmin.name
        = settings.value(QString("Username"), QString("root")).toString();
    m_cfgAdmin.password = settings.value(QString("Password")).toString();
    settings.endGroup();

    // database related settings
    settings.beginGroup(QString("Database"));
    m_cfgDB.driver
        = settings.value(QString("Driver"), QString("QSQLITE")).toString();
    m_cfgDB.hostname = settings.value(QString("Hostname")).toString();
    m_cfgDB.password = settings.value(QString("Password")).toString();
    // get standard path for temporary files
    QString defaultDB = QString("%1/queued.db")
                            .arg(QStandardPaths::writableLocation(
                                QStandardPaths::TempLocation));
    m_cfgDB.path = settings.value(QString("Path"), defaultDB).toString();
    m_cfgDB.port = settings.value(QString("Port")).toInt();
    m_cfgDB.username = settings.value(QString("Username")).toString();
    settings.endGroup();
}
