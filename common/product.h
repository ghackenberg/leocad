#pragma once

class Product
{
public:
    static Product INSTANCE;

public:
    Product()
        : empty(true)
    {
        // empty
    }
    Product(const QJsonObject& object);

    inline const QString& getProductId() const
    {
        return productId;
    }

    inline const QString& getUserId() const
    {
        return userId;
    }

    inline const QString& getName() const
    {
        return name;
    }
    inline const QString& getDescription() const
    {
        return description;
    }

    inline bool isEmpty() const
    {
        return empty;
    }

    QString toString() const;

private:
    QString productId;

    QString userId;

    QString name;
    QString description;

    bool empty;

};
