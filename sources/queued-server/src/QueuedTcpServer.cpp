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


#include "QueuedTcpServer.h"

#include <queued/Queued.h>

#include "QueuedTcpServerThread.h"


QueuedTcpServer::QueuedTcpServer(QObject *parent)
    : QTcpServer(parent)
{
    qCDebug(LOG_SERV) << __PRETTY_FUNCTION__;
}


QueuedTcpServer::~QueuedTcpServer()
{
    qCDebug(LOG_SERV) << __PRETTY_FUNCTION__;
}


void QueuedTcpServer::init(const int timeout)
{
    m_timeout = timeout;
}


void QueuedTcpServer::incomingConnection(qintptr socketDescriptor)
{
    auto thread = new QueuedTcpServerThread(socketDescriptor, m_timeout, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
