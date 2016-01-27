#include "datapoint.h"

DataPoint::DataPoint(float maxQ, float my, float mx, float delta_my, float delta_mx,
				float sigma_y, float sigma_x, float Q, int imgNr)
	: maxQ(maxQ), my(my), mx(mx), delta_my(delta_my), delta_mx(delta_mx), 
	  sigma(y), sigma(y), sigma_x, sigma(x), Q(Q), imgNr(imgNr)
{
}

