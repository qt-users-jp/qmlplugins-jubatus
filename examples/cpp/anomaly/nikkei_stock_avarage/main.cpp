#include <QtCore>
#include <QtNetwork>
#include <QtWidgets>
#include <QtJubatus>

class Graph : public QWidget
{
    Q_OBJECT
public:
    Graph(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *e);

private:
    QNetworkAccessManager manager;
    struct Data {
        Data() : first(0), last(0), score(0) {}
        float first;
        float last;
        float score;
    };

    QList<Data> data;
};

Graph::Graph(QWidget *parent)
    : QWidget(parent)
{
    QObject::connect(&manager, &QNetworkAccessManager::finished, [this](QNetworkReply *reply) {
        QJubatusAnomaly anomaly;
        bool hasError = false;
        connect(&anomaly, &QJubatusAnomaly::error, [&hasError](const QString &message) { hasError = true; });

        QStringList lines = QTextCodec::codecForName("Shift-JIS")->toUnicode(reply->readAll()).split(QStringLiteral("\r\n"));
        foreach (const QString &line, lines) {
            QStringList columns = line.split(QStringLiteral(","));
            if (columns.length() != 5) continue;
            bool ok;
            QString first = columns.at(2);
            QString last = columns.at(1);
            Data d;
            d.first = first.replace("\"", "").toFloat(&ok);
            d.last = last.replace("\"", "").toFloat(&ok);
            if (ok) {
                d.first = d.first / 20000;
                d.last = d.last / 20000;
                QVariantMap params;
                params.insert("first", d.first * 100);
                params.insert("last", d.last * 100);
                params.insert("diff", (d.last - d.first) * 100);
                if (!hasError) {
                    QJubatusAnomaly::IdAndScore ret = anomaly.add(params);
                    d.score = ret.score;
                }
                data.append(d);
            }
        }
        reply->deleteLater();
        update();
    });

    // http://indexes.nikkei.co.jp/nkave/index
    manager.get(QNetworkRequest(QUrl("http://indexes.nikkei.co.jp/nkave/historical/nikkei_stock_average_daily_jp.csv")));
}

void Graph::paintEvent(QPaintEvent *e)
{
    if (data.isEmpty()) return;

    QPainter p(this);

    float x = 0;
    float dx = (float)width() / data.count();
    float dy = height();

    foreach (const Data &d, data) {
        p.fillRect(QRectF(x, (1.0 - qMax(d.first, d.last)) * dy, dx, qAbs(d.last - d.first) * dy), QColor(qIsInf(d.score) ? Qt::red : Qt::black));
        x += dx;
    }
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Graph graph;
    graph.show();

    return app.exec();
}

#include "main.moc"
