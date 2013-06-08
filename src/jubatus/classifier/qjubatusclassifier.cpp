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

#include "qjubatusclassifier.h"

#include <QtCore/QDebug>

#include <string>
#include <vector>

#include <jubatus/client.hpp>

class QJubatusClassifier::Private
{
public:
    Private(QJubatusClassifier *parent);
    ~Private();

private:
    QJubatusClassifier *q;

public:
    jubatus::classifier::client::classifier *client;

    QString host;
    int port;
    double timeout;
};

QJubatusClassifier::Private::Private(QJubatusClassifier *parent)
    : q(parent)
    , client(0)
    , host(QStringLiteral("localhost"))
    , port(9199)
    , timeout(1.0)
{
}

QJubatusClassifier::Private::~Private()
{
    delete client;
}

QJubatusClassifier::QJubatusClassifier(QObject *parent)
    : QObject(parent)
    , d(new Private(this))
{
    connect(this, &QJubatusClassifier::destroyed, [this](){ delete d; });
}

const QString &QJubatusClassifier::host() const
{
    return d->host;
}

void QJubatusClassifier::setHost(const QString &host)
{
    if (d->host == host) return;
    d->host = host;
    emit hostChanged(host);
}

int QJubatusClassifier::port() const
{
    return d->port;
}

void QJubatusClassifier::setPort(int port)
{
    if (d->port == port) return;
    d->port = port;
    emit portChanged(port);
}

double QJubatusClassifier::timeout() const
{
    return d->timeout;
}

void QJubatusClassifier::setTimeout(double timeout)
{
    if (d->timeout == timeout) return;
    d->timeout = timeout;
    emit timeoutChanged(timeout);
}

jubatus::classifier::datum QJubatusClassifier::convert(const QVariantMap &data)
{
    jubatus::classifier::datum ret;

    foreach (const QString &key, data.keys()) {
        QVariant value = data.value(key);
        switch (value.type()) {
        case QVariant::String:
            ret.string_values.push_back(make_pair(key.toStdString(), value.toString().toStdString()));
            break;
        case QVariant::Int:
            ret.num_values.push_back(make_pair(key.toStdString(), value.toInt()));
            break;
        case QVariant::Double:
            ret.num_values.push_back(make_pair(key.toStdString(), value.toDouble()));
            break;
        default:
            qDebug() << Q_FUNC_INFO << __LINE__ << value << "not supported.";
            break;
        }
    }

    return ret;
}

void QJubatusClassifier::train(const QString &name, const QList<QJubatusClassifier::TrainData> &data)
{
    std::vector<std::pair<std::string, jubatus::classifier::datum>> train_data;
    foreach (const TrainData &v, data) {
        train_data.push_back(make_pair(v.first.toStdString(), convert(v.second)));
    }
    train(name.toStdString(), train_data);
}

void QJubatusClassifier::train(const std::string &name, const std::vector<std::pair<std::string, jubatus::classifier::datum>> &data)
{
    jubatus::classifier::client::classifier client(d->host.toStdString(), d->port, d->timeout);
    client.train(name, data);
}

QList<QList<QJubatusClassifier::EstimateResult>> QJubatusClassifier::classify(const QString &name, const QList<QVariantMap> &data)
{
    QList<QList<EstimateResult>> ret;

    std::vector<jubatus::classifier::datum> test_data;
    foreach (const QVariant &v, data) {
        test_data.push_back(convert(v.toMap()));
    }

    std::vector<std::vector<jubatus::classifier::estimate_result> > results = classify(name.toStdString(), test_data);

    for (size_t i = 0; i < results.size(); ++i) {
        QList<EstimateResult> list;
        for (size_t j = 0; j < results[i].size(); ++j) {
            const jubatus::classifier::estimate_result& jr = results[i][j];
            EstimateResult qr;
            qr.label = QString::fromStdString(jr.label);
            qr.score = jr.score;
            list.append(qr);
        }
        ret.append(list);
    }
    return ret;
}

std::vector<std::vector<jubatus::classifier::estimate_result>> QJubatusClassifier::classify(const std::string &name, const std::vector<jubatus::classifier::datum> &data)
{
    jubatus::classifier::client::classifier client(d->host.toStdString(), d->port, d->timeout);
    return client.classify(name, data);
}


