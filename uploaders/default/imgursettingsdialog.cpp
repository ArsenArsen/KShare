#include "imgursettingsdialog.hpp"
#include "ui_imgursettingsdialog.h"

#include <QDesktopServices>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkReply>
#include <QPushButton>
#include <QUrl>
#include <io/ioutils.hpp>
#include <settings.hpp>

ImgurSettingsDialog::ImgurSettingsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ImgurSettingsDialog) {
    ui->setupUi(this);
    connect(this, &ImgurSettingsDialog::accepted, this, &ImgurSettingsDialog::deleteLater);
    ui->clientId->setText(settings::settings().value("imgur/cid").toString());
    ui->clientSecret->setText(settings::settings().value("imgur/csecret").toString());
}

ImgurSettingsDialog::~ImgurSettingsDialog() {
    delete ui;
}

void ImgurSettingsDialog::on_addApp_clicked() {
    QDesktopServices::openUrl(QUrl("https://api.imgur.com/oauth2/addclient"));
}

void ImgurSettingsDialog::on_getPin_clicked() {
    QDesktopServices::openUrl(
    QUrl(QString("https://api.imgur.com/oauth2/authorize?client_id=%1&response_type=pin").arg(ui->clientId->text())));
}

void ImgurSettingsDialog::on_authorize_clicked() {
    if (ui->pin->text().isEmpty() || ui->clientId->text().isEmpty() || ui->clientSecret->text().isEmpty()) return;
    ui->buttonBox->setEnabled(false);
    QJsonObject object;
    object.insert("client_id", ui->clientId->text());
    object.insert("client_secret", ui->clientSecret->text());
    object.insert("grant_type", "pin");
    object.insert("pin", ui->pin->text());
    settings::settings().setValue("imgur/cid", ui->clientId->text());
    settings::settings().setValue("imgur/csecret", ui->clientSecret->text());

    ioutils::postJson(QUrl("https://api.imgur.com/oauth2/token"),
                      QList<QPair<QString, QString>>({ QPair<QString, QString>("Content-Type", "applicaton/json") }),
                      QJsonDocument::fromVariant(object.toVariantMap()).toJson(),
                      [&](QJsonDocument response, QByteArray, QNetworkReply *r) {
                          if (r->error() != QNetworkReply::NoError || !response.isObject()) {
                              ui->buttonBox->setEnabled(true);
                              return;
                          }
                          QJsonObject res = response.object();
                          if (res.value("success").toBool()) {
                              ui->buttonBox->setEnabled(true);
                              return;
                          }

                          settings::settings().setValue("imgur/expire",
                                                        QDateTime::currentDateTimeUtc().addSecs(res["expires_in"].toInt()));
                          settings::settings().setValue("imgur/refresh", res["refresh_token"].toString());
                          settings::settings().setValue("imgur/access", res["refresh_token"].toString());
                          ui->status->setText(tr("It works!"));
                          ui->status->setStyleSheet("* { color: green; }");

                          ui->authorize->setEnabled(false);
                          ui->addApp->setEnabled(false);
                          ui->clientSecret->setEnabled(false);
                          ui->clientId->setEnabled(false);

                          ui->buttonBox->setEnabled(true);
                      });
}
