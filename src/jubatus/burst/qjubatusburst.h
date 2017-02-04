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

#ifndef QJUBATUSBURST_H
#define QJUBATUSBURST_H

#include "jubatus_global.h"
#include "qjubatusclient.h"

#include <QtCore/QVariant>

namespace jubatus {
    namespace burst {
        struct keyword_with_params;
        struct batch;
        struct window;
        struct document;
        namespace client {
            class burst;
        }
    }
}

class JUBATUS_EXPORT QJubatusBurst : public QJubatusClient
{
    Q_OBJECT
public:
    explicit QJubatusBurst(QObject *parent = nullptr);

    struct KeywordWithParams {
        KeywordWithParams() : scalingParam(0.0), gamma(0.0) {}
        QString keyword;
        double scalingParam;
        double gamma;
    };

    struct Batch {
        Batch() : allDataCount(0), relevantDataCount(0), burstWeight(0.0) {}
        qint32 allDataCount;
        qint32 relevantDataCount;
        double burstWeight;
    };

    struct Window {
        Window() : startPos(0.0) {}
        double startPos;
        QList<Batch> batches;
    };

    struct Document {
        Document() : pos(0.0) {}
        double pos;
        QString text;
    };

    int addDocuments(const QList<Document> &data);
    Window getResult(const QString &keyword);
    Window getResultAt(const QString &keyword, double pos);
    QHash<QString, Window> getAllBurstedResults();
    QHash<QString, Window> getAllBurstedResultsAt(double pos);
    QList<KeywordWithParams> getAllKeywords();
    bool addKeyword(const KeywordWithParams &keyword);
    bool removeKeyword(const QString &keyword);
    bool removeAllKeywords();
    bool clear();

protected:
    using QJubatusClient::convert;
    std::vector<jubatus::burst::document> convert(const QList<Document> &data) const;
    jubatus::burst::document convert(const Document &data) const;
    QHash<QString, Window> convert(const std::map<std::string, jubatus::burst::window> &data) const;
    Window convert(const jubatus::burst::window &data) const;
    QList<Batch> convert(const std::vector<jubatus::burst::batch> &data) const;
    Batch convert(const jubatus::burst::batch &data) const;
    KeywordWithParams convert(const jubatus::burst::keyword_with_params &data) const;
    QList<KeywordWithParams> convert(const std::vector<jubatus::burst::keyword_with_params> &data) const;
    jubatus::burst::keyword_with_params convert(const KeywordWithParams &data) const;
    std::vector<jubatus::burst::keyword_with_params> convert(const QList<KeywordWithParams> &data) const;

private:
    jubatus::burst::client::burst *client();
};

#endif // QJUBATUSBURST_H
