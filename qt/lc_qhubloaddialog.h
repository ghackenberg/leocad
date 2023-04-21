#pragma once

#include <QNetworkAccessManager>
#include "product.h"
#include "version.h"

namespace Ui
{
class lcQHubLoadDialog;
}

class lcQHubLoadDialog : public QDialog
{
	Q_OBJECT

public:
    lcQHubLoadDialog(QWidget* Parent);
    ~lcQHubLoadDialog();

    inline const QString& getModel() const
    {
        return model;
    }

public slots:
    void accept() override;

    void finished(QNetworkReply* reply);

    void on_LoadButton_clicked();
    void on_ProductList_itemSelectionChanged();
    void on_VersionList_itemSelectionChanged();

private:
    Ui::lcQHubLoadDialog* ui;

    QNetworkAccessManager* nam;

    QList<Product> products;
    QList<Version> versions;

    QString model;
};

