#include "lc_qhubmergedialog.h"
#include "ui_lc_qhubmergedialog.h"

lcQHubMergeDialog::lcQHubMergeDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::lcQHubMergeDialog)
{
    ui->setupUi(this);
}

lcQHubMergeDialog::~lcQHubMergeDialog()
{
    delete ui;
}
