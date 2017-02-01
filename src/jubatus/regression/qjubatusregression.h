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

#ifndef QJUBATUSREGRESSION_H
#define QJUBATUSREGRESSION_H

#include "jubatus_global.h"
#include "qjubatusclient.h"

#include <QtCore/QVariant>

namespace jubatus {
    namespace regression {
        struct scored_datum;
        namespace client {
            class regression;
        }
    }
}

class JUBATUS_EXPORT QJubatusRegression : public QJubatusClient
{
    Q_OBJECT
public:
    explicit QJubatusRegression(QObject *parent = nullptr);

    struct ScoredDatum {
        float score;
        QVariantMap data;
    };

    Q_INVOKABLE int train(const QList<ScoredDatum> &data);
    Q_INVOKABLE QList<float> estimate(const QList<QVariantMap> &data);

protected:
    using QJubatusClient::convert;
    jubatus::regression::scored_datum convert(const ScoredDatum &data) const;
    std::vector<jubatus::regression::scored_datum> convert(const QList<ScoredDatum> &data) const;
    QList<float> convert(const std::vector<float> &data) const;

private:
    jubatus::regression::client::regression *client();
};

#endif // QJUBATUSREGRESSION_H
