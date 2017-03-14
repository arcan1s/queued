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


void QueuedctlCommon::preprocess(const QStringList &_args,
                                 QCommandLineParser &_parser)
{
    qCDebug(LOG_APP) << "Preprocess command" << _args;

    QString command = _args.isEmpty() ? QString() : _args.first();
    // HACK: workaround to show valid help message
    _parser.addPositionalArgument(command, "Command to execute.");

    if (command == "auth")
        QueuedctlAuth::parser(_parser);
    else if (command == "option-get")
        QueuedctlOption::parserGet(_parser);
    else if (command == "option-set")
        QueuedctlOption::parserSet(_parser);
    else if (command == "task-add")
        QueuedctlTask::parserAdd(_parser);
    else if (command == "task-get")
        QueuedctlTask::parserGet(_parser);
    else if (command == "task-set")
        QueuedctlTask::parserSet(_parser);
    else if (command == "user-add") {
    } else if (command == "user-get") {
    } else if (command == "user-set") {
    } else if (!command.isEmpty())
        checkArgs(_args, -1, _parser);
}


void QueuedctlCommon::print(QueuedctlResult &_result)
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

    if (command == "auth") {
        checkArgs(args, 1, _parser);
        QString token = QueuedctlAuth::auth(_user);
        result.status = !token.isEmpty();
        if (result.status)
            QueuedctlAuth::setToken(token, _cache);
    } else if (command == "option-get") {
        checkArgs(args, 2, _parser);
        QVariant value = QueuedctlOption::getOption(args.at(1));
        result.status = value.isValid();
        result.output = value.toString();
    } else if (command == "option-set") {
        checkArgs(args, 3, _parser);
        QString token = QueuedctlAuth::getToken(_cache, _user);
        result.status
            = QueuedctlOption::editOption(args.at(1), args.at(2), token);
        if (result.status)
            result.output
                = QString("Option %1 set to %2").arg(args.at(1), args.at(2));
        else
            result.output = QString("Could not set option %1 to %2")
                                .arg(args.at(1), args.at(2));
    } else if (command == "task-get") {
        checkArgs(args, 3, _parser);
        QVariant value
            = QueuedctlTask::getTask(args.at(1).toLongLong(), args.at(2));
        result.status = value.isValid();
        result.output = value.toString();
    } else if (command == "task-set") {
        checkArgs(args, 2, _parser);
        QString token = QueuedctlAuth::getToken(_cache, _user);
        auto definitions = QueuedctlTask::getDefinitions(_parser);
        result.status = QueuedctlTask::setTask(args.at(1).toLongLong(),
                                               definitions, token);
    }

    return result;
}
