#pragma once

#include <QNetworkAccessManager>

namespace Ui {
class lcQHubMergeDialog;
}

class lcQHubMergeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit lcQHubMergeDialog(QWidget *parent = nullptr);
    ~lcQHubMergeDialog();

private:
    Ui::lcQHubMergeDialog *ui;
};
