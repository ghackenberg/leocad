#include "user.h"

User::User(const QJsonObject& object)
{
    userId = object.value("userId").toString();

    pictureId = object.value("pictureId").toString();

    name = object.value("name").toString();
    email = object.value("email").toString();
}

QString User::toString() const
{
    QString result;
    result.append(name);
    result.append(" <");
    result.append(email);
    result.append(">");

    return result;
}
