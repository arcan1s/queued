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


#include "QueuedctlOption.h"
#include "QueuedctlCommon.h"

#include <queued/Queued.h>


QueuedctlCommon::QueuedctlResult
QueuedctlOption::editOption(const QString &_option, const QVariant &_value,
                            const QString &_token)
{
    qCDebug(LOG_APP) << "Edit option" << _option << "to" << _value;

    QueuedctlCommon::QueuedctlResult output;
    auto res = QueuedCoreAdaptor::sendOptionEdit(_option, _value, _token);
    Result::match(res, [&output](const bool val) { output.status = val; },
                  [&output](const QueuedError &err) {
                      output.output = err.message().c_str();
                  });

    return output;
}


QueuedctlCommon::QueuedctlResult
QueuedctlOption::getOption(const QString &_option)
{
    qCDebug(LOG_APP) << "Get option" << _option;

    QueuedctlCommon::QueuedctlResult output;
    auto res = QueuedCoreAdaptor::getOption(_option);
    Result::match(res,
                  [&output](const QVariant &val) {
                      output.status = val.isValid();
                      output.output = val.toString();
                  },
                  [&output](const QueuedError &err) {
                      output.output = err.message().c_str();
                  });

    return output;
}


void QueuedctlOption::parserGet(QCommandLineParser &_parser)
{
    _parser.addPositionalArgument("option", "Option name.", "<option>");
}


void QueuedctlOption::parserSet(QCommandLineParser &_parser)
{
    _parser.addPositionalArgument("option", "Option name.", "<option>");
    _parser.addPositionalArgument("value", "Option value.", "<value>");
}
