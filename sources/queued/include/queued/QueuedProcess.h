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
 * @file QueuedProcess.h
 * Header of Queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDPROCESS_H
#define QUEUEDPROCESS_H

#include <QDateTime>
#include <QProcess>
#include <QVariant>


class QueuedControlGroupsAdaptor;
namespace QueuedLimits
{
struct Limits;
};

/**
 * @brief implementation over QProcess to run processes
 */
class QueuedProcess : public QProcess
{
    Q_OBJECT
    Q_PROPERTY(QList<Q_PID> childrenPids READ childrenPids)
    Q_PROPERTY(long long index READ index)
    Q_PROPERTY(QString name READ name)
    // mutable properties
    Q_PROPERTY(QDateTime endTime READ endTime WRITE setEndTime)
    Q_PROPERTY(uint gid READ uid WRITE setGid)
    Q_PROPERTY(QString limits READ limits WRITE setLimits)
    Q_PROPERTY(QString logError READ logError WRITE setLogError)
    Q_PROPERTY(QString logOutput READ logOutput WRITE setLogOutput)
    Q_PROPERTY(uint nice READ nice WRITE setNice)
    Q_PROPERTY(QDateTime startTime READ startTime WRITE setStartTime)
    Q_PROPERTY(uint uid READ uid WRITE setUid)
    Q_PROPERTY(long long user READ user WRITE setUser)
    Q_PROPERTY(QString workDirectory READ workDirectory WRITE setWorkDirectory)

public:
    /**
     * @struct QueuedProcessModDefinitions
     * @brief structure to define process modifications
     * @var QueuedProcessModDefinitions::field
     * field name
     * @var QueuedProcessModDefinitions::value
     * new field value
     * @var QueuedProcessModDefinitions::task
     * task ID
     * @var QueuedProcessModDefinitions::time
     * modification time
     * @var QueuedProcessModDefinitions::user
     * user ID, who modified the task
     */
    struct QueuedProcessModDefinitions {
        QString field;
        QVariant value;
        long long task = 0;
        QDateTime time;
        long long user = 0;
    };
    /**
     * @struct QueuedProcessDefinition
     * @brief structure to define process
     * @var QueuedProcessDefinitions::command
     * command line
     * @var QueuedProcessDefinitions::arguments
     * command line arguments
     * @var QueuedProcessDefinitions::workingDirectory
     * path to working directory
     * @var QueuedProcessDefinitions::uid
     * UID of process
     * @var QueuedProcessDefinitions::gid
     * GID of process
     * @var QueuedProcessDefinitions::nice
     * nice level of process
     * @var QueuedProcessDefinitions::startTime
     * process start time
     * @var QueuedProcessDefinitions::endTime
     * process end time
     * @var QueuedProcessDefinitions::user
     * task owner ID
     * @var QueuedProcessDefinitions::limits
     * task limits
     */
    struct QueuedProcessDefinitions {
        QString command;
        QStringList arguments;
        QString workingDirectory;
        uint uid = 0;
        uint gid = 0;
        uint nice = 0;
        QDateTime startTime;
        QDateTime endTime;
        long long user = 0;
        QString limits;
        QList<QueuedProcessModDefinitions> modifications;
    };

    /**
     * @brief QueuedProcess class constructor
     * @param _parent
     * pointer to parent item
     * @param definitions
     * definitions of process
     * @param index
     * index of process
     */
    explicit QueuedProcess(QObject *_parent, const QueuedProcessDefinitions &definitions,
                           const long long index);
    /**
     * @brief QueuedProcess class destructor
     */
    virtual ~QueuedProcess();
    /**
     * @brief force kill ald children
     */
    void killChildren();
    // properties
    /**
     * @brief children processes
     * @return list of pids of children processes
     */
    QList<Q_PID> childrenPids() const;
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
     * @brief process error log
     * @return path to process error log
     */
    QString logError() const;
    /**
     * @brief process output log
     * @return path to process output log
     */
    QString logOutput() const;
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
     * @brief set process error log
     */
    void setLogError(const QString &);
    /**
     * @brief set process output log
     */
    void setLogOutput(const QString &);
    /**
     * @brief set process nice
     * @param _nice
     * new process nice
     */
    void setNice(const uint _nice);
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

public slots:
    /**
     * @brief method which will be called to apply cgroup after start
     */
    void applyCGroup();

protected:
    /**
     * @brief apply child process properties
     */
    void setupChildProcess();

private:
    QueuedControlGroupsAdaptor *m_cgroup = nullptr;
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
