#ifndef LSBSTEGANOGRAPHY_H
#define LSBSTEGANOGRAPHY_H

#include <QPixmap>

class LsbSteganography
{
public:
    LsbSteganography();
    static int calcMaxNumOfCharToHide(const QPixmap &imgContainer);
};

#endif // LSBSTEGANOGRAPHY_H
