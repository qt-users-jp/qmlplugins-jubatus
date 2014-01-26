#include "qmljubatusrecommender.h"

QmlJubatusRecommender::QmlJubatusRecommender(QObject *parent) :
    QJubatusRecommender(parent)
{
}

QVariantList QmlJubatusRecommender::similarRowFromId(const QString &id, uint size)
{
    return convert(QJubatusRecommender::similarRowFromId(id, size));
}

QVariantList QmlJubatusRecommender::similarRowFromDatum(const QVariantMap &data, uint size)
{
    return convert(QJubatusRecommender::similarRowFromDatum(data, size));
}

QVariantMap QmlJubatusRecommender::convert(const QJubatusRecommender::IdWithScore &data) const
{
    QVariantMap ret;
    ret.insert(QStringLiteral("id"), data.id);
    ret.insert(QStringLiteral("score"), data.score);
    return ret;
}

QVariantList QmlJubatusRecommender::convert(const QList<QJubatusRecommender::IdWithScore> &data) const
{
    QVariantList ret;
    foreach (const IdWithScore &idWithScore, data) {
        ret.append(convert(idWithScore));
    }
    return ret;
}
