#include <QNetworkReply>
#include "hub.h"
#include "lc_qhubloaddialog.h"
#include "ui_lc_qhubloaddialog.h"

lcQHubLoadDialog::lcQHubLoadDialog(QWidget* Parent)
    : QDialog(Parent), ui(new Ui::lcQHubLoadDialog), nam(new QNetworkAccessManager), image(1000, 1000)
{
    ui->setupUi(this);

    ui->SchemeCombo->addItem("http");
    ui->SchemeCombo->addItem("https");
    ui->SchemeCombo->setCurrentText(Hub::get().getScheme());

    ui->HostEdit->setText(Hub::get().getHost());

    ui->PortSpin->setValue(Hub::get().getPort());

    ui->TokenEdit->setText(Hub::get().getToken());

    ui->ProductList->setDisabled(true);
    ui->ProductLabel->setDisabled(true);

    image.fill(Qt::transparent);

    int height = ui->VersionList->height();

    ui->VersionList->setDisabled(true);
    ui->VersionImage->setPixmap(image.scaled(height, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->VersionLabel->setDisabled(true);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);

    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));

    setWindowTitle("Load model from ProductBoard");
}

lcQHubLoadDialog::~lcQHubLoadDialog()
{
    // empty
}

void lcQHubLoadDialog::accept()
{
    if (ui->VersionList->selectedItems().size() == 1)
    {
        int index = ui->VersionList->currentRow();

        const Version& version = versions[index];

        if (version.isEmpty())
        {
            model = "";
            model.append("0\n");
            model.append("0 Name: ");
            model.append(Product::get().getName());
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
                path.append(version.getId());
                path.append(".");
                path.append(version.getModelType());

                QUrl url;
                url.setScheme(Hub::get().getScheme());
                url.setHost(Hub::get().getHost());
                url.setPort(Hub::get().getPort());
                url.setPath(path);

                QString bearer("Bearer ");
                bearer.append(Hub::get().getToken());

                QNetworkRequest request(url);
                request.setRawHeader("Authorization", bearer.toUtf8());

                nam->get(request);
            }
            else
            {
                ui->ErrorLabel->setText("Model type not supported");
            }
        }
    }
    else
    {
        model = "";
        model.append("0\n");
        model.append("0 Name: ");
        model.append(Product::get().getName());
        model.append(".ldr\n");
        model.append("0 Author: ");
        model.append("TODO");
        model.append("\n");

        QDialog::accept();
    }
}

void lcQHubLoadDialog::finished(QNetworkReply* reply)
{
    if (ui->ProductList->selectedItems().size() == 0)
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
                    ui->ProductList->addItem("Please create a product first");
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

                            ui->ProductList->insertItem(0, product.toString());

                            products.insert(0, product);
                        }
                    }

                    ui->ProductList->setEnabled(true);
                    ui->ProductLabel->setEnabled(true);
                }
            }
            else
            {
                ui->ProductList->addItem("Reponse invalid");
            }
        }
    }
    else if (ui->VersionList->selectedItems().size() == 0)
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

                        ui->VersionList->insertItem(0, version.toString());

                        versions.insert(0, version);
                    }
                }

                ui->VersionList->insertItem(0, "Start from scratch");

                versions.insert(0, Version());

                ui->VersionList->setEnabled(true);
                ui->VersionLabel->setEnabled(true);
            }
            else
            {
                ui->VersionList->addItem("Reponse invalid.");
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
    else
    {
        if (reply->error())
        {
            ui->ErrorLabel->setText(reply->errorString());
        }
        else
        {
            model = reply->readAll();

            QDialog::accept();
        }
    }
}

void lcQHubLoadDialog::on_LoadButton_clicked()
{
    Hub::get().setScheme(ui->SchemeCombo->currentText());
    Hub::get().setHost(ui->HostEdit->text());
    Hub::get().setPort(ui->PortSpin->value());
    Hub::get().setToken(ui->TokenEdit->text());

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
    ui->VersionList->clear();
    ui->VersionList->setDisabled(true);
    ui->VersionLabel->setDisabled(true);

    Version::set(Version());

    if (ui->ProductList->selectedItems().size() == 1)
    {
        int index = ui->ProductList->currentRow();

        const Product& product = products[index];

        QUrlQuery query;
        query.addQueryItem("product", product.getId());

        QUrl url;
        url.setScheme(ui->SchemeCombo->currentText());
        url.setHost(ui->HostEdit->text());
        url.setPort(ui->PortSpin->value());
        url.setPath("/rest/versions");
        url.setQuery(query);

        QString bearer("Bearer ");
        bearer.append(ui->TokenEdit->text());

        QNetworkRequest request(url);
        request.setRawHeader("Authorization", bearer.toUtf8());

        nam->get(request);

        Product::set(product);
    }
    else
    {
        Product::set(Product());
    }
}

void lcQHubLoadDialog::on_VersionList_itemSelectionChanged()
{
    image = QPixmap(1000, 1000);

    image.fill(Qt::transparent);

    int height = ui->VersionImage->height();

    ui->VersionImage->setPixmap(image.scaled(height, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->ErrorLabel->setText("");

    if (ui->VersionList->selectedItems().size() == 1)
    {
        int index = ui->VersionList->currentRow();

        const Version& version = versions[index];

        if (!version.isEmpty())
        {
            QString path("/rest/files/");
            path.append(version.getId());
            path.append(".");
            path.append(version.getImageType());

            QUrl url;
            url.setScheme(Hub::get().getScheme());
            url.setHost(Hub::get().getHost());
            url.setPort(Hub::get().getPort());
            url.setPath(path);

            QString bearer("Bearer ");
            bearer.append(Hub::get().getToken());

            QNetworkRequest request(url);
            request.setRawHeader("Authorization", bearer.toUtf8());

            nam->get(request);
        }

        Version::set(version);

        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    else
    {
        Version::set(Version());

        ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);
    }
}

void lcQHubLoadDialog::resizeEvent(QResizeEvent* event)
{
    int height = ui->VersionList->height();

    ui->VersionImage->setPixmap(image.scaled(height, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QDialog::resizeEvent(event);
}
