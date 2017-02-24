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
 * @file QueuedAdvancedSettings.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include "queued/Queued.h"


/**
 * @class QueuedAdvancedSettings
 */
/**
 * @fn QueuedAdvancedSettings
 */
QueuedAdvancedSettings::QueuedAdvancedSettings(QObject *parent)
    : QObject(parent)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;
}


/**
 * @fn ~QueuedAdvancedSettings
 */
QueuedAdvancedSettings::~QueuedAdvancedSettings()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;
}


/**
 * @fn setValues
 */
void QueuedAdvancedSettings::setValues(const QList<QVariantHash> &_values)
{
    qCDebug(LOG_LIB) << "Set values from" << _values;
}
