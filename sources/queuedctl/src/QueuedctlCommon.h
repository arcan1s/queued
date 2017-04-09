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


#ifndef QUEUEDCTLCOMMON_H
#define QUEUEDCTLCOMMON_H

#include <QCommandLineParser>


namespace QueuedctlCommon
{
// types and constants
enum class QueuedctlArgument {
    Invalid,
    Auth,
    OptionGet,
    OptionSet,
    PermissionAdd,
    PermissionRemove,
    PluginAdd,
    PluginList,
    PluginRemove,
    Report,
    Status,
    TaskAdd,
    TaskGet,
    TaskList,
    TaskSet,
    TaskStart,
    TaskStop,
    UserAdd,
    UserGet,
    UserList,
    UserSet
};
typedef struct {
    bool status = false;
    QString output;
} QueuedctlResult;
typedef struct {
    QueuedctlArgument id;
    QString description;
    int positionalArgsCount;
} QueuedctlArgumentInfo;
const QHash<QString, QueuedctlArgumentInfo> QueuedctlArguments = {
    {"auth", {QueuedctlArgument::Auth, "Gets new auth token.", 1}},
    {"option-get", {QueuedctlArgument::OptionGet, "Gets option value.", 2}},
    {"option-set", {QueuedctlArgument::OptionSet, "Sets option value.", 3}},
    {"perm-add",
     {QueuedctlArgument::PermissionAdd, "Sets user permission.", 3}},
    {"perm-remove",
     {QueuedctlArgument::PermissionRemove, "Removes user permission.", 3}},
    {"plugin-add", {QueuedctlArgument::PluginAdd, "Adds plugin to load.", 2}},
    {"plugin-list",
     {QueuedctlArgument::PluginList, "Shows enabled plugins.", 1}},
    {"plugin-remove",
     {QueuedctlArgument::PluginRemove, "Removes plugin to load.", 2}},
    {"report", {QueuedctlArgument::Report, "Shows usage report.", 1}},
    {"status", {QueuedctlArgument::Status, "Server status.", 1}},
    {"task-add", {QueuedctlArgument::TaskAdd, "Adds new task.", 2}},
    {"task-get", {QueuedctlArgument::TaskGet, "Gets task properties.", 3}},
    {"task-list", {QueuedctlArgument::TaskList, "Gets tasks list.", 1}},
    {"task-set", {QueuedctlArgument::TaskSet, "Sets task properties.", 2}},
    {"task-start", {QueuedctlArgument::TaskStart, "Starts task.", 2}},
    {"task-stop", {QueuedctlArgument::TaskStop, "Stops task.", 2}},
    {"user-add", {QueuedctlArgument::UserAdd, "Adds new user.", 2}},
    {"user-get", {QueuedctlArgument::UserGet, "Gets user properties.", 3}},
    {"user-list", {QueuedctlArgument::UserList, "Gets users list.", 1}},
    {"user-set", {QueuedctlArgument::UserSet, "Sets user properties.", 2}}};
// methods
void checkArgs(const QStringList &_args, const int _count,
               QCommandLineParser &_parser);
QString commandsHelp();
QString hashHashToString(const QHash<QString, QHash<QString, QString>> &_hash);
QString hashToString(const QVariantHash &_hash);
QString hashListToString(const QList<QVariantHash> &_list);
void preprocess(const QStringList &_args, QCommandLineParser &_parser);
void print(const QueuedctlResult &_result);
QueuedctlResult process(QCommandLineParser &_parser, const QString &_cache,
                        const QString &_user);
};


#endif /* QUEUEDCTLCOMMON_H */
