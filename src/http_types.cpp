#include "http_types.h"

Resource::Resource()
    : name()
    , type()
    , uuid()
{}

Resource::Resource(const QString &name, int type, const QString &uuid)
    : name(name)
    , type(type)
    , uuid(uuid)
{}

const Resource Resource::fromJsonObject(const QJsonObject &obj)
{
    QString name = "", uuid = "";
    int type = 0;
    if(obj.contains("name"))
        name = obj["name"].toString();
    if(obj.contains("uuid"))
        uuid = obj["uuid"].toString();
    if(obj.contains("type"))
        type = obj["type"].toInt();
    return Resource(name, type, uuid);
}

Sound::Sound(const QString &local_path, const QString &uuid)
    : resource()
    , local_path(local_path)
    , uuid(uuid)
{}

const Sound Sound::fromJsonObject(const QJsonObject &obj)
{
    QString local_path = "", uuid = "";
    if(obj.contains("local_path"))
        local_path = obj["local_path"].toString();
    if(obj.contains("uuid"))
        uuid = obj["uuid"].toString();
    Sound s(local_path, uuid);
    if(obj.contains("resource"))
        s.resource = Resource::fromJsonObject(obj["resource"].toObject());
    return s;
}
