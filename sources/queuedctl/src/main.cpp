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


#include <QCommandLineParser>
#include <QCoreApplication>

#include <queued/Queued.h>

#include <iostream>

#include "QueuedctlCommon.h"
#include "version.h"

extern "C" {
#include <unistd.h>
}


int main(int argc, char *argv[])
{
    QueuedDebug::applyLogFormat();

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
                                  "Shows additional info.");
    parser.addOption(infoOption);

    // debug mode
    QCommandLineOption debugOption(QStringList() << "d"
                                                 << "debug",
                                   "Prints debug information.");
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

    // additional help option
    QCommandLineOption commandsOption("commands", "Lists available commands.");
    parser.addOption(commandsOption);

    parser.addPositionalArgument("command", "Command to execute.", "<command>");

    // pre-parse
    parser.parse(QCoreApplication::arguments());
    QStringList args = parser.positionalArguments();
    parser.clearPositionalArguments();
    QueuedctlCommon::preprocess(args, parser);
    // parse
    parser.process(app);

    // show info and exit
    if (parser.isSet(infoOption)) {
        auto metadata = QueuedDebug::getBuildData();
        for (auto &string : metadata)
            std::cout << qPrintable(string) << std::endl;
        return 0;
    }
    if (parser.isSet(commandsOption)) {
        std::cout << qPrintable(parser.helpText()) << std::endl;
        std::cout << qPrintable(QueuedctlCommon::commandsHelp()) << std::endl;
        return 0;
    }

    // enable debug
    if (parser.isSet(debugOption))
        QueuedDebug::enableDebug();

    // process
    auto result = QueuedctlCommon::process(parser, parser.value(tokenOption),
                                           parser.value(userOption));
    QueuedctlCommon::print(result);

    return result.status ? 0 : 1;
}
