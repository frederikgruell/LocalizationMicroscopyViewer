#ifndef __DATAPOINT_H__
#define __DATAPOINT_H__

struct DataPoint {
	
	DataPoint(float maxQ, float my, float mx, float delta_my, float delta_mx,
				float sigma_y, float sigma_x, float Q, int imgNr);
	
	float maxQ;
	float my;
	float mx;
	float delta_my;
	float delta_mx;
	float sigma_y;
	float sigma_x;
	float Q;
	int imgNr;
}


#endif // __DATAPOINT_H__
