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


#include "QueuedctlOption.h"

#include <queued/Queued.h>

#include "QueuedctlAuth.h"


bool QueuedctlOption::editOption(const QString &_option, const QVariant &_value,
                                 const QString &_cache, const QString &_user)
{
    qCDebug(LOG_APP) << "Edit option" << _option << "to" << _value;

    QString token = QueuedctlAuth::getToken(_cache, _user);
    return QueuedCoreAdaptor::sendOptionEdit(_option, _value, token);
}


void QueuedctlOption::parser(QCommandLineParser &_parser)
{
    _parser.clearPositionalArguments();
    _parser.addPositionalArgument("option", "Option name.");
    _parser.addPositionalArgument("value", "Option value.");
}
