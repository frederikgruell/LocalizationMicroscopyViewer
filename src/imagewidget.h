#ifndef __IMAGEWIDGET_H__
#define __IMAGEWIDGET_H__

#include <QWidget>

class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    ImageWidget( QWidget *parent = 0);
    ~ImageWidget();
    
    void setPixmap(const QPixmap& newPixmap);
    QPixmap& pixmap();
    void scale(float factor);
    void saveImage(QString fileName);
    
protected:
    void paintEvent( QPaintEvent *);
    void resizeEvent( QResizeEvent *);


private:

    QPixmap	pm_;
    QPixmap pmScaled_;
    float scaleFactor_;

private slots:



};

#endif // __IMAGEWIDGET_H__
