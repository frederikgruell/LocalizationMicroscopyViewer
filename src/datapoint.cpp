#include "datapoint.h"

#include <QPainter>

#include <cmath>
#include <algorithm>
#include <map>
#include <iostream>

DataPoint::DataPoint(float x, float y, float deltaX, float deltaY)
 :  x_(x), y_(y), deltaX_(deltaX), deltaY_(deltaY)
{}


void DataPoint::draw(QRgb *pixmapData, QPoint sizePx, QPoint posPx, float pxPerNm)
{	
	//QPoint redPosPx = posPx;
	
	float deltaNm = (deltaX_ + deltaY_) / 2.0;
	int deltaPx = deltaNm * pxPerNm;
    deltaPx = std::max(1, deltaPx);
	//std::cerr << "deltaPx       :  " << deltaPx << std::endl;
	int imgRadiusPx = imgRadius(deltaPx);
	//std::cerr << "imgRadiusPx   :  " << imgRadiusPx << std::endl;
	
	//std::cerr << "posPx before  : (" << posPx.x() << ", " << posPx.y() << ")" << std::endl; 
	posPx -= QPoint(imgRadiusPx, imgRadiusPx);
	//std::cerr << "posPx after   : (" << posPx.x() << ", " << posPx.y() << ")" << std::endl;

	//painter.drawImage(posPx, getGaussImage(deltaPx));
	uchar **img = getGaussImage(deltaPx);
	
	QRgb maxValue = 0;
	int length = 2 * imgRadiusPx;
	for(int x = 0; x < length; x++) {
		for(int y = 0; y < length; y++) {
			int imgX = x + posPx.x();
			int imgY = y + posPx.y(); 
			if(imgX < 0 || imgX >= sizePx.x() || imgY < 0 || imgY >= sizePx.y())
				continue;
			
			int oldValue = pixmapData[imgX + imgY * sizePx.x()] & 255;
			QRgb newValue = (uchar) std::min<int>(maxBrightness, oldValue + img[x][y]); 
			pixmapData[imgX + imgY * (int) sizePx.x()] = newValue | (newValue << 8) | (newValue << 16) | (255 << 24);
			maxValue = std::max<QRgb>(maxValue, newValue | (newValue << 8) | (newValue << 16) | (255 << 24)); 
		}
	}
	
	//std::cerr << maxValue << std::endl;
	
	//painter.setPen(Qt::red);
	//painter.drawPoint(redPosPx.x(), redPosPx.y());
}



//static
uchar** DataPoint::getGaussImage(int widthPx)
{	
	static std::map<int, uchar**> imgs;
	std::map<int, uchar**>::iterator it = imgs.find(widthPx);
	if(it != imgs.end()) {
		return it->second;
	}

	int imgRadiusPx = imgRadius(widthPx);

	int length = 2 * imgRadiusPx;
	uchar** image = new uchar*[length];
	for(int x = 0; x < length; x++) {
		image[x] = new uchar[length];
	}
	
	for(int x = 0; x < length; x++) {
		for(int y = 0; y < length; y++) {
			uchar grayValue = calcGauss(x - imgRadiusPx, y - imgRadiusPx, widthPx);
			image[x][y] = std::min<uchar>(grayValue, 255);
		}
	}
	
	it = imgs.insert(std::pair<int, uchar**>(widthPx, image)).first;
	return it->second;
}


uchar DataPoint::calcGauss(float x, float y, float width)
{
    //return 100;
	
	const float pi = 3.14159265;
	const float Q = 2.0;
	
	float width2 = width * width;
	float x_over_width2 = x * x / width2;
	float y_over_width2 = y * y / width2;
	float value = Q * 256.0 / (2 * pi * width2) * exp(-(x_over_width2 + y_over_width2) / 2.0);
	return std::min<int>(value, maxBrightness);
	
}

// private static
int DataPoint::imgRadius(int deltaPx)
{
	return 4 *  deltaPx;
}

const int DataPoint::maxBrightness = 255;