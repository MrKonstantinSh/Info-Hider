#ifndef ARC4_H
#define ARC4_H

#include "QVector"

class ARC4
{
public:
    ARC4(const QVector<unsigned char> sessionKey);
    QVector<unsigned char> encrypt(const QVector<unsigned char> plaintext);
    QVector<unsigned char> decrypt(const QVector<unsigned char> ciphertext);
    void swap(unsigned char &firstElem, unsigned char &secElem);
private:
    QVector<unsigned char> sBlock;
    QVector<unsigned char> sessionKey;
    void initializeSBlock(void);
    QVector<unsigned char> genPseudoRandSeq(const int length);
};

#endif // ARC4_H
