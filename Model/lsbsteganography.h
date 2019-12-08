#ifndef LSBSTEGANOGRAPHY_H
#define LSBSTEGANOGRAPHY_H

#include <QPixmap>

class LsbSteganography
{
public:
    LsbSteganography();
    static int calcMaxNumOfCharToHide(const QPixmap &imgContainer);
    QBitArray convertToBitArray(const QByteArray &byteArray); // TODO private
    QByteArray convertToByteArray(const QBitArray &bitArray); // TODO private
    QVector<int> getRandomList(const int seed, const int size);
    QImage inject(const QImage imgContainer, const QString data);
    QString extract(const QImage imgContainer);
};

#endif // LSBSTEGANOGRAPHY_H
