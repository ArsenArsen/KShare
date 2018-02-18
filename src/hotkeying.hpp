#ifndef HOTKEYING_HPP
#define HOTKEYING_HPP

#include <QKeySequence>
#include <QString>
#include <functional>

namespace hotkeying {
    void hotkey(QString seqName, QKeySequence seq, std::function<void()> func);
    bool valid(QString seq);
    void load(QString seqName, std::function<void()> func, QString def = QString());
    QString sequence(QString seqName);
} // namespace hotkeying

#endif // HOTKEYING_HPP
