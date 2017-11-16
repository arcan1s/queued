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


#ifndef QUEUEDEMAILNOTIFYHELPER_H
#define QUEUEDEMAILNOTIFYHELPER_H

#include <QObject>


class QueuedEmailNotifyHelper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString from READ from WRITE setFrom)
    Q_PROPERTY(bool insecureCurl READ isInsecureCurl WRITE setInsecureCurl)
    Q_PROPERTY(QString password READ password WRITE setPassword)
    Q_PROPERTY(int port READ port WRITE setPort)
    Q_PROPERTY(QString server READ server WRITE setServer)
    Q_PROPERTY(bool ssl READ isSslEnabled WRITE setSslEnabled)
    Q_PROPERTY(QString username READ username WRITE setUsername)

public:
    typedef struct {
        QStringList text;
        int currentLine = 0;
    } MailBody;

    explicit QueuedEmailNotifyHelper(QObject *_parent);
    virtual ~QueuedEmailNotifyHelper() = default;
    static QString curlEmail(const QString &_from);
    static char *curlString(const QString &_source);
    static size_t curlReadCallback(char *buffer, size_t size, size_t nitems,
                                   void *instream);
    // properties
    QString from() const;
    bool isInsecureCurl() const;
    bool isSslEnabled() const;
    QString password() const;
    int port() const;
    QString server() const;
    QString username() const;
    void setFrom(const QString &_from);
    void setInsecureCurl(const bool _insecureCurl);
    void setPassword(const QString &_password);
    void setPort(const int &_port);
    void setServer(const QString &_server);
    void setSslEnabled(const bool _sslEnabled);
    void setUsername(const QString &_username);

public slots:
    void sendEmail(const long long _id);

private:
    QString getEmail(const long long _id) const;
    QStringList getEmailText(const long long _id, const QString &_to) const;
    QString m_from;
    bool m_insecure = false;
    QString m_password;
    int m_port = 0;
    QString m_server;
    bool m_ssl = false;
    QString m_username;
};


#endif /* QUEUEDEMAILNOTIFYHELPER_H */
