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

    ui->HubEdit->setText(Hub::INSTANCE.toString());

    ui->TokenEdit->setText(Hub::INSTANCE.getToken());

    ui->ProductEdit->setText(Product::INSTANCE.toString());

    for (QList<Version>::const_iterator iter = Version::INSTANCES.cbegin(); iter != Version::INSTANCES.cend(); iter++)
    {
        const Version version = (*iter);
        ui->BaseVersionList->addItem(version.toString());
    }

    ui->NumberLabel->setDisabled(true);

    ui->MajorLabel->setDisabled(true);
    ui->MinorLabel->setDisabled(true);
    ui->PatchLabel->setDisabled(true);

    ui->MajorSpin->setDisabled(true);
    ui->MinorSpin->setDisabled(true);
    ui->PatchSpin->setDisabled(true);

    ui->DescriptionLabel->setDisabled(true);

    ui->DescriptionEdit->setDisabled(true);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);

    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));

    QUrlQuery query;
    query.addQueryItem("product", Product::INSTANCE.getId());

    QUrl url;
    url.setScheme(Hub::INSTANCE.getScheme());
    url.setHost(Hub::INSTANCE.getHost());
    url.setPort(Hub::INSTANCE.getPort());
    url.setPath("/rest/versions");
    url.setQuery(query);

    QString bearer("Bearer ");
    bearer.append(Hub::INSTANCE.getToken());

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", bearer.toUtf8());

    nam->get(request);
}

lcQHubPushDialog::~lcQHubPushDialog()
{
    // empty
}

void lcQHubPushDialog::accept()
{
    QUrl url;
    url.setScheme(Hub::INSTANCE.getScheme());
    url.setHost(Hub::INSTANCE.getHost());
    url.setPort(Hub::INSTANCE.getPort());
    url.setPath("/rest/versions");

    QString bearer("Bearer ");
    bearer.append(Hub::INSTANCE.getToken());

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", bearer.toUtf8());

    QHttpMultiPart* multipart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QJsonArray baseVersionIds;
    for (QList<Version>::const_iterator iter = Version::INSTANCES.cbegin(); iter != Version::INSTANCES.cend(); iter++)
    {
        const Version version = (*iter);
        baseVersionIds.append(version.getId());
    }

    QJsonObject dataJsonObject;
    dataJsonObject.insert("productId", Product::INSTANCE.getId());
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
    qInfo() << "[lcQHubPushDialog] " << reply->request().url();

    if (reply->request().url().path().endsWith("/rest/versions") && reply->request().url().hasQuery())
    {
        if (reply->error())
        {
            ui->ErrorLabel->setText(reply->errorString());
        }
        else
        {
            QByteArray content = reply->readAll();

            QJsonDocument document = QJsonDocument::fromJson(content);

            if (document.isArray())
            {
                QJsonArray array = document.array();

                for (int index = 0; index < array.size(); index++)
                {
                    QJsonValue item = array[index];

                    if (item.isObject())
                    {
                        QJsonObject object = array[index].toObject();

                        Version version(object);

                        versions.append(version);
                    }
                }

                unsigned int major = 0;
                unsigned int minor = 0;
                unsigned int patch = 0;

                if (Version::INSTANCES.empty())
                {
                    if (!versions.empty())
                    {
                        // Find latest major version
                        for (QList<Version>::const_iterator iter = versions.cbegin(); iter != versions.cend(); iter++)
                        {
                            const Version version = (*iter);
                            if (major < version.getMajor()) {
                                major = version.getMajor();
                                minor = version.getMinor();
                                patch = version.getPatch();
                            } else if (major == version.getMajor()) {
                                if (minor < version.getMinor()) {
                                    minor = version.getMinor();
                                    patch = version.getPatch();
                                } else if (minor == version.getMinor()) {
                                    if (patch < version.getPatch()) {
                                        patch = version.getPatch();
                                    }
                                }
                            }
                        }
                        // Increase latest major version
                        major = major + 1;
                        minor = 0;
                        patch = 0;
                    }
                }
                else
                {
                    // Find latest major base version
                    for (QList<Version>::const_iterator iter = Version::INSTANCES.cbegin(); iter != Version::INSTANCES.cend(); iter++)
                    {
                        const Version version = (*iter);
                        if (major < version.getMajor()) {
                            major = version.getMajor();
                            minor = version.getMinor();
                            patch = version.getPatch();
                        } else if (major == version.getMajor()) {
                            if (minor < version.getMinor()) {
                                minor = version.getMinor();
                                patch = version.getPatch();
                            } else if (minor == version.getMinor()) {
                                if (patch < version.getPatch()) {
                                    patch = version.getPatch();
                                }
                            }
                        }
                    }
                    // Find latest patch version for that major version
                    for (QList<Version>::const_iterator iter = versions.cbegin(); iter != versions.cend(); iter++)
                    {
                        const Version version = (*iter);
                        if (major == version.getMajor()) {
                            if (minor == version.getMinor()) {
                                if (patch < version.getPatch()) {
                                    patch = version.getPatch();
                                }
                            }
                        }
                    }
                    // Increase latest patch version
                    patch = patch + 1;
                }

                ui->NumberLabel->setDisabled(false);

                ui->MajorSpin->setValue(major);
                ui->MinorSpin->setValue(minor);
                ui->PatchSpin->setValue(patch);

                ui->MajorLabel->setDisabled(false);
                ui->MinorLabel->setDisabled(false);
                ui->PatchLabel->setDisabled(false);

                ui->MajorSpin->setDisabled(false);
                ui->MinorSpin->setDisabled(false);
                ui->PatchSpin->setDisabled(false);

                ui->DescriptionLabel->setDisabled(false);

                ui->DescriptionEdit->setDisabled(false);
            }
            else
            {
                ui->ErrorLabel->setText("CADdrive response is not supported");
            }
        }
    }
    else if (reply->request().url().path().endsWith("/rest/versions") && !reply->request().url().hasQuery())
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

                Version::INSTANCES.clear();
                Version::INSTANCES.append(version);

                QDialog::accept();
            }
            else
            {
                ui->ErrorLabel->setText("CADdrive response is not supported");
            }
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

