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
 * @file QueuedCOnfiguration.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDCONFIGURATION_H
#define QUEUEDCONFIGURATION_H

#include <QString>


/**
 * @defgroup QueuedCfg
 * @brief Queued configuration related types
 */
namespace QueuedCfg
{
/**
 * @ingroup QueuedCfg
 * @struct QueuedAdminSetup
 * @brief structure to define administrator user
 * @var name
 * administrator user name
 * @var password
 * administrator user password
 */
typedef struct {
    QString name;
    QString password;
} QueuedAdminSetup;
/**
 * m@ingroup QueuedCfg
 * @struct QueuedDBSetup
 * @brief structure to define database setup
 * @var driver
 * driver name
 * @var hostname
 * hostname to connect
 * @var password
 * password to connect if any
 * @var path
 * path to database
 * @var port
 * port to connect
 * @var username
 * username to connect if any
 */
typedef struct {
    QString driver;
    QString hostname;
    QString password;
    QString path;
    int port;
    QString username;
} QueuedDBSetup;
};


#endif /* QUEUEDCONFIGURATION_H */
