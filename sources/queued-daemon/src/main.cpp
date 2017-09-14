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


#include <QCommandLineParser>
#include <QDBusConnection>

#include <queued/Queued.h>

#include <csignal>
#include <iostream>

#include "QueuedApplication.h"
#include "version.h"

extern "C" {
#include <unistd.h>
}


QueuedApplication *instance = nullptr;


bool existingSessionOperation(const QString &operation)
{
    QVariantList arguments = QueuedCoreAdaptor::sendRequest(
        QueuedConfig::DBUS_SERVICE, QueuedConfig::DBUS_APPLICATION_PATH,
        QueuedConfig::DBUS_SERVICE, operation, QVariantList());

    return (!arguments.isEmpty() && arguments.at(0).type() == QVariant::Bool
            && arguments[0].toBool());
}


int main(int argc, char *argv[])
{
    // HACK preparse arguments to find out if --daemon is set
    for (int i = 0; i < argc; i++) {
        if (std::string(argv[i]) != "--daemon")
            continue;
        ::daemon(0, 0);
        break;
    }
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
                                  "Show additional info.");
    parser.addOption(infoOption);

    // configuration option
    QCommandLineOption configOption(QStringList() << "c"
                                                  << "config",
                                    "Read initial configuration from file.",
                                    "config", QueuedSettings::defaultPath());
    parser.addOption(configOption);

    // debug mode
    QCommandLineOption debugOption(QStringList() << "d"
                                                 << "debug",
                                   "Print debug information.");
    parser.addOption(debugOption);

    // daemon mode
    QCommandLineOption daemonOption(QStringList() << "daemon",
                                    "Start detached.");
    parser.addOption(daemonOption);

    parser.process(app);

    // show info and exit
    if (parser.isSet(infoOption)) {
        auto metadata = QueuedDebug::getBuildData();
        for (auto &string : metadata)
            std::cout << qPrintable(string) << std::endl;
        return 0;
    }

    // check if exists
    if (existingSessionOperation(QString("Active"))) {
        qCWarning(LOG_APP) << "Another session is active";
        return 1;
    }

    // enable debug
    if (parser.isSet(debugOption))
        QueuedDebug::enableDebug();

    // build initial options hash
    QVariantHash arguments = {{"config", parser.value(configOption)}};

    // start application
    instance = new QueuedApplication(nullptr, arguments);
    // catch SIGHUP
    signal(SIGHUP, [](int sig) -> void {
        qCInfo(LOG_APP) << "Received SIGHUP signal, reinit components";
        instance->init();
    });
    return app.exec();
}
