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

    auto current = get(QueuedDB::USERS_TABLE, "WHERE name=:username", {{"username", _user}});
    if (!current.isEmpty())
        return;

    qCInfo(LOG_LIB) << "Create administrator user" << _user;
    QVariantHash payload = {{"name", _user},
                            {"password", _password},
                            {"permissions", static_cast<int>(QueuedEnums::Permission::SuperAdmin)}};

    if (add(QueuedDB::USERS_TABLE, payload) <= 0)
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
        auto queryString = QString("ALTER TABLE %1 ADD `%2` %3")
                               .arg(_table)
                               .arg(column)
                               .arg(field.sqlDescription);
        execute(queryString, QVariantHash());
    }
}


/**
 * @fn createTable
 */
void QueuedDatabase::createTable(const QString &_table)
{
    qCDebug(LOG_LIB) << "Create table" << _table;

    auto queryString
        = QString("CREATE TABLE %1 (`_id` INTEGER PRIMARY KEY AUTOINCREMENT)").arg(_table);
    execute(queryString, QVariantHash());
}


/**
 * @fn execute
 */
QList<QVariantHash> QueuedDatabase::execute(const QString &_query, const QVariantHash &_params)
{
    qCDebug(LOG_LIB) << "Execute query" << _query << "with parameters" << _params;

    QList<QVariantHash> output;
    QSqlQuery query(m_database);
    query.prepare(_query);
    for (auto &key : _params.keys())
        query.bindValue(QString(":%1").arg(key), _params[key]);
    query.exec();

    auto error = query.lastError();
    if (error.isValid()) {
        qCWarning(LOG_LIB) << "Could not get records using query" << _query << "message"
                           << error.text();
        throw QueuedDatabaseException(error.text());
    }
    auto record = query.record();

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
QList<QVariantHash> QueuedDatabase::get(const QString &_table, const QString &_condition,
                                        const QVariantHash &_params)
{
    qCDebug(LOG_LIB) << "Get records in table" << _table;

    auto queryString = QString("SELECT * FROM %1 %2 ORDER BY _id ASC").arg(_table).arg(_condition);

    try {
        return execute(queryString, _params);
    } catch (QueuedDatabaseException &) {
        return QList<QVariantHash>();
    }
}


/**
 * @fn get
 */
QVariantHash QueuedDatabase::get(const QString &_table, const long long _id)
{
    qCDebug(LOG_LIB) << "Get record" << _id << "in table" << _table;

    auto output = get(_table, "WHERE _id=:_id", {{"_id", _id}});
    switch (output.count()) {
    case 0:
        qCWarning(LOG_LIB) << "Could not find records for" << _id;
        return QVariantHash();
    case 1:
        return output.first();
    default:
        qCWarning(LOG_LIB) << "Too many records found for" << _id;
        return output.first();
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
    auto queryString = QString("INSERT INTO %1 (%2) VALUES (%3)")
                           .arg(_table)
                           .arg(payload.keys().join(','))
                           .arg(payload.values().join(','));

    try {
        execute(queryString, _value);
    } catch (QueuedDatabaseException &) {
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
    auto queryString
        = QString("UPDATE %1 SET %2 WHERE _id=:_id").arg(_table).arg(stringPayload.join(','));

    try {
        execute(queryString, _value);
    } catch (QueuedDatabaseException &) {
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

    auto queryString = QString("DELETE FROM %1 WHERE _id=:_id").arg(_table);

    try {
        execute(queryString, {{"_id", _id}});
    } catch (QueuedDatabaseException &) {
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

    auto queryString = QString("DELETE FROM %1 WHERE datetime(endTime) < datetime(:time)")
                           .arg(QueuedDB::TASKS_TABLE);

    try {
        execute(queryString, {{"time", _endTime}});
    } catch (QueuedDatabaseException &) {
    }
}


/**
 * @fn removeTokens
 */
void QueuedDatabase::removeTokens()
{
    auto now = QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs);
    auto queryString = QString("DELETE FROM %1 WHERE datetime(validUntil) < datetime(:time)")
                           .arg(QueuedDB::TOKENS_TABLE);

    try {
        execute(queryString, {{"time", now}});
    } catch (QueuedDatabaseException &) {
    }
}


/**
 * @fn removeUsers
 */
void QueuedDatabase::removeUsers(const QDateTime &_lastLogin)
{
    qCDebug(LOG_LIB) << "Remove all users which logged older than" << _lastLogin;

    auto queryString = QString("DELETE FROM %1 WHERE datetime(lastLogin) < datetime(:time)")
                           .arg(QueuedDB::USERS_TABLE);

    try {
        execute(queryString, {{"time", _lastLogin}});
    } catch (QueuedDatabaseException &) {
    }
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
 * @fn lastInsertionId
 */
long long QueuedDatabase::lastInsertionId(const QString &_table)
{
    qCDebug(LOG_LIB) << "Get last row ID from" << _table;

    QList<QVariantHash> result;
    try {
        result = execute(QString("SELECT max(_id) FROM %1").arg(_table), QVariantHash());
    } catch (QueuedDatabaseException &) {
        return -1;
    }

    if (result.isEmpty())
        return -1;

    return result.first()["max(_id)"].toLongLong();
}


/**
 * @fn getQueryPayload
 */
QHash<QString, QString> QueuedDatabase::getQueryPayload(const QString &_table,
                                                        const QVariantHash &_value) const
{
    qCDebug(LOG_LIB) << "Get payload from" << _value << "to table" << _table;

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
