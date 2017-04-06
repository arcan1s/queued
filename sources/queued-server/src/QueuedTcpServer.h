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


#ifndef QUEUEDTCPSERVER_H
#define QUEUEDTCPSERVER_H

#include <QTcpServer>


class QueuedTcpServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit QueuedTcpServer(const int timeout, QObject *parent);
    virtual ~QueuedTcpServer();
    void deinit();
    void init();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    int m_timeout = -1;
};


#endif /* QUEUEDTCPSERVER_H */
