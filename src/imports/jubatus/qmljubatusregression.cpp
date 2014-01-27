#include "qmljubatusregression.h"

QmlJubatusRegression::QmlJubatusRegression(QObject *parent) :
    QJubatusRegression(parent)
{
}
bool QmlJubatusRegression::train(const QVariantMap &data, float score)
{
    QJubatusRegression::ScoredDatum datum;
    datum.data = data;
    datum.score = score;
    return QJubatusRegression::train(QList<ScoredDatum>() << datum) == 1;
}

float QmlJubatusRegression::estimate(const QVariantMap &data)
{
    float ret = 0.0;
    QList<float> results = QJubatusRegression::estimate(QList<QVariantMap>() << data);
    if (!results.isEmpty()) {
        ret = results.first();
    }
    return ret;
}
