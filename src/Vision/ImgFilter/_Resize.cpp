/*
 * _Resize.cpp
 *
 *  Created on: April 23, 2019
 *      Author: yankai
 */

#include "_Resize.h"

#ifdef USE_OPENCV

namespace kai
{

_Resize::_Resize()
{
	m_type = vision_resize;
	m_pV = NULL;
}

_Resize::~_Resize()
{
	close();
}

bool _Resize::init(void* pKiss)
{
	IF_F(!_VisionBase::init(pKiss));
	Kiss* pK = (Kiss*) pKiss;

	string n;
	n = "";
	pK->v("_VisionBase", &n);
	m_pV = (_VisionBase*) (pK->getInst(n));
	IF_Fl(!m_pV, n + ": not found");

	return true;
}

bool _Resize::open(void)
{
	NULL_F(m_pV);
	m_bOpen = m_pV->isOpened();

	return m_bOpen;
}

void _Resize::close(void)
{
	this->_VisionBase::close();
}

bool _Resize::start(void)
{
    NULL_F(m_pT);
	return m_pT->start(getUpdate, this);
}

void _Resize::update(void)
{
	while(m_pT->bRun())
	{
		if (!m_bOpen)
			open();

		m_pT->autoFPSfrom();

		if(m_bOpen)
		{
			if(m_fIn.tStamp() < m_pV->BGR()->tStamp())
				filter();
		}

		m_pT->autoFPSto();
	}
}

void _Resize::filter(void)
{
	IF_(m_pV->BGR()->bEmpty());

	m_fBGR.copy(m_pV->BGR()->resize(m_vSize.x, m_vSize.y));
}

}
#endif
