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
 * @file QueuedDatabaseSchema.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDDATABASESCHEMA_H
#define QUEUEDDATABASESCHEMA_H

#include <QHash>
#include <QVariant>


/**
 * @defgroup QueuedDB
 * @brief Queued database related constants
 */
namespace QueuedDB
{
/**
 * @ingroup QueuedDB
 * @brief settings table name
 */
const char SETTINGS_TABLE[] = "settings";
/**
* @ingroup QueuedDB
* @brief tasks table name
*/
const char TASKS_TABLE[] = "tasks";
/**
* @ingroup QueuedDB
* @brief tokens table name
*/
const char TOKENS_TABLE[] = "tokens";
/**
* @ingroup QueuedDB
* @brief users table name
*/
const char USERS_TABLE[] = "users";
/**
 *@ingroup QueuedDB
* @struct QueuedDBField
* @brief describes database column
* @var name
* column name
* @var sqlDescription
* description to create column
* @var type
* Qt type of column for cast
* @var adminField
* is admin permissions required to edit or not
*/
typedef struct {
    QString name;
    QString sqlDescription;
    QVariant::Type type;
    bool adminField;
} QueuedDBField;
/**
 * @ingroup QueuedDB
 * @typedef QueuedDBSchema
 * custom map for database schemas descriptions
 */
typedef QHash<QString, QHash<QString, QueuedDBField>> QueuedDBSchema;
/**
 * @ingroup QueuedDB
 * @brief database schema
 */
const QueuedDBSchema DBSchema = {
    {SETTINGS_TABLE,
     {{"_id",
       {"_id", "INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE", QVariant::LongLong,
        true}},
      {"key", {"key", "TEXT NOT NULL DEFAULT '0'", QVariant::String, true}},
      {"value", {"value", "TEXT", QVariant::String, true}}}},
    {TASKS_TABLE,
     {{"_id",
       {"_id", "INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE", QVariant::LongLong,
        true}},
      {"user", {"user", "INT NOT NULL DEFAULT 0", QVariant::LongLong, false}},
      {"command", {"command", "TEXT", QVariant::String, false}},
      {"commandArguments",
       {"commandArguments", "TEXT", QVariant::String, false}},
      {"workDirectory", {"workDirectory", "TEXT", QVariant::String, false}},
      {"nice", {"nice", "INT", QVariant::UInt, false}},
      {"uid", {"uid", "INT", QVariant::UInt, false}},
      {"gid", {"gid", "INT", QVariant::UInt, false}},
      {"state", {"state", "INT", QVariant::UInt, true}},
      {"limits", {"limits", "TEXT", QVariant::String, false}},
      {"startTime", {"startTime", "INT", QVariant::LongLong, true}},
      {"endTime", {"endTime", "INT", QVariant::LongLong, true}}}},
    {TOKENS_TABLE,
     {{"_id",
       {"_id", "INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE", QVariant::LongLong,
        true}},
      {"token", {"token", "TEXT NOT NULL DEFAULT '0'", QVariant::String, true}},
      {"validUntil",
       {"validUntil", "TEXT NOT NULL DEFAULT '0'", QVariant::String, true}}}},
    {USERS_TABLE,
     {{"_id",
       {"_id", "INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE", QVariant::LongLong,
        true}},
      {"name", {"name", "TEXT NOT NULL DEFAULT '0'", QVariant::String, true}},
      {"password", {"password", "TEXT", QVariant::String, false}},
      {"email", {"email", "TEXT", QVariant::String, false}},
      {"lastLogin", {"lastLogin", "TEXT", QVariant::String, true}},
      {"limits", {"limits", "TEXT", QVariant::String, true}},
      {"permissions", {"permissions", "INT", QVariant::UInt, true}}}}};
};

#endif /* QUEUEDDATABASESCHEMA_H */
