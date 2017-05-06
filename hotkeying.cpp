#include "hotkeying.hpp"

#include <QDebug>
#include <QHotkey>
#include <QMap>
#include <settings.hpp>

QMap<QString, QHotkey *> hotkeys;

// func gets bound only on first set, or load
void hotkeying::hotkey(QString seqName, QKeySequence seq,
                       std::function<void()> func) {
  if (hotkeys.contains(seqName))
    hotkeys.value(seqName)->setShortcut(seq, true);
  else {
    QHotkey *hotkey = new QHotkey(seq, true);
    QObject::connect(hotkey, &QHotkey::activated, func);
    hotkeys.insert(seqName, hotkey);
  }
  settings::settings().setValue(seqName.prepend("hotkey_"), seq.toString());
}

// forces the hotkey from settings
void hotkeying::load(QString seqName, std::function<void()> func) {
  QHotkey *h;
  QString name = seqName;
  name.prepend("hotkey_");
  if (settings::settings().contains(name))
    h = new QHotkey(QKeySequence(settings::settings().value(name).toString()),
                    true);
  else
    h = new QHotkey;
  QObject::connect(h, &QHotkey::activated, func);
  hotkeys.insert(seqName, h);
}

bool hotkeying::valid(QString seq) {
  return seq.isEmpty() || !QKeySequence(seq).toString().isEmpty();
}

QString hotkeying::sequence(QString seqName) {
  return hotkeys.contains(seqName)
             ? hotkeys.value(seqName)->shortcut().toString()
             : "";
}
