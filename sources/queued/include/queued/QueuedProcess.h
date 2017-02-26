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
 * @file QueuedProcess.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDPROCESS_H
#define QUEUEDPROCESS_H

#include <QDateTime>
#include <QProcess>

#include "QueuedLimits.h"


/**
 * @brief implementation over QProcess to run processes
 */
class QueuedProcess : public QProcess
{
    Q_OBJECT
    Q_PROPERTY(long long index READ index)
    Q_PROPERTY(QString name READ name)

public:
    /**
     * @struct QueuedProcessDefinition
     * @brief structure to define process
     * @var command
     * command line
     * @var arguments
     * command line arguments
     * @var workingDirectory
     * path to working directory
     * @var uid
     * UID of process
     * @var gid
     * GID of process
     * @var nice
     * nice level of process
     * @var startTime
     * process start time
     * @var endTime
     * process end time
     * @var user
     * task owner ID
     * @var limits
     * task limits
     */
    typedef struct {
        QString command;
        QStringList arguments;
        QString workingDirectory;
        unsigned int uid;
        unsigned int gid;
        unsigned int nice;
        QDateTime startTime;
        QDateTime endTime;
        long long user;
        QueuedLimits::Limits limits;
    } QueuedProcessDefinitions;

    /**
     * @brief QueuedProcess class constructor
     * @param parent         pointer to parent item
     * @param definitions    definitions of process
     * @param index          index of process
     */
    explicit QueuedProcess(QObject *parent,
                           const QueuedProcessDefinitions definitions,
                           const long long index);
    /**
     * @brief QueuedProcess class destructor
     */
    virtual ~QueuedProcess();
    /**
     * @brief index of process
     * @return assigned index of process
     */
    long long index() const;
    /**
     * @brief name of process
     * @return generated name of process
     */
    QString name() const;
    /**
     * @brief equal operator implementation
     * @param _other         other object
     * @return true if objects are equal
     */
    bool operator==(const QueuedProcess &_other);

protected:
    /**
     * @brief additional method with sets rules for QProcess
     */
    void setupChildProcess();

public slots:
    /**
     * @brief run process
     */
    void run();
    /**
     * @brief set end time
     * @param _time          process end time
     */
    void setEndTime(const QDateTime &_time);

private:
    /**
     * @brief process definitions
     */
    QueuedProcessDefinitions m_definitions;
    /**
     * @brief index of process
     */
    long long m_index = -1;
};


#endif /* QUEUEDPROCESS_H */
