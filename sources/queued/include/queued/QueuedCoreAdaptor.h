/*
 * Copyright (c) 2016 Evgeniy Alekseev
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
/**
 * @file QueuedCoreAdaptor.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDCOREADAPTOR_H
#define QUEUEDCOREADAPTOR_H

#include <QObject>


/**
 * @brief DBus adaptor for core interface
 */
class QueuedCoreAdaptor : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief QueuedCoreAdaptor class constructor
     * @param parent
     * pointer to parent item
     */
    explicit QueuedCoreAdaptor(QObject *parent);
    /**
     * @brief QueuedCoreAdaptor class destructor
     */
    virtual ~QueuedCoreAdaptor();

private:
};


#endif /* QUEUEDCOREADAPTOR_H */
