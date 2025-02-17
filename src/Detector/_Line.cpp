/*
 * _Line.cpp
 *
 *  Created on: April 23, 2019
 *      Author: yankai
 */

#include "_Line.h"

#ifdef USE_OPENCV

namespace kai
{

_Line::_Line()
{
	m_pV = NULL;
	m_wSlide = 0.01;
	m_minPixLine = 0.005;
	m_line = -1.0;
}

_Line::~_Line()
{
}

bool _Line::init(void *pKiss)
{
	IF_F(!this->_DetectorBase::init(pKiss));
	Kiss *pK = (Kiss*) pKiss;

	pK->v("minPixLine", &m_minPixLine);
	pK->v("wSlide", &m_wSlide);

	m_nClass = 1;

	string n;
	n = "";
	pK->v("_VisionBase", &n );
	m_pV = (_VisionBase*) (pK->getInst( n ));
	IF_Fl(!m_pV, n + ": not found");

	return true;
}

bool _Line::start(void)
{
    NULL_F(m_pT);
	return m_pT->start(getUpdate, this);
}

void _Line::update(void)
{
	while(m_pT->bRun())
	{
		m_pT->autoFPSfrom();

		detect();

		if (m_pT->bGoSleep())
			m_pU->m_pPrev->clear();

		m_pT->autoFPSto();
	}
}

int _Line::check(void)
{
	NULL__(m_pU, -1);
	NULL__(m_pV, -1);
	IF__(m_pV->BGR()->bEmpty(), -1);

	return this->_DetectorBase::check();
}

void _Line::detect(void)
{
    IF_(check()<0);
    
	m_pV->BGR()->m()->copyTo(m_mIn);
	float nP = m_mIn.rows * m_mIn.cols;

	Mat mR;
	cv::reduce(m_mIn, mR, 1, cv::REDUCE_SUM, CV_32SC1);
	mR *= (float) 1.0 / 255.0;

	//sliding window
	int i, j;
	int iFrom = -1;
	int iTo = -1;
	int nSlide = m_wSlide * m_mIn.rows * 0.5;

	for (i = 0; i < mR.rows; i++)
	{
		int sFrom = constrain(i - nSlide, 0, mR.rows);
		int sTo = constrain(i + nSlide, 0, mR.rows);

		int v = 0;
		for (j = sFrom; j < sTo; j++)
			v += mR.at<int>(j, 0);

		if ((float) v / nP > m_minPixLine)
		{
			iFrom = sFrom;
            iTo = sTo;
			break;
		}
	}

	if (iFrom < 0)
	{
		m_line = -1.0;
		return;
	}

	float kx = 1.0/m_mIn.cols;
	float ky = 1.0/m_mIn.rows;

	Rect rBB;
	rBB.x = 0;
	rBB.y = iFrom;
	rBB.width = m_mIn.cols;
	rBB.height = iTo - iFrom;

	_Object o;
	o.init();
//	o.m_tStamp = m_pT->getTfrom();
	o.setBB2D(rect2BB <vFloat4> (rBB));
	o.scale(kx,ky);
	o.setTopClass(0, 1.0);
	m_pU->add(o);

	m_line = o.getY();
	LOG_I("Line pos: " + f2str(m_line));

	m_pU->updateObj();

}

void _Line::draw(void)
{
	this->_DetectorBase::draw();
	addMsg("line = " + f2str(m_line), 1);
}

}
#endif
