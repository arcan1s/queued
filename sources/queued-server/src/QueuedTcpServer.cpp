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


#include "QueuedTcpServer.h"

#include <queued/Queued.h>

#include "QueuedTcpServerThread.h"


QueuedTcpServer::QueuedTcpServer(const int timeout, QObject *parent)
    : QTcpServer(parent)
    , m_timeout(timeout)
{
    qCDebug(LOG_SERV) << __PRETTY_FUNCTION__;
}


QueuedTcpServer::~QueuedTcpServer()
{
    qCDebug(LOG_SERV) << __PRETTY_FUNCTION__;

    deinit();
}


void QueuedTcpServer::deinit()
{
}


void QueuedTcpServer::init()
{
    deinit();
}


void QueuedTcpServer::incomingConnection(qintptr socketDescriptor)
{
    QueuedTcpServerThread *thread
        = new QueuedTcpServerThread(socketDescriptor, m_timeout, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
