#ifndef UPLOADER_HPP
#define UPLOADER_HPP

#include <QPixmap>
#include <QString>

class Uploader {
public:
  virtual void doUpload(QPixmap *pixmap) = 0;
  virtual QString name() = 0;
  virtual QString description() = 0;
};

#endif // UPLOADER_HPP
