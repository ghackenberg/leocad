#pragma once

class User
{
public:
    User(const QJsonObject& object);

    inline const QString& getId() const
    {
        return id;
    }
    inline const QString& getName() const
    {
        return name;
    }
    inline const QString& getEmail() const
    {
        return email;
    }
    inline const QString& getPictureId() const
    {
        return pictureId;
    }

    QString toString() const;

private:
    QString id;
    QString name;
    QString email;
    QString pictureId;
};
