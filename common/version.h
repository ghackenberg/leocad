#pragma once

class Version
{
public:
    static const Version& get()
    {
        return INSTANCE;
    }
    static void set(const Version& version)
    {
        INSTANCE = version;
    }

private:
    static Version INSTANCE;

public:
    Version()
        : major(0), minor(0), patch(-1), empty(true)
    {
        // empty
    }
    Version(const QJsonObject& object);

    inline const QString& getId() const
    {
        return id;
    }

    inline const QString& getUserId() const
    {
        return userId;
    }
    inline const QString& getProductId() const
    {
        return productId;
    }

    inline const QStringList& getBaseVersionIds() const
    {
        return baseVersionIds;
    }

    inline unsigned int getMajor() const
    {
        return major;
    }
    inline unsigned int getMinor() const
    {
        return minor;
    }
    inline unsigned int getPatch() const
    {
        return patch;
    }

    inline const QString& getTime() const
    {
        return time;
    }
    inline const QString& getDescription() const
    {
        return description;
    }

    inline const QString& getModelType() const
    {
        return modelType;
    }
    inline const QString& getImageType() const
    {
        return imageType;
    }

    inline const bool isEmpty() const
    {
        return empty;
    }

    QString toString() const;

private:
    QString id;

    QString userId;
    QString productId;

    QStringList baseVersionIds;

    unsigned int major;
    unsigned int minor;
    unsigned int patch;

    QString time;
    QString description;

    QString modelType;
    QString imageType;

    bool empty;

};
