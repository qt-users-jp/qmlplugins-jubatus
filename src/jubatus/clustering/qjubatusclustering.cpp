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

#include "qjubatusclustering.h"

#include <QtCore/QDebug>

#include <jubatus/client/clustering_client.hpp>

QJubatusClustering::QJubatusClustering(QObject *parent)
    : QJubatusClient(parent)
{
}

bool QJubatusClustering::push(const QList<QVariantMap> &data)
{
    bool ret = false;
    EXEC_JUBATUS_COMMAND( ret = client()->push(convert(data)); )
    return ret;
}

uint QJubatusClustering::getRevision()
{
    uint ret = 0;
    EXEC_JUBATUS_COMMAND( ret = client()->get_revision(); )
    return ret;
}

QList<QList<QJubatusClustering::WeightedDatum> > QJubatusClustering::getCoreMembers()
{
    QList<QList<QJubatusClustering::WeightedDatum> > ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->get_core_members()); )
    return ret;
}

QList<QVariantMap> QJubatusClustering::getKCenter()
{
    QList<QVariantMap> ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->get_k_center()); )
    return ret;
}

QVariantMap QJubatusClustering::getNearestCenter(const QVariantMap &point)
{
    QVariantMap ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->get_nearest_center(convert(point))); )
    return ret;
}

QList<QJubatusClustering::WeightedDatum> QJubatusClustering::getNearestMembers(const QVariantMap &point)
{
    QList<QJubatusClustering::WeightedDatum> ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->get_nearest_members(convert(point))); )
    return ret;
}

QJubatusClustering::WeightedDatum QJubatusClustering::convert(const jubatus::clustering::weighted_datum &data) const
{
    QJubatusClustering::WeightedDatum ret;
    ret.weight = data.weight;
    ret.point = convert(data.point);
    return ret;
}

QList<QJubatusClustering::WeightedDatum> QJubatusClustering::convert(const std::vector<jubatus::clustering::weighted_datum> &data) const
{
    QList<QJubatusClustering::WeightedDatum> ret;
    for (auto it = data.begin(); it != data.end(); ++it) {
        ret.append(convert(*it));
    }
    return ret;
}
QList<QList<QJubatusClustering::WeightedDatum> > QJubatusClustering::convert(const std::vector<std::vector<jubatus::clustering::weighted_datum> > &data) const
{
    QList<QList<QJubatusClustering::WeightedDatum> > ret;
    for (auto it = data.begin(); it != data.end(); ++it) {
        ret.append(convert(*it));
    }
    return ret;
}

jubatus::clustering::client::clustering *QJubatusClustering::client() {
    return QJubatusClient::client<jubatus::clustering::client::clustering>();
}

#ifndef QT_NO_DEBUG_STREAM
QDebug &operator<<(QDebug &dbg, const QJubatusClustering::WeightedDatum &weightedDatum)
{
    dbg.nospace() << "QJubatusClustering::WeightedDatum(weight:" << weightedDatum.weight << ", point: " << weightedDatum.point << ')';
    return dbg.space();
}
#endif
