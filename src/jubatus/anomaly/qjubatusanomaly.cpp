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

#include "qjubatusanomaly.h"

#include <QtCore/QDebug>

#include <vector>

#include <jubatus/client/anomaly_client.hpp>

QJubatusAnomaly::QJubatusAnomaly(QObject *parent)
    : QJubatusClient(parent)
{
}

bool QJubatusAnomaly::clearRow(const QString &id)
{
    bool ret = false;
    EXEC_JUBATUS_COMMAND( ret = client()->clear_row(convert(id)); )
    return ret;
}

QJubatusAnomaly::IdAndScore QJubatusAnomaly::add(const QVariantMap &data)
{
    QJubatusAnomaly::IdAndScore ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->add(convert(data))); )
    return ret;
}

float QJubatusAnomaly::update(const QString &id, const QVariantMap &data)
{
    float ret = 0.0;
    EXEC_JUBATUS_COMMAND( ret = client()->update(convert(id), convert(data)); )
    return ret;
}

float QJubatusAnomaly::overwrite(const QString &id, const QVariantMap &data)
{
    float ret = 0.0;
    EXEC_JUBATUS_COMMAND( client()->overwrite(convert(id), convert(data)); )
    return ret;
}

float QJubatusAnomaly::calcScore(const QVariantMap &data)
{
    float ret = 0.0;
    EXEC_JUBATUS_COMMAND( ret = client()->calc_score(convert(data)); )
    return ret;
}

QStringList QJubatusAnomaly::getAllRows()
{
    QStringList ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->get_all_rows()); )
    return ret;
}

jubatus::anomaly::id_with_score QJubatusAnomaly::convert(const QJubatusAnomaly::IdAndScore &data) const
{
    jubatus::anomaly::id_with_score ret;
    ret.id = convert(data.id);
    ret.score = data.score;
    return ret;
}

QJubatusAnomaly::IdAndScore QJubatusAnomaly::convert(const jubatus::anomaly::id_with_score &data) const
{
    IdAndScore ret;
    ret.id = convert(data.id);
    ret.score = data.score;
    return ret;
}

jubatus::anomaly::client::anomaly *QJubatusAnomaly::client()
{
    return QJubatusClient::client<jubatus::anomaly::client::anomaly>();
}
