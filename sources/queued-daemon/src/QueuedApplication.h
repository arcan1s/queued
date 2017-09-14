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


#ifndef QUEUEDAPPLICATION_H
#define QUEUEDAPPLICATION_H

#include <QObject>
#include <QVariant>


class QueuedCore;

class QueuedApplication : public QObject
{
    Q_OBJECT

public:
    explicit QueuedApplication(QObject *parent, const QVariantHash &args);
    virtual ~QueuedApplication();
    void init();

private:
    // backend
    void initDBus();
    // library
    QueuedCore *m_core = nullptr;
    // configuration
    QVariantHash m_configuration;
};


#endif /* QUEUEDAPPLICATION_H */
