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
 * @file QueuedDatabase.cpp
 * Source code of queued library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include "queued/Queued.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>


/**
 * @fn QueuedDatabase
 */
QueuedDatabase::QueuedDatabase(QObject *parent, const QString path,
                               const QString driver)
    : QObject(parent)
    , m_path(path)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    m_database = QSqlDatabase::addDatabase(driver);
    m_database.setDatabaseName(path);
}


/**
 * @fn ~QueuedDatabase
 */
QueuedDatabase::~QueuedDatabase()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;
}


/**
 * @fn open
 */
void QueuedDatabase::open(const QString _hostname, const int _port,
                          const QString _username, const QString _password)
{
    qCDebug(LOG_LIB) << "Open database at" << _hostname << _port << "as user"
                     << _username;

    if (!_hostname.isEmpty())
        m_database.setHostName(_hostname);
    if (_port > 0)
        m_database.setPort(_port);
    bool status = _username.isEmpty() ? m_database.open()
                                      : m_database.open(_username, _password);

    qCDebug(LOG_LIB) << "Open database status" << status;
    if (status)
        return checkDatabase();
}


/**
 * @fn checkDatabase
 */
void QueuedDatabase::checkDatabase()
{
    QStringList tables = m_database.tables();
    for (auto table : DBSchema.keys()) {
        // create table if does not exist
        if (!tables.contains(table))
            createTable(table);
        // update schema
        createSchema(table);
    }
}


/**
 * @fn path
 */
QString QueuedDatabase::path() const
{
    return m_path;
}


/**
 * @fn createSchema
 */
void QueuedDatabase::createSchema(const QString _table)
{
    qCDebug(LOG_LIB) << "Create schema for" << _table;

    QSqlRecord record = m_database.record(_table);
    // get column names
    QStringList columns;
    for (int i = 0; i < record.count(); i++)
        columns.append(record.fieldName(i));

    // check and append if any
    for (auto column : DBSchema[_table]) {
        if (columns.contains(column))
            continue;
        QSqlQuery query
            = m_database.exec(QString("ALTER TABLE '%1' add column `%2`")
                                  .arg(_table)
                                  .arg(column));
        QSqlError error = query.lastError();
        if (error.isValid())
            qCCritical(LOG_LIB) << "Could not insert column" << column
                                << "to table" << _table
                                << "error:" << error.text();
    }
}


/**
 * @fn createTable
 */
void QueuedDatabase::createTable(const QString _table)
{
    qCDebug(LOG_LIB) << "Create table" << _table;

    QSqlQuery query = m_database.exec(
        QString("CREATE TABLE '%1' (`_id` INTEGER PRIMARY KEY AUTOINCREMENT)")
            .arg(_table));
    QSqlError error = query.lastError();
    if (error.isValid())
        qCCritical(LOG_LIB) << "Could not create table" << _table
                            << "error:" << error.text();
}
