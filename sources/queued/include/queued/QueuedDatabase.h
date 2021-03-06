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
 * @file QueuedDatabase.h
 * Header of Queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDDATABASE_H
#define QUEUEDDATABASE_H

#include <QHash>
#include <QObject>
#include <QSqlDatabase>
#include <QVariant>


/**
 * @brief queued adaptor to databases
 */
class QueuedDatabase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path)

public:
    /**
     * @brief QueuedDatabase class constructor
     * @param parent
     * pointer to parent item
     * @param path
     * path to database
     * @param driver
     * database driver
     */
    explicit QueuedDatabase(QObject *parent, const QString path, const QString driver);
    /**
     * @brief QueuedDatabase class destructor
     */
    virtual ~QueuedDatabase();
    /**
     * @brief check and create database
     */
    void checkDatabase();
    /**
     * @brief close database connection
     */
    void close();
    /**
     * @brief check and create queued administrator if missing
     * @param _user
     * administrator username
     * @param _password
     * administrator password SHA512
     */
    void createAdministrator(const QString &_user, const QString &_password);
    /**
     * @brief create or update actual schema in table
     * @param _table
     * table name
     */
    void createSchema(const QString &_table);
    /**
     * @brief create given table
     * @param _table
     * table name
     */
    void createTable(const QString &_table);
    /**
     * @brief execute arbitrary query
     * @throw QueuedDatabaseException
     * in case if error occurs
     * @param _query
     * sql query
     * @param _params
     * sql query parameters
     * @return query result
     */
    QList<QVariantHash> execute(const QString &_query, const QVariantHash &_params);
    /**
     * @brief get all records from table
     * @param _table
     * table name
     * @param _condition
     * optional condition string
     * @param _params
     * optional condition parameters
     * @return list of records from table
     */
    QList<QVariantHash> get(const QString &_table, const QString &_condition = "",
                            const QVariantHash &_params = QVariantHash());
    /**
     * @brief get record from table with given id
     * @param _table
     * table name
     * @param _id
     * record id
     * @return variant map from table
     */
    QVariantHash get(const QString &_table, const long long _id);
    /**
     * @brief open database
     * @param _hostname
     * hostname to connect, may be empty
     * @param _port
     * port to connect, may be 0
     * @param _username
     * username to connect, may be empty
     * @param _password
     * password to connect, will be ignored if _username is empty
     * @return true on successful opening
     */
    bool open(const QString &_hostname, const int _port, const QString &_username,
              const QString &_password);
    /**
     * @brief path to database
     * @return path to used database
     */
    QString path() const;

public slots:
    /**
     * @brief add record to database
     * @param _table
     * table name
     * @param _value
     * value to insert
     * @return index of inserted record or -1 if no insertion
     */
    long long add(const QString &_table, const QVariantHash &_value);
    /**
     * @brief modify record in table
     * @param _table
     * table name
     * @param _id
     * id for search
     * @param _value
     * value to update
     * @return true on successfully modification
     */
    bool modify(const QString &_table, const long long _id, const QVariantHash &_value);
    /**
     * @brief remove record in table by ID
     * @param _table
     * table name
     * @param _id
     * id to remove
     * @return true on successfully removal
     */
    bool remove(const QString &_table, const long long _id);
    /**
     * @brief remove ended task
     * @param _endTime
     * last task end time
     */
    void removeTasks(const QDateTime &_endTime);
    /**
     * @brief remove unused tokens
     */
    void removeTokens();
    /**
     * @brief remove user which where not logged into system
     * @param _lastLogin
     * last user login
     */
    void removeUsers(const QDateTime &_lastLogin);

private:
    /**
     * @brief database
     */
    QSqlDatabase m_database;
    /**
     * @brief database path
     */
    QString m_path;
    /**
     * @brief additional function to get column numbers from table
     * @param _record
     * SQL record from query
     * @return list of columns in table
     */
    QStringList getColumnsInRecord(const QSqlRecord &_record) const;
    /**
     * @brief last insertion ID
     * @param _table
     * table name
     * @return last insertion id from table
     */
    long long lastInsertionId(const QString &_table);
    /**
     * @brief additional function to get payload for query
     * @param _table
     * table name for search
     * @param _value
     * value to build payload
     * @return list of keys and list of values
     */
    QHash<QString, QString> getQueryPayload(const QString &_table,
                                            const QVariantHash &_value) const;
};


#endif /* QUEUEDDATABASE_H */
