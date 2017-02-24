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
 *@ingroup QueuedDB
* @struct QueuedDBField
* @brief describes database column
* @var name
* column name
* @var sqlDescription
* description to create column
* @var type
* Qt type of column for cast
*/
typedef struct {
    QString name;
    QString sqlDescription;
    QVariant::Type type;
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
    {"settings",
     {{"_id",
       {"_id", "INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE", QVariant::LongLong}},
      {"key", {"key", "TEXT NOT NULL DEFAULT '0'", QVariant::String}},
      {"value", {"value", "TEXT", QVariant::String}}}},
    {"tasks",
     {{"_id",
       {"_id", "INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE", QVariant::LongLong}},
      {"userId", {"userId", "INT NOT NULL DEFAULT 0", QVariant::LongLong}},
      {"command", {"command", "TEXT", QVariant::String}},
      {"arguments", {"arguments", "TEXT", QVariant::String}},
      {"workDirectory", {"workDirectory", "TEXT", QVariant::String}},
      {"nice", {"nice", "INT", QVariant::UInt}},
      {"startTime", {"startTime", "INT", QVariant::LongLong}},
      {"endTime", {"endTime", "INT", QVariant::LongLong}}}},
    {"tokens",
     {{"_id",
       {"_id", "INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE", QVariant::LongLong}},
      {"token", {"token", "TEXT NOT NULL DEFAULT '0'", QVariant::String}},
      {"validUntil",
       {"validUntil", "TEXT NOT NULL DEFAULT '0'", QVariant::String}}}},
    {"users",
     {{"_id",
       {"_id", "INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE", QVariant::LongLong}},
      {"name", {"name", "TEXT NOT NULL DEFAULT '0'", QVariant::String}},
      {"passwordSHA512", {"passwordSHA512", "TEXT", QVariant::String}},
      {"email", {"email", "TEXT", QVariant::String}},
      {"cpu", {"cpu", "INT", QVariant::LongLong}},
      {"gpu", {"gpu", "INT", QVariant::LongLong}},
      {"memory", {"memory", "INT", QVariant::LongLong}},
      {"gpumemory", {"gpumemory", "INT", QVariant::LongLong}},
      {"storage", {"storage", "INT", QVariant::LongLong}},
      {"permissions", {"permissions", "INT", QVariant::UInt}}}}};
};

#endif /* QUEUEDDATABASESCHEMA_H */
