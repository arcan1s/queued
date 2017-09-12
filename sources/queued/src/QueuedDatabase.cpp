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
/**
 * @file QueuedDatabase.cpp
 * Source code of queued library
 * @author Queued team
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <queued/QueuedDatabaseSchema.h>


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

    m_database.close();
}


/**
 * @fn checkDatabase
 */
void QueuedDatabase::checkDatabase()
{
    QStringList tables = m_database.tables();
    QStringList schemaTables = QueuedDB::DBSchema.keys();
    for (auto &table : schemaTables) {
        // create table if does not exist
        if (!tables.contains(table))
            createTable(table);
        // update schema
        createSchema(table);
    }
}


/**
 * @fn createAdministrator
 */
void QueuedDatabase::createAdministrator(const QString &_user,
                                         const QString &_password)
{
    qCDebug(LOG_LIB) << "Check for user" << _user;

    QSqlQuery query
        = m_database.exec(QString("SELECT * FROM '%1' WHERE name='%2'")
                              .arg(QueuedDB::USERS_TABLE)
                              .arg(_user));
    QSqlError error = query.lastError();
    if (error.isValid())
        qCWarning(LOG_LIB) << "Could not get record" << _user << "from"
                           << QueuedDB::USERS_TABLE << "message"
                           << error.text();
    else if (query.next())
        return;

    qCInfo(LOG_LIB) << "Create administrator user" << _user;
    QVariantHash payload = {
        {"name", _user},
        {"password", _password},
        {"permissions", static_cast<int>(QueuedEnums::Permission::SuperAdmin)}};

    if (!add(QueuedDB::USERS_TABLE, payload))
        qCCritical(LOG_LIB) << "Could not create administrator";
}


/**
 * @fn get
 */
QList<QVariantHash> QueuedDatabase::get(const QString &_table,
                                        const QString &_condition)
{
    qCDebug(LOG_LIB) << "Get records in table" << _table << "with condition"
                     << _condition;

    QList<QVariantHash> output;
    QSqlQuery query
        = m_database.exec(QString("SELECT * FROM '%1' %2 ORDER BY _id ASC")
                              .arg(_table)
                              .arg(_condition));

    QSqlError error = query.lastError();
    if (error.isValid()) {
        qCWarning(LOG_LIB) << "Could not get records from" << _table
                           << "message" << error.text();
        return output;
    }
    QSqlRecord record = query.record();

    QStringList columns = QueuedDB::DBSchema[_table].keys();
    auto dbColumns = getColumnsInRecord(columns, record);
    while (query.next()) {
        QVariantHash entry;
        for (auto &column : columns)
            entry[column] = query.value(dbColumns[column]);
        output.append(entry);
    }

    return output;
}


/**
 * @fn get
 */
QVariantHash QueuedDatabase::get(const QString &_table, const long long _id)
{
    qCDebug(LOG_LIB) << "Get record" << _id << "in table" << _table;

    auto output = get(_table, QString("WHERE _id=%1").arg(_id));
    if (output.count() == 0) {
        qCWarning(LOG_LIB) << "Could not find records for" << _id;
        return QVariantHash();
    } else if (output.count() == 1) {
        return output.at(0);
    } else {
        qCWarning(LOG_LIB) << "Too many records found for" << _id;
        return output.at(0);
    }
}


/**
 * @fn open
 */
bool QueuedDatabase::open(const QString &_hostname, const int _port,
                          const QString &_username, const QString &_password)
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
        checkDatabase();
    return status;
}


/**
 * @fn path
 */
QString QueuedDatabase::path() const
{
    return m_path;
}


/**
 * @fn add
 */
long long QueuedDatabase::add(const QString &_table, const QVariantHash &_value)
{
    qCDebug(LOG_LIB) << "Add record" << _value << "to table" << _table;

    auto payload = getQueryPayload(_table, _value);
    // build query
    QSqlQuery query
        = m_database.exec(QString("INSERT INTO %1 (%2) VALUES (%3)")
                              .arg(_table)
                              .arg(payload.first.join(QChar(',')))
                              .arg(payload.second.join(QChar(','))));
    QSqlError error = query.lastError();
    if (error.isValid()) {
        qCCritical(LOG_LIB) << "Could not add record" << _value << "to table"
                            << _table << "message" << error.text();
        return -1;
    }

    return lastInsertionId(_table);
}


/**
 * @fn modify
 */
bool QueuedDatabase::modify(const QString &_table, const long long _id,
                            const QVariantHash &_value)
{
    qCDebug(LOG_LIB) << "Modify record" << _id << "in table" << _table
                     << "with value" << _value;

    auto payload = getQueryPayload(_table, _value);
    QStringList stringPayload;
    for (int i = 0; i < payload.first.count(); i++)
        stringPayload.append(QString("%1=%2")
                                 .arg(payload.first.at(i))
                                 .arg(payload.second.at(i)));
    // build query
    QSqlQuery query = m_database.exec(QString("UPDATE %1 SET %2 WHERE _id=%3")
                                          .arg(_table)
                                          .arg(stringPayload.join(QChar(',')))
                                          .arg(_id));
    QSqlError error = query.lastError();
    if (error.isValid()) {
        qCCritical(LOG_LIB) << "Could not modify record" << _value << "in table"
                            << _table << "message" << error.text();
        return false;
    }

    return true;
}


/**
 * @fn remove
 */
bool QueuedDatabase::remove(const QString &_table, const long long _id)
{
    qCDebug(LOG_LIB) << "Remove row" << _id << "from" << _table;

    QSqlQuery query = m_database.exec(
        QString("DELETE FROM %1 WHERE _id=%2").arg(_table).arg(_id));
    QSqlError error = query.lastError();
    if (error.isValid()) {
        qCCritical(LOG_LIB) << "Could not remove record" << _id << "in table"
                            << _table << "message" << error.text();
        return false;
    }

    return true;
}


/**
 * @fn removeTasks
 */
void QueuedDatabase::removeTasks(const QDateTime &_endTime)
{
    qCDebug(LOG_LIB) << "Remove all tasks which are older than" << _endTime;

    QSqlQuery query = m_database.exec(
        QString("DELETE FROM %1 WHERE datetime(endTime) < datetime('%2')")
            .arg(QueuedDB::TASKS_TABLE)
            .arg(_endTime.toString(Qt::ISODateWithMs)));

    QSqlError error = query.lastError();
    if (error.isValid())
        qCCritical(LOG_LIB) << "Could not remove tasks in table"
                            << "message" << error.text();
}


/**
 * @fn removeTokens
 */
void QueuedDatabase::removeTokens()
{
    QString now = QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs);
    QSqlQuery query = m_database.exec(
        QString("DELETE FROM %1 WHERE datetime(validUntil) > datetime('%2')")
            .arg(QueuedDB::TOKENS_TABLE)
            .arg(now));

    QSqlError error = query.lastError();
    if (error.isValid())
        qCCritical(LOG_LIB) << "Could not remove tokens in table"
                            << "message" << error.text();
}


/**
 * @fn removeUsers
 */
void QueuedDatabase::removeUsers(const QDateTime &_lastLogin)
{
    qCDebug(LOG_LIB) << "Remove all users which logged older than"
                     << _lastLogin;

    QSqlQuery query = m_database.exec(
        QString("DELETE FROM %1 WHERE datetime(lastLogin) < datetime('%2')")
            .arg(QueuedDB::USERS_TABLE)
            .arg(_lastLogin.toString(Qt::ISODateWithMs)));

    QSqlError error = query.lastError();
    if (error.isValid())
        qCCritical(LOG_LIB) << "Could not remove users in table"
                            << "message" << error.text();
}


/**
 * @fn createSchema
 */
void QueuedDatabase::createSchema(const QString &_table)
{
    qCDebug(LOG_LIB) << "Create schema for" << _table;

    QSqlRecord record = m_database.record(_table);
    // get column names
    QStringList columns;
    for (int i = 0; i < record.count(); i++)
        columns.append(record.fieldName(i));

    // check and append if any
    QStringList schemaColumns = QueuedDB::DBSchema[_table].keys();
    for (auto &column : schemaColumns) {
        if (columns.contains(column))
            continue;
        QueuedDB::QueuedDBField field = QueuedDB::DBSchema[_table][column];
        QSqlQuery query
            = m_database.exec(QString("ALTER TABLE '%1' ADD `%2` %3")
                                  .arg(_table)
                                  .arg(column)
                                  .arg(field.sqlDescription));
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
void QueuedDatabase::createTable(const QString &_table)
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


/**
 * @fn getColumnsInRecord
 */
QHash<QString, int>
QueuedDatabase::getColumnsInRecord(const QStringList &_columns,
                                   const QSqlRecord &_record) const
{
    qCDebug(LOG_LIB) << "Search for columns" << _columns;

    return std::accumulate(
        _columns.begin(), _columns.end(), QHash<QString, int>(),
        [&_record](QHash<QString, int> &map, const QString &column) {
            map[column] = _record.indexOf(column);
            return map;
        });
}


/**
 * @fn lastInsertionId
 */
long long QueuedDatabase::lastInsertionId(const QString &_table) const
{
    qCDebug(LOG_LIB) << "Get last row ID from" << _table;

    QSqlQuery query
        = m_database.exec(QString("SELECT max(_id) FROM '%1'").arg(_table));
    QSqlError error = query.lastError();
    if (error.isValid()) {
        qCCritical(LOG_LIB) << "Could not get last insertion ID";
        return -1;
    }

    long long id;
    while (query.next())
        id = query.value(0).toLongLong();

    return id;
}


/**
 * @fn getQueryPayload
 */
QPair<QStringList, QStringList>
QueuedDatabase::getQueryPayload(const QString &_table,
                                const QVariantHash &_value) const
{
    qCDebug(LOG_LIB) << "Add record" << _value << "to table" << _table;

    QStringList keys;
    QStringList values;
    QStringList schemaColumns = QueuedDB::DBSchema[_table].keys();
    for (auto &key : _value.keys()) {
        if (!schemaColumns.contains(key)) {
            qCWarning(LOG_LIB) << "No key" << key << "found in schema of"
                               << _table;
            continue;
        }
        if (key == QString("_id")) {
            qCWarning(LOG_LIB) << "Modifying record ID is not allowed";
            continue;
        }
        keys.append(key);
        values.append(QString("'%1'").arg(_value[key].toString()));
    }

    return {keys, values};
}
