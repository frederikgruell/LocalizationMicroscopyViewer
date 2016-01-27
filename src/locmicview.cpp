#include "locmicview.h"
 
//Qt includes
#include <QGraphicsScene>
#include <QBrush>
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QTextStream>
#include <QScrollBar>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <QStatusBar>
#include <QMessageBox>
#include <QGridLayout>
#include <QCoreApplication>
#include <QColor> 

#include <iostream>


/**
* Sets up the subclassed QGraphicsView
*/
LocMicView::LocMicView(QWidget* parent)
	: QScrollArea(parent)
{

	imageWidget_ = new ImageWidget(this);
	
	pixmapData_ = 0;
	
	setWidget(imageWidget_);
	setBackgroundRole(QPalette::Base);
	setFrameShape(QFrame::Box);
}

void LocMicView::setStatusBar(QStatusBar *statusBar)
{
	statusBar_ = statusBar;
}


void LocMicView::newPixmap(QRectF dimensionsNm)
{
	dimensionsNm_ = dimensionsNm;
	QPoint sizePx = nmToPx(dimensionsNm.bottomRight());
	int pixmapDataSize = sizePx.x() * sizePx.y();
	pixmapData_ = new QRgb[pixmapDataSize];
	QRgb black = QColor(Qt::black).rgb();
	for(int i = 0; i < pixmapDataSize; i++) {
		pixmapData_[i] = black;
	}
	
//	std::cerr << "New pixmap data with size "
//	          << pixmapDataSize << " created from "
//	          << sizePx.x() << ", " << sizePx.y()
//	          << std::endl;
//	std::cerr << "white " << QColor(Qt::white).rgba() << std::endl;
//	std::cerr << "black " << QColor(Qt::black).rgba() << std::endl;
	          
	scaleFactor_ = 1.0;
	dataPointCount_ = 0;
}

void LocMicView::addDataPoints(std::vector<DataPoint>& dataPoints)
{
	
	QPoint sizePx = nmToPx(dimensionsNm_.bottomRight());	
	
	// Draw a grey ramp
	for(int i = 0; i < 255; i++) {
		pixmapData_[i + sizePx.x()] = i | (i << 8) | (i << 16) | (255 << 24);
	}
	
	int count = 0;
	for(std::vector<DataPoint>::iterator i = dataPoints.begin(); i != dataPoints.end(); ++i, ++count) {
		DataPoint &dp = *i;
		dp.draw(pixmapData_, sizePx, nmToPx(QPointF(dp.x(), dp.y())), pxPerNm_);
		if(count % 10000 == 0) {
			QCoreApplication::processEvents();
            QPoint point = nmToPx(QPointF(dp.x(), dp.y()));
//            std::cerr << "signal " << count << ": (" << point.x() << ", " << point.y() << ")" << std::endl;
		}
	}
}

void LocMicView::showNewPixmap()
{
//	std::cerr << "showing new pixmap ...";
	QPoint sizePx = nmToPx(dimensionsNm_.bottomRight());
	QImage img((uchar*) pixmapData_, sizePx.x(), sizePx.y(), QImage::Format_ARGB32);
	
	QPixmap pixmap = QPixmap::fromImage(img);
	drawScale(&pixmap);
	imageWidget_->setPixmap(pixmap);
	
	scaleFactor_ = 1.0;

	imageWidget_->repaint();
//	std::cerr << "done" << std::endl;
}

void LocMicView::saveImage(QString fileName)
{
	QPixmap pm = imageWidget_->pixmap();	
	
	if (fileName.isEmpty() || pm.isNull() || !pm.save(fileName))
	    QMessageBox::warning( this, "Save failed", "Error saving file" );
}

void LocMicView::closeImage()
{
	imageWidget_->setPixmap(QPixmap());
	scaleFactor_ = 1.0;
	imageWidget_->repaint();
}

void LocMicView::zoomIn()
{
//	std::cerr << "zoomIn()" << std::endl;
	scaleImage(scaleStepFactor);
}

void LocMicView::zoomOut()
{
//	std::cerr << "zoomOut()" << std::endl;
	scaleImage(1.0 / scaleStepFactor);
}

void LocMicView::actualSize()
{
//	std::cerr << "actualSize()" << std::endl;
	
	scaleFactor_ = 1.0;
	imageWidget_->scale(scaleFactor_);
}

void LocMicView::zoomToFit()
{
//	std::cerr << "zoomToFit()" << std::endl;
	
	if(imageWidget_->pixmap().isNull()) return;
	
	float scaleX = ((float) width())  / imageWidget_->pixmap().width();
	float scaleY = ((float) height()) / imageWidget_->pixmap().height();	
	
	scaleFactor_ = std::min(scaleX, scaleY);
	
	imageWidget_->scale(scaleFactor_);
}

//void LocMicView::updateStatusBarPos(QPointF point)
//{
	//if(statusBar) {
		//statusBar->showMessage(QString("(") + QString::number(point.x()) + ";" + QString::number(point.y()) + ")");
	//}
//}




void LocMicView::mousePressEvent(QMouseEvent* event)
{
	//std::cerr << "mousePressEvent" << std::endl;
	QScrollArea::mousePressEvent(event);
	lastPanPoint_ = event->pos();
	imageWidget_->setCursor(Qt::ClosedHandCursor);
}

void LocMicView::mouseReleaseEvent(QMouseEvent* event)
{
	//std::cerr << "mouseReleaseEvent" << std::endl;
	QScrollArea::mouseReleaseEvent(event);
	lastPanPoint_ = QPoint();
	imageWidget_->setCursor(Qt::OpenHandCursor);
}


void LocMicView::mouseMoveEvent(QMouseEvent* event)
{
	//std::cerr << "mouseMoveEvent" << std::endl;
	QScrollArea::mouseMoveEvent(event);
	if(!lastPanPoint_.isNull()) {
		QPoint delta = lastPanPoint_ - event->pos();

		horizontalScrollBar()->setValue(horizontalScrollBar()->value() + delta.x());
		verticalScrollBar()->setValue(verticalScrollBar()->value() +  delta.y());

		lastPanPoint_ = event->pos();
	}
}

void LocMicView::wheelEvent(QWheelEvent* event)
{
	//std::cerr << "wheelEvent " << event->delta() << std::endl;
	
	if(event->delta() < 0) {
		scaleImage(1.0 / scaleStepFactor);
	} else {
		scaleImage(scaleStepFactor);
	}
	
	
	
	//QScrollArea::wheelEvent(event);
}

void LocMicView::resizeEvent(QResizeEvent* event)
{
	//std::cerr << "resizeEvent" << std::endl;
	QScrollArea::resizeEvent(event);
}

bool LocMicView::event(QEvent *e)
{
	//std::cout << "event(QEvent *)" << std::endl;
	return QScrollArea::event(e);
}

void LocMicView::scaleImage(float factor)
{
	if(scaleFactor_ > 1.5 && factor > 1.0) {
		emit zoomInEnabled(false);
		return;
	} else if(scaleFactor_ < 0.25 && factor < 1.0) {
		emit zoomOutEnabled(false);
		return;
	} else {
		emit zoomInEnabled(true);
		emit zoomOutEnabled(true);
	}
	
    Q_ASSERT(imageWidget_->pixmap());
    scaleFactor_ *= factor;
    imageWidget_->scale(scaleFactor_);

    adjustScrollBar(this->horizontalScrollBar(), factor);
    adjustScrollBar(this->verticalScrollBar(), factor);
}


void LocMicView::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}


void LocMicView::drawScale(QPixmap *pixmap)
{
	const int margin = 10;
	const int ScaleInNm = 1000;
	const int minorScaleInNm = 100;
	const int majorTickLength = 10;
	const int minorTickLength = 6;
	const QString caption = QString::fromUtf8("1µm");
	
	QSize size = pixmap->size();

	QPoint beginScale(size.width() - margin - (int) (ScaleInNm * pxPerNm_), size.height() - margin);
	QPoint scaleLength((int) (ScaleInNm * pxPerNm_), 0);
	QPoint endScale = beginScale + scaleLength;

	QPainter painter(pixmap);
	QPen pen(Qt::white);
	pen.setWidth(1);
	painter.setPen(pen);
	painter.setBrush(Qt::white);
	
	// draw base line of the scale
  painter.drawLine(beginScale, endScale);
  
  // draw major ticks at the end
  QPoint beginMajorTick = beginScale - QPoint(0, -majorTickLength / 2);
  QPoint endMajorTick   = beginScale   - QPoint(0, majorTickLength / 2);
  painter.drawLine(beginMajorTick, endMajorTick);
  
  // draw minor ticks between the major ticks
  beginMajorTick += scaleLength;
  endMajorTick   += scaleLength;
  painter.drawLine(beginMajorTick, endMajorTick);
  
  QPoint minorTickDistance = QPoint((int) (minorScaleInNm * pxPerNm_), 0);
  QPoint beginMinorTick = beginScale + QPoint(0, -minorTickLength / 2) + minorTickDistance;
  QPoint endMinorTick   = beginScale + QPoint(0, 0)                    + minorTickDistance;
  for(int i = 1; i < ScaleInNm / minorScaleInNm; i++) {
		painter.drawLine(beginMinorTick, endMinorTick);
		beginMinorTick += minorTickDistance;
		endMinorTick += minorTickDistance;
  }
  
  // draw the caption
  painter.drawText(beginScale + QPoint(-40, 0), caption);
	
}

QPoint LocMicView::nmToPx(QPointF point)
{
	int xPx = (point.x() - dimensionsNm_.x()) * pxPerNm_;
	int yPx = (point.y() - dimensionsNm_.y()) * pxPerNm_;
	
	return QPoint(xPx, yPx);
}


// static
const float LocMicView::scaleStepFactor = 1.25;
const float LocMicView::pxPerNm_ = 0.1;
