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
#include "QueuedctlPermissions.h"
#include "QueuedctlPlugins.h"
#include "QueuedctlTask.h"
#include "QueuedctlUser.h"


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


QString QueuedctlCommon::hashToString(const QVariantHash &_hash)
{
    qCDebug(LOG_APP) << "Convert hash to string" << _hash;

    QStringList output;
    QStringList keys = _hash.keys();
    keys.sort();
    for (auto &key : keys)
        output += QString("%1: %2").arg(key).arg(
            _hash[key].toString().replace('\n', ' '));

    return output.join('\n');
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
    case QueuedctlArgument::PermissionAdd:
    case QueuedctlArgument::PermissionRemove:
        QueuedctlPermissions::parser(_parser);
        break;
    case QueuedctlArgument::PluginAdd:
    case QueuedctlArgument::PluginRemove:
        QueuedctlPlugins::parser(_parser);
        break;
    case QueuedctlArgument::PluginList:
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
    case QueuedctlArgument::TaskStart:
    case QueuedctlArgument::TaskStop:
        QueuedctlTask::parserStart(_parser);
        break;
    case QueuedctlArgument::UserAdd:
        QueuedctlUser::parserAdd(_parser);
        break;
    case QueuedctlArgument::UserGet:
        QueuedctlUser::parserGet(_parser);
        break;
    case QueuedctlArgument::UserSet:
        QueuedctlUser::parserSet(_parser);
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
            QueuedctlAuth::setToken(token, _user, _cache);
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
    case QueuedctlArgument::PermissionAdd: {
        auto userId = QueuedctlUser::getUserId(args.at(1));
        QString token = QueuedctlAuth::getToken(_cache, _user);
        result.status
            = QueuedctlPermissions::addPermission(userId, args.at(2), token);
        if (result.status)
            result.output = QString("Add permission %2 to user %1")
                                .arg(args.at(1))
                                .arg(args.at(2));
        else
            result.output = QString("Could not add permission %2 to user %1")
                                .arg(args.at(1))
                                .arg(args.at(2));
        break;
    }
    case QueuedctlArgument::PermissionRemove: {
        auto userId = QueuedctlUser::getUserId(args.at(1));
        QString token = QueuedctlAuth::getToken(_cache, _user);
        result.status
            = QueuedctlPermissions::removePermission(userId, args.at(2), token);
        if (result.status)
            result.output = QString("Remove permission %2 from user %1")
                                .arg(args.at(1))
                                .arg(args.at(2));
        else
            result.output
                = QString("Could not remove permission %2 from user %1")
                      .arg(args.at(1))
                      .arg(args.at(2));
        break;
    }
    case QueuedctlArgument::PluginAdd: {
        QString token = QueuedctlAuth::getToken(_cache, _user);
        result.status = QueuedctlPlugins::addPlugin(args.at(1), token);
        if (result.status)
            result.output = QString("Add plugin %1").arg(args.at(1));
        else
            result.output = QString("Could not add plugin %1").arg(args.at(1));
        break;
    }
    case QueuedctlArgument::PluginList: {
        result.status = true;
        result.output = QueuedctlPlugins::listPlugins().join('\n');
        break;
    }
    case QueuedctlArgument::PluginRemove: {
        QString token = QueuedctlAuth::getToken(_cache, _user);
        result.status = QueuedctlPlugins::removePlugin(args.at(1), token);
        if (result.status)
            result.output = QString("Remove plugin %1").arg(args.at(1));
        else
            result.output
                = QString("Could not remove plugin %1").arg(args.at(1));
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
        result.output = args.at(2).isEmpty() ? hashToString(value.toHash())
                                             : value.toString();
        break;
    }
    case QueuedctlArgument::TaskSet: {
        QString token = QueuedctlAuth::getToken(_cache, _user);
        auto definitions = QueuedctlTask::getDefinitions(_parser, true);
        result.status = QueuedctlTask::setTask(args.at(1).toLongLong(),
                                               definitions, token);
        break;
    }
    case QueuedctlArgument::TaskStart: {
        QString token = QueuedctlAuth::getToken(_cache, _user);
        result.status
            = QueuedctlTask::startTask(args.at(1).toLongLong(), token);
        break;
    }
    case QueuedctlArgument::TaskStop: {
        QString token = QueuedctlAuth::getToken(_cache, _user);
        result.status = QueuedctlTask::stopTask(args.at(1).toLongLong(), token);
        break;
    }
    case QueuedctlArgument::UserAdd: {
        QString token = QueuedctlAuth::getToken(_cache, _user);
        auto definitions = QueuedctlUser::getDefinitions(_parser, false);
        long long id = QueuedctlUser::addUser(definitions, token);
        result.status = (id > 0);
        if (result.status)
            result.output = QString("User %1 added").arg(id);
        else
            result.output = QString("Could not add user");
        break;
    }
    case QueuedctlArgument::UserGet: {
        auto userId = QueuedctlUser::getUserId(args.at(1));
        QVariant value = QueuedctlUser::getUser(userId, args.at(2));
        result.status = value.isValid();
        result.output = args.at(2).isEmpty() ? hashToString(value.toHash())
                                             : value.toString();
        break;
    }
    case QueuedctlArgument::UserSet: {
        auto userId = QueuedctlUser::getUserId(args.at(1));
        QString token = QueuedctlAuth::getToken(_cache, _user);
        auto definitions = QueuedctlUser::getDefinitions(_parser, true);
        result.status = QueuedctlUser::setUser(userId, definitions, token);
        break;
    }
    case QueuedctlArgument::Invalid: {
        break;
    }
    }

    return result;
}
