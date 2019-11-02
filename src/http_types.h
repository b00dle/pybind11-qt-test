#ifndef HTTP_TYPES_H
#define HTTP_TYPES_H

#include <QString>
#include <QJsonObject>

struct Resource {
    Resource();
    Resource(const QString& name, int type, const QString& uuid);

    static const Resource fromJsonObject(const QJsonObject& obj);

    QString name;
    int type;
    QString uuid;
};

struct Sound {
    Sound(const QString& local_path, const QString& uuid);

    static const Sound fromJsonObject(const QJsonObject& obj);

    Resource resource;
    QString local_path;
    QString uuid;
};

#endif // HTTP_TYPES_H
