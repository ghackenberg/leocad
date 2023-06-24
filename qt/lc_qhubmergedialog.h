#pragma once

#include <QNetworkAccessManager>

namespace Ui {
class lcQHubMergeDialog;
}

class lcQHubMergeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit lcQHubMergeDialog(QWidget* parent);
    ~lcQHubMergeDialog();

    inline const QString& getModel() const
    {
        return model;
    }

public slots:
    void accept() override;

protected slots:
    void finished(QNetworkReply* reply);

    void on_AdditionalVersionList_itemSelectionChanged();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    Ui::lcQHubMergeDialog* ui;

    QNetworkAccessManager* nam;

    QPixmap image;
    QString model;
};
