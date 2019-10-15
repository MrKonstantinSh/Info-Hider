#include "texttohide.h"
#include "Model/lsbsteganography.h"

TextToHide::TextToHide(const QPixmap &imgContainer)
{
    this->maxLength = LsbSteganography::calcMaxNumOfCharToHide(imgContainer);
}

int TextToHide::getMaxLength()
{
    return this->maxLength;
}
