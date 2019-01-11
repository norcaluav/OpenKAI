/*
 * _DNNdetect.h
 *
 *  Created on: Jan 11, 2019
 *      Author: yankai
 */

#ifndef OpenKAI_src_DNN_Darknet__DNNdetect_H_
#define OpenKAI_src_DNN_Darknet__DNNdetect_H_

#include "../Base/common.h"
#include "../Vision/_VisionBase.h"
#include "../Base/_ObjectBase.h"

namespace kai
{

class _DNNdetect: public _ObjectBase
{
public:
	_DNNdetect();
	~_DNNdetect();

	bool init(void* pKiss);
	bool start(void);
	bool draw(void);
	int check(void);

private:
	bool detect(void);
	void update(void);
	static void* getUpdateThread(void* This)
	{
		((_DNNdetect*) This)->update();
		return NULL;
	}

public:
	cv::dnn::Net m_net;
	double	m_thr;
	double	m_nms;
	int		m_nW;
	int		m_nH;
	Mat		m_blob;
	vector<string> m_vLayerName;

	int m_iBackend;
	int m_iTarget;

};

}
#endif
