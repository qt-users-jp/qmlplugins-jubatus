#ifndef QMLJUBATUSCLASSIFIER_H
#define QMLJUBATUSCLASSIFIER_H

#include <QtJubatus/QJubatusClassifier>

class QmlJubatusClassifier : public QJubatusClassifier
{
    Q_OBJECT
public:
    QmlJubatusClassifier(QObject *parent = 0);

    Q_INVOKABLE void train(const QString &label, const QVariantMap &data);
    Q_INVOKABLE QVariantList classify(const QVariantMap &data);

protected:
    using QJubatusClassifier::convert;
    QVariantMap convert(const QJubatusClassifier::EstimateResult &data) const;
    QVariantList convert(const QList<QJubatusClassifier::EstimateResult> &data) const;
};

#endif // QMLJUBATUSCLASSIFIER_H
