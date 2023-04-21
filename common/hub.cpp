#include "hub.h"

Hub Hub::INSTANCE;

Hub::Hub()
    : scheme("http"), host("localhost"), port(3000), token("eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCIsImtpZCI6InZLVFRHN2ZMSVJ4ZkdLekdfbk9CbCJ9.eyJpc3MiOiJodHRwczovL3Byb2R1Y3Rib2FyZC5ldS5hdXRoMC5jb20vIiwic3ViIjoiYXV0aDB8NjNiYzc0OGIxMjMyNDUzYmYwZGI3ODI2IiwiYXVkIjpbImh0dHBzOi8vcHJvZHVjdGJvYXJkLmlvdC5maC13ZWxzLmF0LyIsImh0dHBzOi8vcHJvZHVjdGJvYXJkLmV1LmF1dGgwLmNvbS91c2VyaW5mbyJdLCJpYXQiOjE2ODIwMDMyODcsImV4cCI6MTY4MjA4OTY4NywiYXpwIjoiVktVN2IzWHR3Mk1xYWtPQ2lhVUJOV1ZXa25ERnhCM24iLCJzY29wZSI6Im9wZW5pZCBwcm9maWxlIGVtYWlsIiwicGVybWlzc2lvbnMiOlsiY3JlYXRlOnByb2R1Y3RzIiwiY3JlYXRlOnVzZXJzIl19.BFU-LG-ZfMMQeR0tgmdV6TN3RYVf946hhRBp00gLpAPulvfLenkAuc6YNP5rCIzliUfNDK3nC6l0eJRMrE_v2QMI9i8pvI06h1dPGD5XVFNcocUMBdXTiMSgtjzzzDSa1XUB9Un3DN3ReJ8wKpd5nPgawmQ-Om-ajNp4ug7ByBW6rM43aXIwYVw5LCNP_oukxaufDi0UcWEHMdMFbG7dzMUTxOYT9uZWJFDdML6NoIjkauSZXNbnu7wi-xlpkVWS5hPpqUomBdKW5iKA1l3tGAEBGLoDtm4TPDSqR2XxTJt1Bp5pPQdU2JW-fa-PZ8PLMIDgAP9tFezDn-Uf4wPTxQ")
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
