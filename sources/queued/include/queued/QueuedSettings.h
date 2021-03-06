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
 * @file QueuedSettings.h
 * Header of Queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDSETTINGS_H
#define QUEUEDSETTINGS_H

#include <QObject>

#include "QueuedStaticConfig.h"


/**
 * @brief implementation over QSettings
 */
class QueuedSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path)
    Q_PROPERTY(QueuedConfig::QueuedAdminSetup admin READ admin)
    Q_PROPERTY(QueuedConfig::QueuedDBSetup db READ db)

public:
    /**
     * @brief QueuedSettings class constructor
     * @param _parent
     * pointer to parent item
     * @param _path
     * path to configuration file
     */
    explicit QueuedSettings(QObject *_parent, const QString _path);
    /**
     * @brief QueuedSettings class destructor
     */
    virtual ~QueuedSettings();
    /**
     * @brief administrator settings
     * @return QueuedAdminSetup structure
     */
    QueuedConfig::QueuedAdminSetup admin() const;
    /**
     * @brief database settings
     * @return QueuedDBSetup structure
     */
    QueuedConfig::QueuedDBSetup db() const;
    /**
     * @brief default path to configuration
     * @return default path to configuration file
     */
    static QString defaultPath();
    /**
     * @brief default path to cached token
     * @return default path to cached token file
     * @return
     */
    static QString defaultTokenPath();
    /**
     * @brief path to database
     * @return path to used database
     */
    QString path() const;

public slots:
    /**
     * @brief read configuration from file
     */
    void readConfiguration();

private:
    /**
     * @brief admin configuration
     */
    QueuedConfig::QueuedAdminSetup m_cfgAdmin;
    /**
     * @brief database configuration
     */
    QueuedConfig::QueuedDBSetup m_cfgDB;
    /**
     * @brief path to configuration
     */
    QString m_path;
};


#endif /* QUEUEDSETTINGS_H */
