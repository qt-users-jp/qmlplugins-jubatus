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

#include "widget.h"
#include "ui_widget.h"

static QVariantMap makeDatum(const Ui::Widget *ui)
{
    QVariantMap ret;
    ret.insert(QStringLiteral("hair"), ui->hair->currentText());
    ret.insert(QStringLiteral("top"), ui->top->currentText());
    ret.insert(QStringLiteral("bottom"), ui->bottom->currentText());
    ret.insert(QStringLiteral("height"), ui->height->value());
    return ret;
}

static QList<QJubatusClassifier::TrainData> makeTrainData(const QString &label, const Ui::Widget *ui)
{
    QList<QJubatusClassifier::TrainData> ret;
    QJubatusClassifier::TrainData data;
    data.first = label;
    data.second = makeDatum(ui);
    ret.append(data);
    return ret;
}

Widget::Widget(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->male, &QPushButton::clicked, [&] {
        classifier.train(QStringLiteral("test"), makeTrainData(QStringLiteral("male"), ui));
    });

    connect(ui->female, &QPushButton::clicked, [&] {
        classifier.train(QStringLiteral("test"), makeTrainData(QStringLiteral("female"), ui));
    });

    connect(ui->other, &QPushButton::clicked, [&] {
        classifier.train(QStringLiteral("test"), makeTrainData(QStringLiteral("other"), ui));
    });

    connect(ui->classify, &QPushButton::clicked, [&] {
        QStringList text;
        QList<QList<QJubatusClassifier::EstimateResult>> results = classifier.classify(QStringLiteral("test"), QList<QVariantMap>() << makeDatum(ui));
        foreach (const QList<QJubatusClassifier::EstimateResult> &result, results) {
            foreach (const QJubatusClassifier::EstimateResult &r, result) {
                text.append(QString("%1\t%2").arg(r.label).arg(r.score));
            }
        }
        ui->result->setPlainText(text.join(QStringLiteral("\n")));
    });
}

Widget::~Widget()
{
    delete ui;
}
