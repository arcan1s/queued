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
 * @file QueuedControlGroupsAdaptor.h
 * Header of Queued library
 * @author Queued team
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDCONTROLGROUPSADAPTOR_H
#define QUEUEDCONTROLGROUPSADAPTOR_H

#include <QObject>


/**
 * @brief adaptor to control processes by using control groups
 */
class QueuedControlGroupsAdaptor : public QObject
{
    Q_OBJECT
    // static
    Q_PROPERTY(QStringList controlPaths READ controlPaths)
    Q_PROPERTY(QString cpuPath READ cpuPath)
    Q_PROPERTY(QString memoryPath READ memoryPath)
    // dynamic
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(long long cpuLimit READ cpuLimit WRITE setCpuLimit)
    Q_PROPERTY(long long memoryLimit READ memoryLimit WRITE setMemoryLimit)

public:
    // constants
    /**
     * @brief name of file contains cpu limit
     */
    const char *CG_CPU_LIMIT = "cpu.cfs_quota_us";
    /**
     * @brief name of file contains memory limit
     */
    const char *CG_MEMORY_LIMIT = "memory.limit_in_bytes";
    /**
     * @brief name of file contains notify status
     */
    const char *CG_NOTIFY_ON_RELEASE_FILE = "notify_on_release";
    /**
     * @brief name of file contains processes list
     */
    const char *CG_PROC_FILE = "cgroup.procs";
    /**
     * @brief name of file contains release command
     */
    const char *CG_RELEASE_FILE = "release_agent";

    /**
     * @brief QueuedControlGroupsAdaptor class constructor
     * @param _parent
     * pointer to parent object
     * @param _name
     * control group name
     */
    explicit QueuedControlGroupsAdaptor(QObject *_parent, QString _name);
    /**
     * @brief QueuedControlGroupsAdaptor class destructor
     */
    virtual ~QueuedControlGroupsAdaptor();
    /**
     * @brief build group path for specific base directory
     * @param _base
     * full path to base directory
     * @return full path to group properties
     */
    QString groupPath(const QString &_base) const;
    // static properties
    /**
     * @brief paths to all control directories
     * @return full paths to all known control directories
     */
    static QStringList controlPaths();
    /**
     * @brief path to CPU control
     * @return full path to CPU control directory
     */
    static QString cpuPath();
    /**
     * @brief path to memory control
     * @return full path to memory control directory
     */
    static QString memoryPath();
    // instance depended properties
    /**
     * @brief CPU limit
     * @return current CPU limit level
     */
    long long cpuLimit() const;
    /**
     * @brief memory limit
     * @return current memory limit level
     */
    long long memoryLimit() const;
    /**
     * @brief control group name
     * @return control group name
     */
    QString name() const;
    /**
     * @brief set CPU limit
     * @param _value
     * new CPU limit level
     */
    void setCpuLimit(const long long _value);
    /**
     * @brief set memory limit
     * @param _value
     * new memory limit level
     */
    void setMemoryLimit(const long long _value);
    // methods
    /**
     * @brief assign control group to process
     * @param _pid
     * process ID
     * @param _name
     * control group name
     * @return process assignment status
     */
    bool addProcess(const uint _pid);
    /**
     * @brief create empty group
     * @param _name
     * control group name
     * @return group creation status
     */
    bool createGroup();
    /**
     * @brief remove control group
     * @param _name
     * control group name
     * @return group removal status
     */
    bool removeGroup();

private:
    // properties
    /**
     * @brief control group name
     */
    QString m_name;
};


#endif /* QUEUEDCONTROLGROUPSADAPTOR_H */
