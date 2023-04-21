#pragma once

class Product
{
public:
    static const Product& get()
    {
        return INSTANCE;
    }
    static void set(const Product& product)
    {
        INSTANCE = product;
    }

private:
    static Product INSTANCE;

public:
    Product()
        : empty(true)
    {
        // empty
    }
    Product(const QJsonObject& object);

    inline const QString& getId() const
    {
        return id;
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
    QString id;
    QString name;
    QString description;

    bool empty;

};
