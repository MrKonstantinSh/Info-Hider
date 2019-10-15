#include <QException>
#include "imagecontainer.h"

ImageContainer::ImageContainer(const QString pathToImgContainer)
{
    if (pathToImgContainer.size() > 0) {
        this->imgContainer = QPixmap(pathToImgContainer);
        this->pathToImgContainer = pathToImgContainer;
        this->imgContainerResolution = QString::number(this->imgContainer.size().width())
            + "x" + QString::number(this->imgContainer.size().height());
    } else {
        throw QString("Choose the correct file path!");
    }
}

QPixmap ImageContainer::getImgContainer()
{
    return this->imgContainer;
}

QString ImageContainer::getImgContainerResolution()
{
    return this->imgContainerResolution;
}
