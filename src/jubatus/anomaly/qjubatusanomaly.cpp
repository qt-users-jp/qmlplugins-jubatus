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

#include "qjubatusanomaly.h"

#include <QtCore/QDebug>

#include <vector>

#include <jubatus/client/anomaly_client.hpp>

class QJubatusAnomaly::Private
{
public:
    Private();
    ~Private();

    jubatus::anomaly::client::anomaly *client;
};

QJubatusAnomaly::Private::Private()
    : client(0)
{
}

QJubatusAnomaly::Private::~Private()
{
    delete client;
}

QJubatusAnomaly::QJubatusAnomaly(QObject *parent)
    : QJubatusClient(parent)
    , d(new Private)
{
    connect(this, &QJubatusAnomaly::destroyed, [this](){ delete d; });
    auto deleteClient = [this]() { delete d->client; d->client = 0; };
    connect(this, &QJubatusAnomaly::hostChanged, deleteClient);
    connect(this, &QJubatusAnomaly::portChanged, deleteClient);
    connect(this, &QJubatusAnomaly::nameChanged, deleteClient);
    connect(this, &QJubatusAnomaly::timeoutChanged, deleteClient);
}

bool QJubatusAnomaly::clearRow(const QString &id)
{
    if (!d->client) {
        d->client = new jubatus::anomaly::client::anomaly(host().toStdString(), port(), name().toStdString(), timeout());
    }
    return d->client->clear_row(id.toStdString());
}

QJubatusAnomaly::IdAndScore QJubatusAnomaly::add(const QVariantMap &data)
{
    if (!d->client) {
        d->client = new jubatus::anomaly::client::anomaly(host().toStdString(), port(), name().toStdString(), timeout());
    }
    jubatus::anomaly::id_with_score id_with_score = d->client->add(convert(data));
    IdAndScore ret;
    ret.id = QString::fromStdString(id_with_score.id);
    ret.score = id_with_score.score;
    return ret;
}

float QJubatusAnomaly::update(const QString &id, const QVariantMap &data)
{
    if (!d->client) {
        d->client = new jubatus::anomaly::client::anomaly(host().toStdString(), port(), name().toStdString(), timeout());
    }
    return d->client->update(id.toStdString(), convert(data));
}

float QJubatusAnomaly::overwrite(const QString &id, const QVariantMap &data)
{
    if (!d->client) {
        d->client = new jubatus::anomaly::client::anomaly(host().toStdString(), port(), name().toStdString(), timeout());
    }
    return d->client->overwrite(id.toStdString(), convert(data));
}

float QJubatusAnomaly::calcScore(const QVariantMap &data)
{
    if (!d->client) {
        d->client = new jubatus::anomaly::client::anomaly(host().toStdString(), port(), name().toStdString(), timeout());
    }
    return d->client->calc_score(convert(data));
}

QStringList QJubatusAnomaly::getAllRows()
{
    if (!d->client) {
        d->client = new jubatus::anomaly::client::anomaly(host().toStdString(), port(), name().toStdString(), timeout());
    }
    QStringList ret;
    foreach (std::string id, d->client->get_all_rows())
        ret.append(QString::fromStdString(id));
    return ret;
}

