#pragma once

class Version
{
public:
    static QList<Version> INSTANCES;

public:
    Version()
        : major(0), minor(0), patch(-1), empty(true)
    {
        // empty
    }
    Version(const QJsonObject& object);

    inline const QString& getProductId() const
    {
        return productId;
    }
    inline const QString& getVersionId() const
    {
        return versionId;
    }

    inline const QString& getUserId() const
    {
        return userId;
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

    inline QString getNumber() const
    {
        QString a = QString::number(major);
        QString b = QString::number(minor);
        QString c = QString::number(patch);
        return a.append(".").append(b).append(".").append(c);
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

    inline bool isEmpty() const
    {
        return empty;
    }

    QString toString() const;

private:
    QString productId;
    QString versionId;

    QString userId;

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
