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
 * @file QueuedEnums.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDENUMS_H
#define QUEUEDENUMS_H


/**
 * @defgroup QueuedEnums
 * @brief Queued enumerations
 */
namespace QueuedEnums
{
/**
 * @ingroup QueuedEnums
 * @enum LimitType
 * @brief available limit types
 * @var LimitType::CPUThreads
 * limit on CPU threads count
 * @var LimitType::GPUThreads
 * limit on GPU threads count
 * @var LimitType::Memory
 * limit on physical memory
 * @var LimitType::GPUMemory
 * limit on GPU memory
 * @var LimitType::Storage
 * limit on storage
 */
enum LimitType {
    CPUThreads = 1 << 0,
    GPUThreads = 1 << 1,
    Memory = 1 << 2,
    GPUMemory = 1 << 3,
    Storage = 1 << 4
};
Q_DECLARE_FLAGS(LimitTypes, LimitType)
Q_DECLARE_OPERATORS_FOR_FLAGS(LimitTypes)
/**
 * @ingroup QueuedEnums
 * @enum Permissions
 * @brief available user permissions
 * @var Permissions::Admin
 * administrative permissions
 * @var Permissions::JobOwner
 * owner job related permissions
 * @var Permissions::User
 * user related permissions
 * @var Permissions::Web
 * web server access
 * @var Permissions::Reports
 * access to reports
 */
enum class Permission {
    SuperAdmin = 1 << 0,
    Admin = 1 << 1,
    JobOwner = 1 << 2,
    User = 1 << 3,
    Web = 1 << 4,
    Reports = 1 << 5
};
Q_DECLARE_FLAGS(Permissions, Permission)
Q_DECLARE_OPERATORS_FOR_FLAGS(Permissions)
/**
 * @ingroup QueuedEnums
 * @enum ProcessState
 * @brief process state description
 * @var ProcessState::NotRunning
 * process is not running yet
 * @var ProcessState::Running
 * process running at the moment
 * @var ProcessState::Exited
 * process exit
 */
enum class ProcessState {
    NotRunning = 1 << 0,
    Running = 1 << 1,
    Exited = 1 << 2
};
};


#endif /* QUEUEDENUMS_H */
