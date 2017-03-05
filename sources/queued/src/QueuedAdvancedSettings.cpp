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
#include <queued/QueuedConfiguration.h>


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
 * @fn get
 */
QVariant QueuedAdvancedSettings::get(const QString &_key) const
{
    qCDebug(LOG_LIB) << "Looking for key" << _key;

    QString key = _key.toLower();
    if (m_values.contains(key))
        return m_values.value(key);
    else
        return QueuedCfg::QueuedSettingsDefaults[internalId(_key)].defaultValue;
}


/**
 * @fn get
 */
QVariant QueuedAdvancedSettings::get(const QueuedCfg::QueuedSettings _key) const
{
    qCDebug(LOG_LIB) << "Looking for key" << static_cast<int>(_key);

    for (auto &key : QueuedCfg::QueuedSettingsDefaults.keys()) {
        if (QueuedCfg::QueuedSettingsDefaults[key].id != _key)
            continue;
        return get(key);
    }

    return QVariant();
}


/**
 * @fn id
 */
long long QueuedAdvancedSettings::id(const QString &_key) const
{
    qCDebug(LOG_LIB) << "Looking for ID for key" << _key;

    return m_ids.value(_key.toLower(), -1);
}


/**
 * @fn internalId
 */
QString QueuedAdvancedSettings::internalId(const QString &_key)
{
    qCDebug(LOG_LIB) << "Looking for key" << _key;

    QString key = _key.toLower();
    for (auto &internal : QueuedCfg::QueuedSettingsDefaults.keys()) {
        if (internal.toLower() != key)
            continue;
        return internal;
    }

    return QString();
}


/**
 * @fn set
 */
void QueuedAdvancedSettings::set(const QString &_key, const QVariant &_value)
{
    qCDebug(LOG_LIB) << "Set value" << _value << "for key" << _key;

    m_values[_key.toLower()] = _value;
    auto id = QueuedCfg::QueuedSettingsDefaults[internalId(_key)].id;
    emit(valueUpdated(id, _value));
}


/**
 * @fn set
 */
void QueuedAdvancedSettings::set(const QList<QVariantHash> &_values)
{
    qCDebug(LOG_LIB) << "Set values from" << _values;

    for (auto &pair : _values) {
        QString key = pair[QString("key")].toString().toLower();
        m_ids[key] = pair[QString("_id")].toLongLong();
        set(key, pair[QString("value")]);
    }
}
