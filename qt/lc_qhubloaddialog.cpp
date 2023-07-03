#include <QNetworkReply>
#include "lc_qhubloaddialog.h"
#include "ui_lc_qhubloaddialog.h"
#include "hub.h"

lcQHubLoadDialog::lcQHubLoadDialog(QWidget* Parent)
    : QDialog(Parent), ui(new Ui::lcQHubLoadDialog), nam(new QNetworkAccessManager), image(1000, 1000)
{
    ui->setupUi(this);

    ui->SchemeCombo->addItem("http");
    ui->SchemeCombo->addItem("https");
    ui->SchemeCombo->setCurrentText(Hub::INSTANCE.getScheme());

    ui->HostEdit->setText(Hub::INSTANCE.getHost());

    ui->PortSpin->setValue(Hub::INSTANCE.getPort());

    ui->TokenEdit->setText(Hub::INSTANCE.getToken());

    ui->ProductList->setDisabled(true);

    ui->ProductLabel->setDisabled(true);

    ui->VersionLabel->setDisabled(true);

    ui->VersionList->setDisabled(true);

    image.fill(Qt::transparent);

    int height = ui->VersionList->height();

    ui->VersionImage->setPixmap(image.scaled(height, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);

    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
}

lcQHubLoadDialog::~lcQHubLoadDialog()
{
    // empty
}

void lcQHubLoadDialog::accept()
{
    if (version.isEmpty())
    {
        // Update hub
        Hub::INSTANCE.setScheme(ui->SchemeCombo->currentText());
        Hub::INSTANCE.setHost(ui->HostEdit->text());
        Hub::INSTANCE.setPort(ui->PortSpin->value());
        Hub::INSTANCE.setToken(ui->TokenEdit->text());
        Hub::INSTANCE.save();

        // Update product
        Product::INSTANCE = product;

        // Update version
        Version::INSTANCES.clear();

        // Update model
        model = "";
        model.append("0\n");
        model.append("0 Name: ");
        model.append(product.getName());
        model.append(".ldr\n");
        model.append("0 Author: ");
        model.append("TODO");
        model.append("\n");

        QDialog::accept();
    }
    else
    {
        if (version.getModelType().compare("ldr") == 0 || version.getModelType().compare("mpd") == 0)
        {
            QString path("/rest/files/");
            path.append(version.getVersionId());
            path.append(".");
            path.append(version.getModelType());

            QUrl url;
            url.setScheme(ui->SchemeCombo->currentText());
            url.setHost(ui->HostEdit->text());
            url.setPort(ui->PortSpin->value());
            url.setPath(path);

            QString bearer("Bearer ");
            bearer.append(ui->TokenEdit->text());

            QNetworkRequest request(url);
            request.setRawHeader("Authorization", bearer.toUtf8());

            nam->get(request);
        }
        else
        {
            ui->ErrorLabel->setText("Model type is not supported");
        }
    }
}

void lcQHubLoadDialog::finished(QNetworkReply* reply)
{
    qInfo() << "[lcQHubLoadDialog] " << reply->request().url();

    if (reply->request().url().path().endsWith("/products"))
    {
        if (reply->error())
        {
            ui->ProductList->addItem(reply->errorString());
        }
        else
        {
            QByteArray content = reply->readAll();

            QJsonDocument document = QJsonDocument::fromJson(content);

            if (document.isArray())
            {
                QJsonArray array = document.array();

                if (array.empty())
                {
                    ui->ProductList->addItem("No products found");
                }
                else
                {
                    for (int index = 0; index < array.size(); index++)
                    {
                        QJsonValue item = array[index];

                        if (item.isObject())
                        {
                            QJsonObject object = array[index].toObject();

                            Product product(object);

                            products.insert(0, product);

                            ui->ProductList->insertItem(0, product.toString());
                        }
                    }

                    ui->ProductList->setEnabled(true);

                    ui->ProductLabel->setEnabled(true);
                }
            }
            else
            {
                ui->ProductList->addItem("CADdrive response is not supported");
            }
        }
    }
    else if (reply->request().url().path().endsWith("/versions"))
    {
        if (reply->error())
        {
            ui->VersionList->addItem(reply->errorString());
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

                        versions.insert(0, version);

                        ui->VersionList->insertItem(0, version.toString());
                    }
                }

                ui->VersionList->insertItem(0, "Start from scratch");

                ui->VersionList->setEnabled(true);

                ui->VersionLabel->setEnabled(true);
            }
            else
            {
                ui->VersionList->addItem("CADdrive response is not supported");
            }
        }
    }
    else if (reply->request().url().path().endsWith("png"))
    {
        if (reply->error())
        {
            ui->ErrorLabel->setText(reply->errorString());
        }
        else
        {
            image.loadFromData(reply->readAll());

            int height = ui->VersionList->height();

            ui->VersionImage->setPixmap(image.scaled(height, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
    else if (reply->request().url().path().endsWith("ldr") || reply->request().url().path().endsWith("mpd"))
    {
        if (reply->error())
        {
            ui->ErrorLabel->setText(reply->errorString());
        }
        else
        {
            // Update hub
            Hub::INSTANCE.setScheme(ui->SchemeCombo->currentText());
            Hub::INSTANCE.setHost(ui->HostEdit->text());
            Hub::INSTANCE.setPort(ui->PortSpin->value());
            Hub::INSTANCE.setToken(ui->TokenEdit->text());
            Hub::INSTANCE.save();

            // Update product
            Product::INSTANCE = product;

            // Update version
            Version::INSTANCES.clear();
            if (!version.isEmpty())
            {
                Version::INSTANCES.append(version);
            }

            // Update model
            model = reply->readAll();

            QDialog::accept();
        }
    }
}

void lcQHubLoadDialog::on_LoadButton_clicked()
{
    qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString();

    products.clear();

    ui->ProductList->clear();
    ui->ProductList->setDisabled(true);

    ui->ProductLabel->setDisabled(true);

    QUrl url;
    url.setScheme(ui->SchemeCombo->currentText());
    url.setHost(ui->HostEdit->text());
    url.setPort(ui->PortSpin->value());
    url.setPath("/rest/products");

    QString bearer("Bearer ");
    bearer.append(ui->TokenEdit->text());

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", bearer.toUtf8());

    nam->get(request);
}

void lcQHubLoadDialog::on_ProductList_itemSelectionChanged()
{
    product = Product();

    versions.clear();

    ui->VersionList->clear();
    ui->VersionList->setDisabled(true);

    ui->VersionLabel->setDisabled(true);

    if (ui->ProductList->selectedItems().size() == 1)
    {
        int productIdx = ui->ProductList->currentRow();

        product = products[productIdx];

        QString path("/rest/products/");
        path.append(product.getProductId());
        path.append("/versions");

        QUrl url;
        url.setScheme(ui->SchemeCombo->currentText());
        url.setHost(ui->HostEdit->text());
        url.setPort(ui->PortSpin->value());
        url.setPath(path);

        QString bearer("Bearer ");
        bearer.append(ui->TokenEdit->text());

        QNetworkRequest request(url);
        request.setRawHeader("Authorization", bearer.toUtf8());

        nam->get(request);
    }
}

void lcQHubLoadDialog::on_VersionList_itemSelectionChanged()
{
    version = Version();

    image = QPixmap(1000, 1000);
    image.fill(Qt::transparent);

    int height = ui->VersionImage->height();

    ui->VersionImage->setPixmap(image.scaled(height, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->ErrorLabel->setText("");

    if (ui->VersionList->selectedItems().size() == 1)
    {
        int versionIdx = ui->VersionList->currentRow();

        if (versionIdx > 0)
        {
            version = versions[versionIdx - 1];

            QString path("/rest/files/");
            path.append(version.getVersionId());
            path.append(".");
            path.append(version.getImageType());

            QUrl url;
            url.setScheme(ui->SchemeCombo->currentText());
            url.setHost(ui->HostEdit->text());
            url.setPort(ui->PortSpin->value());
            url.setPath(path);

            QString bearer("Bearer ");
            bearer.append(ui->TokenEdit->text());

            QNetworkRequest request(url);
            request.setRawHeader("Authorization", bearer.toUtf8());

            nam->get(request);
        }

        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    else
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);
    }
}

void lcQHubLoadDialog::resizeEvent(QResizeEvent* event)
{
    int height = ui->VersionList->height();

    ui->VersionImage->setPixmap(image.scaled(height, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QDialog::resizeEvent(event);
}
