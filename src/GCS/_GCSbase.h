#ifndef OpenKAI_src_GCS__GCSbase_H_
#define OpenKAI_src_GCS__GCSbase_H_

#include "../Mission/_MissionBase.h"

namespace kai
{

struct GCS_STATE
{
    int8_t STANDBY;
    int8_t TAKEOFF_REQUEST;
    int8_t TAKEOFF_READY;
    int8_t AIRBORNE;
	int8_t LANDING_REQUEST;
    int8_t LANDING_READY;

	bool bValid(void)
	{
		IF_F(STANDBY < 0);
		IF_F(TAKEOFF_REQUEST < 0);
		IF_F(TAKEOFF_READY < 0);
		IF_F(AIRBORNE < 0);
		IF_F(LANDING_REQUEST < 0);
		IF_F(LANDING_READY < 0);

		return true;
	}
	
	bool assign(_MissionControl* pMC)
    {
        NULL_F(pMC);
        STANDBY = pMC->getMissionIdxByName ("STANDBY");
        TAKEOFF_REQUEST = pMC->getMissionIdxByName ("TAKEOFF_REQUEST");
        TAKEOFF_READY = pMC->getMissionIdxByName ("TAKEOFF_READY");
        AIRBORNE = pMC->getMissionIdxByName ("AIRBORNE");
        LANDING_REQUEST = pMC->getMissionIdxByName ("LANDING_REQUEST");
        LANDING_READY = pMC->getMissionIdxByName ("LANDING_READY");
        
        return bValid();
    }
};

class _GCSbase: public _MissionBase
{
public:
	_GCSbase();
	~_GCSbase();

	virtual bool init(void* pKiss);
	virtual int check(void);
	virtual void draw(void);
    
    virtual int getID (void);    
    virtual bool landingRequest(int vID);
    virtual bool bLandingReady(int vID);
    virtual void landingStatus(int vID);
    virtual bool takeoffRequest(int vID);
    virtual bool bTakeoffReady(int vID);
    virtual void takeoffStatus (int vID);

protected:
    virtual void updateGCS (void);

public:
    int m_gcsID;

    GCS_STATE m_state;
};

}
#endif
