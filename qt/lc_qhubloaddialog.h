#pragma once

#include <QNetworkAccessManager>

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

protected slots:
    void finished(QNetworkReply* reply);

    void on_LoadButton_clicked();
    void on_ProductList_itemSelectionChanged();
    void on_VersionList_itemSelectionChanged();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    Ui::lcQHubLoadDialog* ui;

    QNetworkAccessManager* nam;

    QPixmap image;
    QString model;
};

