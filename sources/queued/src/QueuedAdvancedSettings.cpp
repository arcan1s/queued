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
 * @file QueuedAdvancedSettings.cpp
 * Source code of queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>
#include <queued/QueuedStaticConfig.h>


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
 * @fn checkDatabase
 */
bool QueuedAdvancedSettings::checkDatabaseVersion() const
{
    QString key = internalId(QueuedConfig::QueuedSettings::DatabaseVersion);

    return m_values.contains(key.toLower()) && (get(key).toInt() == QueuedConfig::DATABASE_VERSION);
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
        return QueuedConfig::QueuedSettingsDefaults[internalId(_key)].defaultValue;
}


/**
 * @fn get
 */
QVariant QueuedAdvancedSettings::get(const QueuedConfig::QueuedSettings _key) const
{
    qCDebug(LOG_LIB) << "Looking for key" << static_cast<int>(_key);

    for (auto &key : QueuedConfig::QueuedSettingsDefaults.keys()) {
        if (QueuedConfig::QueuedSettingsDefaults[key].id != _key)
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
    for (auto &internal : QueuedConfig::QueuedSettingsDefaults.keys()) {
        if (internal.toLower() != key)
            continue;
        return internal;
    }

    return "";
}


/**
 * @fn internalId
 */
QString QueuedAdvancedSettings::internalId(const QueuedConfig::QueuedSettings _key)
{
    qCDebug(LOG_LIB) << "Looking for key" << static_cast<int>(_key);

    for (auto &internal : QueuedConfig::QueuedSettingsDefaults.keys()) {
        if (QueuedConfig::QueuedSettingsDefaults[internal].id != _key)
            continue;
        return internal;
    }

    return "";
}


/**
 * @fn isAdmin
 */
bool QueuedAdvancedSettings::isAdmin(const QString &_key) const
{
    qCDebug(LOG_LIB) << "Check if admin option" << _key;

    if ((_key.startsWith("Plugin.")) || (_key.startsWith("plugin.")))
        return true;
    else
        return QueuedConfig::QueuedSettingsDefaults[internalId(_key)].isAdmin;
}


/**
 * @fn isAdmin
 */
bool QueuedAdvancedSettings::isAdmin(const QueuedConfig::QueuedSettings &_key) const
{
    qCDebug(LOG_LIB) << "Check if admin option" << static_cast<int>(_key);

    return QueuedConfig::QueuedSettingsDefaults[internalId(_key)].isAdmin;
}


/**
 * @fn set
 */
void QueuedAdvancedSettings::set(const QString &_key, const QVariant &_value)
{
    qCDebug(LOG_LIB) << "Set value" << _value << "for key" << _key;

    m_values[_key.toLower()] = _value;
    auto id = QueuedConfig::QueuedSettingsDefaults[internalId(_key)].id;
    emit(valueUpdated(id, _key, _value));
}


/**
 * @fn set
 */
void QueuedAdvancedSettings::set(const QList<QVariantHash> &_values)
{
    qCDebug(LOG_LIB) << "Set values from" << _values;

    for (auto &pair : _values) {
        QString key = pair["key"].toString().toLower();
        m_ids[key] = pair["_id"].toLongLong();
        set(key, pair["value"]);
    }
}
