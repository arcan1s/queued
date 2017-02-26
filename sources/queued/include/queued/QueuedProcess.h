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

#include "QueuedEnums.h"
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
    Q_PROPERTY(unsigned int gid READ uid WRITE setGid)
    Q_PROPERTY(QueuedLimits::Limits limits READ limits WRITE setLimits)
    Q_PROPERTY(unsigned int nice READ nice WRITE setNice)
    Q_PROPERTY(QueuedEnums::ProcessState pstate READ pstate WRITE setPState)
    Q_PROPERTY(QDateTime startTime READ startTime WRITE setStartTime)
    Q_PROPERTY(unsigned int uid READ uid WRITE setUid)
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
     * @var state
     * current task state
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
        QueuedEnums::ProcessState state;
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
    unsigned int gid() const;
    /**
     * @brief process limits
     * @return process defined limits
     */
    QueuedLimits::Limits limits() const;
    /**
     * @brief process nice
     * @return process nice
     */
    unsigned int nice() const;
    /**
     * @brief process state
     * @return process defined state
     */
    QueuedEnums::ProcessState pstate() const;
    /**
     * @brief process start time
     * @return process start time
     */
    QDateTime startTime() const;
    /**
     * @brief process UID
     * @return process UID
     */
    unsigned int uid() const;
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
     * @param _commandArguments new command line arguments
     */
    void setCommandArguments(const QStringList &_commandArguments);
    /**
     * @brief set end time
     * @param _time          process end time
     */
    void setEndTime(const QDateTime &_time);
    /**
     * @brief set process GID
     * @param _gid           new process GID
     */
    void setGid(const unsigned int _gid);
    /**
     * @brief set process limits
     * @param _limits        new process limits
     */
    void setLimits(const QueuedLimits::Limits &_limits);
    /**
     * @brief set process nice
     * @param _nice          new process nice
     */
    void setNice(const unsigned int _nice);
    /**
     * @brief set process state
     * @param _limits        new process state
     */
    void setPState(const QueuedEnums::ProcessState _state);
    /**
     * @brief set start time
     * @param _time          process start time
     */
    void setStartTime(const QDateTime &_time);
    /**
     * @brief set process UID
     * @param _uid           new process UID
     */
    void setUid(const unsigned int _uid);
    /**
     * @brief set user ID
     * @param _user          new user ID
     */
    void setUser(const long long _user);
    /**
     * @brief set working directory
     * @param _workDirectory new process working directory
     */
    void setWorkDirectory(const QString &_workDirectory);
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
