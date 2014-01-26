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

#ifndef QJUBATUSRECOMMENDER_H
#define QJUBATUSRECOMMENDER_H

#include "jubatus_global.h"
#include "qjubatusclient.h"

#include <QtCore/QVariant>

namespace jubatus {
    namespace recommender {
        struct id_with_score;
        namespace client {
            class recommender;
        }
    }
}

class JUBATUS_EXPORT QJubatusRecommender : public QJubatusClient
{
    Q_OBJECT
public:
    explicit QJubatusRecommender(QObject *parent = 0);

    struct IdWithScore {
        QString id;
        float score;
    };

    Q_INVOKABLE bool clearRow(const QString &id);
    Q_INVOKABLE bool updateRow(const QString &id, const QVariantMap &data);
    Q_INVOKABLE QVariantMap completeRowFromId(const QString &id);
    Q_INVOKABLE QVariantMap completeRowFromDatum(const QVariantMap &data);
    Q_INVOKABLE QList<IdWithScore> similarRowFromId(const QString &id, uint size);
    Q_INVOKABLE QList<IdWithScore> similarRowFromDatum(const QVariantMap &data, uint size);
    Q_INVOKABLE QVariantMap decodeRow(const QString &id);
    Q_INVOKABLE QStringList getAllRows();
    Q_INVOKABLE float calcSimilarity(const QVariantMap &data1, const QVariantMap &data2);
    Q_INVOKABLE float calcL2Norm(const QVariantMap &data);

protected:
    using QJubatusClient::convert;
    IdWithScore convert(const jubatus::recommender::id_with_score &data) const;
    QList<IdWithScore> convert(const std::vector<jubatus::recommender::id_with_score> &data) const;

private:
    jubatus::recommender::client::recommender *client();
};

#endif // QJUBATUSRECOMMENDER_H
