#include "hotkeying.hpp"

#include <QHotkey>
#include <QMap>
#include <settings.hpp>

QMap<QString, QHotkey *> hotkeys;

// func gets bound only on first set, or load
void hotkeying::hotkey(QString seqName, QKeySequence seq, std::function<void()> func) {
    QHotkey *hotkey;
    if (hotkeys.contains(seqName))
        (hotkey = hotkeys.value(seqName))->setShortcut(seq, true);
    else {
        hotkey = new QHotkey(seq, true);
        QObject::connect(hotkey, &QHotkey::activated, func);
        hotkeys.insert(seqName, hotkey);
    }
    settings::settings().setValue(seqName.prepend("hotkey_"), seq.toString());
    if (!hotkey->isRegistered() && !seq.toString().isEmpty())
        qWarning().noquote().nospace()
        << QObject::tr("Could not bind the hotkey %1! Is the keybind already registered?").arg(seqName);
}

// forces the hotkey from settings
void hotkeying::load(QString seqName, std::function<void()> func, QString def) {
    QHotkey *h;
    QString name = seqName;
    name.prepend("hotkey_");
    if (hotkeys.contains(seqName)) return;
    QString k = settings::settings().value(name).toString();
    if (!k.isEmpty()) {
        h = new QHotkey(QKeySequence(k), true);
    } else
        h = new QHotkey(def.isEmpty() ? "" : def, true);
    QObject::connect(h, &QHotkey::activated, func);
    hotkeys.insert(seqName, h);
    if (!h->isRegistered() && !h->shortcut().toString().isEmpty())
        qWarning().noquote().nospace()
        << QObject::tr("Could not bind the hotkey %1! Is the keybind already registered?").arg(seqName);
    ;
}

bool hotkeying::valid(QString seq) {
    return seq.isEmpty() || !QKeySequence(seq).toString().isEmpty();
}

QString hotkeying::sequence(QString seqName) {
    return hotkeys.contains(seqName) ?
           hotkeys.value(seqName)->isRegistered() ? hotkeys.value(seqName)->shortcut().toString() : "" :
           "";
}
