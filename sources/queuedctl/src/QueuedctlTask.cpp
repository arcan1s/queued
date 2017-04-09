/*
 * Copyright (c) 2017 Evgeniy Alekseev
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


#include "QueuedctlTask.h"

#include <QDir>

#include <queued/Queued.h>

#include "QueuedctlUser.h"

extern "C" {
#include <unistd.h>
}


long long QueuedctlTask::addTask(
    const QueuedProcess::QueuedProcessDefinitions &_definitions,
    const QString &_token)
{
    qCDebug(LOG_APP) << "Add task" << _definitions.command;

    return QueuedCoreAdaptor::sendTaskAdd(_definitions, _token);
}


QueuedProcess::QueuedProcessDefinitions
QueuedctlTask::getDefinitions(const QCommandLineParser &_parser,
                              const bool _expandAll)
{
    qCDebug(LOG_APP) << "Parse task definitions from parser, expand all"
                     << _expandAll;

    QueuedProcess::QueuedProcessDefinitions definitions;

    std::for_each(_parser.values("argument").cbegin(),
                  _parser.values("argument").cend(),
                  [&definitions](const QString &arg) {
                      if (!arg.isEmpty())
                          definitions.arguments += arg;
                  });

    definitions.nice = _parser.value("nice").toUInt();
    definitions.user
        = _parser.value("task-user").isEmpty()
              ? 0
              : QueuedCoreAdaptor::getUserId(_parser.value("task-user"));
    definitions.workingDirectory
        = QFileInfo(_parser.value("directory")).absoluteFilePath();
    // limits now
    QueuedLimits::Limits limits(
        _parser.value("limit-cpu").toLongLong(),
        _parser.value("limit-gpu").toLongLong(),
        QueuedLimits::convertMemory(_parser.value("limit-memory")),
        QueuedLimits::convertMemory(_parser.value("limit-gpumemory")),
        QueuedLimits::convertMemory(_parser.value("limit-storage")));
    definitions.limits = limits.toString();

    // all options
    if (_expandAll) {
        definitions.command
            = QFileInfo(_parser.value("program")).absoluteFilePath();
        definitions.endTime
            = QDateTime::fromString(_parser.value("stop"), Qt::ISODateWithMs);
        definitions.gid = _parser.value("gid").toUInt();
        definitions.startTime
            = QDateTime::fromString(_parser.value("start"), Qt::ISODateWithMs);
        definitions.uid = _parser.value("uid").toUInt();
    } else {
        // queuedctl -- task-add /path/to/application
        definitions.command
            = QFileInfo(_parser.positionalArguments().at(1)).absoluteFilePath();
    }

    return definitions;
}


QVariant QueuedctlTask::getTask(const long long _id, const QString &_property)
{
    qCDebug(LOG_APP) << "Get property" << _property << "from task" << _id;

    if (_property.isEmpty())
        return QueuedCoreAdaptor::getTask(_id);
    else
        return QueuedCoreAdaptor::getTask(_id, _property);
}


QList<QVariantHash> QueuedctlTask::getTasks(const QCommandLineParser &_parser,
                                            const QString &_token)
{
    long long user
        = _parser.value("task-user").isEmpty()
              ? -1
              : QueuedCoreAdaptor::getUserId(_parser.value("task-user"));
    QDateTime stop
        = QDateTime::fromString(_parser.value("stop"), Qt::ISODateWithMs);
    QDateTime start
        = QDateTime::fromString(_parser.value("start"), Qt::ISODateWithMs);

    return QueuedCoreAdaptor::getTasks(user, start, stop, _token);
}


void QueuedctlTask::parserAdd(QCommandLineParser &_parser)
{
    _parser.addPositionalArgument("program", "Command line.", "<program>");

    // command line arguments
    QCommandLineOption argumentOption(QStringList() << "a"
                                                    << "argument",
                                      "Command line argument.", "argument", "");
    _parser.addOption(argumentOption);
    // working directory
    QCommandLineOption directoryOption("directory",
                                       "Command working directory.",
                                       "directory", QDir::currentPath());
    _parser.addOption(directoryOption);
    // user
    QCommandLineOption userOption("task-user", "Task user.", "task-user",
                                  ::getlogin());
    _parser.addOption(userOption);
    // nice
    QCommandLineOption niceOption("nice", "Task nice level.", "nice", "0");
    _parser.addOption(niceOption);
    // cpu limit
    QCommandLineOption cpuOption("limit-cpu", "Task CPU limit.", "limit-cpu",
                                 "0");
    _parser.addOption(cpuOption);
    // gpu limit
    QCommandLineOption gpuOption("limit-gpu", "Task GPU limit.", "limit-gpu",
                                 "0");
    _parser.addOption(gpuOption);
    // memory limit
    QCommandLineOption memoryOption("limit-memory", "Task memory limit.",
                                    "limit-memory", "0");
    _parser.addOption(memoryOption);
    // gpu memory limit
    QCommandLineOption gpumemoryOption(
        "limit-gpumemory", "Task GPU memory limit.", "limit-gpumemory", "0");
    _parser.addOption(gpumemoryOption);
    // storage limit
    QCommandLineOption storageOption("limit-storage", "Task storage limit.",
                                     "limit-storage", "0");
    _parser.addOption(storageOption);
}


void QueuedctlTask::parserGet(QCommandLineParser &_parser)
{
    _parser.addPositionalArgument("id", "Task ID.", "<id>");
    _parser.addPositionalArgument("property", "Task property name.",
                                  "<property>");
}


void QueuedctlTask::parserList(QCommandLineParser &_parser)
{
    // user
    QCommandLineOption userOption("task-user", "Task user.", "task-user", "");
    _parser.addOption(userOption);
    // start
    QCommandLineOption startOption("start", "Task start time.", "start", "");
    _parser.addOption(startOption);
    // stop
    QCommandLineOption stopOption("stop", "Task stop time.", "stop", "");
    _parser.addOption(stopOption);
}


void QueuedctlTask::parserSet(QCommandLineParser &_parser)
{
    _parser.addPositionalArgument("id", "Task ID.", "<id>");

    // command line
    QCommandLineOption commandOption(QStringList() << "p"
                                                   << "program",
                                     "Command line.", "program", "");
    _parser.addOption(commandOption);
    // command line arguments
    QCommandLineOption argumentOption(QStringList() << "a"
                                                    << "argument",
                                      "Command line argument.", "argument", "");
    _parser.addOption(argumentOption);
    // working directory
    QCommandLineOption directoryOption(
        "directory", "Command working directory.", "directory", "");
    _parser.addOption(directoryOption);
    // user
    QCommandLineOption userOption("task-user", "Task user.", "task-user", "");
    _parser.addOption(userOption);
    // nice
    QCommandLineOption niceOption("nice", "Task nice level.", "nice", "0");
    _parser.addOption(niceOption);
    // uid
    QCommandLineOption uidOption("uid", "UID.", "uid", "0");
    _parser.addOption(uidOption);
    // gid
    QCommandLineOption gidOption("gid", "GID.", "gid", "0");
    _parser.addOption(gidOption);
    // start
    QCommandLineOption startOption("start", "Task start time.", "start", "");
    _parser.addOption(startOption);
    // stop
    QCommandLineOption stopOption("stop", "Task stop time.", "stop", "");
    _parser.addOption(stopOption);
    // cpu limit
    QCommandLineOption cpuOption("limit-cpu", "Task CPU limit.", "limit-cpu",
                                 "-1");
    _parser.addOption(cpuOption);
    // gpu limit
    QCommandLineOption gpuOption("limit-gpu", "Task GPU limit.", "limit-gpu",
                                 "-1");
    _parser.addOption(gpuOption);
    // memory limit
    QCommandLineOption memoryOption("limit-memory", "Task memory limit.",
                                    "limit-memory", "-1");
    _parser.addOption(memoryOption);
    // gpu memory limit
    QCommandLineOption gpumemoryOption(
        "limit-gpumemory", "Task GPU memory limit.", "limit-gpumemory", "-1");
    _parser.addOption(gpumemoryOption);
    // storage limit
    QCommandLineOption storageOption("limit-storage", "Task storage limit.",
                                     "limit-storage", "-1");
    _parser.addOption(storageOption);
}


void QueuedctlTask::parserStart(QCommandLineParser &_parser)
{
    _parser.addPositionalArgument("id", "Task ID.", "<id>");
}


bool QueuedctlTask::setTask(
    const long long _id,
    const QueuedProcess::QueuedProcessDefinitions &_definitions,
    const QString &_token)
{
    qCDebug(LOG_APP) << "Edit task" << _id;

    return QueuedCoreAdaptor::sendTaskEdit(_id, _definitions, _token);
}


bool QueuedctlTask::startTask(const long long _id, const QString &_token)
{
    qCDebug(LOG_APP) << "Start task" << _id;

    return QueuedCoreAdaptor::sendTaskStart(_id, _token);
}


bool QueuedctlTask::stopTask(const long long _id, const QString &_token)
{
    qCDebug(LOG_APP) << "Stop task" << _id;

    return QueuedCoreAdaptor::sendTaskStop(_id, _token);
}
