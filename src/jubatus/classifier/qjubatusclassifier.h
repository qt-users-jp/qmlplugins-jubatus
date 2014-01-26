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

#ifndef QJUBATUSCLASSIFIER_H
#define QJUBATUSCLASSIFIER_H

#include "jubatus_global.h"
#include "qjubatusclient.h"

#include <QtCore/QVariant>


namespace jubatus {
    namespace classifier {
        struct estimate_result;
        struct labeled_datum;
        namespace client {
            class classifier;
        }
    }
}

class JUBATUS_EXPORT QJubatusClassifier : public QJubatusClient
{
    Q_OBJECT
public:
    explicit QJubatusClassifier(QObject *parent = 0);

    struct LabeledDatum {
        QString label;
        QVariantMap data;
    };

    struct EstimateResult {
        QString label;
        double score;
    };

    typedef QPair<QString, QVariantMap> TrainData;

    Q_INVOKABLE void train(const QList<QJubatusClassifier::LabeledDatum> &data);
    Q_INVOKABLE QList<QList<QJubatusClassifier::EstimateResult>> classify(const QList<QVariantMap> &data);

protected:
    using QJubatusClient::convert;
    jubatus::classifier::labeled_datum convert(const QJubatusClassifier::LabeledDatum &data) const;
    std::vector<jubatus::classifier::labeled_datum> convert(const QList<QJubatusClassifier::LabeledDatum> &data) const;
    QJubatusClassifier::EstimateResult convert(const jubatus::classifier::estimate_result &data) const;
    QList<QJubatusClassifier::EstimateResult> convert(const std::vector<jubatus::classifier::estimate_result> &data) const;
    QList<QList<QJubatusClassifier::EstimateResult>> convert(const std::vector<std::vector<jubatus::classifier::estimate_result>> &data) const;

private:
    jubatus::classifier::client::classifier *client();
};

#endif // QJUBATUSCLASSIFIER_H
