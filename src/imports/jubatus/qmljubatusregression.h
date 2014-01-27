#ifndef QMLJUBATUSREGRESSION_H
#define QMLJUBATUSREGRESSION_H

#include <QtJubatus/QJubatusRegression>

class QmlJubatusRegression : public QJubatusRegression
{
    Q_OBJECT
public:
    explicit QmlJubatusRegression(QObject *parent = 0);

    Q_INVOKABLE bool train(const QVariantMap &data, float score);
    Q_INVOKABLE float estimate(const QVariantMap &data);
};

#endif // QMLJUBATUSREGRESSION_H
