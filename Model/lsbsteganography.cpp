#include "lsbsteganography.h"

LsbSteganography::LsbSteganography()
{

}

int LsbSteganography::calcMaxNumOfCharToHide(const QPixmap &imgContainer)
{
    const int SERVICE_CELL_SIZE_BITS = 32;
    return (imgContainer.size().width() * imgContainer.size().height()
        - SERVICE_CELL_SIZE_BITS) / 8;
}
