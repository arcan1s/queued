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


#include "QueuedctlCommon.h"

#include <queued/Queued.h>

#include "QueuedctlAuth.h"
#include "QueuedctlOption.h"
#include "QueuedctlTask.h"


void QueuedctlCommon::checkArgs(const QStringList &_args, const int _count,
                                QCommandLineParser &_parser)
{
    qCDebug(LOG_APP) << "Check args count" << _args << _count;

    if (_args.count() != _count) {
        qWarning() << "Invalid command";
        _parser.showHelp(1);
    }
}


QString QueuedctlCommon::commandsHelp()
{
    QStringList cmdList = {"Commands:"};

    QStringList commands = QueuedctlArguments.keys();
    commands.sort();
    for (auto &cmd : commands)
        // align like default help message
        cmdList += QString("  %1%2").arg(cmd, -21).arg(
            QueuedctlArguments[cmd].description);

    return cmdList.join('\n');
}


void QueuedctlCommon::preprocess(const QStringList &_args,
                                 QCommandLineParser &_parser)
{
    qCDebug(LOG_APP) << "Preprocess command" << _args;

    QString command = _args.isEmpty() ? QString() : _args.first();
    // HACK: workaround to show valid help message
    auto id = QueuedctlArguments.contains(command)
                  ? QueuedctlArguments[command].id
                  : QueuedctlArgument::Invalid;
    _parser.addPositionalArgument(id == QueuedctlArgument::Invalid ? "command"
                                                                   : command,
                                  "Command to execute.");

    if (command.isEmpty())
        return;

    switch (id) {
    case QueuedctlArgument::Auth:
        QueuedctlAuth::parser(_parser);
        break;
    case QueuedctlArgument::OptionGet:
        QueuedctlOption::parserGet(_parser);
        break;
    case QueuedctlArgument::OptionSet:
        QueuedctlOption::parserSet(_parser);
        break;
    case QueuedctlArgument::TaskAdd:
        QueuedctlTask::parserAdd(_parser);
        break;
    case QueuedctlArgument::TaskGet:
        QueuedctlTask::parserGet(_parser);
        break;
    case QueuedctlArgument::TaskSet:
        QueuedctlTask::parserSet(_parser);
        break;
    case QueuedctlArgument::UserAdd:
        break;
    case QueuedctlArgument::UserGet:
        break;
    case QueuedctlArgument::UserSet:
        break;
    case QueuedctlArgument::Invalid:
        checkArgs(_args, -1, _parser);
        break;
    }
}


void QueuedctlCommon::print(const QueuedctlResult &_result)
{
    if (!_result.status)
        qInfo() << "Subprocess returns error";
    if (!_result.output.isEmpty())
        QDebug(QtMsgType::QtInfoMsg).noquote() << _result.output;
}


QueuedctlCommon::QueuedctlResult
QueuedctlCommon::process(QCommandLineParser &_parser, const QString &_cache,
                         const QString &_user)
{
    qCDebug(LOG_APP) << "Process command with args"
                     << "using auth" << _cache << _user;

    QueuedctlResult result;
    QStringList args = _parser.positionalArguments();
    QString command = args.isEmpty() ? QString() : args.first();

    auto id = QueuedctlArguments.contains(command)
                  ? QueuedctlArguments[command].id
                  : QueuedctlArgument::Invalid;
    checkArgs(args, QueuedctlArguments[command].positionalArgsCount, _parser);

    switch (id) {
    case QueuedctlArgument::Auth: {
        QString token = QueuedctlAuth::auth(_user);
        result.status = !token.isEmpty();
        if (result.status)
            QueuedctlAuth::setToken(token, _cache);
        break;
    }
    case QueuedctlArgument::OptionGet: {
        QVariant value = QueuedctlOption::getOption(args.at(1));
        result.status = value.isValid();
        result.output = value.toString();
        break;
    }
    case QueuedctlArgument::OptionSet: {
        QString token = QueuedctlAuth::getToken(_cache, _user);
        result.status
            = QueuedctlOption::editOption(args.at(1), args.at(2), token);
        if (result.status)
            result.output
                = QString("Option %1 set to %2").arg(args.at(1), args.at(2));
        else
            result.output = QString("Could not set option %1 to %2")
                                .arg(args.at(1), args.at(2));
        break;
    }
    case QueuedctlArgument::TaskAdd: {
        QString token = QueuedctlAuth::getToken(_cache, _user);
        auto definitions = QueuedctlTask::getDefinitions(_parser, false);
        long long id = QueuedctlTask::addTask(definitions, token);
        result.status = (id > 0);
        if (result.status)
            result.output = QString("Task %1 added").arg(id);
        else
            result.output = QString("Could not add task");
        break;
    }
    case QueuedctlArgument::TaskGet: {
        QVariant value
            = QueuedctlTask::getTask(args.at(1).toLongLong(), args.at(2));
        result.status = value.isValid();
        result.output = value.toString();
        break;
    }
    case QueuedctlArgument::TaskSet: {
        QString token = QueuedctlAuth::getToken(_cache, _user);
        auto definitions = QueuedctlTask::getDefinitions(_parser, true);
        result.status = QueuedctlTask::setTask(args.at(1).toLongLong(),
                                               definitions, token);
        break;
    }
    case QueuedctlArgument::UserAdd: {
        break;
    }
    case QueuedctlArgument::UserGet: {
        break;
    }
    case QueuedctlArgument::UserSet: {
        break;
    }
    case QueuedctlArgument::Invalid: {
        break;
    }
    }

    return result;
}
