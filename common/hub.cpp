#include "hub.h"

Hub Hub::INSTANCE;

Hub::Hub()
    : scheme("https"), host("caddrive.com"), port(443), token("")
{

}

QString Hub::toString() const
{
    QString result;
    result.append(scheme);
    result.append("://");
    result.append(host);
    result.append(":");
    result.append(QString::number(port));
    result.append("/");

    return result;
}
