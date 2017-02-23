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
 * @file QueuedProcess.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDPROCESS_H
#define QUEUEDPROCESS_H

#include <QProcess>
#include <QVariant>

#include "QueuedEnums.h"


/**
 * @struct QueuedProcessDefinition
 * @brief structure to define process
 * @var cmd
 * command line
 * @var args
 * command line arguments
 * @var workingDirectory
 * path to working directory
 * @var uid
 * UID of process
 * @var gid
 * GID of process
 */
typedef struct {
    QString cmd;
    QStringList args;
    QString workingDirectory;
    unsigned int uid;
    unsigned int gid;
} QueuedProcessDefinitions;


/**
 * @brief implementation over QProcess to run processes
 */
class QueuedProcess : public QProcess
{
    Q_OBJECT
    Q_PROPERTY(long long index READ index)
    Q_PROPERTY(QString name READ name)

public:
    /**
     * @brief QueuedProcess class constructor
     * @param parent         pointer to parent item
     * @param definitions    definitions of process
     * @param index          index of process
     */
    explicit QueuedProcess(QObject *parent,
                           const QueuedProcessDefinitions definitions,
                           const long long index);
    /**
     * @brief QueuedProcess class destructor
     */
    virtual ~QueuedProcess();
    /**
     * @brief index of process
     * @return assigned index of process
     */
    long long index() const;
    /**
     * @brief name of process
     * @return generated name of process
     */
    QString name() const;
    /**
     * @brief remove limit
     * @param _limitType     limit type
     */
    virtual void removeLimit(const QueuedEnums::LimitType _limitType);
    /**
     * @brief set limit
     * @param _limitType     limit type
     * @param _value         limit value
     */
    virtual void setLimit(const QueuedEnums::LimitType _limitType,
                          const QVariant &_value = QVariant());
    /**
     * @brief equal operator implementation
     * @param _other         other object
     * @return true if objects are equal
     */
    bool operator==(const QueuedProcess &_other);

protected:
    /**
     * @brief additional method with sets rules for QProcess
     */
    void setupChildProcess();

public slots:
    /**
     * @brief run process
     */
    void run();

private:
    /**
     * @brief process definitions
     */
    QueuedProcessDefinitions m_definitions;
    /**
     * @brief index of process
     */
    long long m_index = -1;
    /**
     * @brief limits array
     */
    QMap<QueuedEnums::LimitType, long long> m_limits;
    /**
     * @brief convert QString memory value to integer
     * @param _value         value to convert
     * @param _status        convertion status
     * @return converted integer
     */
    long long convertMemory(QString _value, bool &_status) const;
};

#endif /* QUEUEDPROCESS_H */
