#pragma once

#include <QNetworkAccessManager>

namespace Ui
{
class lcQHubPushDialog;
}

class lcQHubPushDialog : public QDialog
{
	Q_OBJECT

public:
    lcQHubPushDialog(QWidget* Parent);
    ~lcQHubPushDialog();

public slots:
	void accept() override;

    void finished(QNetworkReply* reply);

    void on_DescriptionEdit_textChanged();

private:
    Ui::lcQHubPushDialog* ui;

    QNetworkAccessManager* nam;

};

