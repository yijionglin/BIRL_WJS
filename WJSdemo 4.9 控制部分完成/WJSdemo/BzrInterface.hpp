/***************************************************************************
  tag: Rock Chen  Friday October 21 2016,  BzrInterface.hpp

                        BzrInterface.hpp -  description
                           -------------------
    begin                : October 21 2016
    copyright            : (C) 2016 Rock Chen
    email                : rockcdr@bz-intelligentrobot.com

******************************************************************************/
 /*****************************************************************************
 *  \author
 *  	Rock Chen, BZ Robot INC
 *
 *  \version
 *	           BZROS_TASK V0.1
 *
 *	\par History
 *		- $log$
 *
 *	\par Release
 *		$Id: BzrInterface.hpp$
 *		$Name:  $
 *  
 ****************************************************************************/

/**
 * \file BzrInterface.hpp
 *
 * This is the C++ Interface for Bzr clients.
 *
 *
**/

#ifndef BZROS_BZRINTERFACE_HPP
#define BZROS_BZRINTERFACE_HPP

#include "Bzr.hpp"
#include <string>

enum BUnits {
    URatio,      //[0..1]
    UPercent,    //[0..100]
    URadian,
    UDegree,
    UMillimeter,
    UMeter
};

/*
 * Sub-classes may support different applications with extensions. 
 * all 'int index' for robot, are 0 based.
 * all 'int axis' mean 0 based index.
 */
class BZR_API BzrInterface {
public:
    BzrInterface(const char *libpath = NULL);
    virtual ~BzrInterface();
    
    //Optional, if there are multiple controllers in this local network
    int scanControllers(double timeoutSec = 2.0);

    /**
     * @brief Connect to the remote controller with IP address
     * @param ip The IP address string, e.g. "192.168.3.100"; if ip == NULL, auto call scanControllers(), then use the first scanned IP address.
     * @param waitSeconds The timeout for connecting
     * @param waitSecondsReconnect The timeout to start re-connecting
     * @return 0 when no errors.
     */
    int connect(const char *ip = NULL, int waitSeconds = 5, double waitSecondsReconnect = 5.0);
    
    int destroy();
    
    bool connected();
    
    void setIpAddr(const char *ip) { ipAddr = ip; }
    std::string getIpAddr() { return ipAddr; }
    
    int getNumOfIOChannels(IOChannels type); //get the number of IOs of a given type (DI, DO, SI,SO,AI,AO,SF) 
    
    //safety in use to: emergency-stop, deadman-button, mode-switch
    bool hasSafetyInUse();
    

    int getDI(int _io_number, int &val) {
        return bzrGetDI(h,_io_number,val);
    }
    
    int getAI(int _io_number,double &val) {
        return bzrGetAI(h,_io_number,val);
    }
    
    int setDO(int _io_number, int val) {
        return bzrSetDO(h,_io_number,val);
    }
    
    int setAO(int _io_number, double val) {
        return bzrSetAO(h,_io_number,val);
    }
    
    //get the number of robots that this controller controls
    int getNumOfRobots();
    
    //for each robot got from the previous API, query its info (robot name and number of axes)
    int getRobotInfo(int index, RobotInfo& robotInfo);
    
    //After select operation, all subsequent operations use the selected one.
    void setCurrentRobot(int index);
    int currentRobot();


    int getAxisInfo(int axis, JointInfo& jntInfo);
    int setSpeed(double val, BUnits unit);
    int getSpeed(double &val, BUnits unit);
    int sleep(int ms);
    
    /**
     * @brief Set units for subsequent operations
     * @param rotate: Radian or Degree;
     * @param linear: Millimeter or Meter;
     * @return 0 when no errors.
     */
    int setUnits(BUnits rotate, BUnits linear);
    int getUnits(BUnits &rotate, BUnits &linear);
    
    int teachBase(int method, int idBase, int idPoint, EFrame &frame);
    int teachTool(int method, int idTool, int idPoint, EFrame &frame);
    int setBaseTool(int baseNo, int toolNo);
    
    /**
     * @brief Get the current system status
     * @param type ST_EMO,...,ST_PROG;
     * @param status: Output value;
     * @return 0 when no errors.
     */
    int getSystemStatus(int type, int &status);
    
    int servoOn(bool enable);

    /**
     * @brief Change the current operation mode
     * @param mode MODE_MANUAL,...,MODE_AUTO;
     * @return 0 when no errors.
     */
    int setMode(int mode);
    
    /**
     * @brief Reset alarm
     * @return 0 when no errors.
     */
    int resetAlarm();
    
    int start();
    int stop();
    int cancel();
    int suspend(bool pause);  //pause or resume
    
    int homeRob(double spd_pris=-1.0, double spd_revolute=-1.0);
    int homeJoint(JogJoint joint,double spd_pris=-1.0, double spd_revolute=-1.0);
    int getRobCartProfLimits(double & max_vel, double &max_acc, double &max_jerk);
    
    /*
     *  JogSpace {
    ID_CART_WORLD=1,
    ID_CART_TOOL=2,
    ID_CART_BASE=3,
    ID_JOINT=4
};
     * 
     * 
     * JogJoint{
    ID_X_A1=1,
    ID_Y_A2=2,
    ID_Z_A3=3,
    ID_A_A4=4,
    ID_B_A5=5,
    ID_C_A6=6
}
     * 
     */

    int startJog(JogSpace space, int joint, double val);
    int endJog();
    bool isJogging();
    bool isExecutingTrajectory();
    
    /*
     * set and get current profile used in motion command
     */
    int setProfile(const EProfile &profile);
    int getProfile(EProfile &profile);
    
    /*
     * set and get current framedata used in motion command
     */
    int setFrameData(const EFrameData &fd);
    int getFrameData(EFrameData &fd);
    
    /*
     * Enabel/Disable script loading. To use lin/ptp/arc command, disable script loading first
     */
    int enableScriptLoading(bool enable);
    
    int pathTrigger(const std::vector<std::string> &ports, double ratio, int trigType, bool action);
    
    /*
     *  motion commands, some prof and framedata parameters, some doesn't; the latter will use the currentProf and curFd,
     * note: if there are prof and framedata parameters, the curProf and curFd will be changed accordingly
     */
    virtual int lineto(const EPos &pos);

    virtual int lineto(const EPos &pos, const EProfile &profile, const EFrameData &fd);
    //linear spd motion command (go to a target point with given spd)
    virtual int linesto(const EPos &pos, double targetSpd, double approx, const EFrameData &fd);

    virtual int arcto(const EPos &mid, const EPos &end);

    virtual int arcto(const EPos &mid, const EPos &end, const EProfile &profile, const EFrameData &fd);

    virtual int arcto( const EPos &center, 
                       const std::vector<double>& startOrientation, 
                       const std::vector<double>& endOrientation,
                       double radius, double startAng, double endAng );

    virtual int arcto( const EPos &center, 
                       const std::vector<double>& startOrientation, 
                       const std::vector<double>& endOrientation,
                       double radius, double startAng, double endAng, 
                       const EProfile& prof, const EFrameData& fd );
    //arc spd motion command (go to a target point with given spd)
    virtual int arcsto( const EPos &center, 
                       const std::vector<double>& startOrientation, 
                       const std::vector<double>& endOrientation,
                       double radius, double startAng, double endAng, 
                       const double spd, double approx, const EFrameData& fd );
   
    virtual int ptpto(const EPos &pos);

    virtual int ptpto(const EPos &pos, const EProfile &profile, const EFrameData &fd);
    
    int calibrateRobot(int joint);
    int restart();
    
    const EPos& getCartPos();
    const EAxis& getJointPos();
    double getSpeed();
    
    int waitEOM(double timeout);
    
public:
    /* callback functions, inherit and overwrite the following functions for any purpose;
     * If sub-classes overwrite it, it should also call this default one. 
     */
    virtual int onControllerFound(const char *ip,  const char *sn, const char *host) {return 0;}
    virtual int onSystemStatusChanged(int index, int value);
    virtual int onProgressStatusChanged(int value) {return 0;}
    virtual int onErrorMessage(int code, const char*msg);
    virtual int onUpdateCart(int robot_index, const EPos *d);
    virtual int onUpdateJnt(int robot_index, const EAxis *d);
    virtual int onUpdateSpd(int robot_index, double value);
    virtual int onSafetyInChanged(const char *name, int value) {return 0;}
    virtual int onSafetyOutChanged(const char *name, int value) {return 0;}
    virtual int onDigitalInChanged(const char *name, int value) {return 0;}
    virtual int onDigitalOutChanged(const char *name, int value) {return 0;}
    virtual int onSoftIOChanged(const char *name, int value) {return 0;}
    virtual int onAnalogyInChanged(const char *name, double value) {return 0;}
    virtual int onAnalogyOutChanged(const char *name, double value) {return 0;}
    virtual int onMotionStatusChanged(int state);
    
protected:
    HBZR h;
    EventCallbacks cb;
    std::string ipAddr;
    bool bConnected;
    bool bSafetyInUse;
    int idxCurRobot;
    int curBase, curTool;
    double cur_maxvel, cur_maxacc, cur_maxjerk; //current cart prof limits
    BUnits uRotate, uLinear;
    int sysStatus[ST_COUNT];
    bool bStepJogCompleted;
    bool bExecutingTraj;
    EProfile curProfile;
    EFrameData curFd;
    std::vector<EPos> cart_pos;
    std::vector<EAxis> jnt_pos;
    std::vector<double> speed;
};

#endif