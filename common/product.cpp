#include "product.h"

Product Product::INSTANCE;

Product::Product(const QJsonObject& object)
    : empty(false)
{
    id = object.value("id").toString();
    userId = object.value("userId").toString();
    name = object.value("name").toString();
    description = object.value("description").toString();
}

QString Product::toString() const
{
    if (empty)
    {
        return "";
    }
    else
    {
        QString result;
        result.append(name);
        result.append(" - ");
        result.append(description);

        return result;
    }
}
