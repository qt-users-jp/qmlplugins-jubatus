#include <QtCore>
#include <QtJubatus>

void add(QJubatusBurst *burst, int pos, int burstCount, int nonburstCount)
{
    QList<QJubatusBurst::Document> documents;
    documents.reserve(burstCount + nonburstCount);
    for (int i = 0; i < burstCount; i++) {
        QJubatusBurst::Document document;
        document.pos = pos;
        document.text = QString::fromUtf8("バルス!!");
        documents.append(document);
    }
    for (int i = 0; i < nonburstCount; i++) {
        QJubatusBurst::Document document;
        document.pos = pos;
        document.text = QString::fromUtf8("ユバタス");
        documents.append(document);
    }
    burst->addDocuments(documents);
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    QJubatusBurst burst;

    QJubatusBurst::KeywordWithParams kWP;
    kWP.keyword = QString::fromUtf8("バルス");
    kWP.scalingParam = 1.001;
    kWP.gamma = 0.1;
    burst.addKeyword(kWP);

    add(&burst,   1,      5,        30);
    add(&burst,  11,     15,        50);
    add(&burst,  21,    500,        10);
    add(&burst,  31,   2000,        10);
    add(&burst,  41,  22222,        40); // バルスの高まり
    add(&burst,  51,     10,        10);
    add(&burst,  61,      5,        25);

    for (const QJubatusBurst::Batch &batch : burst.getResult(QString::fromUtf8("バルス")).batches) {
        qDebug() << batch.allDataCount << batch.relevantDataCount << batch.burstWeight;
    }

    return 0;
}
