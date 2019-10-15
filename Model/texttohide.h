#ifndef TEXTTOHIDE_H
#define TEXTTOHIDE_H

#include <QPixmap>

class TextToHide
{
public:
    TextToHide(const QPixmap &imgContainer);
    int getMaxLength();

private:
    int maxLength;
};

#endif // TEXTTOHIDE_H
