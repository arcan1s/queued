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
 * @file QueuedPluginManagerInterface.h
 * Header of Queued library
 * @author Evgeniy Alekseev
 * @copyright MIT
 * @bug https://github.com/arcan1s/queued/issues
 */


#ifndef QUEUEDPLUGINMANAGERINTERFACE_H
#define QUEUEDPLUGINMANAGERINTERFACE_H

#include <QObject>


/**
 * @brief signal interface for plugin manager
 */
class QueuedPluginManagerInterface : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief QueuedPluginManagerInterface class constructor
     */
    explicit QueuedPluginManagerInterface()
        : QObject(nullptr){};
    /**
     * @brief QueuedPluginManagerInterface class destructor
     */
    virtual ~QueuedPluginManagerInterface(){};

signals:
};


#endif /* QUEUEDPLUGINMANAGERINTERFACE_H */
