#include "ImageTextBrowser.h"

#include <QDebug>
#include <QResizeEvent>

ImageTextBrowser::ImageTextBrowser(QWidget* parent)
    : QTextBrowser(parent)
    , mResizeTimer(new QTimer(this))
{
    mResizeTimer->setSingleShot(true);
    mResizeTimer->setInterval(300);
    connect(mResizeTimer, &QTimer::timeout, this, [this]()
    {
        qDebug() << "timeout";
        setText(toHtml());
    });
}

QVariant ImageTextBrowser::loadResource(int type, const QUrl & name)
{
    auto url = name.toString();
    if(url.startsWith("http"))
    {
        // TODO: download
    }
    else if(url.startsWith("data:"))
    {
        auto base64Index = url.indexOf(";base64,");
        if(base64Index != -1)
        {
            auto data = QByteArray::fromBase64(url.mid(base64Index + 8).toUtf8());
            auto image = QImage::fromData(data);
            auto maxWidth = document()->textWidth() - document()->documentMargin() * 2 - 1;
            if(image.width() > maxWidth)
            {
                image = image.scaledToWidth((int)maxWidth, Qt::SmoothTransformation);
            }
            return image;
        }
    }
    return QTextBrowser::loadResource(type, name);
}

void ImageTextBrowser::resizeEvent(QResizeEvent* event)
{
    if(event->size() != event->oldSize())
    {
        //qDebug() << "resizeEvent";
        //mResizeTimer->start();
    }
    QTextBrowser::resizeEvent(event);
}
