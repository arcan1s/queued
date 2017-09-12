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


#ifndef QUEUEDSERVER_H
#define QUEUEDSERVER_H

#include <QObject>
#include <QVariant>


class QueuedTcpServer;

class QueuedServer : public QObject
{
    Q_OBJECT

public:
    explicit QueuedServer(QObject *parent, const QVariantHash &args);
    virtual ~QueuedServer();
    void deinit();
    void init();

private:
    // backend
    QueuedTcpServer *m_server = nullptr;
    // configuration
    QVariantHash m_configuration;
};


#endif /* QUEUEDSERVER_H */
