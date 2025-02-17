/*
 * _DepthSegment.cpp
 *
 *  Created on: May 29, 2019
 *      Author: yankai
 */

#include "_DepthSegment.h"

#ifdef USE_OPENCV

namespace kai
{

_DepthSegment::_DepthSegment()
{
	m_rL = 0.0;
	m_rH = 1.0;
	m_rD = 0.1;
	m_rArea = 0.8;
}

_DepthSegment::~_DepthSegment()
{
}

bool _DepthSegment::init(void *pKiss)
{
	IF_F(!this->_DetectorBase::init(pKiss));
	Kiss *pK = (Kiss*) pKiss;

	pK->v<float>("rL", &m_rL);
	pK->v<float>("rH", &m_rH);
	pK->v<float>("rD", &m_rD);
	pK->v<float>("rArea", &m_rArea);

	m_nClass = 1;

	return true;
}

bool _DepthSegment::start(void)
{
    NULL_F(m_pT);
	return m_pT->start(getUpdate, this);
}

void _DepthSegment::update(void)
{
	while(m_pT->bRun())
	{
		m_pT->autoFPSfrom();

		if (check() >= 0)
		{
			detect();

			if (m_pT->bGoSleep())
				m_pU->m_pPrev->clear();
		}

		m_pT->autoFPSto();
	}
}

int _DepthSegment::check(void)
{
	NULL__(m_pV, -1);
	NULL__(m_pU, -1);
	IF__(m_pV->BGR()->bEmpty(), -1);

	return this->_DetectorBase::check();
}

void _DepthSegment::detect(void)
{
	Mat m;
	m_pV->BGR()->m()->copyTo(m);

	float kx = 1.0 / m.cols;
	float ky = 1.0 / m.rows;

	vector<vector<Point> > vvC;
	for (float r = m_rL; r < m_rH; r += m_rD)
	{
		cv::inRange(m, 1, (r - m_rL) * 255.0 + 2, m_mR);
		findContours(m_mR, vvC, RETR_EXTERNAL, CHAIN_APPROX_NONE);

		_Object o;
		vector<Point> vPoly;
		float rArea = 0.0;
		for (unsigned int i = 0; i < vvC.size(); i++)
		{
			vPoly.clear();
			approxPolyDP(vvC[i], vPoly, 3, true);
			Rect re = boundingRect(vPoly);

			o.init();
//			o.m_tStamp = m_pT->getTfrom();
			o.setBB2D(rect2BB<vFloat4>(re));
			o.scale(kx, ky);
			o.setZ(r);
			o.setTopClass(0, o.area());

			//TODO: classify

			m_pU->add(o);
			rArea += o.area();
		}

		if (rArea > m_rArea)
			break;
	}

	m_pU->updateObj();
}

void _DepthSegment::draw(void)
{
	this->_DetectorBase::draw();

	if (!m_mR.empty())
	{
		imshow(*this->getName() + ":Thr", m_mR);
	}
}

}
#endif
