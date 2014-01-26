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

#include "qjubatusrecommender.h"

#include <QtCore/QDebug>

#include <vector>

#include <jubatus/client/recommender_client.hpp>

QJubatusRecommender::QJubatusRecommender(QObject *parent)
    : QJubatusClient(parent)
{
}

bool QJubatusRecommender::clearRow(const QString &id)
{
    bool ret = false;
    EXEC_JUBATUS_COMMAND( ret = client()->clear_row(convert(id)); )
    return ret;
}

bool QJubatusRecommender::updateRow(const QString &id, const QVariantMap &data)
{
    bool ret = false;
    EXEC_JUBATUS_COMMAND( ret = client()->update_row(convert(id), convert(data)); )
    return ret;
}

QVariantMap QJubatusRecommender::completeRowFromId(const QString &id)
{
    QVariantMap ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->complete_row_from_id(convert(id))); )
    return ret;
}

QVariantMap QJubatusRecommender::completeRowFromDatum(const QVariantMap &data)
{
    QVariantMap ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->complete_row_from_datum(convert(data))); )
    return ret;
}

QList<QJubatusRecommender::IdWithScore> QJubatusRecommender::similarRowFromId(const QString &id, uint size)
{
    QList<QJubatusRecommender::IdWithScore> ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->similar_row_from_id(convert(id), size)); )
    return ret;
}

QList<QJubatusRecommender::IdWithScore> QJubatusRecommender::similarRowFromDatum(const QVariantMap &data, uint size)
{
    QList<QJubatusRecommender::IdWithScore> ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->similar_row_from_datum(convert(data), size)); )
    return ret;
}

QVariantMap QJubatusRecommender::decodeRow(const QString &id)
{
    QVariantMap ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->decode_row(id.toStdString())); )
    return ret;
}

QStringList QJubatusRecommender::getAllRows()
{
    QStringList ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->get_all_rows()); )
    return ret;
}

float QJubatusRecommender::calcSimilarity(const QVariantMap &data1, const QVariantMap &data2)
{
    float ret = 0.0;
    EXEC_JUBATUS_COMMAND( ret = client()->calc_similarity(convert(data1), convert(data2)); )
    return ret;
}

float QJubatusRecommender::calcL2Norm(const QVariantMap &data)
{
    float ret = 0.0;
    EXEC_JUBATUS_COMMAND( ret = client()->calc_l2norm(convert(data)); )
    return ret;
}

QJubatusRecommender::IdWithScore QJubatusRecommender::convert(const jubatus::recommender::id_with_score &data) const
{
    QJubatusRecommender::IdWithScore ret;
    ret.id = convert(data.id);
    ret.score = data.score;
    return ret;
}

QList<QJubatusRecommender::IdWithScore> QJubatusRecommender::convert(const std::vector<jubatus::recommender::id_with_score> &data) const
{
    QList<QJubatusRecommender::IdWithScore> ret;
    foreach (const jubatus::recommender::id_with_score id_with_score, data) {
        ret.append(convert(id_with_score));
    }
    return ret;
}

jubatus::recommender::client::recommender *QJubatusRecommender::client() {
    return QJubatusClient::client<jubatus::recommender::client::recommender>();
}
