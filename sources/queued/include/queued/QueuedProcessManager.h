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
 * @file QueuedProcessManager.h
 * Header of Queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDPROCESSMANAGER_H
#define QUEUEDPROCESSMANAGER_H

#include <QDateTime>
#include <QHash>
#include <QObject>

#include "QueuedProcess.h"


class QueuedPluginManagerInterface;
namespace QueuedEnums
{
enum class ExitAction;
};

/**
 * @brief implementation over QProcess to run processes
 */
class QueuedProcessManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QueuedEnums::ExitAction onExit READ onExit WRITE setExitAction)

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
     * @brief QueuedProcessManager class constructor
     * @param _parent
     * pointer to parent item
     */
    explicit QueuedProcessManager(QObject *_parent);
    /**
     * @brief QueuedProcessManager class destructor
     */
    virtual ~QueuedProcessManager();
    /**
     * @brief parse task definitions from table data
     * @param _properties
     * map of task properties
     * @return data mapped to internal format
     */
    static QueuedProcess::QueuedProcessDefinitions
    parseDefinitions(const QVariantHash &_properties);
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
     * @brief select and start task automatically
     */
    void start();
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
    QueuedEnums::ExitAction onExit() const;
    /**
     * @brief set on exit action
     * @param _action
     * new on exit action
     */
    void setExitAction(const QueuedEnums::ExitAction _action);
    /**
     * @brief get used limits
     * @return used system limits
     */
    QueuedLimits::Limits usedLimits();

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
    QueuedEnums::ExitAction m_onExit;
    /**
     * @brief processes list
     */
    QueuedProcessMap m_processes;
};


#endif /* QUEUEDPROCESS_H */
