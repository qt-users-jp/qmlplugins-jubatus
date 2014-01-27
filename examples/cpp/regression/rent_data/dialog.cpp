#include "dialog.h"
#include "ui_dialog.h"

#include <QtCore>
#include <QtWidgets>
#include <QtJubatus>

class Dialog::Private
{
public:
    Private(Dialog *parent);

private:
    Dialog *q;
    Ui::Dialog ui;
    QJubatusRegression regression;
};

Dialog::Private::Private(Dialog *parent)
    : q(parent)
{
    ui.setupUi(q);

    regression.setName("rent_data");
    regression.setTimeout(10);
    connect(&regression, &QJubatusRegression::error, [this](const QString &message) {
        QMessageBox::warning(q, tr("Error"), message);
        QMetaObject::invokeMethod(q, "close", Qt::QueuedConnection);
    });

    QFile file(":/rent-data.csv");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    QList<QJubatusRegression::ScoredDatum> scoredData;
    while (!stream.atEnd()) {
        QStringList list = stream.readLine().split(",");
        if (list.size() != 6)
            continue;
        QVariantMap data;
        float rent = list.takeFirst().toFloat();
        data.insert(QStringLiteral("distance"), list.takeFirst().toFloat());
        data.insert(QStringLiteral("size"), list.takeFirst().toFloat());
        data.insert(QStringLiteral("age"), list.takeFirst().toFloat());
        data.insert(QStringLiteral("floor"), list.takeFirst().toFloat());
        data.insert(QStringLiteral("direction"), list.takeFirst());
        QJubatusRegression::ScoredDatum scoredDatum;
        scoredDatum.score = rent;
        scoredDatum.data = data;
        scoredData.append(scoredDatum);
    }
    file.close();
    regression.train(scoredData);

    connect(ui.buttonBox, &QDialogButtonBox::rejected, q, &QDialog::close);
    connect(ui.estimate, &QPushButton::clicked, [this]() {
        QVariantMap data;
        data.insert(QStringLiteral("distance"), ui.distance->value());
        data.insert(QStringLiteral("size"), ui.size->value());
        data.insert(QStringLiteral("age"), ui.age->value());
        data.insert(QStringLiteral("floor"), ui.floor->value());
        data.insert(QStringLiteral("direction"), ui.direction->currentText());
        QList<float> ret = regression.estimate(QList<QVariantMap>() << data);
        QMessageBox::information(q, tr("estimated rent"), tr("%1 JPY/Month").arg(ret.first() * 10000));
    });
}

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , d(new Private(this))
{
    connect(this, &QObject::destroyed, [this]() { delete d; });
}
