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


#include "QueuedEmailNotifyHelper.h"

#include <queued/Queued.h>

extern "C" {
#include <curl/curl.h>
}


QueuedEmailNotifyHelper::QueuedEmailNotifyHelper(QObject *_parent)
    : QObject(_parent)
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;
}


QString QueuedEmailNotifyHelper::curlEmail(const QString &_from)
{
    qCDebug(LOG_PL) << "Convert email to curl format from" << _from;

    return QString("<%1>").arg(_from);
}


char *QueuedEmailNotifyHelper::curlString(const QString &_source)
{
    return strdup(_source.toStdString().c_str());
}


size_t QueuedEmailNotifyHelper::curlReadCallback(char *buffer, size_t size,
                                                 size_t nitems, void *instream)
{
    // FIXME not really best choice to use here
    auto text = reinterpret_cast<MailBody *>(instream);

    // check sizes and whatever
    if ((size == 0) || (nitems == 0) || ((size * nitems) < 1))
        return 0;
    if (text->text.count() <= text->currentLine)
        return 0;

    // get char* string from related qstring
    auto data = curlString(text->text.at(text->currentLine));
    size_t len = strlen(data);
    memcpy(buffer, data, len);
    text->currentLine++;

    return len;
}


QString QueuedEmailNotifyHelper::from() const
{
    return m_from;
}


bool QueuedEmailNotifyHelper::isInsecureCurl() const
{
    return m_insecure;
}


bool QueuedEmailNotifyHelper::isSslEnabled() const
{
    return m_ssl;
}


QString QueuedEmailNotifyHelper::password() const
{
    return m_password;
}


int QueuedEmailNotifyHelper::port() const
{
    return m_port;
}


QString QueuedEmailNotifyHelper::server() const
{
    return m_server;
}


QString QueuedEmailNotifyHelper::token() const
{
    return m_token;
}


QString QueuedEmailNotifyHelper::username() const
{
    return m_username;
}


void QueuedEmailNotifyHelper::setFrom(const QString &_from)
{
    qCDebug(LOG_PL) << "Set from" << _from;

    m_from = _from;
}


void QueuedEmailNotifyHelper::setInsecureCurl(const bool _insecureCurl)
{
    qCDebug(LOG_PL) << "Set insecure curl" << _insecureCurl;

    m_insecure = _insecureCurl;
}


void QueuedEmailNotifyHelper::setPassword(const QString &_password)
{
    qCDebug(LOG_PL) << "Set password";

    m_password = _password;
}


void QueuedEmailNotifyHelper::setPort(const int &_port)
{
    qCDebug(LOG_PL) << "Set port" << _port;

    m_port = _port;
}


void QueuedEmailNotifyHelper::setServer(const QString &_server)
{
    qCDebug(LOG_PL) << "Set server" << _server;

    m_server = _server;
}


void QueuedEmailNotifyHelper::setSslEnabled(const bool _sslEnabled)
{
    qCDebug(LOG_PL) << "Set ssl enabled" << _sslEnabled;

    m_ssl = _sslEnabled;
}


void QueuedEmailNotifyHelper::setToken(const QString &_token)
{
    m_token = _token;
}


void QueuedEmailNotifyHelper::setUsername(const QString &_username)
{
    qCDebug(LOG_PL) << "Set username" << _username;

    m_username = _username;
}


void QueuedEmailNotifyHelper::sendEmail(const long long _id)
{
    auto rcpt = getEmail(_id);
    if (rcpt.isEmpty())
        return;
    MailBody text = {getEmailText(_id, rcpt), 0};

    CURL *curl = curl_easy_init();
    QString url = QString("%1:%2").arg(server()).arg(port());
    curl_easy_setopt(curl, CURLOPT_URL, curlString(url));

    // ssl options
    if (isSslEnabled())
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    if (isInsecureCurl())
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

    // auth if applicable
    if ((!username().isEmpty()) && (!password().isEmpty())) {
        curl_easy_setopt(curl, CURLOPT_USERNAME, curlString(username()));
        curl_easy_setopt(curl, CURLOPT_PASSWORD, curlString(password()));
    }

    // from & to
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, curlString(curlEmail(from())));
    curl_slist *recipients = nullptr;
    recipients = curl_slist_append(recipients, curlString(curlEmail(rcpt)));
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    // mail body
    curl_easy_setopt(curl, CURLOPT_READFUNCTION,
                     &QueuedEmailNotifyHelper::curlReadCallback);
    curl_easy_setopt(curl, CURLOPT_READDATA, &text);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    // send
    auto res = curl_easy_perform(curl);
    if (res != CURLE_OK)
        qCWarning(LOG_PL) << "Could not perform curl action"
                          << curl_easy_strerror(res);

    // cleanup
    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);
}


QString QueuedEmailNotifyHelper::getEmail(const long long _id) const
{
    qCDebug(LOG_PL) << "Get email for task ID" << _id;

    auto task = QueuedCoreAdaptor::getTask(_id, "user", token());
    if (task.type() != Result::Content::Value) {
        qCWarning(LOG_LIB) << "Could not get task information" << _id;
        return "";
    }

    auto userId = task.get().toLongLong();
    auto user = QueuedCoreAdaptor::getUser(userId, "email", token());
    if (user.type() != Result::Content::Value) {
        qCWarning(LOG_LIB) << "Could not get user information" << userId;
        return "";
    }

    return user.get().toString();
}


QStringList QueuedEmailNotifyHelper::getEmailText(const long long _id,
                                                  const QString &_to) const
{
    qCDebug(LOG_PL) << "Get email text for user" << _to << "for task" << _id;

    auto now
        = QDateTime::currentDateTimeUtc().toString(Qt::DateFormat::RFC2822Date);

    return {QString("Date: %1\r\n").arg(now),
            QString("To: %1\r\n").arg(curlEmail(_to)),
            QString("From: %1\r\n").arg(curlEmail(from())),
            // message-id?
            QString("Subject: %1\r\n").arg("Job %1 done").arg(_id), "\r\n",
            QString("Job %1 done\r\n").arg(_id)};
}
