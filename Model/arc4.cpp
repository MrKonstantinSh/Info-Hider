#include "arc4.h"

ARC4::ARC4(const QVector<unsigned char> sessionKey)
{
    this->sBlock = QVector<unsigned char>(256);
    this->sessionKey = sessionKey;
}

void ARC4::swap(unsigned char &firstElem, unsigned char &secElem)
{
    unsigned char temp = firstElem;
    firstElem = secElem;
    secElem = temp;
}

void ARC4::initializeSBlock(void)
{
    for (int i = 0; i < 256; i++)
    {
        sBlock[i] = char(i);
    }

    int j = 0;
    for (int i = 0; i < 256; i++)
    {
        j = (j + sBlock[i] + sessionKey[i % sessionKey.size() % 256]) % 256;
        swap(sBlock[i], sBlock[j]);
    }
}

QVector<unsigned char> ARC4::genPseudoRandSeq(const int length)
{
    initializeSBlock();
    QVector<unsigned char> key(length);

    int i = 0;
    int j = 0;

    for (int k = 0; k < length; k++)
    {
        i = (i + 1) % 256;
        j = (j + sBlock[i]) % 256;
        swap(sBlock[i], sBlock[j]);
        int t = (sBlock[i] + sBlock[j]) % 256;
        key[k] = sBlock[t];
    }

    return key;
}

QVector<unsigned char> ARC4::encrypt(const QVector<unsigned char> plaintext)
{
    QVector<unsigned char> key = genPseudoRandSeq(plaintext.size());
    QVector<unsigned char> ciphertext(plaintext.size());

    for (int i = 0; i < plaintext.size(); i++)
    {
        ciphertext[i] = plaintext[i] ^ key[i];
    }

    return ciphertext;
}

QVector<unsigned char> ARC4::decrypt(const QVector<unsigned char> ciphertext)
{
    QVector<unsigned char> key = genPseudoRandSeq(ciphertext.size());
    QVector<unsigned char> plaintext(ciphertext.size());

    for (int i = 0; i < ciphertext.size(); i++)
    {
        plaintext[i] = ciphertext[i] ^ key[i];
    }

    return plaintext;
}
