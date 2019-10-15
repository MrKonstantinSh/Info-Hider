#ifndef IMAGECONTAINER_H
#define IMAGECONTAINER_H

#include <QPixmap>

class ImageContainer
{
public:
    ImageContainer(const QString pathToImgContainer);
    QPixmap getImgContainer();
    QString getImgContainerResolution();
private:
    QPixmap imgContainer;
    QString pathToImgContainer;
    QString imgContainerResolution;
};

#endif // IMAGECONTAINER_H
