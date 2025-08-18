#pragma once

#include <QTextBrowser>
#include <QTimer>

class ImageTextBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    explicit ImageTextBrowser(QWidget* parent = nullptr);

protected:
    QVariant loadResource(int type, const QUrl & name) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QTimer* mResizeTimer = nullptr;
};
