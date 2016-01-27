#ifndef __LOCMICVIEW_H__
#define __LOCMICVIEW_H__

#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QStatusBar>
#include <QLabel>

#include <map>
#include <vector>

#include "imagewidget.h"
#include "datapoint.h"
 
class LocMicView : public QScrollArea
{
	Q_OBJECT;
	
public:
	LocMicView(QWidget* parent = NULL);
	
	void setStatusBar(QStatusBar *statusbar);
	
	void newPixmap(QRectF dimensionsNm);
	//void addDataPoints(std::map<int, DataPoint>& dataPoints);
	void addDataPoints(std::vector<DataPoint>& dataPoints);
	void showNewPixmap();
	void saveImage(QString fileName);
	void closeImage();
	
public slots:
	void zoomIn();
	void zoomOut();
	void actualSize();
	void zoomToFit();
	
	
signals:
	void zoomInEnabled(bool enabled);
	void zoomOutEnabled(bool enabled);
protected:
 
	//Take over the interaction
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void wheelEvent(QWheelEvent* event);
	virtual void resizeEvent(QResizeEvent* event);
	virtual bool event(QEvent *e);
	
private:
	void scaleImage(float factor);
	void adjustScrollBar(QScrollBar *scrollBar, double factor);

	void drawScale(QPixmap *pixmap);
	void drawDataPoint(DataPoint const& dp);
	
	QImage const& getGaussImage(int width, int imageRadius);
	int calcGauss(float x, float y, float width);

	QPoint nmToPx(QPointF p);

	QStatusBar *statusBar_;
	ImageWidget *imageWidget_;
	QRgb   *pixmapData_;
	float scaleFactor_;
	QPoint lastPanPoint_;
	QRectF dimensionsNm_;
	
	static const float scaleStepFactor;
	static const float pxPerNm_;
	int dataPointCount_;
};

#endif // __LOCMICVIEW_H__
