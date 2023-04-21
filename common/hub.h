#pragma once

class Hub
{
public:
    inline static Hub& get()
    {
        return INSTANCE;
    }

private:
    static Hub INSTANCE;

public:
    inline void setScheme(const QString& scheme)
    {
        this->scheme = scheme;
    }
    inline const QString& getScheme() const
    {
        return scheme;
    }

    inline void setHost(const QString& host)
    {
        this->host = host;
    }
    inline const QString& getHost() const
    {
        return host;
    }

    inline void setPort(unsigned short port)
    {
        this->port = port;
    }
    inline unsigned short getPort() const
    {
        return port;
    }

    inline void setToken(const QString& token)
    {
        this->token = token;
    }
    inline const QString& getToken() const
    {
        return token;
    }

    QString toString() const;

private:
    Hub();

private:
    QString scheme;
    QString host;
    unsigned short port;
    QString token;

};
