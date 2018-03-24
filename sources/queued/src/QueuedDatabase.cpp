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
 * @copyright MIT
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
QueuedDatabase::QueuedDatabase(QObject *parent, const QString path, const QString driver)
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

    close();
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
 * @fn close
 */
void QueuedDatabase::close()
{
    m_database.close();
}


/**
 * @fn createAdministrator
 */
void QueuedDatabase::createAdministrator(const QString &_user, const QString &_password)
{
    qCDebug(LOG_LIB) << "Check for user" << _user;

    QSqlQuery query(m_database);
    query.prepare("SELECT * FROM :table WHERE name=:username");
    query.bindValue(":table", QueuedDB::USERS_TABLE);
    query.bindValue(":username", _user);
    query.exec();

    QSqlError error = query.lastError();
    if (error.isValid())
        qCWarning(LOG_LIB) << "Could not get record" << _user << "from" << QueuedDB::USERS_TABLE
                           << "message" << error.text();
    else if (query.next())
        return;

    qCInfo(LOG_LIB) << "Create administrator user" << _user;
    QVariantHash payload = {{"name", _user},
                            {"password", _password},
                            {"permissions", static_cast<int>(QueuedEnums::Permission::SuperAdmin)}};

    if (!add(QueuedDB::USERS_TABLE, payload))
        qCCritical(LOG_LIB) << "Could not create administrator";
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
        QSqlQuery query(m_database);
        query.prepare(
            QString("ALTER TABLE :table ADD `%1` %2").arg(column).arg(field.sqlDescription));
        query.bindValue(":table", _table);
        query.exec();

        QSqlError error = query.lastError();
        if (error.isValid())
            qCCritical(LOG_LIB) << "Could not insert column" << column << "to table" << _table
                                << "error:" << error.text();
    }
}


/**
 * @fn createTable
 */
void QueuedDatabase::createTable(const QString &_table)
{
    qCDebug(LOG_LIB) << "Create table" << _table;

    QSqlQuery query(m_database);
    query.prepare("CREATE TABLE :table (`_id` INTEGER PRIMARY KEY AUTOINCREMENT)");
    query.bindValue(":table", _table);
    query.exec();

    QSqlError error = query.lastError();
    if (error.isValid())
        qCCritical(LOG_LIB) << "Could not create table" << _table << "error:" << error.text();
}


/**
 * @fn get
 */
QList<QVariantHash> QueuedDatabase::get(const QString &_table,
                                        const QList<QueuedDatabaseCondition> &_condition)
{
    qCDebug(LOG_LIB) << "Get records in table" << _table;

    QList<QVariantHash> output;
    QSqlQuery query(m_database);
    query.prepare("SELECT * FROM :table " + getCondition(_condition) + " ORDER BY _id ASC");
    query.bindValue(":table", _table);
    for (auto &cond : _condition)
        query.bindValue(QString(":%1").arg(cond.key), cond.value);
    query.exec();

    QSqlError error = query.lastError();
    if (error.isValid()) {
        qCWarning(LOG_LIB) << "Could not get records from" << _table << "message" << error.text();
        return output;
    }
    QSqlRecord record = query.record();

    auto columns = getColumnsInRecord(record);
    while (query.next()) {
        QVariantHash entry;
        for (auto &column : columns)
            entry[column] = query.value(column);
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

    auto output = get(_table, {{"_id", _id, "="}});
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
bool QueuedDatabase::open(const QString &_hostname, const int _port, const QString &_username,
                          const QString &_password)
{
    qCDebug(LOG_LIB) << "Open database at" << _hostname << _port << "as user" << _username;

    if (!_hostname.isEmpty())
        m_database.setHostName(_hostname);
    if (_port > 0)
        m_database.setPort(_port);
    bool status = _username.isEmpty() ? m_database.open() : m_database.open(_username, _password);

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
    QSqlQuery query(m_database);
    query.prepare(QString("INSERT INTO :table (%1) VALUES (%2)")
                      .arg(payload.keys().join(','))
                      .arg(payload.values().join(',')));
    query.bindValue(":table", _table);
    for (auto &key : _value.keys())
        query.bindValue(QString(":%1").arg(key), _value[key]);
    query.exec();

    QSqlError error = query.lastError();
    if (error.isValid()) {
        qCCritical(LOG_LIB) << "Could not add record" << _value << "to table" << _table << "message"
                            << error.text();
        return -1;
    }

    return lastInsertionId(_table);
}


/**
 * @fn modify
 */
bool QueuedDatabase::modify(const QString &_table, const long long _id, const QVariantHash &_value)
{
    qCDebug(LOG_LIB) << "Modify record" << _id << "in table" << _table << "with value" << _value;

    auto payload = getQueryPayload(_table, _value);
    QStringList stringPayload;
    for (auto &key : payload.keys())
        stringPayload.append(QString("%1=:%1").arg(key));
    // build query
    QSqlQuery query(m_database);
    query.prepare(QString("UPDATE :table SET %1 WHERE _id=:_id").arg(stringPayload.join(',')));
    query.bindValue(":table", _table);
    query.bindValue(":_id", _id);
    for (auto &key : _value.keys())
        query.bindValue(QString(":%1").arg(key), _value[key]);
    query.exec();

    QSqlError error = query.lastError();
    if (error.isValid()) {
        qCCritical(LOG_LIB) << "Could not modify record" << _value << "in table" << _table
                            << "message" << error.text();
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

    QSqlQuery query(m_database);
    query.prepare("DELETE FROM :table WHERE _id=:_id");
    query.bindValue(":table", _table);
    query.bindValue(":_id", _id);
    query.exec();

    QSqlError error = query.lastError();
    if (error.isValid()) {
        qCCritical(LOG_LIB) << "Could not remove record" << _id << "in table" << _table << "message"
                            << error.text();
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

    QSqlQuery query(m_database);
    query.prepare("DELETE FROM :table WHERE datetime(endTime) < datetime(:time)");
    query.bindValue(":table", QueuedDB::TASKS_TABLE);
    query.bindValue(":time", _endTime.toString(Qt::ISODateWithMs));
    query.exec();

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
    QSqlQuery query(m_database);
    query.prepare("DELETE FROM :table WHERE datetime(validUntil) < datetime(:time)");
    query.bindValue(":table", QueuedDB::TOKENS_TABLE);
    query.bindValue(":time", now);
    query.exec();

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
    qCDebug(LOG_LIB) << "Remove all users which logged older than" << _lastLogin;

    QSqlQuery query(m_database);
    query.prepare("DELETE FROM :table WHERE datetime(lastLogin) < datetime(:time)");
    query.bindValue(":table", QueuedDB::USERS_TABLE);
    query.bindValue(":time", _lastLogin.toString(Qt::ISODateWithMs));
    query.exec();

    QSqlError error = query.lastError();
    if (error.isValid())
        qCCritical(LOG_LIB) << "Could not remove users in table"
                            << "message" << error.text();
}


/**
 * @fn getColumnsInRecord
 */
QStringList QueuedDatabase::getColumnsInRecord(const QSqlRecord &_record) const
{
    qCDebug(LOG_LIB) << "Search for columns" << _record;

    QStringList output;
    for (int i = 0; i < _record.count(); i++)
        output += _record.fieldName(i);

    return output;
}


/**
 * @fn getCondition
 */
QString QueuedDatabase::getCondition(const QList<QueuedDatabaseCondition> &_condition) const
{
    if (_condition.isEmpty())
        return "";

    QStringList query;
    for (auto &cond : _condition)
        query += QString("%1 %2 :%1").arg(cond.key).arg(cond.operation);

    return "WHERE " + query.join(',');
}


/**
 * @fn lastInsertionId
 */
long long QueuedDatabase::lastInsertionId(const QString &_table) const
{
    qCDebug(LOG_LIB) << "Get last row ID from" << _table;

    QSqlQuery query(m_database);
    query.prepare("SELECT max(_id) FROM :table");
    query.bindValue(":table", _table);
    query.exec();

    QSqlError error = query.lastError();
    if (error.isValid()) {
        qCCritical(LOG_LIB) << "Could not get last insertion ID";
        return -1;
    }

    long long id = -1;
    while (query.next())
        id = query.value(0).toLongLong();

    return id;
}


/**
 * @fn getQueryPayload
 */
QHash<QString, QString> QueuedDatabase::getQueryPayload(const QString &_table,
                                                        const QVariantHash &_value) const
{
    qCDebug(LOG_LIB) << "Add record" << _value << "to table" << _table;

    QHash<QString, QString> output;
    auto schemaColumns = QueuedDB::DBSchema[_table].keys();
    for (auto &key : _value.keys()) {
        // we would check it only if there is data about this table
        if (!schemaColumns.isEmpty() && !schemaColumns.contains(key)) {
            qCWarning(LOG_LIB) << "No key" << key << "found in schema of" << _table;
            continue;
        }
        if (key == "_id") {
            qCWarning(LOG_LIB) << "Modifying record ID is not allowed";
            continue;
        }
        output[key] = QString(":%1").arg(key);
    }

    return output;
}
