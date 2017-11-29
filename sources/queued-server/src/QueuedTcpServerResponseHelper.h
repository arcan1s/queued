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


#ifndef QUEUEDTCPSERVERRESPONSEHELPER_H
#define QUEUEDTCPSERVERRESPONSEHELPER_H

#include <QVariant>


namespace QueuedTcpServerResponseHelper
{
enum class RequestPath {
    Unknown,
    Auth,
    Option,
    Permissions,
    Plugin,
    Plugins,
    Reports,
    Status,
    Task,
    Tasks,
    User,
    Users
};
typedef struct {
    int apiVersion;
    QString arg;
    RequestPath path;
    QString type;
    bool valid;
} Request;
static const QHash<int, QByteArray> HTTPCodeMap
    = {{100, "Continue"},
       {101, "Switching Protocols"},
       {200, "OK"},
       {201, "Created"},
       {202, "Accepted"},
       {203, "Non-Authoritative Information"},
       {204, "No Content"},
       {205, "Reset Content"},
       {206, "Partial Content"},
       {300, "Multiple Choices"},
       {301, "Moved Permanently"},
       {302, "Found"},
       {303, "See Other"},
       {304, "Not Modified"},
       {305, "Use Proxy"},
       {307, "Temporary Redirect"},
       {400, "Bad Request"},
       {401, "Unauthorized"},
       {402, "Payment Required"},
       {403, "Forbidden"},
       {404, "Not Found"},
       {405, "Method Not Allowed"},
       {406, "Not Acceptable"},
       {407, "Proxy Authentication Required"},
       {408, "Request Time-out"},
       {409, "Conflict"},
       {410, "Gone"},
       {411, "Length Required"},
       {412, "Precondition Failed"},
       {413, "Request Entity Too Large"},
       {414, "Request-URI Too Large"},
       {415, "Unsupported Media Type"},
       {416, "Requested range not satisfiable"},
       {417, "Expectation Failed"},
       {500, "Internal Server Error"},
       {501, "Not Implemented"},
       {502, "Bad Gateway"},
       {503, "Service Unavailable"},
       {504, "Gateway Time-out"},
       {505, "HTTP Version not supported"}};
QVariantHash getData(const Request &_request, const QVariantHash &_data,
                     const QString &_token);
Request parsePath(const QString &_path);
RequestPath pathToEnum(const QString &_path);
};


#endif /* QUEUEDTCPSERVERRESPONSEHELPER_H */
