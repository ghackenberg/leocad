#pragma once

class User
{
public:
    User(const QJsonObject& object);

    inline const QString& getUserId() const
    {
        return userId;
    }

    inline const QString& getPictureId() const
    {
        return pictureId;
    }

    inline const QString& getName() const
    {
        return name;
    }
    inline const QString& getEmail() const
    {
        return email;
    }

    QString toString() const;

private:
    QString userId;

    QString pictureId;

    QString name;
    QString email;
};
