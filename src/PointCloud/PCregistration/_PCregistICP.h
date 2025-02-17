/*
 * _PCregistICP.h
 *
 *  Created on: Sept 6, 2020
 *      Author: yankai
 */

#ifndef OpenKAI_src_PointCloud_PCregistICP_H_
#define OpenKAI_src_PointCloud_PCregistICP_H_

#ifdef USE_OPEN3D
#include "../PCfilter/_PCtransform.h"
using namespace open3d::pipelines::registration;

namespace kai
{
    
enum PCREGIST_ICP_EST
{
    icp_p2point = 0,
    icp_p2plane = 1,
};

class _PCregistICP: public _ModuleBase
{
public:
	_PCregistICP();
	virtual ~_PCregistICP();

	bool init(void* pKiss);
	bool start(void);
	int check(void);
	void draw(void);

private:
	void updateRegistration(void);
	void update(void);
	static void* getUpdate(void* This)
	{
		(( _PCregistICP *) This)->update();
		return NULL;
	}

public:
	float m_thr;	//ICP threshold
	PCREGIST_ICP_EST m_est;
	_PCframe* m_pSrc;
	_PCframe* m_pTgt;
    RegistrationResult m_RR;
	_PCtransform* m_pTf;
    double m_lastFit;
};

}
#endif
#endif
