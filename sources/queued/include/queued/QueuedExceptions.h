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
 * @file QueuedExceptions.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDEXCEPTIONS_H
#define QUEUEDEXCEPTIONS_H

#include <QException>


/**
 * @brief database operation exception
 */
class QueuedDatabaseException : public QException
{
public:
    /**
     * @brief QueuedDatabaseException class constructor
     * @param message
     * exception message
     */
    QueuedDatabaseException(const QString &message)
        : m_message(message){};
    /**
     * @brief clone QueuedDatabaseException
     */
    QueuedDatabaseException *clone() const
    {
        return new QueuedDatabaseException(*this);
    };
    /**
     * @brief message of this exception
     * @return message for logging, etc
     */
    QString message() const { return m_message; };
    /**
     * @brief raise QueuedDatabaseException
     */
    void raise() const { throw * this; }

private:
    /**
     * @brief exception message
     */
    QString m_message;
};


/**
 * @brief DBus operation exception
 */
class QueuedDBusException : public QException
{
public:
    /**
     * @brief QueuedDBusException class constructor
     * @param message
     * exception message
     */
    QueuedDBusException(const QString &message)
        : m_message(message){};
    /**
     * @brief clone QueuedDBusException
     */
    QueuedDBusException *clone() const
    {
        return new QueuedDBusException(*this);
    };
    /**
     * @brief message of this exception
     * @return message for logging, etc
     */
    QString message() const { return m_message; };
    /**
     * @brief raise QueuedDBusException
     */
    void raise() const { throw * this; }

private:
    /**
     * @brief exception message
     */
    QString m_message;
};


#endif /* QUEUEDEXCEPTIONS_H */
