#pragma once
#include "CNCInterface.hpp"

using namespace CNCControl;

enum JogMode{
	sd,
	joginching,
	jogcontinue,
	jogstep,
	joghome

};
enum AxisNumber{

	AN_X, AN_Y, AN_Z

};
struct ManualMode{
	JogMode jogmode;
	bool jogPositive;
	double jogSpeed;
	double jogstepDistance;
	AxisNumber axisnum;
};



class   CWJSCNCInterface :public CNCInterface

{
	public:
		CWJSCNCInterface(const char *libpath = NULL);
		~CWJSCNCInterface();
	//ManualMode CNCmanualmode;
	double m_jogstepdis;
	
};

