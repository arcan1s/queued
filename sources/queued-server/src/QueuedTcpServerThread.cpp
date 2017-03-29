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


#include "QueuedTcpServerThread.h"

#include <QDataStream>
#include <QJsonDocument>
#include <QUrl>
#include <QUrlQuery>

#include <queued/Queued.h>


QueuedTcpServerThread::QueuedTcpServerThread(int socketDescriptor,
                                             QObject *parent)
    : QThread(parent)
    , m_socketDescriptor(socketDescriptor)
{
    qCDebug(LOG_SERV) << __PRETTY_FUNCTION__;
}


QueuedTcpServerThread::~QueuedTcpServerThread()
{
    qCDebug(LOG_SERV) << __PRETTY_FUNCTION__;
}


QList<QByteArray> QueuedTcpServerThread::defaultResponse(const int code)
{
    qCDebug(LOG_SERV) << "Build server response with code" << code;

    QList<QByteArray> output;
    output += "HTTP/1.1 " + QByteArray::number(code) + " OK\r\n";
    output += "Server: QueuedServer/Qt" + QByteArray(qVersion()) + "\r\n";
    output += "Date: "
              + QLocale::c()
                    .toString(QDateTime::currentDateTimeUtc(),
                              "ddd, d MMM yyyy HH:mm:dd t")
                    .toUtf8()
              + "\r\n";
    output += "Content-Type: application/json\r\n";
    output += "\r\n";

    return output;
}


QueuedTcpServerThread::QueuedTcpServerRequest
QueuedTcpServerThread::getRequest(const QStringList &headers,
                                  const QByteArray &body)
{
    qCDebug(LOG_SERV) << "Get request object from headers" << headers
                      << "and body" << body;

    QueuedTcpServerThread::QueuedTcpServerRequest request;
    request.valid = true;
    // method
    request.request = headers.first().split(' ').at(0);
    // path
    QUrl url(headers.first().split(' ').at(1));
    request.path = url.path();
    // body
    QJsonParseError error;
    auto jsonDoc = QJsonDocument::fromJson(body, &error);
    if (error.error != QJsonParseError::NoError) {
        qCWarning(LOG_SERV) << "Parse error" << error.errorString();
        request.valid = false;
    } else {
        request.data = jsonDoc.object().toVariantHash();
    }
    // append from url if any
    auto items = QUrlQuery(url.query()).queryItems();
    for (auto &item : items) {
        auto key = item.first;
        auto value = item.second;
        QVariantList values;
        switch (request.data[key].type()) {
        case QVariant::List:
            values = request.data[key].toList();
            break;
        case QVariant::Invalid:
            break;
        default:
            values = QVariantList({request.data[key]});
            break;
        }
        values.append(key);
        request.data[key] = values.count() == 1 ? values.first() : values;
    }

    return request;
}


void QueuedTcpServerThread::run()
{
    m_socket = new QTcpSocket(this);
    if (!m_socket->setSocketDescriptor(m_socketDescriptor)) {
        emit(error(m_socket->error()));
        return;
    }

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()),
            Qt::DirectConnection);
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()),
            Qt::DirectConnection);

    exec();
}


void QueuedTcpServerThread::disconnected()
{
    m_socket->deleteLater();
    exit(0);
}


void QueuedTcpServerThread::readyRead()
{
    QStringList headers;
    while (m_socket->canReadLine())
        headers += m_socket->readLine().simplified();
    // request body
    auto body = m_socket->readAll().simplified();

    // get request object
    auto request = getRequest(headers, body);
    if (!request.valid) {
        emit(error(QTcpSocket::UnsupportedSocketOperationError));
        return;
    }

    auto response = defaultResponse(200);
    for (auto &resp : response) {
        m_socket->write(resp);
        m_socket->flush();
    }

    m_socket->waitForBytesWritten(3000);
    m_socket->disconnectFromHost();
    if (m_socket->state() != QAbstractSocket::UnconnectedState)
        m_socket->waitForDisconnected();
}
