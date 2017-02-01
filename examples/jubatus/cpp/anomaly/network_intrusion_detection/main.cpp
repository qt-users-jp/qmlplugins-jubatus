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
    QJubatusAnomaly anomaly;

    QFile file("kddcup.data_10_percent");
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << file.errorString();
        return;
    }

    QTextStream stream(&file);
    while (!stream.atEnd()) {
        QStringList fields = stream.readLine().split(',');
        QVariantMap params;
        int i = 0;

        params["duration"] = fields.at(i++).toFloat();
        params["protocol_type"] = fields.at(i++);
        params["service"] = fields.at(i++);
        params["flag"] = fields.at(i++);
        params["src_bytes"] = fields.at(i++).toFloat();
        params["dst_bytes"] = fields.at(i++).toFloat();
        params["land"] = fields.at(i++);
        params["wrong_fragment"] = fields.at(i++).toFloat();
        params["urgent"] = fields.at(i++).toFloat();
        params["hot"] = fields.at(i++).toFloat();
        params["num_failed_logins"] = fields.at(i++).toFloat();
        params["logged_in"] = fields.at(i++);
        params["num_compromised"] = fields.at(i++).toFloat();
        params["root_shell"] = fields.at(i++).toFloat();
        params["su_attempted"] = fields.at(i++).toFloat();
        params["num_root"] = fields.at(i++).toFloat();
        params["num_file_creations"] = fields.at(i++).toFloat();
        params["num_shells"] = fields.at(i++).toFloat();
        params["num_access_files"] = fields.at(i++).toFloat();
        params["num_outbound_cmds"] = fields.at(i++).toFloat();
        params["is_host_login"] = fields.at(i++);
        params["is_guest_login"] = fields.at(i++);
        params["count"] = fields.at(i++).toFloat();
        params["srv_count"] = fields.at(i++).toFloat();
        params["serror_rate"] = fields.at(i++).toFloat();
        params["srv_serror_rate"] = fields.at(i++).toFloat();
        params["rerror_rate"] = fields.at(i++).toFloat();
        params["srv_rerror_rate"] = fields.at(i++).toFloat();
        params["same_srv_rate"] = fields.at(i++).toFloat();
        params["diff_srv_rate"] = fields.at(i++).toFloat();
        params["srv_diff_host_rate"] = fields.at(i++).toFloat();
        params["dst_host_count"] = fields.at(i++).toFloat();
        params["dst_host_srv_count"] = fields.at(i++).toFloat();
        params["dst_host_same_srv_rate"] = fields.at(i++).toFloat();
        params["dst_host_diff_srv_rate"] = fields.at(i++).toFloat();
        params["dst_host_same_src_port_rate"] = fields.at(i++).toFloat();
        params["dst_host_srv_diff_host_rate"] = fields.at(i++).toFloat();
        params["dst_host_serror_rate"] = fields.at(i++).toFloat();
        params["dst_host_srv_serror_rate"] = fields.at(i++).toFloat();
        params["dst_host_rerror_rate"] = fields.at(i++).toFloat();
        params["dst_host_srv_rerror_rate"] = fields.at(i++).toFloat();
        QString label = fields.at(i++);
        QJubatusAnomaly::IdAndScore ret = anomaly.add(params);
        if (!qIsInf(ret.score) && !qFuzzyCompare(ret.score, 1.0f)) {
            qDebug() << label << ret.id << ret.score;
        }
    }

    file.close();
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
