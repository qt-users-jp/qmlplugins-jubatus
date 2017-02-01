#ifndef QMLJUBATUSRECOMMENDER_H
#define QMLJUBATUSRECOMMENDER_H

#include <QtJubatus/QJubatusRecommender>

class QmlJubatusRecommender : public QJubatusRecommender
{
    Q_OBJECT
public:
    explicit QmlJubatusRecommender(QObject *parent = nullptr);

    Q_INVOKABLE QVariantList similarRowFromId(const QString &id, uint size);
    Q_INVOKABLE QVariantList similarRowFromDatum(const QVariantMap &data, uint size);

protected:
    QVariantMap convert(const IdWithScore &data) const;
    QVariantList convert(const QList<IdWithScore> &data) const;
};

#endif // QMLJUBATUSRECOMMENDER_H
