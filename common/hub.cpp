#include "hub.h"

Hub Hub::INSTANCE;

Hub::Hub()
    : scheme("http"), host("localhost"), port(3000), token("eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCIsImtpZCI6InZLVFRHN2ZMSVJ4ZkdLekdfbk9CbCJ9.eyJpc3MiOiJodHRwczovL3Byb2R1Y3Rib2FyZC5ldS5hdXRoMC5jb20vIiwic3ViIjoiYXV0aDB8NjNiYzc0OGIxMjMyNDUzYmYwZGI3ODI2IiwiYXVkIjpbImh0dHBzOi8vcHJvZHVjdGJvYXJkLmlvdC5maC13ZWxzLmF0LyIsImh0dHBzOi8vcHJvZHVjdGJvYXJkLmV1LmF1dGgwLmNvbS91c2VyaW5mbyJdLCJpYXQiOjE2ODIzMTc0MDcsImV4cCI6MTY4MjQwMzgwNywiYXpwIjoiVktVN2IzWHR3Mk1xYWtPQ2lhVUJOV1ZXa25ERnhCM24iLCJzY29wZSI6Im9wZW5pZCBwcm9maWxlIGVtYWlsIiwicGVybWlzc2lvbnMiOlsiY3JlYXRlOnByb2R1Y3RzIiwiY3JlYXRlOnVzZXJzIl19.Shp4kFReenMofvm-RhO-oYarRFfS1JTgsL1KjgNLR_0qqNSr2_vy6jJkSq3s5TbXfENLydACw6HYL4w65-NJe2Sgzt-vYwfLZwawMUmCFiJixMF9f-zJJzP4mNtEaDXXZlp0j9BssHJIw9Tk9i7pu_qHPaQDWmnuxsZ0VCoz1lRlAnUdr6v1FMNBymOBvAiKE3xcdfzFbJzZJS6iqbYIW4NauhpNtIVQj7h-cAuYRHxAMEfpyPXW4y5LhbsxaN1EtiT8EKbCPI9lRZnNbV8dTCrUx0jG4Uct_DFAIg48nziVjjEkWUfLmouz8jc5f6E0-6KahYKhM59fSIsh5RCQPA")
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
