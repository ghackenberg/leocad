#include <QNetworkReply>
#include <QHttpMultiPart>
#include "lc_qhubpushdialog.h"
#include "ui_lc_qhubpushdialog.h"
#include "hub.h"
#include "product.h"
#include "version.h"

lcQHubPushDialog::lcQHubPushDialog(QWidget* Parent)
    : QDialog(Parent), ui(new Ui::lcQHubPushDialog), nam(new QNetworkAccessManager)
{
    ui->setupUi(this);

    ui->HubEdit->setText(Hub::get().toString());
    ui->ProductEdit->setText(Product::get().toString());
    ui->BaseVersionEdit->setText(Version::get().toString());

    ui->MajorSpin->setValue(Version::get().getMajor());
    ui->MinorSpin->setValue(Version::get().getMinor());
    ui->PatchSpin->setValue(Version::get().getPatch() + 1);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);

    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));

    setWindowTitle("Push model to CADdrive");
}

lcQHubPushDialog::~lcQHubPushDialog()
{
    // empty
}

void lcQHubPushDialog::accept()
{
    QUrl url;
    url.setScheme(Hub::get().getScheme());
    url.setHost(Hub::get().getHost());
    url.setPort(Hub::get().getPort());
    url.setPath("/rest/versions");

    QString bearer("Bearer ");
    bearer.append(Hub::get().getToken());

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", bearer.toUtf8());

    QHttpMultiPart* multipart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QJsonArray baseVersionIds;
    if (!Version::get().isEmpty())
    {
        baseVersionIds.append(Version::get().getId());
    }

    QJsonObject dataJsonObject;
    dataJsonObject.insert("productId", Product::get().getId());
    dataJsonObject.insert("baseVersionIds", baseVersionIds);
    dataJsonObject.insert("major", ui->MajorSpin->value());
    dataJsonObject.insert("minor", ui->MajorSpin->value());
    dataJsonObject.insert("patch", ui->PatchSpin->value());
    dataJsonObject.insert("description", ui->DescriptionEdit->toPlainText());

    QJsonDocument dataJson;
    dataJson.setObject(dataJsonObject);

    QHttpPart dataPart;
    dataPart.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    dataPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"data\""));
    dataPart.setBody(dataJson.toJson());

    multipart->append(dataPart);

    QFile *modelFile = new QFile("hub.ldr");
    modelFile->open(QIODevice::ReadOnly);

    modelFile->setParent(multipart);

    QHttpPart modelPart;
    modelPart.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-ldraw");
    modelPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"model\"; filename=\"hub.ldr\""));
    modelPart.setBodyDevice(modelFile);

    multipart->append(modelPart);

    QNetworkReply* reply = nam->post(request, multipart);

    multipart->setParent(reply);
}

void lcQHubPushDialog::finished(QNetworkReply* reply)
{
    if (reply->error())
    {
        ui->ErrorLabel->setText(reply->errorString());
    }
    else
    {
        QByteArray content = reply->readAll();

        QJsonDocument document = QJsonDocument::fromJson(content);

        if (document.isObject()) {
            QJsonObject object = document.object();

            Version version(object);

            Version::set(version);

            QDialog::accept();
        }
    }
}

void lcQHubPushDialog::on_DescriptionEdit_textChanged()
{
    if (ui->DescriptionEdit->toPlainText().isEmpty())
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);
    }
    else
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

