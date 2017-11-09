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


#include "QueuedServer.h"

#include <queued/Queued.h>

#include <QCoreApplication>

#include "QueuedTcpServer.h"


QueuedServer::QueuedServer(QObject *parent, const QVariantHash &args)
    : QObject(parent)
    , m_configuration(args)
{
    qCDebug(LOG_SERV) << __PRETTY_FUNCTION__;

    m_server = new QueuedTcpServer(this);

    init();
}


QueuedServer::~QueuedServer()
{
    qCDebug(LOG_SERV) << __PRETTY_FUNCTION__;

    m_server->deleteLater();
}


void QueuedServer::init()
{
    while (QueuedCoreAdaptor::getStatus().type() != Result::Content::Value) {
        qCWarning(LOG_SERV)
            << "Daemon seems to be unavailable wait" << WAIT_FOR_DAEMON;

        QTime timer = QTime::currentTime().addMSecs(WAIT_FOR_DAEMON);
        while (QTime::currentTime() < timer)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    m_server->init(QueuedCoreAdaptor::getOption(
                       QueuedConfig::QueuedSettings::ServerTimeout)
                       .get()
                       .toInt());
    QString address = QueuedCoreAdaptor::getOption(
                          QueuedConfig::QueuedSettings::ServerAddress)
                          .get()
                          .toString();
    ushort port
        = QueuedCoreAdaptor::getOption(QueuedConfig::QueuedSettings::ServerPort)
              .get()
              .toUInt();
    m_server->listen(QHostAddress(address), port);
    m_server->setMaxPendingConnections(
        QueuedCoreAdaptor::getOption(
            QueuedConfig::QueuedSettings::ServerMaxConnections)
            .get()
            .toInt());

    qCInfo(LOG_SERV) << "Server listen on" << m_server->serverAddress()
                     << m_server->serverPort();
}
