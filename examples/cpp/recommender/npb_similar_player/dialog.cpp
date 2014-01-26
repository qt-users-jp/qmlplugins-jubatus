#include "dialog.h"
#include "ui_dialog.h"

#include <QtCore>
#include <QtJubatus>

class Dialog::Private
{
public:
    Private(Dialog *parent);

private:
    void playerChanged(const QString &id);

private:
    Dialog *q;
    Ui::Dialog ui;
    QJubatusRecommender recommender;
};

Dialog::Private::Private(Dialog *parent)
    : q(parent)
{
    ui.setupUi(q);

    recommender.setName("npb-similar-player");
    recommender.setTimeout(10);

    bool learn = recommender.getAllRows().isEmpty();

    QFile file(":/baseball.csv");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    QStringList fields = QStringList() << "チーム" << "打率" << "試合数" << "打席" << "打数" << "安打" << "本塁打" << "打点" << "盗塁" << "四球" << "死球" << "三振" << "犠打" << "併殺打" << "長打率" << "出塁率" << "OPS" << "RC27" << "XR27";

    while (!stream.atEnd()) {
        QStringList list = stream.readLine().split(",");
        QString id = list.takeFirst();
        if (learn) {
            QVariantMap data;
            data.insert(fields.at(0), list.at(0));
            for(int i = 1; i < fields.length(); i++) {
                data.insert(fields.at(i), list.at(i).toFloat());
            }
            recommender.updateRow(id, data);
        }
        ui.player->addItem(id);
    }
    file.close();

    connect(ui.buttonBox, &QDialogButtonBox::rejected, q, &QDialog::close);

    connect(ui.player, &QComboBox::currentTextChanged, [this](const QString &id) {
        playerChanged(id);
    });
    playerChanged(ui.player->currentText());
}

void Dialog::Private::playerChanged(const QString &id)
{
    QStringList results;
    QList<QJubatusRecommender::IdWithScore> list = recommender.similarRowFromId(id, 10);
    foreach (const QJubatusRecommender::IdWithScore idWithScore, list) {
        if (idWithScore.id == id) continue;
        results.append(QString("%1 (%2)").arg(idWithScore.id).arg(idWithScore.score));
    }
    ui.result->setPlainText(results.join(QStringLiteral("\n")));
}

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , d(new Private(this))
{
    connect(this, &QObject::destroyed, [this]() { delete d; });
}
