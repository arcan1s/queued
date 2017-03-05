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
/**
 * @file QueuedDebug.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include "queued/Queued.h"

#include "version.h"


Q_LOGGING_CATEGORY(LOG_APP, "org.queued.application", QtMsgType::QtWarningMsg)
Q_LOGGING_CATEGORY(LOG_CTL, "org.queued.control", QtMsgType::QtWarningMsg)
Q_LOGGING_CATEGORY(LOG_DBUS, "org.queued.dbus", QtMsgType::QtWarningMsg)
Q_LOGGING_CATEGORY(LOG_LIB, "org.queued.library", QtMsgType::QtWarningMsg)
Q_LOGGING_CATEGORY(LOG_PL, "org.queued.plugin", QtMsgType::QtWarningMsg)
Q_LOGGING_CATEGORY(LOG_SERV, "org.queued.server", QtMsgType::QtWarningMsg)


/**
 * @fn enableDebug
 */
void QueuedDebug::enableDebug()
{
    QLoggingCategory::setFilterRules(QString("org.queued.*=true"));
}


/**
 * @fn getBuildData
 */
QStringList QueuedDebug::getBuildData()
{
    QStringList metadata;

    return metadata;
}
