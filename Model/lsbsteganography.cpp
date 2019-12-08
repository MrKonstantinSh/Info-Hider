#include <QBitArray>
#include <QByteArray>
#include <QtGui>

#include "lsbsteganography.h"

const int NUM_OF_BITS_IN_BYTE = 8;

LsbSteganography::LsbSteganography()
{

}

int LsbSteganography::calcMaxNumOfCharToHide(const QPixmap &imgContainer)
{
    const int SCSB = 32;
    return (imgContainer.size().width() * imgContainer.size().height()
        - SCSB) / 8;
}

QBitArray LsbSteganography::convertToBitArray(const QByteArray &byteArray)
{
    const int quantityBit = byteArray.size() * NUM_OF_BITS_IN_BYTE;
    int index = 0;

    if (quantityBit <= 0)
        return QBitArray();

    QBitArray bitArray(quantityBit);
    for (int byte : byteArray)
    {
        for (int bit = 0; bit < NUM_OF_BITS_IN_BYTE; bit++)
        {
            if (byte & 0b00000001)
                bitArray.setBit(index, true);

            byte >>= 1;
            index++;
        }
    }

    return bitArray;
}

QByteArray LsbSteganography::convertToByteArray(const QBitArray &bitArray)
{
    const int quantityBit = bitArray.count();
    const int quantityByte = (quantityBit + 7) / 8;

    if (quantityByte <= 0)
        return QByteArray();

    QByteArray byteArray(quantityByte, 0);

    int index = 0;

    for (int i = 0; i < quantityBit;)
    {
        unsigned char currentByte = 0;
        for (int bit = 0; bit < NUM_OF_BITS_IN_BYTE; bit++)
        {
            currentByte >>= 1;
            if (i < quantityBit && bitArray[i++])
                currentByte |= 0x80;
        }
        byteArray[index++] = currentByte;
    }

    return byteArray;
}

QVector<int> LsbSteganography::getRandomList(const int seed, const int size)
{
    QVector<int> randomList(size);
    for (int i = 0; i < size; i++)
    {
        randomList[i] = i;
    }

    std::srand(seed);
    std::random_shuffle(randomList.begin(), randomList.end());

    return randomList;
}

QImage LsbSteganography::inject(const QImage imgContainer, const QString data)
{
    const int RANDOM_SEED = 123;
    const int CONTAINER_WIDTH = imgContainer.size().width();
    const int CONTAINER_HEIGHT = imgContainer.size().height();
    const int PIXEL_COUNT = CONTAINER_WIDTH * CONTAINER_HEIGHT;
    const int COLOR_COUNT = PIXEL_COUNT * 3;

    QByteArray dataBytes = data.toUtf8();
    QVector<unsigned char> colorList(COLOR_COUNT);
    int index = 0;

    for (int y = 0; y < CONTAINER_HEIGHT; y++)
    {
        for (int x = 0; x < CONTAINER_WIDTH; x++)
        {
            QRgb rgb = imgContainer.pixel(x, y);
            colorList[index++] = char(qRed(rgb));
            colorList[index++] = char(qGreen(rgb));
            colorList[index++] = char(qBlue(rgb));
        }
    }

    QVector<int> randomList = getRandomList(RANDOM_SEED, COLOR_COUNT);

    int sizeDataBytes = dataBytes.size();
    QByteArray dataWithSize;
    dataWithSize.append((char*)&sizeDataBytes, sizeof(int));
    dataWithSize.append(dataBytes);

    QBitArray dataBits = convertToBitArray(dataWithSize);

    const int quantityBit = qMin(COLOR_COUNT, dataBits.count());

    int changeBitCounter = 0;

    for (int i = 0; i < quantityBit; i++)
    {
        int randomNumber = randomList[i];

        if (!dataBits[i])
        {
            if (colorList[randomNumber] & 0x0001)
                changeBitCounter++;

            colorList[randomNumber] &= 0x00FE;
        }

        if (dataBits[i])
        {
            if (!(colorList[randomNumber] & 0x0001))
                changeBitCounter++;

            colorList[randomNumber] |= 0x0001;
        }
    }

    index = 0;

    QImage resultImg = QImage(CONTAINER_WIDTH, CONTAINER_HEIGHT, QImage::Format_RGB888);

    for (int y = 0; y < CONTAINER_HEIGHT; y++)
    {
        for (int x = 0; x < CONTAINER_WIDTH; x++)
        {
            const int red = colorList[index++];
            const int green = colorList[index++];
            const int blue = colorList[index++];
            const QRgb rgb = qRgb(red, green, blue);

            resultImg.setPixel(x, y, rgb);
        }
    }

    return resultImg;
}

QString LsbSteganography::extract(const QImage imgContainer)
{
    const int RANDOM_SEED = 123;
    const int CONTAINER_WIDTH = imgContainer.size().width();
    const int CONTAINER_HEIGHT = imgContainer.size().height();
    const int PIXEL_COUNT = CONTAINER_WIDTH * CONTAINER_HEIGHT;
    const int COLOR_COUNT = PIXEL_COUNT * 3;

    QVector<int> randomList = getRandomList(RANDOM_SEED, COLOR_COUNT);

    QVector<unsigned char> rgbList(COLOR_COUNT);
    int index = 0;

    for (int x = 0; x < CONTAINER_HEIGHT; x++)
    {
        for (int y = 0; y < CONTAINER_WIDTH; y++)
        {
            QRgb rgb = imgContainer.pixel(y, x);
            rgbList[index++] = char(qRed(rgb));
            rgbList[index++] = char(qGreen(rgb));
            rgbList[index++] = char(qBlue(rgb));
        }
    }

    QBitArray num(32);
    for (int i = 0; i < 32; i++)
    {
        int randomNum = randomList[i];

        if (rgbList[randomNum] & 0x0001)
            num.setBit(i, true);
        else
            num.setBit(i, false);
    }

    QByteArray dataBytes = convertToByteArray(num);
    int arrg = *(int*)dataBytes.data();

    QByteArray dataWithSize;
    int rt = arrg * 8;
    QBitArray message(rt);
    int randomNum;

    for (int i = 32, j = 0; i < COLOR_COUNT && j < rt; i++, j++)
    {
        randomNum = randomList[i];
        if (!(rgbList[randomNum] & 0x0001))
        {
            message.setBit(j, false);
        }

        if (rgbList[randomNum] & 0x0001)
        {
            message.setBit(j, true);
        }
    }

    QByteArray messageBytes = convertToByteArray(message);

    return QString::fromUtf8(messageBytes.constData());
}
