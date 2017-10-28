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
 * @file QueuedReportManager.cpp
 * Source code of queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#include <queued/Queued.h>

#include <queued/QueuedDatabaseSchema.h>


/**
 * @class QueuedReportManager
 */
/**
 * @fn QueuedReportManager
 */
QueuedReportManager::QueuedReportManager(QObject *_parent,
                                         QueuedDatabase *_database)
    : QObject(_parent)
    , m_database(_database)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;
}


/**
 * @fn ~QueuedReportManager
 */
QueuedReportManager::~QueuedReportManager()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;
}


/**
 * @fn performance
 */
QList<QVariantHash> QueuedReportManager::performance(const QueuedCore *_core,
                                                     const QDateTime &_from,
                                                     const QDateTime &_to) const
{
    qCDebug(LOG_LIB) << "Build performance report from" << _from << "to" << _to;

    QStringList conditions;
    if (_from.isValid())
        conditions += QString("((datetime(startTime) > datetime('%1')) OR "
                              "(startTime IS NULL))")
                          .arg(_from.toString(Qt::ISODateWithMs));
    if (_to.isValid())
        conditions += QString("((datetime(endTime) < datetime('%1')) AND "
                              "(endTime NOT NULL))")
                          .arg(_to.toString(Qt::ISODateWithMs));

    QString condition
        = conditions.isEmpty()
              ? ""
              : QString("WHERE (%1)").arg(conditions.join(" AND "));
    qCInfo(LOG_LIB) << "Task condition select" << condition;
    auto tasks = m_database->get(QueuedDB::TASKS_TABLE, condition);

    // build hash first
    QHash<long long, QVariantHash> hashOutput;
    for (auto &task : tasks) {
        QueuedLimits::Limits limits
            = QueuedLimits::Limits(task["limits"].toString());
        // update values to system ones if empty
        if (limits.cpu == 0)
            limits.cpu = QueuedSystemInfo::cpuCount();
        if (limits.memory == 0)
            limits.memory = QueuedSystemInfo::memoryCount();
        // calculate usage stats
        long long taskTime
            = QDateTime::fromString(task["endTime"].toString(),
                                    Qt::ISODateWithMs)
                  .toMSecsSinceEpoch()
              - QDateTime::fromString(task["startTime"].toString(),
                                      Qt::ISODateWithMs)
                    .toMSecsSinceEpoch();
        limits *= taskTime / 1000;

        // append
        long long userId = task.value("user").toLongLong();
        auto userObj = _core->user(userId);
        QVariantHash currentData = hashOutput[userId];
        currentData["cpu"]
            = currentData.value("cpu", 0).toLongLong() + limits.cpu;
        currentData["memory"]
            = currentData.value("memory", 0).toLongLong() + limits.memory;
        currentData["gpu"]
            = currentData.value("gpu", 0).toLongLong() + limits.gpu;
        currentData["gpumemory"]
            = currentData.value("gpumemory", 0).toLongLong() + limits.gpumemory;
        currentData["storage"]
            = currentData.value("storage", 0).toLongLong() + limits.storage;
        currentData["count"] = currentData.value("count", 0).toLongLong() + 1;
        // internal fields
        currentData["user"] = userObj ? userObj->name() : "";
        currentData["email"] = userObj ? userObj->email() : "";
        currentData["_id"] = userId;

        hashOutput[userId] = currentData;
    }

    // append output data
    auto userList = hashOutput.keys();
    std::sort(userList.begin(), userList.end(),
              [](const long long first, const long long second) {
                  return first < second;
              });
    QList<QVariantHash> output;
    for (auto userId : userList)
        output.append(hashOutput[userId]);

    return output;
}


/**
 * @fn tasks
 */
QList<QVariantHash> QueuedReportManager::tasks(const long long _user,
                                               const QDateTime &_from,
                                               const QDateTime &_to) const
{
    qCDebug(LOG_LIB) << "Search for tasks in" << _user << _from << _to;

    QStringList conditions;
    if (_user > 0)
        conditions += QString("(user = %1)").arg(_user);
    if (_from.isValid())
        conditions += QString("((datetime(startTime) > datetime('%1')) OR "
                              "(startTime IS NULL))")
                          .arg(_from.toString(Qt::ISODateWithMs));
    if (_to.isValid())
        conditions += QString("((datetime(endTime) < datetime('%1')) AND "
                              "(endTime NOT NULL))")
                          .arg(_to.toString(Qt::ISODateWithMs));

    QString condition
        = conditions.isEmpty()
              ? ""
              : QString("WHERE (%1)").arg(conditions.join(" AND "));
    qCInfo(LOG_LIB) << "Task condition select" << condition;

    return m_database->get(QueuedDB::TASKS_TABLE, condition);
}


/**
 * @fn users
 */
QList<QVariantHash>
QueuedReportManager::users(const QDateTime &_lastLogged,
                           const QueuedEnums::Permission _permission) const
{
    qCDebug(LOG_LIB) << "Search for users in" << _lastLogged
                     << static_cast<uint>(_permission);

    QStringList conditions;
    if (_lastLogged.isValid())
        conditions += QString("((datetime(lastLogin) > datetime('%1')) AND "
                              "(lastLogin NOT NULL))")
                          .arg(_lastLogged.toString(Qt::ISODateWithMs));
    if (_permission != QueuedEnums::Permission::Invalid)
        conditions += QString("((permissions & ~%1) != permissions)")
                          .arg(static_cast<uint>(_permission));

    QString condition
        = conditions.isEmpty()
              ? ""
              : QString("WHERE (%1)").arg(conditions.join(" AND "));
    qCInfo(LOG_LIB) << "User condition select" << condition;

    return m_database->get(QueuedDB::USERS_TABLE, condition);
}
