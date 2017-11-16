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


#ifndef QUEUEDEMAILNOTIFY_H
#define QUEUEDEMAILNOTIFY_H

#include <queued/QueuedPluginInterface.h>


class QueuedEmailNotifyHelper;

class QueuedEmailNotify : public QObject, public QueuedPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.queued.emailnotify/1.0")
    Q_INTERFACES(QueuedPluginInterface)

public:
    virtual ~QueuedEmailNotify() = default;
    void init(const QVariantHash &_settings) override;
    void setToken(const QString &_token) override;
    void setup(const QueuedPluginManagerInterface *_manager) override;
    void updateSettings(const QString &_key, const QVariant &_value) override;

private:
    QueuedEmailNotifyHelper *m_helper = nullptr;
    QString m_token;
};


#endif /* QUEUEDEMAILNOTIFY_H */
