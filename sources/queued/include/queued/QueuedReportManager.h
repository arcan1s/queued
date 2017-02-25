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
 * @file QueuedReportManager.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDREPORTMANAGER_H
#define QUEUEDREPORTMANAGER_H

#include <QObject>


class QueuedDatabase;

/**
 * @brief report manager for queued
 */
class QueuedReportManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief QueuedReportManager class constructor
     * @param parent         pointer to parent item
     * @param database       pointer to database object
     */
    explicit QueuedReportManager(QObject *parent, QueuedDatabase *database);
    /**
     * @brief QueuedReportManager class destructor
     */
    virtual ~QueuedReportManager();

private:
    /**
     * @brief pointer to database object
     */
    QueuedDatabase *m_database = nullptr;
};


#endif /* QUEUEDREPORTMANAGER_H */
