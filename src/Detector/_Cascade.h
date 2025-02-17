/*
 * _Cascade.h
 *
 *  Created on: Aug 17, 2016
 *      Author: yankai
 */

#ifndef OpenKAI_src_Detector__Cascade_H_
#define OpenKAI_src_Detector__Cascade_H_
#ifdef USE_OPENCV
#ifdef USE_CUDA
#include "../Detector/_DetectorBase.h"

namespace kai
{

class _Cascade: public _DetectorBase
{
public:
	_Cascade();
	~_Cascade();

	bool init(void* pKiss);
	bool start(void);
	void draw(void);
	int check(void);

private:
	void detectCPU(void);
	void detectGPU(void);
	void update(void);
	static void* getUpdate(void* This)
	{
		((_Cascade*) This)->update();
		return NULL;
	}

public:
	cv::CascadeClassifier	m_CC;
	Ptr<cuda::CascadeClassifier> m_pGCC;

	bool m_bGPU;
	double m_scaleFactor;
	int m_minNeighbors;
	string m_className;

};

}
#endif
#endif
#endif
