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


#include "QueuedTcpServerThread.h"

#include <QDataStream>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QTcpSocket>
#include <QUrlQuery>

#include <queued/Queued.h>

#include "QueuedTcpServerResponseHelper.h"


QueuedTcpServerThread::QueuedTcpServerThread(int socketDescriptor,
                                             const int timeout, QObject *parent)
    : QThread(parent)
    , m_socketDescriptor(socketDescriptor)
    , m_timeout(timeout)
{
    qCDebug(LOG_SERV) << __PRETTY_FUNCTION__;
}


QueuedTcpServerThread::~QueuedTcpServerThread()
{
    qCDebug(LOG_SERV) << __PRETTY_FUNCTION__;

    if (m_socket)
        m_socket->deleteLater();
}


QByteArrayList QueuedTcpServerThread::defaultResponse(const int code,
                                                      const QVariantHash &json)
{
    qCDebug(LOG_SERV) << "Build server response with code" << code
                      << "and json";

    QList<QByteArray> output;
    output += "HTTP/1.1 " + QByteArray::number(code) + " "
              + QueuedTcpServerResponseHelper::HTTPCodeMap[code] + "\r\n";
    output += "Server: QueuedServer/Qt" + QByteArray(qVersion()) + "\r\n";
    output += "Date: "
              + QLocale::c()
                    .toString(QDateTime::currentDateTimeUtc(),
                              "ddd, d MMM yyyy HH:mm:dd t")
                    .toUtf8()
              + "\r\n";
    output += "Content-Type: application/json\r\n";
    output += "\r\n";

    // json response
    if (!json.isEmpty()) {
        auto jsonObj = QJsonObject::fromVariantHash(json);
        auto jsonByte
            = QJsonDocument(jsonObj).toJson(QJsonDocument::JsonFormat::Compact);
        output += jsonByte;
    }

    return output;
}


QueuedTcpServerThread::QueuedTcpServerHeaders
QueuedTcpServerThread::getHeaders(const QStringList &headers)
{
    qCDebug(LOG_SERV) << "Get headers object from" << headers;

    auto headersObj = QueuedTcpServerThread::QueuedTcpServerHeaders();

    // metadata
    auto protocolData = headers.first().split(' ');
    if (protocolData.count() >= 3) {
        headersObj.request = protocolData.takeFirst();
        headersObj.protocol = protocolData.takeLast();
        headersObj.query = protocolData.join(' ');
    }
    // headers
    for (auto &header : headers.mid(1)) {
        auto parsed = header.split(": ");
        if (parsed.count() < 2)
            continue;
        headersObj.headers += QPair<QByteArray, QByteArray>(
            {parsed.first().toUtf8().toLower(),
             parsed.mid(1).join(": ").toUtf8()});
    }

    return headersObj;
}


QueuedTcpServerThread::QueuedTcpServerRequest QueuedTcpServerThread::getRequest(
    const QByteArray &body,
    const QueuedTcpServerThread::QueuedTcpServerHeaders &headers)
{
    qCDebug(LOG_SERV) << "Get request object from body" << body;

    QueuedTcpServerThread::QueuedTcpServerRequest request;
    request.headers = headers;

    // body
    auto error = QJsonParseError();
    auto jsonDoc = QJsonDocument::fromJson(body, &error);
    if (error.error == QJsonParseError::NoError)
        request.data = jsonDoc.object().toVariantHash();
    else
        qCWarning(LOG_SERV) << "Parse error" << error.errorString();

    // append from url if any
    auto items = QUrlQuery(headers.query.query()).queryItems();
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
        values.append(value);
        request.data[key] = values.count() == 1 ? values.first() : values;
    }

    return request;
}


QueuedTcpServerThread::QueuedTcpServerResponse
QueuedTcpServerThread::response(const QueuedTcpServerRequest &request) const
{
    qCDebug(LOG_SERV) << "Build response for request" << request.headers.query;

    QueuedTcpServerThread::QueuedTcpServerResponse response;

    // HACK additional workaround to parse content type header
    QNetworkRequest netRequest;
    for (auto &headers : request.headers.headers)
        netRequest.setRawHeader(headers.first, headers.second);

    // prepend code
    if (netRequest.header(QNetworkRequest::KnownHeaders::ContentTypeHeader)
            .toString()
        != "application/json")
        response.code = 415;
    else
        response.code = 200;
    QString token = netRequest.rawHeader(QueuedConfig::WEBAPI_TOKEN_HEADER);

    // json data
    if (response.code == 200) {
        auto req = QueuedTcpServerResponseHelper::parsePath(
            request.headers.query.path());
        req.type = request.headers.request;
        if (req.valid) {
            response.data = QueuedTcpServerResponseHelper::getData(
                req, request.data, token);
            response.code = response.data["code"].toInt();
        } else {
            response.code = 404;
        }
    }

    return response;
}


void QueuedTcpServerThread::run()
{
    m_socket = new QTcpSocket(nullptr);
    if (!m_socket->setSocketDescriptor(m_socketDescriptor)) {
        qCWarning(LOG_SERV) << "Socket error" << m_socket->error();
        return;
    }

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()),
            Qt::DirectConnection);

    exec();
}


void QueuedTcpServerThread::readyRead()
{
    QStringList headers;
    while (m_socket->canReadLine())
        headers += m_socket->readLine().simplified();
    // request body
    auto body = m_socket->readAll().simplified();

    // get request object
    auto headersObj = getHeaders(headers);
    auto requestObj = getRequest(body, headersObj);
    auto responseObj = response(requestObj);

    auto responseList = defaultResponse(responseObj.code, responseObj.data);
    for (auto &resp : responseList)
        m_socket->write(resp);
    m_socket->flush();

    // TODO use timeouts?
    if (m_socket->state() != QAbstractSocket::UnconnectedState)
        m_socket->waitForBytesWritten(m_timeout);
    m_socket->disconnectFromHost();
    if (m_socket->state() != QAbstractSocket::UnconnectedState)
        m_socket->waitForDisconnected();

    exit(0);
}
