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

#include "qjubatusburst.h"

#include <QtCore/QDebug>

#include <vector>

#include <jubatus/client/burst_client.hpp>

QJubatusBurst::QJubatusBurst(QObject *parent)
    : QJubatusClient(parent)
{
}

int QJubatusBurst::addDocuments(const QList<QJubatusBurst::Document> &data)
{
    bool ret = 0;
    EXEC_JUBATUS_COMMAND( ret = client()->add_documents(convert(data)) );
    return ret;
}

QJubatusBurst::Window QJubatusBurst::getResult(const QString &keyword)
{
    Window ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->get_result(convert(keyword))) );
    return ret;
}

QJubatusBurst::Window QJubatusBurst::getResultAt(const QString &keyword, double pos)
{
    Window ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->get_result_at(convert(keyword), pos)) );
    return ret;
}

QHash<QString, QJubatusBurst::Window> QJubatusBurst::getAllBurstedResults()
{
    QHash<QString, QJubatusBurst::Window> ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->get_all_bursted_results()) );
    return ret;
}

QHash<QString, QJubatusBurst::Window> QJubatusBurst::getAllBurstedResultsAt(double pos)
{
    QHash<QString, QJubatusBurst::Window> ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->get_all_bursted_results_at(pos)) );
    return ret;
}

QList<QJubatusBurst::KeywordWithParams> QJubatusBurst::getAllKeywords()
{
    QList<QJubatusBurst::KeywordWithParams> ret;
    EXEC_JUBATUS_COMMAND( ret = convert(client()->get_all_keywords()) );
    return ret;
}

bool QJubatusBurst::addKeyword(const QJubatusBurst::KeywordWithParams &keyword)
{
    bool ret = false;
    EXEC_JUBATUS_COMMAND( ret = client()->add_keyword(convert(keyword)) );
    return ret;
}

bool QJubatusBurst::removeKeyword(const QString &keyword)
{
    bool ret = false;
    EXEC_JUBATUS_COMMAND( ret = client()->remove_keyword(convert(keyword)) );
    return ret;
}

bool QJubatusBurst::removeAllKeywords()
{
    bool ret = false;
    EXEC_JUBATUS_COMMAND( ret = client()->remove_all_keywords() );
    return ret;
}

bool QJubatusBurst::clear()
{
    bool ret = false;
    EXEC_JUBATUS_COMMAND( ret = client()->clear() );
    return ret;
}

std::vector<jubatus::burst::document> QJubatusBurst::convert(const QList<QJubatusBurst::Document> &data) const
{
    std::vector<jubatus::burst::document> ret;
    foreach (const Document &datum, data) {
        ret.push_back(convert(datum));
    }
    return ret;
}

jubatus::burst::document QJubatusBurst::convert(const Document &data) const
{
    return jubatus::burst::document(data.pos, convert(data.text));
}

QHash<QString, QJubatusBurst::Window> QJubatusBurst::convert(const std::map<std::string, jubatus::burst::window> &data) const
{
    QHash<QString, Window> ret;
    for(const auto &itr : data) {
        ret.insert(convert(itr.first), convert(itr.second));
    }
    return ret;
}

QJubatusBurst::Window QJubatusBurst::convert(const jubatus::burst::window &data) const
{
    QJubatusBurst::Window ret;
    ret.startPos = data.start_pos;
    ret.batches = convert(data.batches);
    return ret;
}

QList<QJubatusBurst::Batch> QJubatusBurst::convert(const std::vector<jubatus::burst::batch> &data) const
{
    QList<QJubatusBurst::Batch> ret;
    for(const jubatus::burst::batch &datum : data) {
        ret.append(convert(datum));
    }
    return ret;
}

QJubatusBurst::Batch QJubatusBurst::convert(const jubatus::burst::batch &data) const
{
    QJubatusBurst::Batch ret;
    ret.allDataCount = data.all_data_count;
    ret.relevantDataCount = data.relevant_data_count;
    ret.burstWeight = data.burst_weight;
    return ret;
}

QJubatusBurst::KeywordWithParams QJubatusBurst::convert(const jubatus::burst::keyword_with_params &data) const
{
    QJubatusBurst::KeywordWithParams ret;
    ret.keyword = convert(data.keyword);
    ret.scalingParam = data.scaling_param;
    ret.gamma = data.gamma;
    return ret;
}
QList<QJubatusBurst::KeywordWithParams> QJubatusBurst::convert(const std::vector<jubatus::burst::keyword_with_params> &data) const
{
    QList<QJubatusBurst::KeywordWithParams> ret;
    for (const jubatus::burst::keyword_with_params &datum : data) {
        ret.append(convert(datum));
    }
    return ret;
}

jubatus::burst::keyword_with_params QJubatusBurst::convert(const QJubatusBurst::KeywordWithParams &data) const
{
    jubatus::burst::keyword_with_params ret;
    ret.keyword = convert(data.keyword);
    ret.scaling_param = data.scalingParam;
    ret.gamma = data.gamma;
    return ret;
}

std::vector<jubatus::burst::keyword_with_params> QJubatusBurst::convert(const QList<QJubatusBurst::KeywordWithParams> &data) const
{
    std::vector<jubatus::burst::keyword_with_params> ret;
    foreach (const KeywordWithParams &datum, data) {
        ret.push_back(convert(datum));
    }
    return ret;
}

jubatus::burst::client::burst *QJubatusBurst::client()
{
    return QJubatusClient::client<jubatus::burst::client::burst>();
}
