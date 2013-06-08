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

#include <QtQml/QQmlExtensionPlugin>
#include <QtQml/qqml.h>

#include <QtJubatus/QJubatusClassifier>

#include <string>
#include <vector>

#include <jubatus/client.hpp>

using std::make_pair;
using std::pair;
using std::string;
using std::vector;
using jubatus::classifier::datum;
using jubatus::classifier::estimate_result;

class QmlJubatusClassifier : public QJubatusClassifier
{
    Q_OBJECT
    Q_PROPERTY(QString name MEMBER m_name)
public:
    QmlJubatusClassifier(QObject *parent = 0) : QJubatusClassifier(parent) {}

    using QJubatusClassifier::train;
    Q_INVOKABLE void train(const QVariantList &data);
    Q_INVOKABLE void train(const QString &label, const QVariantMap &data);
    using QJubatusClassifier::classify;
    Q_INVOKABLE QVariantList classify(const QVariantList &data);
    Q_INVOKABLE QVariantList classify(const QVariantMap &data);

    QString m_name;
};

void QmlJubatusClassifier::train(const QVariantList &data)
{
    vector<pair<string, datum>> train_data;
    foreach (const QVariant &v, data) {
        switch (v.type()) {
        case QVariant::List: {
            QVariantList list = v.toList();
            if (!list.isEmpty()) {
                QString name = list.takeFirst().toString();
                foreach (const QVariant &v2, list) {
                    train_data.push_back(make_pair(name.toStdString(), convert(v2.toMap())));
                }
            }
            break; }
        default:
            qDebug() << Q_FUNC_INFO << __LINE__ << v.type();
            break;
        }
    }
    train(m_name.toStdString(), train_data);
}

void QmlJubatusClassifier::train(const QString &label, const QVariantMap &data)
{
    vector<pair<string, datum>> train_data;
    train_data.push_back(make_pair(label.toStdString(), convert(data)));
    train(m_name.toStdString(), train_data);
}

QVariantList QmlJubatusClassifier::classify(const QVariantList &data)
{
    QVariantList ret;

    vector<datum> test_data;
    foreach (const QVariant &v, data) {
        test_data.push_back(convert(v.toMap()));
    }

    vector<vector<estimate_result> > results = classify(m_name.toStdString(), test_data);

    for (size_t i = 0; i < results.size(); ++i) {
        QVariantList list;
        for (size_t j = 0; j < results[i].size(); ++j) {
            const estimate_result& jr = results[i][j];
            QVariantMap qr;
            qr.insert(QStringLiteral("label"), QString::fromStdString(jr.label));
            qr.insert(QStringLiteral("score"), jr.score);
            list.append(qr);
        }
        ret.append(QVariant(list));
    }
    return ret;
}

QVariantList QmlJubatusClassifier::classify(const QVariantMap &data)
{
    QVariantList ret;

    vector<datum> test_data;
    test_data.push_back(convert(data));

    vector<vector<estimate_result> > results = classify(m_name.toStdString(), test_data);

    for (size_t i = 0; i < results[0].size(); ++i) {
        const estimate_result& jr = results[0][i];
        QVariantMap qr;
        qr.insert(QStringLiteral("label"), QString::fromStdString(jr.label));
        qr.insert(QStringLiteral("score"), jr.score);
        ret.append(qr);
    }
    return ret;
}

class QmlJubatusPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    virtual void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("QtJubatus"));
        // @uri QtFluentd
        qmlRegisterType<QmlJubatusClassifier>(uri, 0, 1, "Classifier");
    }
};

#include "main.moc"
