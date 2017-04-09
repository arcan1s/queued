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


#include "QueuedctlUser.h"

#include <queued/Queued.h>

#include <iostream>

extern "C" {
#include <termios.h>
#include <unistd.h>
}


long long
QueuedctlUser::addUser(const QueuedUser::QueuedUserDefinitions &_definitions,
                       const QString &_token)
{
    qCDebug(LOG_APP) << "Add user" << _definitions.name;

    return QueuedCoreAdaptor::sendUserAdd(_definitions, _token);
}


QList<QVariantHash> QueuedctlUser::getReport(const QCommandLineParser &_parser,
                                             const QString &_token)
{
    qCDebug(LOG_APP) << "Get usage report";

    QDateTime stop
        = QDateTime::fromString(_parser.value("stop"), Qt::ISODateWithMs);
    QDateTime start
        = QDateTime::fromString(_parser.value("start"), Qt::ISODateWithMs);

    return QueuedCoreAdaptor::getPerformance(start, stop, _token);
}


QueuedUser::QueuedUserDefinitions
QueuedctlUser::getDefinitions(const QCommandLineParser &_parser,
                              const bool _expandAll)
{
    qCDebug(LOG_APP) << "Parse user definitions from parser, expand all"
                     << _expandAll;

    QueuedUser::QueuedUserDefinitions definitions;

    definitions.email = _parser.value("email");
    definitions.password = _parser.isSet("stdin-password")
                               ? getPassword()
                               : _parser.value("password");
    // transform to hash
    definitions.password
        = definitions.password.isEmpty()
              ? ""
              : QueuedUser::hashFromPassword(definitions.password);
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
        definitions.name = _parser.value("name");
    } else {
        definitions.permissions = _parser.value("access").toUInt();
        // queuedctl -- user-add username
        definitions.name = _parser.positionalArguments().at(1);
    }

    return definitions;
}


QString QueuedctlUser::getPassword()
{
    // do not show input characters
    struct termios tty;
    ::tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);

    std::cout << "Password" << std::endl;
    QTextStream stream(stdin);
    QString password;
    stream >> password;

    return password;
}


QVariant QueuedctlUser::getUser(const long long _id, const QString &_property)
{
    qCDebug(LOG_APP) << "Get property" << _property << "from user" << _id;

    if (_property.isEmpty())
        return QueuedCoreAdaptor::getUser(_id);
    else
        return QueuedCoreAdaptor::getUser(_id, _property);
}


QList<QVariantHash> QueuedctlUser::getUsers(const QCommandLineParser &_parser,
                                            const QString &_token)
{
    QDateTime lastLogin = QDateTime::fromString(_parser.value("last-logged"),
                                                Qt::ISODateWithMs);
    auto permission
        = _parser.value("access").isEmpty()
              ? QueuedEnums::Permission::Invalid
              : QueuedEnums::Permission(_parser.value("access").toInt());

    return QueuedCoreAdaptor::getUsers(lastLogin, permission, _token);
}


void QueuedctlUser::parserAdd(QCommandLineParser &_parser)
{
    _parser.addPositionalArgument("name", "User name.", "<name>");

    // permissions
    QCommandLineOption accessOption(QStringList() << "a"
                                                  << "access",
                                    "User permission.", "access", "0");
    _parser.addOption(accessOption);
    // email
    QCommandLineOption emailOption(QStringList() << "e"
                                                 << "email",
                                   "User email.", "email", "");
    _parser.addOption(emailOption);
    // password
    QCommandLineOption passwordOption("password", "User password.", "password",
                                      "");
    _parser.addOption(passwordOption);
    // password
    QCommandLineOption stdinPasswordOption("stdin-password",
                                           "User password from stdin.");
    _parser.addOption(stdinPasswordOption);
    // cpu limit
    QCommandLineOption cpuOption("limit-cpu", "User CPU limit.", "limit-cpu",
                                 "0");
    _parser.addOption(cpuOption);
    // gpu limit
    QCommandLineOption gpuOption("limit-gpu", "User GPU limit.", "limit-gpu",
                                 "0");
    _parser.addOption(gpuOption);
    // memory limit
    QCommandLineOption memoryOption("limit-memory", "User memory limit.",
                                    "limit-memory", "0");
    _parser.addOption(memoryOption);
    // gpu memory limit
    QCommandLineOption gpumemoryOption(
        "limit-gpumemory", "User GPU memory limit.", "limit-gpumemory", "0");
    _parser.addOption(gpumemoryOption);
    // storage limit
    QCommandLineOption storageOption("limit-storage", "User storage limit.",
                                     "limit-storage", "0");
    _parser.addOption(storageOption);
}


void QueuedctlUser::parserGet(QCommandLineParser &_parser)
{
    _parser.addPositionalArgument("id", "User ID.", "<id>");
    _parser.addPositionalArgument("property", "User property name.",
                                  "<property>");
}


void QueuedctlUser::parserList(QCommandLineParser &_parser)
{
    // last logged in
    QCommandLineOption loggedOption("last-logged", "User last logged time.",
                                    "last-logged", "");
    _parser.addOption(loggedOption);
    // permissions
    QCommandLineOption accessOption("access", "User permission.", "access", "");
    _parser.addOption(accessOption);
}


void QueuedctlUser::parserReport(QCommandLineParser &_parser)
{
    // start
    QCommandLineOption startOption("start", "Task start time.", "start", "");
    _parser.addOption(startOption);
    // stop
    QCommandLineOption stopOption("stop", "Task stop time.", "stop", "");
    _parser.addOption(stopOption);
}


void QueuedctlUser::parserSet(QCommandLineParser &_parser)
{
    _parser.addPositionalArgument("id", "User ID.", "<id>");

    // email
    QCommandLineOption emailOption(QStringList() << "e"
                                                 << "email",
                                   "User email.", "email", "");
    _parser.addOption(emailOption);
    // name
    QCommandLineOption nameOption(QStringList() << "n"
                                                << "name",
                                  "User name.", "name", "");
    _parser.addOption(nameOption);
    // password
    QCommandLineOption passwordOption("password", "User password.", "password",
                                      "");
    _parser.addOption(passwordOption);
    // password
    QCommandLineOption stdinPasswordOption("stdin-password",
                                           "User password from stdin.");
    _parser.addOption(stdinPasswordOption);
    // cpu limit
    QCommandLineOption cpuOption("limit-cpu", "User CPU limit.", "limit-cpu",
                                 "0");
    _parser.addOption(cpuOption);
    // gpu limit
    QCommandLineOption gpuOption("limit-gpu", "User GPU limit.", "limit-gpu",
                                 "0");
    _parser.addOption(gpuOption);
    // memory limit
    QCommandLineOption memoryOption("limit-memory", "User memory limit.",
                                    "limit-memory", "0");
    _parser.addOption(memoryOption);
    // gpu memory limit
    QCommandLineOption gpumemoryOption(
        "limit-gpumemory", "User GPU memory limit.", "limit-gpumemory", "0");
    _parser.addOption(gpumemoryOption);
    // storage limit
    QCommandLineOption storageOption("limit-storage", "User storage limit.",
                                     "limit-storage", "0");
    _parser.addOption(storageOption);
}


bool QueuedctlUser::setUser(
    const long long _id, const QueuedUser::QueuedUserDefinitions &_definitions,
    const QString &_token)
{
    qCDebug(LOG_APP) << "Edit user" << _id;

    return QueuedCoreAdaptor::sendUserEdit(_id, _definitions, _token);
}
