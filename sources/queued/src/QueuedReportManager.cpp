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
 * @file QueuedReportManager.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include "queued/Queued.h"


/**
 * @class QueuedReportManager
 */
/**
 * @fn QueuedReportManager
 */
QueuedReportManager::QueuedReportManager(QObject *parent,
                                         QueuedDatabase *database)
    : QObject(parent)
    , m_database(database)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;
}


/**
 * @fn ~QueuedReportManager
 */
QueuedReportManager::~QueuedReportManager()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;
}
