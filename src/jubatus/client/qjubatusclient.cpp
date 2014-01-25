/* Copyright (c) 2012 Silk Project.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Silk nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL SILK BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "qjubatusclient.h"

#include <QtCore/QDebug>

#include <jubatus/client/common/datum.hpp>

class QJubatusClient::Private
{
public:
    Private();

    QString host;
    int port;
    QString name;
    double timeout;
};

QJubatusClient::Private::Private()
    : host(QStringLiteral("localhost"))
    , port(9199)
    , name(QStringLiteral("client"))
    , timeout(1.0)
{
}

QJubatusClient::QJubatusClient(QObject *parent)
    : QObject(parent)
    , d(new Private())
{
    connect(this, &QJubatusClient::destroyed, [this](){ delete d; });
}

const QString &QJubatusClient::host() const
{
    return d->host;
}

void QJubatusClient::setHost(const QString &host)
{
    if (d->host == host) return;
    d->host = host;
    emit hostChanged(host);
}

int QJubatusClient::port() const
{
    return d->port;
}

void QJubatusClient::setPort(int port)
{
    if (d->port == port) return;
    d->port = port;
    emit portChanged(port);
}

const QString &QJubatusClient::name() const
{
    return d->name;
}

void QJubatusClient::setName(const QString &name)
{
    if (d->name == name) return;
    d->name = name;
    emit nameChanged(name);
}

double QJubatusClient::timeout() const
{
    return d->timeout;
}

void QJubatusClient::setTimeout(double timeout)
{
    if (d->timeout == timeout) return;
    d->timeout = timeout;
    emit timeoutChanged(timeout);
}

jubatus::client::common::datum QJubatusClient::convert(const QVariantMap &data) const
{
    jubatus::client::common::datum ret;

    foreach (const QString &key, data.keys()) {
        QVariant value = data.value(key);
        switch (value.type()) {
        case QVariant::String:
            ret.string_values.push_back(make_pair(key.toStdString(), value.toString().toStdString()));
            break;
        case QVariant::Int:
            ret.num_values.push_back(make_pair(key.toStdString(), value.toInt()));
            break;
        case QMetaType::Float:
            ret.num_values.push_back(make_pair(key.toStdString(), value.toFloat()));
            break;
        case QVariant::Double:
            ret.num_values.push_back(make_pair(key.toStdString(), value.toDouble()));
            break;
        default:
            qDebug() << Q_FUNC_INFO << __LINE__ << value.type() << value << "not supported.";
            break;
        }
    }

    return ret;
}
