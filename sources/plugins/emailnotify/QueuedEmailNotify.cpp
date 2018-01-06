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


#include "QueuedEmailNotify.h"

#include <queued/Queued.h>

#include "QueuedEmailNotifyHelper.h"


void QueuedEmailNotify::init(const QVariantHash &_settings)
{
    m_helper = new QueuedEmailNotifyHelper(this);

    m_helper->setFrom(_settings.value("From", "mail@example.com").toString());
    m_helper->setInsecureCurl(_settings.value("InsecureCurl", false).toBool());
    m_helper->setPassword(_settings.value("Password", "").toString());
    m_helper->setPort(_settings.value("Port", 465).toInt());
    m_helper->setServer(_settings.value("Server", "smtp://smtp.example.com").toString());
    m_helper->setSslEnabled(_settings.value("UseSSL", false).toBool());
    m_helper->setUsername(_settings.value("Username", "").toString());
}


void QueuedEmailNotify::setToken(const QString &_token)
{
    m_token = _token;
    m_helper->setToken(_token);
}


void QueuedEmailNotify::setup(const QueuedPluginManagerInterface *_manager)
{
    connect(_manager, SIGNAL(onStopTask(const long long)), m_helper,
            SLOT(sendEmail(const long long)));
}


void QueuedEmailNotify::updateSettings(const QString &_key, const QVariant &_value)
{
    qCDebug(LOG_PL) << "Update settings for" << _key;

    if (!m_helper) {
        qCWarning(LOG_PL) << "Helper is not initialized. Did you forget to call ::init()?";
        return;
    }

    if (_key == "From") {
        m_helper->setFrom(_value.toString());
    } else if (_key == "Password") {
        m_helper->setPassword(_value.toString());
    } else if (_key == "Port") {
        m_helper->setPort(_value.toInt());
    } else if (_key == "Server") {
        m_helper->setServer(_value.toString());
    } else if (_key == "Username") {
        m_helper->setUsername(_value.toString());
    }
}
