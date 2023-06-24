#include "hub.h"

Hub Hub::INSTANCE;

Hub::Hub()
    : scheme("https"), host("caddrive.com"), port(443), token("")
{
    load();
}

void Hub::load()
{
    QString name("hub.json");

    QFile file(name);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString json = file.readAll();

        QJsonDocument document = QJsonDocument::fromJson(json.toUtf8());

        if (document.isObject())
        {
            QJsonObject object = document.object();

            QJsonValue scheme = object["scheme"];
            if (scheme.isString())
            {
                this->scheme = scheme.toString();
            }

            QJsonValue host = object["host"];
            if (host.isString())
            {
                this->host = host.toString();
            }

            QJsonValue port = object["port"];
            if (port.isDouble())
            {
                this->port = port.toInt();
            }

            QJsonValue token = object["token"];
            if (token.isString())
            {
                this->token = token.toString();
            }
        }

        file.close();
    }
}

void Hub::save() const
{
    QString name("hub.json");

    QFile file(name);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QJsonObject object;

        object["scheme"] = scheme;
        object["host"] = host;
        object["port"] = port;
        object["token"] = token;

        QJsonDocument document(object);

        file.write(document.toJson());

        file.close();
    }
}

QString Hub::toString() const
{
    QString result;
    result.append(scheme);
    result.append("://");
    result.append(host);
    result.append(":");
    result.append(QString::number(port));
    result.append("/");

    return result;
}
