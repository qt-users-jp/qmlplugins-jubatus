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

#include "qjubatusregression.h"

#include <QtCore/QDebug>

#include <jubatus/client/regression_client.hpp>

QJubatusRegression::QJubatusRegression(QObject *parent)
    : QJubatusClient(parent)
{
}

int QJubatusRegression::train(const QList<ScoredDatum> &data)
{
    int ret = 0;
    EXEC_JUBATUS_COMMAND( ret = client()->train(convert(data)); )
    return ret;
}

QList<float> QJubatusRegression::estimate(const QList<QVariantMap> &data)
{
    QList<float> ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->estimate(convert(data))); )
    return ret;
}

jubatus::regression::scored_datum QJubatusRegression::convert(const QJubatusRegression::ScoredDatum &data) const
{
    jubatus::regression::scored_datum ret;
    ret.score = data.score;
    ret.data = convert(data.data);
    return ret;
}

std::vector<jubatus::regression::scored_datum> QJubatusRegression::convert(const QList<QJubatusRegression::ScoredDatum> &data) const
{
    std::vector<jubatus::regression::scored_datum> ret;
    foreach (const QJubatusRegression::ScoredDatum &datum, data) {
        ret.push_back(convert(datum));
    }
    return ret;
}

QList<float> QJubatusRegression::convert(const std::vector<float> &data) const
{
    QList<float> ret;
    for (auto it = data.begin(); it != data.end(); ++it) {
        ret.append(*it);
    }
    return ret;
}

jubatus::regression::client::regression *QJubatusRegression::client() {
    return QJubatusClient::client<jubatus::regression::client::regression>();
}
