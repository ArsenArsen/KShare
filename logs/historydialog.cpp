#include "historydialog.h"
#include "requestlogging.hpp"
#include "ui_historydialog.h"

#include <monospacetextdialog.h>

using requestlogging::LoggedRequest;

HistoryDialog::HistoryDialog(QWidget *parent) : QDialog(parent), ui(new Ui::HistoryDialog) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->treeWidget->setColumnWidth(0, 50);
    ui->treeWidget->setColumnWidth(1, 150);
    ui->treeWidget->setColumnWidth(2, 50);
    ui->treeWidget->setColumnWidth(3, 100);

    QList<LoggedRequest> requests = requestlogging::getRequests();
    for (LoggedRequest req : requests) {
        ui->treeWidget->addTopLevelItem(new QTreeWidgetItem(
        { req.getType(), req.getUrl(), QString::number(req.getResponseCode()), req.getTime() + " UTC" }));
    }
}

HistoryDialog::~HistoryDialog() {
    delete ui;
}

void HistoryDialog::on_treeWidget_doubleClicked(const QModelIndex &) {
    QString file = ui->treeWidget->currentItem()->text(3);
    file = settings::dir().absoluteFilePath("responses/" + file.left(file.length() - 4));

    QFile dataFile(file);
    if (!dataFile.open(QIODevice::ReadOnly)) return;
    MonospaceTextDialog *dialog = new MonospaceTextDialog(file, dataFile.readAll());
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}
