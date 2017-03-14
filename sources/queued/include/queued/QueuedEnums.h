/*
 * Copyright (c) 2017 Evgeniy Alekseev
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
 * @addtogroup QueuedEnums
 * @brief Queued enumerations
 */
namespace QueuedEnums
{
/**
 * @enum Permission
 * @brief available user permissions
 * @var Permission::SuperAdmin
 * "allow all" permissions
 * @var Permission::Admin
 * administrative permissions
 * @var Permission::JobOwner
 * owner job related permissions
 * @var Permission::Web
 * web server access
 * @var Permission::Reports
 * access to reports
 */
enum class Permission {
    SuperAdmin = 1 << 1,
    Admin = 1 << 2,
    JobOwner = 1 << 3,
    Web = 1 << 4,
    Reports = 1 << 5
};
Q_DECLARE_FLAGS(Permissions, Permission)
Q_DECLARE_OPERATORS_FOR_FLAGS(Permissions)
/**
 * @enum ExitAction
 * @brief action with child process on destruction
 * @var ExitAction::Terminate
 * send SIGTERM on exit
 * @var ExitAction::Kill
 * send SIGKILL on exit
 */
enum class ExitAction { Terminate = 1 << 1, Kill = 1 << 2 };
};


#endif /* QUEUEDENUMS_H */
