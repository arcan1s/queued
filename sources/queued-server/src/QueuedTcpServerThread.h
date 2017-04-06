/*
 * Copyright (c) 2017 Evgeniy Alekseev
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


#ifndef QUEUEDTCPSERVERTHREAD_H
#define QUEUEDTCPSERVERTHREAD_H

#include <QThread>
#include <QUrl>
#include <QVariant>


class QTcpSocket;

class QueuedTcpServerThread : public QThread
{
    Q_OBJECT

public:
    typedef struct {
        QList<QPair<QByteArray, QByteArray>> headers;
        QString protocol;
        QUrl query;
        QString request;
    } QueuedTcpServerHeaders;
    typedef struct {
        QueuedTcpServerHeaders headers;
        QVariantHash data;
    } QueuedTcpServerRequest;
    typedef struct {
        int code;
        QVariantHash data;
    } QueuedTcpServerResponse;

    explicit QueuedTcpServerThread(int socketDescriptor, const int timeout,
                                   QObject *parent);
    virtual ~QueuedTcpServerThread();
    static QByteArrayList defaultResponse(const int code,
                                          const QVariantHash &json);
    static QueuedTcpServerHeaders getHeaders(const QStringList &headers);
    static QueuedTcpServerRequest
    getRequest(const QByteArray &body, const QueuedTcpServerHeaders &headers);
    QueuedTcpServerResponse
    response(const QueuedTcpServerRequest &request) const;
    void run() override;

private slots:
    void readyRead();

private:
    QTcpSocket *m_socket = nullptr;
    int m_socketDescriptor;
    int m_timeout = -1;
};


#endif /* QUEUEDTCPSERVERTHREAD_H */
