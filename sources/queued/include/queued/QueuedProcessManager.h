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
 * @file QueuedProcessManager.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDPROCESSMANAGER_H
#define QUEUEDPROCESSMANAGER_H

#include <QDateTime>
#include <QHash>
#include <QObject>

#include "QueuedProcess.h"


/**
 * @brief implementation over QProcess to run processes
 */
class QueuedProcessManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(OnExitAction onExit READ onExit WRITE setOnExitAction)

public:
    /**
 * @typedef QueuedProcessMap
 * map of indices to QueuedProcess pointers
 */
    typedef QHash<long long, QueuedProcess *> QueuedProcessMap;
    /**
     * @typedef QueuedProcessConnectionMap
     * map of indices to related QMetaObject::Connection
     */
    typedef QHash<long long, QMetaObject::Connection>
        QueuedProcessConnectionMap;

    /**
     * @enum OnExitAction
     * @brief action with child process on destruction
     * @var OnExitAction::Terminate
     * send SIGTERM on exit
     * @var OnExitAction::Kill
     * send SIGKILL on exit
     */
    enum class OnExitAction { Terminate = 1 << 1, Kill = 1 << 2 };

    /**
     * @brief QueuedProcessManager class constructor
     * @param parent
     * pointer to parent item
     * @param onExit
     * default action on exit
     */
    explicit QueuedProcessManager(QObject *parent, const OnExitAction onExit);
    /**
     * @brief QueuedProcessManager class destructor
     */
    virtual ~QueuedProcessManager();
    /**
     * @brief add task
     * @param _properties
     * task properties from database
     * @param _index
     * task index
     * @return pointer to created task
     */
    QueuedProcess *add(const QVariantHash &_properties, const long long _index);
    /**
     * @brief add task
     * @param _definitions
     * process definitions
     * @param _index
     * task index
     * @return pointer to created task
     */
    QueuedProcess *
    add(const QueuedProcess::QueuedProcessDefinitions &_definitions,
        const long long _index);
    /**
     * @brief add tasks from database
     * @param _processes
     * database stored tasks
     */
    void loadProcesses(const QList<QVariantHash> &_processes);
    /**
     * @brief task
     * @param _index
     * task index
     * @return task found by index or nullptr
     */
    QueuedProcess *process(const long long _index);
    /**
     * @brief all tasks
     * @return list of all tasks
     */
    QueuedProcessMap processes();
    /**
     * @brief remove task from list
     * @param _index
     * task index
     */
    void remove(const long long _index);
    /**
     * @brief force start task
     * @param _index
     * task index
     */
    void start(const long long _index);
    /**
     * @brief force stop task
     * @param _index
     * task index
     */
    void stop(const long long _index);
    // properties
    /**
     * @brief default action on exit
     * @return default action from possible ones
     */
    OnExitAction onExit() const;
    /**
     * @brief set on exit action
     * @param _action
     * new on exit action
     */
    void setOnExitAction(const OnExitAction _action);

signals:
    /**
     * @brief signal which will be called on task start
     * @param _index
     * task index
     * @param _time
     * task start time
     */
    void taskStartTimeReceived(const long long _index, const QDateTime &_time);
    /**
     * @brief signal which will be called on task end
     * @param _index
     * task index
     * @param _time
     * task stop time
     */
    void taskStopTimeReceived(const long long _index, const QDateTime &_time);

private slots:
    /**
     * @brief slot for catching finished tasks
     * @param _exitCode
     * exit code of finished task
     * @param _exitStatus
     * exit status of finished task
     * @param _index
     * index of finished task
     */
    void taskFinished(const int _exitCode,
                      const QProcess::ExitStatus _exitStatus,
                      const long long _index);

private:
    /**
     * @brief connection map
     */
    QueuedProcessConnectionMap m_connections;
    /**
     * @brief action on exit
     */
    OnExitAction m_onExit = OnExitAction::Terminate;
    /**
     * @brief processes list
     */
    QueuedProcessMap m_processes;
};


#endif /* QUEUEDPROCESS_H */
