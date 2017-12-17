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

#include "qjubatusnearestneighbor.h"

#include <QtCore/QDebug>

#include <vector>

#include <jubatus/client/nearest_neighbor_client.hpp>

QJubatusNearestNeighbor::QJubatusNearestNeighbor(QObject *parent)
    : QJubatusClient(parent)
{
}

bool QJubatusNearestNeighbor::clear()
{
    bool ret = false;
    EXEC_JUBATUS_COMMAND( ret = client()->clear(); )
    return ret;
}

bool QJubatusNearestNeighbor::setRow(const QString &id, const QVariantMap &data)
{
    bool ret = false;
    EXEC_JUBATUS_COMMAND( ret = client()->set_row(convert(id), convert(data)); )
    return ret;
}

QList<QJubatusNearestNeighbor::IdWithScore> QJubatusNearestNeighbor::neighborRowFromId(const QString &id, uint size)
{
    QList<QJubatusNearestNeighbor::IdWithScore> ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->neighbor_row_from_id(convert(id), size)); )
    return ret;
}

QList<QJubatusNearestNeighbor::IdWithScore> QJubatusNearestNeighbor::neighborRowFromDatum(const QVariantMap &query, uint size)
{
    QList<QJubatusNearestNeighbor::IdWithScore> ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->neighbor_row_from_datum(convert(query), size)); )
    return ret;
}

QList<QJubatusNearestNeighbor::IdWithScore> QJubatusNearestNeighbor::similarRowFromId(const QString &id, uint size)
{
    QList<QJubatusNearestNeighbor::IdWithScore> ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->similar_row_from_id(convert(id), size)); )
    return ret;
}
QList<QJubatusNearestNeighbor::IdWithScore> QJubatusNearestNeighbor::similarRowFromDatum(const QVariantMap &query, uint size)
{
    QList<QJubatusNearestNeighbor::IdWithScore> ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->similar_row_from_datum(convert(query), size)); )
    return ret;
}

QStringList QJubatusNearestNeighbor::getAllRows()
{
    QStringList ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->get_all_rows()); )
    return ret;
}

jubatus::nearest_neighbor::id_with_score QJubatusNearestNeighbor::convert(const QJubatusNearestNeighbor::IdWithScore &data) const
{
    jubatus::nearest_neighbor::id_with_score ret;
    ret.id = convert(data.id);
    ret.score = data.score;
    return ret;
}

QJubatusNearestNeighbor::IdWithScore QJubatusNearestNeighbor::convert(const jubatus::nearest_neighbor::id_with_score &data) const
{
    IdWithScore ret;
    ret.id = convert(data.id);
    ret.score = data.score;
    return ret;
}

QList<QJubatusNearestNeighbor::IdWithScore> QJubatusNearestNeighbor::convert(const std::vector<jubatus::nearest_neighbor::id_with_score> &data) const
{
    QList<QJubatusNearestNeighbor::IdWithScore> ret;
    foreach (const jubatus::nearest_neighbor::id_with_score id_with_score, data) {
        ret.append(convert(id_with_score));
    }
    return ret;
}

jubatus::nearest_neighbor::client::nearest_neighbor *QJubatusNearestNeighbor::client()
{
    return QJubatusClient::client<jubatus::nearest_neighbor::client::nearest_neighbor>();
}
