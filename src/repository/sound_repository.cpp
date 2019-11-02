#include "sound_repository.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QNetworkReply>
#include <QDebug>

SoundRepository::SoundRepository(QObject *parent)
    : QObject(parent)
    , network_access_(new QNetworkAccessManager(this))
    , ssl_conf_()
    , api_token_()
    , secrets_path_("../src/secret.json")
    , api_base_path_("https://0.0.0.0:5000/api/")
{
    initNetworkAccess();
}

void SoundRepository::getSounds()
{
    QNetworkRequest request;
    request.setUrl(QUrl(api_base_path_+"sound?access_token="+api_token_));
    request.setSslConfiguration(ssl_conf_);
    network_access_->get(request);
}

void SoundRepository::handleReply(QNetworkReply *reply)
{
    if (reply->error()) {
        qDebug() << reply->errorString() << "\n";
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());

    QList<Sound> sounds;
    if(doc.isArray()) {
        for(auto val: doc.array()) {
            if(val.isObject()) {
                sounds.append(Sound::fromJsonObject(val.toObject()));
            }
        }
    }

    if(sounds.size() > 0)
        emit soundsReceived(sounds);
}

void SoundRepository::initNetworkAccess()
{
    loadApiToken();

    connect(network_access_, &QNetworkAccessManager::finished,
            this, &SoundRepository::handleReply);

    ssl_conf_ = QSslConfiguration::defaultConfiguration();
    ssl_conf_.setPeerVerifyMode(QSslSocket::VerifyNone);
}

void SoundRepository::loadApiToken()
{
    QString val;
    QFile file;
    file.setFileName(secrets_path_);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    qDebug() << "content of 'secret.json': " << val;

    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject obj = d.object();
    if(obj.contains("access_token"))
        api_token_ = obj["access_token"].toString();
}
