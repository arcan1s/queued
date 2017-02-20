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

#include <QHash>
#include <QObject>

#include "queued/QueuedProcess.h"


typedef QHash<long long, QueuedProcess *> QueuedProcessMap;
typedef QHash<long long, QMetaObject::Connection> QueuedProcessConnectionMap;

/**
 * @brief implementation over QProcess to run processes
 */
class QueuedProcessManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(OnExitAction onExit READ onExit)

public:
    /**
     * @enum OnExitAction
     * @brief action with child process on destruction
     * @var OnExitAction::Terminate
     * send SIGTERM on exit
     * @var OnExitAction::Kill
     * send SIGKILL on exit
     */
    enum class OnExitAction { Terminate, Kill };

    /**
     * @brief QueuedProcessManager class constructor
     * @param parent         pointer to parent item
     * @param onExit         default action on exit
     */
    explicit QueuedProcessManager(QObject *parent, const OnExitAction onExit);
    /**
     * @brief QueuedProcessManager class destructor
     */
    virtual ~QueuedProcessManager();
    /**
     * @brief add task
     * @param _index         task index
     * @param _definitions   process definitions
     * @return pointer to created task
     */
    QueuedProcess *add(const long long _index,
                       const QueuedProcessDefinitions _definitions);
    /**
     * @brief default action on exit
     * @return default action from possible ones
     */
    OnExitAction onExit() const;
    /**
     * @brief task
     * @param _index         task index
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
     * @param _index         task index
     */
    void remove(const long long _index);

private slots:
    /**
     * @brief slot for catching finished tasks
     * @param _exitCode      exit code of finished task
     * @param _exitStatus    exit status of finished task
     * @param _index         index of finished task
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
    OnExitAction m_onExit = OnExitAction::Kill;
    /**
     * @brief processes list
     */
    QueuedProcessMap m_processes;
};

#endif /* QUEUEDPROCESS_H */
