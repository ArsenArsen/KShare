#include "mainwindow.hpp"
#include "screenshotter.hpp"
#include "screenshotutil.hpp"
#include "ui_mainwindow.h"
#include <QAction>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QDoubleSpinBox>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QMenu>
#include <QStatusBar>
#include <QSystemTrayIcon>
#include <QTimer>
#include <functional>
#include <hotkeying.hpp>
#include <settings.hpp>
#include <uploaders/uploadersingleton.hpp>

MainWindow *MainWindow::instance;

void addHotkeyItem(QString text, QString name, std::function<void()> *func) {
  QListWidgetItem *item =
      new QListWidgetItem(text, MainWindow::inst()->ui->hotkeys);
  item->setData(Qt::UserRole + 1, name);
  MainWindow::inst()->fncs.insert(name, func);
  hotkeying::load(name, *func);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  instance = this;
  ui->setupUi(this);
  setWindowIcon(QIcon(":/icons/icon.png"));
  tray = new QSystemTrayIcon(windowIcon(), this);
  tray->setToolTip("KShare");
  tray->setVisible(true);
  QMenu *menu = new QMenu(this);
  QAction *quit = new QAction("Quit", this);
  QAction *shtoggle = new QAction("Show/Hide", this);
  QAction *fullscreen = new QAction("Take fullscreen shot", this);
  QAction *area = new QAction("Take area shot", this);
  menu->addActions({quit, shtoggle});
  menu->addSeparator();
  menu->addActions({fullscreen, area});
  connect(quit, &QAction::triggered, this, &MainWindow::quit);
  connect(shtoggle, &QAction::triggered, this, &MainWindow::toggleVisible);
  connect(tray, &QSystemTrayIcon::messageClicked, this,
          &MainWindow::toggleVisible);
  connect(tray, &QSystemTrayIcon::activated, this,
          [this](QSystemTrayIcon::ActivationReason reason) {
            if (reason == QSystemTrayIcon::DoubleClick)
              toggleVisible();
          });
  connect(fullscreen, &QAction::triggered, this,
          [] { screenshotter::fullscreenDelayed(); });
  connect(area, &QAction::triggered, this,
          [] { screenshotter::areaDelayed(); });
  tray->setContextMenu(menu);

  ui->uploaderList->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->uploaderList->setSelectionMode(QAbstractItemView::SingleSelection);

  // Add items to uploader selection
  for (Uploader *u : UploaderSingleton::inst().uploaderList())
    newUploader(u);
  connect(&UploaderSingleton::inst(), &UploaderSingleton::newUploader, this,
          &MainWindow::newUploader);

  // Set filename scheme
  if ((settings::settings().contains("fileFormat")))
    setScheme(settings::settings().value("fileFormat").toString());
  else
    setScheme("Screenshot %(yyyy-MM-dd HH:mm:ss)date");

  auto errors = UploaderSingleton::inst().errors();
  if (errors.length() == 1)
    statusBar()->showMessage(errors.at(0).what());
  else
    statusBar()->showMessage(
        QString("Errors visible in console (if present). Count: " +
                QString::number(errors.size())));

  // Set delay
  if ((settings::settings().contains("delay")))
    ui->delay->setValue(settings::settings().value("delay").toDouble());
  else
    ui->delay->setValue(0.25);

  ui->hotkeys->setSelectionMode(QListWidget::SingleSelection);

  addHotkeyItem("Fullscreen image", "fullscreen",
                new std::function<void()>([] { screenshotter::fullscreen(); }));
  addHotkeyItem("Area image", "area",
                new std::function<void()>([] { screenshotter::area(); }));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setScheme(QString scheme) { ui->nameScheme->setText(scheme); }

QDoubleSpinBox *MainWindow::delay() { return ui->delay; }

MainWindow *MainWindow::inst() { return instance; }

void MainWindow::closeEvent(QCloseEvent *event) {
  event->ignore();
  QTimer::singleShot(0, this, &MainWindow::hide);
}

void MainWindow::quit() { QCoreApplication::quit(); }

void MainWindow::toggleVisible() {
  this->setVisible(!this->isVisible());
  if (this->isVisible()) {
    this->raise();
  }
}

void MainWindow::newUploader(Uploader *u) {
  QListWidgetItem *item = new QListWidgetItem(u->name());
  item->setToolTip(u->description());
  ui->uploaderList->addItem(item);
  if (u->name() == UploaderSingleton::inst().selectedUploader())
    item->setSelected(true);
}

void MainWindow::on_actionQuit_triggered() { quit(); }

void MainWindow::on_actionFullscreen_triggered() {
  screenshotter::fullscreenDelayed();
}

void MainWindow::on_actionArea_triggered() { screenshotter::areaDelayed(); }

void MainWindow::on_uploaderList_clicked(const QModelIndex &) {
  QList<QListWidgetItem *> index = ui->uploaderList->selectedItems();
  if (index.size() == 1) {
    UploaderSingleton::inst().set(index.at(0)->text());
  }
}

void MainWindow::on_nameScheme_textEdited(const QString &arg1) {
  settings::settings().setValue("fileFormat", arg1);
}

void MainWindow::on_delay_valueChanged(double arg1) {
  settings::settings().setValue("delay", arg1);
}

void MainWindow::on_hotkeys_doubleClicked(const QModelIndex &) {
  if (ui->hotkeys->selectedItems().length() == 1) {
    QListWidgetItem *i = ui->hotkeys->selectedItems().at(0);
    QString str = i->data(Qt::UserRole + 1).toString();
    QString seq = QInputDialog::getText(ui->centralWidget, "Hotkey Input",
                                        "Insert hotkey:", QLineEdit::Normal,
                                        hotkeying::sequence(str));
    if (hotkeying::valid(seq))
      hotkeying::hotkey(str, QKeySequence(seq), *fncs.value(str));
  }
}
