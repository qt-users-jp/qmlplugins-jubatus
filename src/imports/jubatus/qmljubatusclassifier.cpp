#include "qmljubatusclassifier.h"

QmlJubatusClassifier::QmlJubatusClassifier(QObject *parent) :
    QJubatusClassifier(parent)
{
}

void QmlJubatusClassifier::train(const QString &label, const QVariantMap &data)
{
    LabeledDatum datum;
    datum.label = label;
    datum.data = data;
    QJubatusClassifier::train(QList<QJubatusClassifier::LabeledDatum>() << datum);
}

QVariantList QmlJubatusClassifier::classify(const QVariantMap &data)
{
    QVariantList ret;
    QList<QList<QJubatusClassifier::EstimateResult> > result = QJubatusClassifier::classify(QList<QVariantMap>() << data);
    if (!result.isEmpty())
        ret = convert(result.first());
    return ret;
}

QVariantMap QmlJubatusClassifier::convert(const QJubatusClassifier::EstimateResult &data) const
{
    QVariantMap ret;
    ret.insert(QStringLiteral("label"), data.label);
    ret.insert(QStringLiteral("score"), data.score);
    return ret;
}

QVariantList QmlJubatusClassifier::convert(const QList<QJubatusClassifier::EstimateResult> &data) const
{
    QVariantList ret;
    foreach (const QJubatusClassifier::EstimateResult &datum, data) {
        ret.append(convert(datum));
    }
    return ret;
}
