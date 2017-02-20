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
 * @file QueuedDatabase.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDDATABASE_H
#define QUEUEDDATABASE_H

#include <QHash>
#include <QObject>
#include <QSqlDatabase>


typedef QHash<QString, QStringList> QueuedDBSchema;

/**
 * @brief queued adaptor to databases
 */
class QueuedDatabase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path)

    // TODO add fields SQL parameters (type whatever), Qt specific types
    const QueuedDBSchema DBSchema = {
        {"users",
         {"_id", "name", "uid", "gid", "password_sha512", "email", "cpu", "gpu",
          "memory", "gpumemory", "storage"}},
        {"tasks", {"_id", "userId", "command", "arguments", "workDirectory"}}};

public:
    /**
     * @brief QueuedDatabase class constructor
     * @param parent         pointer to parent item
     * @param path           path to database
     * @param driver         database driver
     */
    explicit QueuedDatabase(QObject *parent, const QString path,
                            const QString driver);
    /**
     * @brief QueuedDatabase class destructor
     */
    virtual ~QueuedDatabase();
    /**
     * @brief check and create database
     */
    void checkDatabase();
    /**
     * @brief open database
     * @param _hostname      hostname to connect, may be empty
     * @param _port          port to connect, may be 0
     * @param _username      username to connect, may be empty
     * @param _password      password to connect, will be ignored if _username
     * is empty
     */
    void open(const QString _hostname, const int _port, const QString _username,
              const QString _password);
    /**
     * @brief path to database
     * @return path to used database
     */
    QString path() const;

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
     * @brief create or update actual schema in table
     * @param _table         table name
     */
    void createSchema(const QString _table);
    /**
     * @brief create given table
     * @param _table         table name
     */
    void createTable(const QString _table);
};

#endif /* QUEUEDDATABASE_H */
