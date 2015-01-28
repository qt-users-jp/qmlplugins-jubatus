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

#include <jubatus/client/common/client.hpp>
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
    , m_client(0)
{
    connect(this, &QJubatusClient::destroyed, [this](){ delete d; });
    auto deleteClient = [this]() { delete m_client; m_client = 0; };
    connect(this, &QJubatusClient::hostChanged, deleteClient);
    connect(this, &QJubatusClient::portChanged, deleteClient);
    connect(this, &QJubatusClient::nameChanged, deleteClient);
    connect(this, &QJubatusClient::timeoutChanged, deleteClient);
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

std::string QJubatusClient::convert(const QString &data) const
{
    return data.toStdString();
}

QString QJubatusClient::convert(const std::string &data) const
{
    return QString::fromStdString(data);
}

std::vector<std::string> QJubatusClient::convert(const QStringList &data) const
{
    std::vector<std::string> ret;
    foreach (const QString &str, data) {
        ret.push_back(convert(str));
    }
    return ret;
}

QStringList QJubatusClient::convert(const std::vector<std::string> &data) const
{
    QStringList ret;
    for (auto it = data.begin(); it != data.end(); ++it) {
        ret.append(convert(*it));
    }
    return ret;
}

jubatus::client::common::datum QJubatusClient::convert(const QVariantMap &data) const
{
    jubatus::client::common::datum ret;

    foreach (const QString &key, data.keys()) {
        QVariant value = data.value(key);
        switch (static_cast<int>(value.type())) {
        case QVariant::String:
            ret.add_string(key.toStdString(), value.toString().toStdString());
            break;
        case QVariant::Int:
            ret.add_number(key.toStdString(), value.toInt());
            break;
        case QVariant::UInt:
            ret.add_number(key.toStdString(), value.toUInt());
            break;
        case QMetaType::Float:
            ret.add_number(key.toStdString(), value.toFloat());
            break;
        case QVariant::Double:
            ret.add_number(key.toStdString(), value.toDouble());
            break;
        default:
            qDebug() << Q_FUNC_INFO << __LINE__ << value.type() << value << "not supported.";
            break;
        }
    }

    return ret;
}

QVariantMap QJubatusClient::convert(const jubatus::client::common::datum &data) const
{
    QVariantMap ret;
    for (auto it = data.num_values.begin(); it != data.num_values.end(); ++it) {
        ret.insert(convert(it->first), it->second);
    }
    for (auto it = data.string_values.begin(); it != data.string_values.end(); ++it) {
        ret.insert(convert(it->first), convert(it->second));
    }
    return ret;
}

std::vector<jubatus::client::common::datum> QJubatusClient::convert(const QList<QVariantMap> &data) const
{
    std::vector<jubatus::client::common::datum> ret;
    foreach (const QVariantMap &datum, data) {
        ret.push_back(convert(datum));
    }
    return ret;
}
QList<QVariantMap> QJubatusClient::convert(const std::vector<jubatus::client::common::datum> &data) const
{
    QList<QVariantMap> ret;
    for (auto it = data.begin(); it != data.end(); ++it) {
        ret.append(convert(*it));
    }
    return ret;
}
