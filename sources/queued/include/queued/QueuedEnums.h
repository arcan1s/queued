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
    SuperAdmin = 1 << 1,
    Admin = 1 << 2,
    JobOwner = 1 << 3,
    Web = 1 << 4,
    Reports = 1 << 5
};
Q_DECLARE_FLAGS(Permissions, Permission)
Q_DECLARE_OPERATORS_FOR_FLAGS(Permissions)
};


#endif /* QUEUEDENUMS_H */
