#include <QtCore>
#include <QtNetwork>
#include <QtJubatus>

class SimpleCSV : public QList<QStringList>
{
public:
    SimpleCSV(const QString &source);
};


SimpleCSV::SimpleCSV(const QString &source)
{
    QLatin1String crlf("\n");
    QLatin1Char quote('"');
    QLatin1Char delimiter(',');

    // 1. fix record
    QStringList records;
    QStringList lines = source.split(crlf);
    bool next = false;
    for (int i = 0; i < lines.length(); i++) {
        const QString &line = lines.at(i);
        if (next) {
            records.last().append(crlf);
            records.last().append(line);
            next = (line.count(quote) % 2 == 0);
        } else {
            records.append(line);
            next = (line.count(quote) % 2 == 1);
        }
    }

    // 2. fix fields
    for (const QString &record : records) {
//        qDebug() << record;
        QString r = record;
        QStringList strings = r.replace(QStringLiteral("\"\""), QStringLiteral("\b")).split(delimiter);
        QStringList fields;
        bool next = false;
        for (int i = 0; i < strings.length(); i++) {
            QString string = strings.at(i);
            if (next) {
                fields.last().append(delimiter);
                fields.last().append(string);
                next = !string.endsWith(quote);
            } else {
                if (string == QStringLiteral("\b")) {
                    fields.append(QStringLiteral(""));
                } else if (string.startsWith(quote)) {
                    fields.append(string);
                    next = !string.endsWith(quote);
                } else {
                    fields.append(string);
                }
            }
        }

        QStringList data;
        for (QString field : fields) {
            field.replace(QStringLiteral("\b"), QStringLiteral("\""));
            if (field.startsWith(quote) && field.endsWith(quote))
                field = field.mid(1, field.length() - 2);
            data.append(field);
        }
//        qDebug() << data;
        append(data);
    }
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    QNetworkAccessManager nam;
    QNetworkRequest request(QUrl("https://archive.ics.uci.edu/ml/machine-learning-databases/00307/%5bUCI%5d%20AAAI-14%20Accepted%20Papers%20-%20Papers.csv"));
    request.setAttribute(QNetworkRequest::SynchronousRequestAttribute, true);
    QNetworkReply *reply = nam.get(request);

    SimpleCSV csv(QString::fromUtf8(reply->readAll()));
    csv.takeFirst();

    QJubatusNearestNeighbor nn;
    nn.clear();

    int i = 0;
    for (const QStringList &fields : csv) {
        QString id = QString::number(i++);
        QVariantMap data;
        data.insert("title", fields.at(0));
//        data.insert("abstract", fields.at(5));
        nn.setRow(id, data);
    }

    for (const QJubatusNearestNeighbor::IdWithScore &iws : nn.similarRowFromId("1", 4)) {
        qDebug() << csv.at(iws.id.toInt()).at(0) << iws.score;
    }

    return 0;
}
