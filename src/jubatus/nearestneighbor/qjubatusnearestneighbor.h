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

#ifndef QJUBATUSNEAREST_NEIGHBOR_H
#define QJUBATUSNEAREST_NEIGHBOR_H

#include "jubatus_global.h"
#include "qjubatusclient.h"

#include <QtCore/QVariant>

namespace jubatus {
    namespace nearest_neighbor {
        struct id_with_score;
        namespace client {
            class nearest_neighbor;
        }
    }
}

class JUBATUS_EXPORT QJubatusNearestNeighbor : public QJubatusClient
{
    Q_OBJECT
public:
    explicit QJubatusNearestNeighbor(QObject *parent = nullptr);

    struct IdWithScore {
        IdWithScore() : score(0.0) {}
        QString id;
        float score;
    };

    bool clear();
    bool setRow(const QString &id, const QVariantMap &data);
    QList<IdWithScore> neighborRowFromId(const QString &id, uint size);
    QList<IdWithScore> neighborRowFromDatum(const QVariantMap &query, uint size);
    QList<IdWithScore> similarRowFromId(const QString &id, uint size);
    QList<IdWithScore> similarRowFromDatum(const QVariantMap &query, uint size);
    QStringList getAllRows();

protected:
    using QJubatusClient::convert;
    jubatus::nearest_neighbor::id_with_score convert(const IdWithScore &data) const;
    IdWithScore convert(const jubatus::nearest_neighbor::id_with_score &data) const;
    QList<IdWithScore> convert(const std::vector<jubatus::nearest_neighbor::id_with_score> &data) const;

private:
    jubatus::nearest_neighbor::client::nearest_neighbor *client();
};

#endif // QJUBATUSNEAREST_NEIGHBOR_H
