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
 * @file QueuedSettings.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDSETTINGS_H
#define QUEUEDSETTINGS_H

#include <QObject>

#include "QueuedConfiguration.h"


/**
 * @brief implementation over QSettings
 */
class QueuedSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path)
    Q_PROPERTY(QueuedCfg::QueuedAdminSetup admin READ admin)
    Q_PROPERTY(QueuedCfg::QueuedDBSetup db READ db)

public:
    /**
     * @brief QueuedSettings class constructor
     * @param parent         pointer to parent item
     * @param path           path to configuration file
     */
    explicit QueuedSettings(QObject *parent, const QString path);
    /**
     * @brief QueuedSettings class destructor
     */
    virtual ~QueuedSettings();
    /**
     * @brief administrator settings
     * @return QueuedAdminSetup structure
     */
    QueuedCfg::QueuedAdminSetup admin() const;
    /**
     * @brief database settings
     * @return QueuedDBSetup structure
     */
    QueuedCfg::QueuedDBSetup db() const;
    /**
     * @brief default path to configuration
     * @return default path to configuration file
     */
    static QString defaultPath();
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
    QueuedCfg::QueuedAdminSetup m_cfgAdmin;
    /**
     * @brief database configuration
     */
    QueuedCfg::QueuedDBSetup m_cfgDB;
    /**
     * @brief path to configuration
     */
    QString m_path;
};


#endif /* QUEUEDSETTINGS_H */
