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

#include <vector>

#include <jubatus/client/classifier_client.hpp>

class QJubatusClassifier::Private
{
public:
    Private();
    ~Private();

    jubatus::classifier::client::classifier *client;
};

QJubatusClassifier::Private::Private()
    : client(0)
{
}

QJubatusClassifier::Private::~Private()
{
    delete client;
}

QJubatusClassifier::QJubatusClassifier(QObject *parent)
    : QJubatusClient(parent)
    , d(new Private)
{
    connect(this, &QJubatusClassifier::destroyed, [this](){ delete d; });
    auto deleteClient = [this]() { delete d->client; d->client = 0; };
    connect(this, &QJubatusClassifier::hostChanged, deleteClient);
    connect(this, &QJubatusClassifier::portChanged, deleteClient);
    connect(this, &QJubatusClassifier::nameChanged, deleteClient);
    connect(this, &QJubatusClassifier::timeoutChanged, deleteClient);
}

void QJubatusClassifier::train(const QList<QJubatusClassifier::TrainData> &data)
{
    std::vector<jubatus::classifier::labeled_datum> train_data;
    foreach (const TrainData &v, data) {
        train_data.push_back(jubatus::classifier::labeled_datum(v.first.toStdString(), convert(v.second)));
    }
    train(train_data);
}

void QJubatusClassifier::train(const std::vector<jubatus::classifier::labeled_datum> &data)
{
    if (!d->client) {
        d->client = new jubatus::classifier::client::classifier(host().toStdString(), port(), name().toStdString(), timeout());
    }
    d->client->train(data);
}

QList<QList<QJubatusClassifier::EstimateResult>> QJubatusClassifier::classify(const QList<QVariantMap> &data)
{
    QList<QList<EstimateResult>> ret;

    std::vector<jubatus::client::common::datum> test_data;
    foreach (const QVariant &v, data) {
        test_data.push_back(convert(v.toMap()));
    }

    std::vector<std::vector<jubatus::classifier::estimate_result> > results = classify(test_data);

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

std::vector<std::vector<jubatus::classifier::estimate_result>> QJubatusClassifier::classify(const std::vector<jubatus::client::common::datum> &data)
{
    if (!d->client) {
        d->client = new jubatus::classifier::client::classifier(host().toStdString(), port(), name().toStdString(), timeout());
    }
    return d->client->classify(data);
}


