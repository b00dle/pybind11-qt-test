#ifndef SOUNDREPOSITORY_H
#define SOUNDREPOSITORY_H

#include <QObject>
#include <QNetworkAccessManager>

#include "http_types.h"

class SoundRepository : public QObject
{
    Q_OBJECT
public:
    explicit SoundRepository(QObject *parent = nullptr);

    void getSounds();

Q_SIGNALS:
    void soundsReceived(const QList<Sound>& sounds);

public Q_SLOTS:

private Q_SLOTS:
    void handleReply(QNetworkReply* reply);

private:
    void initNetworkAccess();
    void loadApiToken();

    QNetworkAccessManager* network_access_;
    QSslConfiguration ssl_conf_;
    QString api_token_;
    QString secrets_path_;
    QString api_base_path_;
};

#endif // SOUNDREPOSITORY_H
