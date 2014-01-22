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

#ifndef QJUBATUSCLASSIFIER_H
#define QJUBATUSCLASSIFIER_H

#include <QtCore/QObject>
#include <QtCore/QVariant>

#include "jubatus_global.h"

namespace jubatus {
    namespace client {
        namespace common {
            struct datum;
        }
    }
    namespace classifier {
        struct estimate_result;
        struct labeled_datum;
    }
}

class JUBATUS_EXPORT QJubatusClassifier : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(double timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged)
public:
    explicit QJubatusClassifier(QObject *parent = 0);

    const QString &host() const;
    int port() const;
    const QString &name() const;
    double timeout() const;

    struct EstimateResult {
        QString label;
        double score;
    };

    typedef QPair<QString, QVariantMap> TrainData;

    void train(const QList<QJubatusClassifier::TrainData> &data);
    QList<QList<QJubatusClassifier::EstimateResult>> classify(const QList<QVariantMap> &data);

public slots:
    void setHost(const QString &host);
    void setPort(int port);
    void setName(const QString &name);
    void setTimeout(double timeout);

signals:
    void hostChanged(const QString &host);
    void portChanged(int port);
    void nameChanged(const QString &name);
    void timeoutChanged(double timeout);

protected:
    void train(const std::vector<jubatus::classifier::labeled_datum> &data);
    std::vector<std::vector<jubatus::classifier::estimate_result>> classify(const std::vector<jubatus::client::common::datum> &data);
    static jubatus::client::common::datum convert(const QVariantMap &data);

private:
    class Private;
    Private *d;
};

#endif // QJUBATUSCLASSIFIER_H
