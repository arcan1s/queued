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
/**
 * @file QueuedDebug.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDDEBUG_H
#define QUEUEDDEBUG_H

#include <QLoggingCategory>


/**
 * @brief daemon logging category
 */
Q_DECLARE_LOGGING_CATEGORY(LOG_APP)
/**
 * @brief control application logging category
 */
Q_DECLARE_LOGGING_CATEGORY(LOG_CTL)
/**
 * @brief DBus logging category
 */
Q_DECLARE_LOGGING_CATEGORY(LOG_DBUS)
/**
 * @brief library logging category
 */
Q_DECLARE_LOGGING_CATEGORY(LOG_LIB)
/**
 * @brief plugin logging category
 */
Q_DECLARE_LOGGING_CATEGORY(LOG_PL)
/**
 * @brief server logging category
 */
Q_DECLARE_LOGGING_CATEGORY(LOG_SERV)

/**
 * @addtogroup QueuedDebug
 * @brief Queued debug functions
 */
namespace QueuedDebug
{
/**
 * @brief default log format
 */
const char LOG_FORMAT[] = "[%{time "
                          "process}][%{if-debug}DD%{endif}%{if-info}II%{endif}%"
                          "{if-warning}WW%{endif}%{if-critical}CC%{endif}%{if-"
                          "fatal}FF%{endif}][%{category}][%{function}] "
                          "%{message}";
/**
 * @brief method to enable debug messages
 */
void enableDebug();
/**
 * @brief additional method to get build details declared in version.h
 */
QStringList getBuildData();
}


#endif /* QUEUEDDEBUG_H */
