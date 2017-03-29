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

#include <QTcpSocket>
#include <QThread>


class QTcpSocket;

class QueuedTcpServerThread : public QThread
{
    Q_OBJECT

public:
    typedef struct {
        QString request;
        QString path;
        QVariantHash data;
        bool valid = false;
    } QueuedTcpServerRequest;

    explicit QueuedTcpServerThread(int socketDescriptor, QObject *parent);
    virtual ~QueuedTcpServerThread();
    static QList<QByteArray> defaultResponse(const int code);
    static QueuedTcpServerRequest getRequest(const QStringList &headers,
                                             const QByteArray &body);
    QVariantHash response(const QueuedTcpServerRequest &request) const;
    void run() override;

signals:
    void error(QTcpSocket::SocketError socketError);

private slots:
    void disconnected();
    void readyRead();

private:
    QTcpSocket *m_socket = nullptr;
    int m_socketDescriptor;
};


#endif /* QUEUEDTCPSERVERTHREAD_H */
