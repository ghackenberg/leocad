#include <QNetworkReply>
#include "lc_qhubmergedialog.h"
#include "ui_lc_qhubmergedialog.h"
#include "hub.h"
#include "product.h"
#include "version.h"

lcQHubMergeDialog::lcQHubMergeDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::lcQHubMergeDialog), nam(new QNetworkAccessManager), image(1000, 1000)
{
    ui->setupUi(this);

    ui->ServerEdit->setText(Hub::INSTANCE.toString());

    ui->ProductEdit->setText(Product::INSTANCE.toString());

    ui->CurrentVersionsList->clear();
    for (QList<Version>::const_iterator iter = Version::BASES.cbegin(); iter != Version::BASES.cend(); iter++)
    {
        const Version version = (*iter);
        ui->CurrentVersionsList->addItem(version.toString());
    }

    ui->AdditionalVersionList->clear();
    for (QList<Version>::const_iterator outerIter = Version::INSTANCES.cbegin(); outerIter != Version::INSTANCES.cend(); outerIter++)
    {
        const Version outerVersion = (*outerIter);

        bool skip = false;

        for (QList<Version>::const_iterator innerIter = Version::BASES.cbegin(); innerIter != Version::BASES.cend(); innerIter++)
        {
            const Version innerVersion = (*innerIter);

            if (outerVersion.getId().compare(innerVersion.getId()) == 0)
            {
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            ui->AdditionalVersionList->addItem(outerVersion.toString());
        }
    }

    image.fill(Qt::transparent);

    int height = ui->AdditionalVersionList->height();

    ui->AdditionalVersionImage->setPixmap(image.scaled(height, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);

    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
}

lcQHubMergeDialog::~lcQHubMergeDialog()
{
    delete ui;
}

void lcQHubMergeDialog::accept()
{
    if (version.getModelType().compare("ldr") == 0 || version.getModelType().compare("mpd") == 0)
    {
        QString path("/rest/files/");
        path.append(version.getId());
        path.append(".");
        path.append(version.getModelType());

        QUrl url;
        url.setScheme(Hub::INSTANCE.getScheme());
        url.setHost(Hub::INSTANCE.getHost());
        url.setPort(Hub::INSTANCE.getPort());
        url.setPath(path);

        QString bearer("Bearer ");
        bearer.append(Hub::INSTANCE.getToken());

        QNetworkRequest request(url);
        request.setRawHeader("Authorization", bearer.toUtf8());

        nam->get(request);
    }
    else
    {
        ui->ErrorLabel->setText("Model type not supported");
    }
}

void lcQHubMergeDialog::finished(QNetworkReply* reply)
{
    if (reply->request().url().path().endsWith("png"))
    {
        if (reply->error())
        {
            ui->ErrorLabel->setText(reply->errorString());
        }
        else
        {
            image.loadFromData(reply->readAll());

            int height = ui->AdditionalVersionList->height();

            ui->AdditionalVersionImage->setPixmap(image.scaled(height, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
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
            Version::BASES.append(version);

            model = reply->readAll();

            QDialog::accept();
        }
    }
}

void lcQHubMergeDialog::on_AdditionalVersionList_itemSelectionChanged()
{
    version = Version();

    image = QPixmap(1000, 1000);
    image.fill(Qt::transparent);

    int height = ui->AdditionalVersionImage->height();

    ui->AdditionalVersionImage->setPixmap(image.scaled(height, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->ErrorLabel->setText("");

    if (ui->AdditionalVersionList->selectedItems().size() == 1)
    {
        int index = ui->AdditionalVersionList->currentRow();

        version = Version::INSTANCES[index];

        QString path("/rest/files/");
        path.append(version.getId());
        path.append(".");
        path.append(version.getImageType());

        QUrl url;
        url.setScheme(Hub::INSTANCE.getScheme());
        url.setHost(Hub::INSTANCE.getHost());
        url.setPort(Hub::INSTANCE.getPort());
        url.setPath(path);

        QString bearer("Bearer ");
        bearer.append(Hub::INSTANCE.getToken());

        QNetworkRequest request(url);
        request.setRawHeader("Authorization", bearer.toUtf8());

        nam->get(request);

        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    else
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);
    }
}

void lcQHubMergeDialog::resizeEvent(QResizeEvent* event)
{
    int height = ui->AdditionalVersionList->height();

    ui->AdditionalVersionImage->setPixmap(image.scaled(height, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QDialog::resizeEvent(event);
}
