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


#include <QCommandLineParser>
#include <QCoreApplication>

#include <queued/Queued.h>

#include "QueuedctlAuth.h"
#include "QueuedctlOption.h"
#include "version.h"

extern "C" {
#include <unistd.h>
}


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName(NAME);
    app.setApplicationVersion(VERSION);

    // parser
    QCommandLineParser parser;
    parser.setApplicationDescription(
        "Daemon for starting jobs to queue of calculations");
    parser.addHelpOption();
    parser.addVersionOption();
    // info
    QCommandLineOption infoOption(QStringList() << "i"
                                                << "info",
                                  "Show additional info.");
    parser.addOption(infoOption);

    // debug mode
    QCommandLineOption debugOption(QStringList() << "d"
                                                 << "debug",
                                   "Print debug information.");
    parser.addOption(debugOption);

    // configuration option
    QCommandLineOption tokenOption(QStringList() << "t"
                                                 << "token",
                                   "Path to cached token.", "token",
                                   QueuedSettings::defaultTokenPath());
    parser.addOption(tokenOption);
    QCommandLineOption userOption(QStringList() << "u"
                                                << "user",
                                  "User to login instead of current one.",
                                  "user", ::getlogin());
    parser.addOption(userOption);

    parser.addPositionalArgument("command", "Command to execute.");

    // pre-parse
    parser.parse(QCoreApplication::arguments());
    QStringList args = parser.positionalArguments();
    QString command = args.isEmpty() ? QString() : args.first();

    if (command == "auth") {
        QueuedctlAuth::parser(parser);
    } else if (command == "option-edit") {
        QueuedctlOption::parser(parser);
    } else if (command == "task-add") {
        parser.clearPositionalArguments();
    } else if (command == "task-edit") {
        parser.clearPositionalArguments();
    } else if (command == "user-add") {
        parser.clearPositionalArguments();
    } else if (command == "user-edit") {
        parser.clearPositionalArguments();
    } else {
        parser.process(app);
        qWarning() << "Unknown command" << command;
        parser.showHelp(1);
    }

    parser.process(app);

    // show info and exit
    if (parser.isSet(infoOption)) {
        auto metadata = QueuedDebug::getBuildData();
        for (auto &string : metadata)
            QDebug(QtMsgType::QtInfoMsg).noquote() << string;
        return 0;
    }

    // enable debug
    if (parser.isSet(debugOption))
        QueuedDebug::enableDebug();

    return 0;
}
