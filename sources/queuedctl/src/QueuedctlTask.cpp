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


QueuedProcess::QueuedProcessDefinitions
QueuedctlTask::getDefinitions(const QCommandLineParser &_parser)
{
    QueuedProcess::QueuedProcessDefinitions definitions;

    definitions.command = _parser.value("program");
    std::for_each(_parser.values("argument").cbegin(),
                  _parser.values("argument").cend(),
                  [&definitions](const QString &arg) {
                      if (!arg.isEmpty())
                          definitions.arguments += arg;
                  });
    definitions.endTime
        = QDateTime::fromString(_parser.value("stop"), Qt::ISODate);
    definitions.gid = _parser.value("gid").toUInt();
    definitions.nice = _parser.value("nice").toUInt();
    definitions.startTime
        = QDateTime::fromString(_parser.value("start"), Qt::ISODate);
    definitions.uid = _parser.value("uid").toUInt();
    definitions.user = _parser.value("task-user").toLongLong();
    definitions.workingDirectory = _parser.value("directory");
    // limits now
    QueuedLimits::Limits limits(
        _parser.value("limit-cpu").toLongLong(),
        _parser.value("limit-gpu").toLongLong(),
        QueuedLimits::convertMemory(_parser.value("limit-memory")),
        QueuedLimits::convertMemory(_parser.value("limit-gpumemory")),
        QueuedLimits::convertMemory(_parser.value("limit-storage")));
    definitions.limits = limits.toString();

    return definitions;
}


QVariant QueuedctlTask::getTask(const long long _id, const QString &_property)
{
    qCDebug(LOG_APP) << "Get property" << _property << "from task" << _id;

    return QueuedCoreAdaptor::getTask(_id, _property);
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
    QCommandLineOption directoryOption(QStringList() << "d"
                                                     << "directory",
                                       "Command working directory.",
                                       "directory", QDir::currentPath());
    _parser.addOption(directoryOption);
    // user
    // TODO grab used user ID
    QCommandLineOption userOption("task-user", "Task user.", "task-user", "0");
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
    QCommandLineOption directoryOption(QStringList() << "d"
                                                     << "directory",
                                       "Command working directory.",
                                       "directory", "");
    _parser.addOption(directoryOption);
    // user
    QCommandLineOption userOption("task-user", "Task user.", "task-user", "0");
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


bool QueuedctlTask::setTask(
    const long long _id,
    const QueuedProcess::QueuedProcessDefinitions &_definitions,
    const QString &_token)
{
    qCDebug(LOG_APP) << "Edit task" << _id;

    return QueuedCoreAdaptor::sendTaskEdit(_id, _definitions, _token);
}
