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
/**
 * @file QueuedExceptions.h
 * Header of Queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDEXCEPTIONS_H
#define QUEUEDEXCEPTIONS_H

#include <QException>


/**
 * @brief generic exception class
 */
class QueuedException : public QException
{
public:
    /**
     * @brief QueuedException class constructor
     * @param message
     * exception message
     */
    explicit QueuedException(QString message)
        : QException()
    {
        m_message = std::move(message);
    };
    /**
     * @brief clone QueuedException
     */
    QueuedException *clone() const override
    {
        return new QueuedException(*this);
    };
    /**
     * @brief message of this exception
     * @return message for logging, etc
     */
    QString message() const { return m_message; };
    /**
     * @brief raise QueuedException
     */
    void raise() const override { throw * this; }

private:
    /**
     * @brief exception message
     */
    QString m_message;
};


/**
 * @brief configuration related exception
 */
class QueuedConfigurationException : public QueuedException
{
public:
    /**
     * @brief QueuedConfigurationException class constructor
     * @param message
     * exception message
     */
    explicit QueuedConfigurationException(const QString &message)
        : QueuedException(message){};
};


/**
 * @brief database operation exception
 */
class QueuedDatabaseException : public QueuedException
{
public:
    /**
     * @brief QueuedDatabaseException class constructor
     * @param message
     * exception message
     */
    explicit QueuedDatabaseException(const QString &message)
        : QueuedException(message){};
};


/**
 * @brief DBus operation exception
 */
class QueuedDBusException : public QueuedException
{
public:
    /**
     * @brief QueuedDBusException class constructor
     * @param message
     * exception message
     */
    explicit QueuedDBusException(const QString &message)
        : QueuedException(message){};
};


#endif /* QUEUEDEXCEPTIONS_H */
