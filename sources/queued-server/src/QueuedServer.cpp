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


#include "QueuedServer.h"

#include <queued/Queued.h>

#include "QueuedTcpServer.h"


QueuedServer::QueuedServer(QObject *parent, const QVariantHash &args)
    : QObject(parent)
    , m_configuration(args)
{
    qCDebug(LOG_SERV) << __PRETTY_FUNCTION__;

    init();
}


QueuedServer::~QueuedServer()
{
    qCDebug(LOG_SERV) << __PRETTY_FUNCTION__;

    deinit();
}


void QueuedServer::deinit()
{
    if (m_server)
        delete m_server;
}


void QueuedServer::init()
{
    deinit();

    m_server = new QueuedTcpServer(this);
    m_server->listen(
        QHostAddress(QueuedCoreAdaptor::getOption("ServerAddress").toString()),
        QueuedCoreAdaptor::getOption("ServerPort").toUInt());
    m_server->setMaxPendingConnections(
        QueuedCoreAdaptor::getOption("ServerMaxConnections").toInt());

    qCInfo(LOG_SERV) << "Server listen on" << m_server->serverAddress()
                     << m_server->serverPort();
}
