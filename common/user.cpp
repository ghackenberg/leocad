#include "user.h"

User::User(const QJsonObject& object)
{
    id = object.value("id").toString();
    name = object.value("name").toString();
    email = object.value("email").toString();
    pictureId = object.value("pictureId").toString();
}

QString User::toString() const
{
    QString result;
    result.append(id);
    result.append(" - ");
    result.append(name);
    result.append(" <");
    result.append(email);
    result.append(">");

    return result;
}
