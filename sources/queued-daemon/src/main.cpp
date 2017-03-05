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
#include <QDBusConnection>
#include <QDBusMessage>

#include <queued/Queued.h>

#include "QueuedApplication.h"
#include "version.h"


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
    // daemon(0, 0);

    QCoreApplication app(argc, argv);
    app.setApplicationName(NAME);
    app.setApplicationVersion(VERSION);

    // parser
    QCommandLineParser parser;
    parser.setApplicationDescription(
        "Daemon for starting jobs to queue of calculations");
    parser.addHelpOption();
    parser.addVersionOption();

    // configuration option
    QCommandLineOption configOption(QStringList() << "c"
                                                  << "config",
                                    "Read initial configuration from file",
                                    "config", QueuedSettings::defaultPath());
    parser.addOption(configOption);

    // debug mode
    QCommandLineOption debugOption(QStringList() << "d"
                                                 << "debug",
                                   "Print debug information");
    parser.addOption(debugOption);

    parser.process(app);

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
    QueuedApplication instance(nullptr, arguments);
    return app.exec();
}
