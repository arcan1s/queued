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
 * @file QueuedAdvancedSettings.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDADVANCEDSETTINGS_H
#define QUEUEDADVANCEDSETTINGS_H

#include <QObject>

#include "QueuedConfiguration.h"


/**
 * @brief implementation over database stored settings
 */
class QueuedAdvancedSettings : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief QueuedAdvancedSettings class constructor
     * @param parent         pointer to parent item
     */
    explicit QueuedAdvancedSettings(QObject *parent);
    /**
     * @brief QueuedAdvancedSettings class destructor
     */
    virtual ~QueuedAdvancedSettings();
    /**
     * @brief upload configuration from database in to internal format
     * @param _value         configuration values from database
     */
    void setValues(const QList<QVariantHash> &_values);

private:
};


#endif /* QUEUEDADVANCEDSETTINGS_H */
