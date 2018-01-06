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
 * @file QueuedDatabaseSchema.h
 * Header of Queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDDATABASESCHEMA_H
#define QUEUEDDATABASESCHEMA_H

#include <QHash>
#include <QVariant>


/**
 * @addtogroup QueuedDB
 * @brief Queued database related constants
 */
namespace QueuedDB
{
/**
 * @brief settings table name
 */
static const char SETTINGS_TABLE[] = "settings";
/**
 * @brief tasks table name
 */
static const char TASKS_TABLE[] = "tasks";
/**
 * @brief tokens table name
 */
static const char TOKENS_TABLE[] = "tokens";
/**
 * @brief users table name
 */
static const char USERS_TABLE[] = "users";
/**
 * @struct QueuedDBField
 * @brief describes database column
 * @var QueuedDBField::name
 * column name
 * @var QueuedDBField::sqlDescription
 * description to create column
 * @var QueuedDBField::type
 * Qt type of column for cast
 * @var QueuedDBField::adminField
 * is admin permissions required to edit or not
 */
typedef struct {
    QString name;
    QString sqlDescription;
    QVariant::Type type;
    bool adminField;
} QueuedDBField;
/**
 * @typedef QueuedDBSchema
 * custom map for database schemas descriptions
 */
typedef QHash<QString, QHash<QString, QueuedDBField>> QueuedDBSchema;
/**
 * @brief database schema
 */
static const QueuedDBSchema DBSchema
    = {{SETTINGS_TABLE,
        {{"_id", {"_id", "INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE", QVariant::LongLong, true}},
         {"key", {"key", "TEXT NOT NULL DEFAULT '0'", QVariant::String, true}},
         {"value", {"value", "TEXT", QVariant::String, true}}}},
       {TASKS_TABLE,
        {{"_id", {"_id", "INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE", QVariant::LongLong, true}},
         {"user", {"user", "INT NOT NULL DEFAULT 0", QVariant::LongLong, true}},
         {"command", {"command", "TEXT", QVariant::String, false}},
         {"commandArguments", {"commandArguments", "TEXT", QVariant::String, false}},
         {"workDirectory", {"workDirectory", "TEXT", QVariant::String, false}},
         {"nice", {"nice", "INT NOT NULL DEFAULT 0", QVariant::UInt, true}},
         {"uid", {"uid", "INT", QVariant::UInt, true}},
         {"gid", {"gid", "INT", QVariant::UInt, true}},
         {"limits", {"limits", "TEXT", QVariant::String, false}},
         {"startTime", {"startTime", "INT", QVariant::LongLong, true}},
         {"endTime", {"endTime", "INT", QVariant::LongLong, true}}}},
       {TOKENS_TABLE,
        {{"_id", {"_id", "INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE", QVariant::LongLong, true}},
         {"token", {"token", "TEXT NOT NULL DEFAULT '0'", QVariant::String, true}},
         {"user", {"user", "TEXT NOT NULL DEFAULT '0'", QVariant::String, true}},
         {"validUntil", {"validUntil", "TEXT NOT NULL DEFAULT '0'", QVariant::String, true}}}},
       {USERS_TABLE,
        {{"_id", {"_id", "INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE", QVariant::LongLong, true}},
         {"name", {"name", "TEXT NOT NULL DEFAULT '0'", QVariant::String, true}},
         {"password", {"password", "TEXT", QVariant::String, false}},
         {"email", {"email", "TEXT", QVariant::String, false}},
         {"lastLogin", {"lastLogin", "TEXT", QVariant::String, true}},
         {"limits", {"limits", "TEXT", QVariant::String, true}},
         {"permissions", {"permissions", "INT", QVariant::UInt, true}},
         {"priotiry", {"priority", "INT", QVariant::UInt, true}}}}};
};

#endif /* QUEUEDDATABASESCHEMA_H */
