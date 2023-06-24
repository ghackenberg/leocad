#include "version.h"

QList<Version> Version::INSTANCES;

QList<Version> Version::BASES;

Version::Version(const QJsonObject& object)
    : empty(false)
{
    id = object.value("id").toString();

    userId = object.value("userId").toString();
    productId = object.value("productId").toString();

    QJsonArray array = object.value("baseVersionIds").toArray();
    for (int index = 0; index < array.size(); index++)
    {
        baseVersionIds.append(array[index].toString());
    }

    major = object.value("major").toDouble();
    minor = object.value("minor").toDouble();
    patch = object.value("patch").toDouble();

    time = object.value("time").toString();
    description = object.value("description").toString();

    modelType = object.value("modelType").toString();
    imageType = object.value("imageType").toString();
}

QString Version::toString() const
{
    if (empty)
    {
        return "";
    }
    else
    {
        QString result;
        result.append(QString::number(major));
        result.append(".");
        result.append(QString::number(minor));
        result.append(".");
        result.append(QString::number(patch));
        result.append(" - ");
        result.append(description);

        return result;
    }
}
