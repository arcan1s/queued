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
    // mutable properties
    Q_PROPERTY(QString command READ command WRITE setCommand)
    Q_PROPERTY(QStringList commandArguments READ commandArguments WRITE
                   setCommandArguments)
    Q_PROPERTY(QDateTime endTime READ endTime WRITE setEndTime)
    Q_PROPERTY(uint gid READ uid WRITE setGid)
    Q_PROPERTY(QString limits READ limits WRITE setLimits)
    Q_PROPERTY(QueuedLimits::Limits nativeLimtis READ nativeLimits)
    Q_PROPERTY(uint nice READ nice WRITE setNice)
    Q_PROPERTY(QString processLine READ processLine WRITE setProcessLine)
    Q_PROPERTY(QDateTime startTime READ startTime WRITE setStartTime)
    Q_PROPERTY(uint uid READ uid WRITE setUid)
    Q_PROPERTY(long long user READ user WRITE setUser)
    Q_PROPERTY(QString workDirectory READ workDirectory WRITE setWorkDirectory)

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
        uint uid;
        uint gid;
        uint nice;
        QDateTime startTime;
        QDateTime endTime;
        long long user;
        QString limits;
    } QueuedProcessDefinitions;

    /**
     * @brief QueuedProcess class constructor
     * @param parent
     * pointer to parent item
     * @param definitions
     * definitions of process
     * @param index
     * index of process
     */
    explicit QueuedProcess(QObject *parent,
                           const QueuedProcessDefinitions &definitions,
                           const long long index);
    /**
     * @brief QueuedProcess class destructor
     */
    virtual ~QueuedProcess();
    /**
     * @brief update command arguments
     */
    void updateArguments();
    // properties
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
    // mutable properties
    /**
     * @brief command line
     * @return process command line
     */
    QString command() const;
    /**
     * @brief command line arguments
     * @return process command line arguments
     */
    QStringList commandArguments() const;
    /**
     * @brief process end time
     * @return process end time
     */
    QDateTime endTime() const;
    /**
     * @brief process GID
     * @return process GID
     */
    uint gid() const;
    /**
     * @brief process limits
     * @return process defined limits
     */
    QString limits() const;
    /**
     * @brief process limits
     * @return process defined limits in native format
     */
    QueuedLimits::Limits nativeLimits() const;
    /**
     * @brief process nice
     * @return process nice
     */
    uint nice() const;
    /**
     * @brief process line
     * @return process line as is in configuration
     */
    QString processLine() const;
    /**
     * @brief process start time
     * @return process start time
     */
    QDateTime startTime() const;
    /**
     * @brief process UID
     * @return process UID
     */
    uint uid() const;
    /**
     * @brief user
     * @return process owner ID
     */
    long long user() const;
    /**
     * @brief working directory
     * @return process working directory
     */
    QString workDirectory() const;
    /**
     * @brief set command line
     * @param _command       new command line
     */
    void setCommand(const QString &_command);
    /**
     * @brief set command line arguments
     * @param _commandArguments
     * new command line arguments
     */
    void setCommandArguments(const QStringList &_commandArguments);
    /**
     * @brief set end time
     * @param _time
     * process end time
     */
    void setEndTime(const QDateTime &_time);
    /**
     * @brief set process GID
     * @param _gid
     * new process GID
     */
    void setGid(const uint _gid);
    /**
     * @brief set process limits
     * @param _limits
     * new process limits
     */
    void setLimits(const QString &_limits);
    /**
     * @brief set process nice
     * @param _nice
     * new process nice
     */
    void setNice(const uint _nice);
    /**
     * @brief set process line
     * @param _processLine
     * original process line
     * @remark values in {} will be replaced
     * 1. Property names, like {name}, {uid}, etc
     * 2. {cpu} will be replaced to QueuedSystemInfo::cpuWeight(limit) in %
     * 3. {memory} will be replaced to limit
     * 4. {application} will be replaced to application line and arguments
     */
    void setProcessLine(const QString &_processLine);
    /**
     * @brief set start time
     * @param _time
     * process start time
     */
    void setStartTime(const QDateTime &_time);
    /**
     * @brief set process UID
     * @param _uid
     * new process UID
     */
    void setUid(const uint _uid);
    /**
     * @brief set user ID
     * @param _user
     * new user ID
     */
    void setUser(const long long _user);
    /**
     * @brief set working directory
     * @param _workDirectory
     * new process working directory
     */
    void setWorkDirectory(const QString &_workDirectory);
    /**
     * @brief equal operator implementation
     * @param _other
     * other object
     * @return true if objects are equal
     */
    bool operator==(const QueuedProcess &_other);

private:
    /**
     * @brief process definitions
     */
    QueuedProcessDefinitions m_definitions;
    /**
     * @brief index of process
     */
    long long m_index = -1;
    /**
     * @brief process line to launch
     */
    QString m_processLine;
};


#endif /* QUEUEDPROCESS_H */
