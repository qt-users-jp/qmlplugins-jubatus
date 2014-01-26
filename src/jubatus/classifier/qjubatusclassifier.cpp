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

QJubatusClassifier::QJubatusClassifier(QObject *parent)
    : QJubatusClient(parent)
{
}

void QJubatusClassifier::train(const QList<QJubatusClassifier::LabeledDatum> &data)
{
    EXEC_JUBATUS_COMMAND( client()->train(convert(data)); )
}

QList<QList<QJubatusClassifier::EstimateResult> > QJubatusClassifier::classify(const QList<QVariantMap> &data)
{
    QList<QList<QJubatusClassifier::EstimateResult> > ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->classify(convert(data))); )
    return ret;
}

jubatus::classifier::labeled_datum QJubatusClassifier::convert(const QJubatusClassifier::LabeledDatum &data) const
{
    jubatus::classifier::labeled_datum ret;
    ret.label = convert(data.label);
    ret.data = convert(data.data);
    return ret;
}

std::vector<jubatus::classifier::labeled_datum> QJubatusClassifier::convert(const QList<QJubatusClassifier::LabeledDatum> &data) const
{
    std::vector<jubatus::classifier::labeled_datum> ret;
    foreach (const QJubatusClassifier::LabeledDatum &datum, data) {
        ret.push_back(convert(datum));
    }
    return ret;
}

QJubatusClassifier::EstimateResult QJubatusClassifier::convert(const jubatus::classifier::estimate_result &data) const
{
    QJubatusClassifier::EstimateResult ret;
    ret.label = convert(data.label);
    ret.score = data.score;
    return ret;
}

QList<QJubatusClassifier::EstimateResult> QJubatusClassifier::convert(const std::vector<jubatus::classifier::estimate_result> &data) const
{
    QList<QJubatusClassifier::EstimateResult> ret;
    for (auto it = data.begin(); it != data.end(); ++it) {
        ret.append(convert(*it));
    }
    return ret;
}

QList<QList<QJubatusClassifier::EstimateResult>> QJubatusClassifier::convert(const std::vector<std::vector<jubatus::classifier::estimate_result>> &data) const
{
    QList<QList<QJubatusClassifier::EstimateResult>> ret;
    for (auto it = data.begin(); it != data.end(); ++it) {
        ret.append(convert(*it));
    }
    return ret;
}

jubatus::classifier::client::classifier *QJubatusClassifier::client()
{
    return QJubatusClient::client<jubatus::classifier::client::classifier>();
}
