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
        << "Could not bind the hotkey " << seqName << "! Is the keybind already registered?";
}

// forces the hotkey from settings
void hotkeying::load(QString seqName, std::function<void()> func, QString def) {
    QHotkey *h;
    QString name = seqName;
    name.prepend("hotkey_");
    if (hotkeys.contains(seqName)) return;
    if (settings::settings().contains(name)) {
        QString k = settings::settings().value(name).toString();
        if (!k.isEmpty())
            h = new QHotkey(QKeySequence(settings::settings().value(k).toString()), true);
        else
            h = new QHotkey(def.isNull() ? "" : def, true);
    } else
        h = new QHotkey(def.isNull() ? "" : def, true);
    QObject::connect(h, &QHotkey::activated, func);
    hotkeys.insert(seqName, h);
    if (!h->isRegistered() && (settings::settings().contains(name) || !def.isEmpty()))
        qWarning().noquote().nospace()
        << "Could not bind the hotkey " << seqName << "! Is the keybind already registered?";
}

bool hotkeying::valid(QString seq) {
    return seq.isEmpty() || !QKeySequence(seq).toString().isEmpty();
}

QString hotkeying::sequence(QString seqName) {
    return hotkeys.contains(seqName) ?
           hotkeys.value(seqName)->isRegistered() ? hotkeys.value(seqName)->shortcut().toString() : "" :
           "";
}
