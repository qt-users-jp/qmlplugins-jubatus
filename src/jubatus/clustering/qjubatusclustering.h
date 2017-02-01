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

#ifndef QJUBATUSCLUSTERING_H
#define QJUBATUSCLUSTERING_H

#include "jubatus_global.h"
#include "qjubatusclient.h"

#include <QtCore/QVariant>

namespace jubatus {
    namespace clustering {
        struct weighted_datum;
        struct indexed_point;
        namespace client {
            class clustering;
        }
    }
}

class JUBATUS_EXPORT QJubatusClustering : public QJubatusClient
{
    Q_OBJECT
public:
    explicit QJubatusClustering(QObject *parent = nullptr);

    struct WeightedDatum {
        double weight;
        QVariantMap point;
    };

    struct IndexedPoint {
        QString id;
        QVariantMap point;
    };

    Q_INVOKABLE bool push(const QList<IndexedPoint> &data);
    Q_INVOKABLE uint getRevision();
    Q_INVOKABLE QList<QList<QJubatusClustering::WeightedDatum> > getCoreMembers();
    Q_INVOKABLE QList<QVariantMap> getKCenter();
    Q_INVOKABLE QVariantMap getNearestCenter(const QVariantMap &point);
    Q_INVOKABLE QList<QJubatusClustering::WeightedDatum> getNearestMembers(const QVariantMap &point);

protected:
    using QJubatusClient::convert;
    QJubatusClustering::WeightedDatum convert(const jubatus::clustering::weighted_datum &data) const;
    QList<QJubatusClustering::WeightedDatum> convert(const std::vector<jubatus::clustering::weighted_datum> &data) const;
    QList<QList<QJubatusClustering::WeightedDatum> > convert(const std::vector<std::vector<jubatus::clustering::weighted_datum> > &data) const;
    jubatus::clustering::indexed_point convert(const QJubatusClustering::IndexedPoint &data) const;
    std::vector<jubatus::clustering::indexed_point> convert(const QList<QJubatusClustering::IndexedPoint> &data) const;

private:
    jubatus::clustering::client::clustering *client();
};

#ifndef QT_NO_DEBUG_STREAM
JUBATUS_EXPORT QDebug &operator<<(QDebug &, const QJubatusClustering::WeightedDatum &);
#endif

#endif // QJUBATUSCLUSTERING_H
