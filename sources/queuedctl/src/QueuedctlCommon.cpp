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


#include "QueuedctlCommon.h"

#include <queued/Queued.h>

#include <iostream>

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
        qCWarning(LOG_APP) << "Invalid command";
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
        cmdList += QString("  %1%2").arg(cmd, -21).arg(QueuedctlArguments[cmd].description);

    return cmdList.join('\n');
}


QString QueuedctlCommon::hashHashToString(const QHash<QString, QHash<QString, QString>> &_hash)
{
    qCDebug(LOG_APP) << "Convert hash to string" << _hash;

    QStringList output;

    QStringList groups = _hash.keys();
    groups.sort();
    for (auto &group : groups) {
        output += group;
        QStringList keys = _hash[group].keys();
        keys.sort();
        for (auto &key : keys)
            output += QString("\t%1: %2").arg(key).arg(_hash[group][key]);
    }

    return output.join('\n');
}


QString QueuedctlCommon::hashToString(const QVariantHash &_hash)
{
    qCDebug(LOG_APP) << "Convert hash to string" << _hash;

    QStringList output;
    QStringList keys = _hash.keys();
    keys.sort();
    for (auto &key : keys)
        output += QString("%1: %2").arg(key).arg(_hash[key].toString().replace('\n', ' '));

    return output.join('\n');
}


QString QueuedctlCommon::hashListToString(const QList<QVariantHash> &_list)
{
    qCDebug(LOG_APP) << "Convert hash list to string" << _list;

    if (_list.isEmpty())
        return "";

    QStringList output;
    // get table header
    QStringList header = _list.first().keys();
    header.sort();
    output += header.join(',');
    // append rows
    for (auto &hash : _list) {
        QStringList row;
        std::for_each(header.cbegin(), header.cend(), [&hash, &row](const QString &column) {
            row += hash[column].toString().replace('\n', ' ');
        });
        output += row.join(',');
    }

    return output.join('\n');
}


void QueuedctlCommon::preprocess(const QStringList &_args, QCommandLineParser &_parser)
{
    qCDebug(LOG_APP) << "Preprocess command" << _args;

    QString command = _args.isEmpty() ? "" : _args.first();
    // HACK: workaround to show valid help message
    auto id = QueuedctlArguments.contains(command) ? QueuedctlArguments[command].id
                                                   : QueuedctlArgument::Invalid;
    _parser.addPositionalArgument(id == QueuedctlArgument::Invalid ? "command" : command,
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
    case QueuedctlArgument::PluginOptions:
    case QueuedctlArgument::PluginRemove:
    case QueuedctlArgument::PluginSpecification:
        QueuedctlPlugins::parser(_parser);
        break;
    case QueuedctlArgument::PluginList:
        break;
    case QueuedctlArgument::Report:
        QueuedctlUser::parserReport(_parser);
        break;
    case QueuedctlArgument::Status:
        break;
    case QueuedctlArgument::TaskAdd:
        QueuedctlTask::parserAdd(_parser);
        break;
    case QueuedctlArgument::TaskGet:
        QueuedctlTask::parserGet(_parser);
        break;
    case QueuedctlArgument::TaskList:
        QueuedctlTask::parserList(_parser);
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
    case QueuedctlArgument::UserList:
        QueuedctlUser::parserList(_parser);
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
        std::cout << "Subprocess returns error" << std::endl;
    if (!_result.output.isEmpty())
        std::cout << qPrintable(_result.output) << std::endl;
}


QueuedctlCommon::QueuedctlResult
QueuedctlCommon::process(QCommandLineParser &_parser, const QString &_cache, const QString &_user)
{
    qCDebug(LOG_APP) << "Process command with args"
                     << "using auth" << _cache << _user;

    auto result = QueuedctlResult();
    QStringList args = _parser.positionalArguments();
    QString command = args.isEmpty() ? "" : args.first();

    auto id = QueuedctlArguments.contains(command) ? QueuedctlArguments[command].id
                                                   : QueuedctlArgument::Invalid;
    checkArgs(args, QueuedctlArguments[command].positionalArgsCount, _parser);

    QString token = (id == QueuedctlArgument::Auth) ? "" : QueuedctlAuth::getToken(_cache, _user);

    switch (id) {
    case QueuedctlArgument::Auth: {
        result = QueuedctlAuth::auth(_user, _cache);
        break;
    }
    case QueuedctlArgument::OptionGet: {
        result = QueuedctlOption::getOption(args.at(1), token);
        break;
    }
    case QueuedctlArgument::OptionSet: {
        result = QueuedctlOption::editOption(args.at(1), args.at(2), token);
        break;
    }
    case QueuedctlArgument::PermissionAdd: {
        auto userIdRes = QueuedCoreAdaptor::getUserId(args.at(1), token);
        long long userId = -1;
        userIdRes.match(
            [&userId](const long long val) { userId = val; },
            [&result](const QueuedError &err) { result.output = err.message().c_str(); });
        if (userId == -1)
            break;
        result = QueuedctlPermissions::addPermission(userId, args.at(2), token);
        break;
    }
    case QueuedctlArgument::PermissionRemove: {
        auto userIdRes = QueuedCoreAdaptor::getUserId(args.at(1), token);
        long long userId = -1;
        userIdRes.match(
            [&userId](const long long val) { userId = val; },
            [&result](const QueuedError &err) { result.output = err.message().c_str(); });
        if (userId == -1)
            break;
        result = QueuedctlPermissions::removePermission(userId, args.at(2), token);
        break;
    }
    case QueuedctlArgument::PluginAdd: {
        result = QueuedctlPlugins::addPlugin(args.at(1), token);
        break;
    }
    case QueuedctlArgument::PluginList: {
        result = QueuedctlPlugins::listPlugins();
        break;
    }
    case QueuedctlArgument::PluginOptions: {
        result = QueuedctlPlugins::getPluginOptions(args.at(1), token);
        break;
    }
    case QueuedctlArgument::PluginRemove: {
        result = QueuedctlPlugins::removePlugin(args.at(1), token);
        break;
    }
    case QueuedctlArgument::PluginSpecification: {
        result = QueuedctlPlugins::getPlugin(args.at(1), token);
        break;
    }
    case QueuedctlArgument::Report: {
        result = QueuedctlUser::getReport(_parser, token);
        break;
    }
    case QueuedctlArgument::Status: {
        auto res = QueuedCoreAdaptor::getStatus();
        res.match(
            [&result](const QueuedStatusMap &val) {
                result.status = true;
                result.output = hashHashToString(val);
            },
            [&result](const QueuedError &err) { result.output = err.message().c_str(); });
        break;
    }
    case QueuedctlArgument::TaskAdd: {
        auto definitions = QueuedctlTask::getDefinitions(_parser, false, token);
        result = QueuedctlTask::addTask(definitions, token);
        break;
    }
    case QueuedctlArgument::TaskGet: {
        result = QueuedctlTask::getTask(args.at(1).toLongLong(), args.at(2), token);
        break;
    }
    case QueuedctlArgument::TaskList: {
        result = QueuedctlTask::getTasks(_parser, token);
        break;
    }
    case QueuedctlArgument::TaskSet: {
        auto definitions = QueuedctlTask::getDefinitions(_parser, true, token);
        result = QueuedctlTask::setTask(args.at(1).toLongLong(), definitions, token);
        break;
    }
    case QueuedctlArgument::TaskStart: {
        result = QueuedctlTask::startTask(args.at(1).toLongLong(), token);
        break;
    }
    case QueuedctlArgument::TaskStop: {
        result = QueuedctlTask::stopTask(args.at(1).toLongLong(), token);
        break;
    }
    case QueuedctlArgument::UserAdd: {
        auto definitions = QueuedctlUser::getDefinitions(_parser, false);
        result = QueuedctlUser::addUser(definitions, token);
        break;
    }
    case QueuedctlArgument::UserGet: {
        auto userIdRes = QueuedCoreAdaptor::getUserId(args.at(1), token);
        long long userId = -1;
        userIdRes.match(
            [&userId](const long long val) { userId = val; },
            [&result](const QueuedError &err) { result.output = err.message().c_str(); });
        if (userId == -1)
            break;
        result = QueuedctlUser::getUser(userId, args.at(2), token);
        break;
    }
    case QueuedctlArgument::UserList: {
        result = QueuedctlUser::getUsers(_parser, token);
        break;
    }
    case QueuedctlArgument::UserSet: {
        auto userIdRes = QueuedCoreAdaptor::getUserId(args.at(1), token);
        long long userId = -1;
        userIdRes.match(
            [&userId](const long long val) { userId = val; },
            [&result](const QueuedError &err) { result.output = err.message().c_str(); });
        if (userId == -1)
            break;
        auto definitions = QueuedctlUser::getDefinitions(_parser, true);
        result = QueuedctlUser::setUser(userId, definitions, token);
        break;
    }
    case QueuedctlArgument::Invalid: {
        break;
    }
    }

    return result;
}
