#ifndef __DATAPOINT_H__
#define __DATAPOINT_H__


#include <QImage> 

class DataPoint {
	public:
		DataPoint(float x, float y, float deltaX, float deltaY);	
	
		float x() const      { return x_; }
		float y() const      { return y_; }
		float deltaX() const { return deltaX_; }
		float deltaY() const { return deltaY_; }		

		void draw(QRgb *pixmapData, QPoint sizePx, QPoint posPx, float pxPerNm);
		
		static uchar** getGaussImage(int widthPx);
		static uchar calcGauss(float x, float y, float width);
		
	private:
		static int imgRadius(int deltaPx);
		static const int maxBrightness;	
	
		float x_;
		float y_;
		float deltaX_;
		float deltaY_;
};

#endif // __DATAPOINT_H__
