/***************************************************************************
  tag: Rock Chen  Friday October 21 2016,  BzrInterface.cpp
                        BzrInterface.cpp -  description
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
 *		$Id: BzrInterface.cpp$
 *		$Name:  $
 *  
 ****************************************************************************/

/**
 * \file BzrInterface.cpp
 *
 * This is the C++ Interface for Bzr clients.
 *
 *
**/

#include "BzrInterface.hpp"
#include <string.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <stdexcept>
#include <cstdio>

int SystemStatusFun(void *_this, int index, int value) {
    return ((BzrInterface*)_this)->onSystemStatusChanged(index, value);
}
int ProgressStatusFun(void *_this, int value) {
    return ((BzrInterface*)_this)->onProgressStatusChanged(value);
}
int ErrorMessageFun(void *_this, int code, const char*msg) {
    return ((BzrInterface*)_this)->onErrorMessage(code, msg);
}
int UpdateCartFun(void *_this, int robot_index, const EPos *d) {
    return ((BzrInterface*)_this)->onUpdateCart(robot_index, d);
}
int UpdateJntFun(void *_this, int robot_index, const EAxis *d) {
    return ((BzrInterface*)_this)->onUpdateJnt(robot_index, d);
}
int UpdateSpdFun(void *_this, int robot_index, double value) {
    return ((BzrInterface*)_this)->onUpdateSpd(robot_index, value);
}
int SafetyInFun(void *_this, const char *name, int value) {
    return ((BzrInterface*)_this)->onSafetyInChanged(name, value);
}
int SafetyOutFun(void *_this, const char *name, int value) {
    return ((BzrInterface*)_this)->onSafetyOutChanged(name, value);
}
int DigitalInFun(void *_this, const char *name, int value) {
    return ((BzrInterface*)_this)->onDigitalInChanged(name, value);
}
int DigitalOutFun(void *_this, const char *name, int value) {
    return ((BzrInterface*)_this)->onDigitalOutChanged(name, value);
}
int SoftIOFun(void *_this, const char *name, int value) {
    return ((BzrInterface*)_this)->onSoftIOChanged(name, value);
}
int AnalogyInFun(void *_this, const char *name, double value) {
    return ((BzrInterface*)_this)->onAnalogyInChanged(name, value);
}
int AnalogyOutFun(void *_this, const char *name, double value) {
    return ((BzrInterface*)_this)->onAnalogyOutChanged(name, value);
}
int MotionStatusFun(void *_this, int state) {
    return ((BzrInterface*)_this)->onMotionStatusChanged(state);
}
int ControllerFound(void *_this, const char *ip, const char *sn, const char *host) {
    BzrInterface *th = ((BzrInterface*)_this);
    if (th->getIpAddr().empty())
        th->setIpAddr(ip);  //copy the first IP Address, during scanning
    return th->onControllerFound(ip, sn, host);
}

BzrInterface::BzrInterface(const char *libpath/* = NULL*/) 
        :   h(NULL), bConnected(false), idxCurRobot(0), curBase(0),curTool(0),
            uRotate(UDegree), uLinear(UMillimeter),
            bSafetyInUse(true), bStepJogCompleted(true)
{
    memset(&cb, 0, sizeof(cb));
    cb.userdata = this;
    cb.status = SystemStatusFun;
    cb.prog = ProgressStatusFun;
    cb.err = ErrorMessageFun;
    //cb.debug = ;
    cb.cart = UpdateCartFun;
    cb.jnt = UpdateJntFun;
    cb.spd = UpdateSpdFun;
    cb.sin = SafetyInFun;
    cb.sout = SafetyOutFun;
    cb.din = DigitalInFun;
    cb.dout = DigitalOutFun;
    cb.softio = SoftIOFun;
    cb.ain = AnalogyInFun;
    cb.aout = AnalogyOutFun;
    cb.mstatus = MotionStatusFun;
    curFd.baseNO=0;  //default base 
    curFd.toolNO=0;  //default tool
    curFd.IPO_MODE=3; //SA
    curFd.Torque_state=0; //no torque control
    curFd.Circ_alpha=-1;// no circular motion extension
    h = bzrCreate(libpath, cb);
}

BzrInterface::~BzrInterface() {
    bzrDestroy(h);
}

int BzrInterface::scanControllers(double timeoutSec/* = 2.0*/) {
    ipAddr = "";  //reset
    return bzrScanController(h, timeoutSec, ControllerFound);
}

int BzrInterface::connect(const char *ip/* = NULL*/, int waitSeconds/* = 5*/, double waitSecondsReconnect/* = 5.0*/) {
    int res = 0;
    if (ip == NULL) {
        res = scanControllers(); //in callback will record the first IP address
        if (res < 0)
            return res;
    } else
        ipAddr = ip;
    
    bConnected = false;
    res = bzrConnect(h, ipAddr.c_str(), waitSeconds, waitSecondsReconnect);
    if (res < 0)
        return res;
    
    res = bzrStart(h, true);
    if (res < 0)
        return res;
    
    bConnected = true;
    
    int nSafetyIn = getNumOfIOChannels(nSI);
    bSafetyInUse = (nSafetyIn < 0 || nSafetyIn > 4);  //If failed to get, assume it has Safety In.
    int n_rob = getNumOfRobots();
    jnt_pos.resize(n_rob);
    cart_pos.resize(n_rob);
    speed.resize(n_rob);
    return res;
}

bool BzrInterface::connected() {
    return h != NULL && bConnected;
}

int BzrInterface::getNumOfIOChannels(IOChannels type) {
    if (!connected())
        return -1;
    return bzrIOChannels(h, type);
}

bool BzrInterface::hasSafetyInUse() {
    return bSafetyInUse;
}



int BzrInterface::getNumOfRobots() {
    if (!connected())
        return -1;
    return bzrRobotsNum(h);
}

int BzrInterface::getRobotInfo(int index, RobotInfo& robotInfo) {
    if (!connected())
        return -1;
    return bzrRobotInfo(h, index, robotInfo);
}

void BzrInterface::setCurrentRobot(int index) {
    if(index < 0 | index >= getNumOfRobots()){
        throw std::invalid_argument("Robot index is out of range");
    }
    idxCurRobot = index; 
    getRobCartProfLimits(cur_maxvel,cur_maxacc, cur_maxjerk);
    return; 
}

int BzrInterface::currentRobot() { 
    return idxCurRobot;
}

int BzrInterface::getAxisInfo(int axis, JointInfo& jntInfo) {
    if (!connected())
        return -1;
    return bzrJointInfo(h, idxCurRobot, axis+1, jntInfo);  //+1: 0-based to 1-based
}

int BzrInterface::setSpeed(double val, BUnits unit) {
    if (!connected())
        return -1;
    //TODO convert units
    return bzrRobotSetSpeed(h, idxCurRobot, val);
}

int BzrInterface::getSpeed(double &val, BUnits unit) {
    if (!connected())
        return -1;
    //TODO convert units
    int spd = 0;
    int res = bzrRobotGetSpeed(h, idxCurRobot, spd);
    val = spd;
    return res;
}

int BzrInterface::sleep(int ms) {
    sleep(ms);        //TODO ?
    return 0;
}

int BzrInterface::setUnits(BUnits rotate, BUnits linear) {
    uRotate = rotate;
    uLinear = linear;
    return 0;
}

int BzrInterface::getUnits(BUnits &rotate, BUnits &linear) {
    rotate = uRotate;
    linear = uLinear;
    return 0;
}

int BzrInterface::teachBase(int method, int idBase, int idPoint, EFrame &frame) {
    if (!connected())
        return -1;
    return bzrRobotTeachBase(h, idxCurRobot, method, idBase, idPoint, frame);
}

int BzrInterface::teachTool(int method, int idTool, int idPoint, EFrame &frame) {
    if (!connected())
        return -1;
    return bzrRobotTeachTool(h, idxCurRobot, method, idTool, idPoint, frame);
}

int BzrInterface::setBaseTool(int baseNo, int toolNo)
{
    if (!connected())
        return -1;
    
    int ret=bzrSetBaseTool(h, idxCurRobot, baseNo, toolNo);
    if (ret==0)
    {
        curBase=baseNo;
        curTool=toolNo;
    }
    return ret;
}

int BzrInterface::getSystemStatus(int index, int &status) {
    if (!connected())
        return -1;
    if (index < 0 || index >= ST_COUNT)
        return -2; //invalid type
    
    status = sysStatus[index];
    return 0;
}

int BzrInterface::onSystemStatusChanged(int index, int value) {
    if (index >= 0 && index < ST_COUNT)
        sysStatus[index] = value;
    switch(index) {
    case ST_EMO:
        break;
    case ST_MODE:
        if (value >= ST_MODE_JUMP_BASE) //safety jumped
            bSafetyInUse = false;
        break;
    }
    return 0;
}

int BzrInterface::onErrorMessage(int code, const char*msg) {
    return 0;
}

int BzrInterface::onUpdateJnt(int robot_index, const EAxis *d){
    jnt_pos[robot_index] = (*d);
    return 0;
}

int BzrInterface::onUpdateCart(int robot_index, const EPos *d){
    cart_pos[robot_index] = (*d);
    return 0;
}

int BzrInterface::onUpdateSpd(int robot_index, double value){
    speed[robot_index] = value;
    return 0;
}

int BzrInterface::onMotionStatusChanged(int state){
    switch(state){
        case 101:  // step jog completed
            printf("Step jog completed!\n");
            bStepJogCompleted = true;
            break;
        case 103:  // continuous jog completed
            printf("Continuous jog completed!\n");
            break;
        case 200:
            bExecutingTraj = true;
            printf("Trajectory execution started\n");
            break;
        case 201:
            bExecutingTraj = false;
            printf("All trajectories completed\n");
            break;
        default:
            printf("Unknown motion state: %d\n", state);
    }
    return 0;
}

const EPos& BzrInterface::getCartPos(){
//    confirm_index_in_range(index);
    return cart_pos.at(idxCurRobot);
}

const EAxis& BzrInterface::getJointPos(){
//    confirm_index_in_range(index);
    return jnt_pos.at(idxCurRobot);
}

double BzrInterface::getSpeed(){
//    confirm_index_in_range(index);
    return speed.at(idxCurRobot);
}

int BzrInterface::setMode(int mode) {
    if (!connected())
        return -1;

    int res;
    res = bzrSetSafetyIO(h, SEVT_PRGM_PAUSE, 1);
    if (res < 0)return -2;
    
    res = bzrSetSafetyIO(h, SEVT_MODE_SWITCH, mode == MODE_AUTO ? 1 : 0);
    if (res < 0)return -3;

    switch(mode) {
    case MODE_RECOVERY: // rec
        return bzrOperationModeSet(h, mode);
        break;
    case MODE_FREE: // free
        break;
    case MODE_MANUAL: // manual
        return bzrOperationModeSet(h, mode);
        break;
    }
    return 0;
}

int BzrInterface::servoOn(bool enable) {
    if (!connected())
        return -1;
    
    if (enable) {
        if (!hasSafetyInUse()) {
            bzrJumpSafeIn(h, true);
            bzrSetSafetyIO(h, SEVT_ESTOP, 1);
            bzrSetSafetyIO(h, SEVT_MOTION_STOP, 1);
        }        
        bzrSetSafetyIO(h, SEVT_ERR_RESET, 1);
        bzrSetSafetyIO(h, SEVT_SERVO_ON, 1);
    } else {
        bzrSetSafetyIO(h, SEVT_PRGM_PAUSE, 1);
        bzrSetSafetyIO(h, SEVT_PRGM_START, 0);  //stop
    }
    return 0;
}

int BzrInterface::resetAlarm(){
    if (!connected())
        return -1;
    int res = bzrResetAlarm(h);
    if( res == 0){
        return 0;
    }
    return res;
}

int BzrInterface::start() {
    if (!connected())
        return -1;
    return bzrSetSafetyIO(h, SEVT_PRGM_START, 1);
}

int BzrInterface::stop() {
    if (!connected())
        return -1;
    int res = bzrSetSafetyIO(h, SEVT_PRGM_PAUSE, 1);
    //if (res < 0)return res;  //continue to stop
    return bzrSetSafetyIO(h, SEVT_PRGM_START, 0);
}

int BzrInterface::cancel() {
    if (!connected())
        return -1;
    return bzrSetSafetyIO(h, SEVT_PRGM_CANCEL, 0);
}

int BzrInterface::suspend(bool pause) {
    if (!connected())
        return -1;
    if (pause)
        return bzrSetSafetyIO(h, SEVT_PRGM_PAUSE, 1); //pause
    else
        return bzrSetSafetyIO(h, SEVT_PRGM_PAUSE, 0); //resume
}

bool BzrInterface::isJogging(){
    return !bStepJogCompleted;
}

bool BzrInterface::isExecutingTrajectory(){
    return bExecutingTraj;
}

int BzrInterface::startJog(JogSpace space, int joint, double step) {
    if (!connected())
        return -1;
    if (step > 0.0){
        if(!bStepJogCompleted){
            return 1;
        }
        if (space == ID_JOINT){
            JointInfo joint_info;
            int res = bzrJointInfo(h, idxCurRobot, joint, joint_info);
            if (res != 0){
                return -1;
            }
            // convert to si unit
            if (joint_info.jntType == JNT_TYPE_PRISMATIC){
                step /= 1000.0;
            }
            else {
                step /= 57.2957795131;
            }
        }
        else{
            int dimension = joint;
            if (dimension < 0){
                dimension *= -1;
            }
            if(dimension < 4){
                step /= 1000.0;
            }
            else{
                step /= 57.2957795131;
            }
        }
        bStepJogCompleted = false;
    }
    return bzrRobotStartJog(h, idxCurRobot, space, joint, step);
}

int BzrInterface::endJog() {
    if (!connected())
        return -1;
    return bzrRobotEndJog(h, idxCurRobot);
}

int BzrInterface::homeRob(double spd_pris, double spd_revolute){
    if (!connected())
        return -1;
    return bzrRobotHome(h,idxCurRobot,spd_pris,spd_revolute);
}

int BzrInterface::getRobCartProfLimits(double & max_vel, double &max_acc, double &max_jerk){
    if (!connected())
        return -1;
    return bzrGetRobCartProfLimits(h,idxCurRobot,max_vel,max_acc, max_jerk);
}

int BzrInterface::homeJoint(JogJoint joint,double spd_pris, double spd_revolute) {
    if (!connected())
        return -1;
    return bzrRobotHomeAxis(h,idxCurRobot,(int)joint-1,spd_pris,spd_revolute);
}

int BzrInterface::enableScriptLoading(bool enable){
    if(!connected())
        return -1;
    return bzrEnableScriptLoading(h, enable);
}

int BzrInterface::pathTrigger(const std::vector<std::string> &ports, double ratio, 
                                                    int trigType, bool action){
    if (!connected())
        return -1;
    return bzrPathTrigger(h, idxCurRobot, ports, ratio, trigType, action);
}

int BzrInterface::lineto(const EPos &pos) {
    if (!connected())
        return -1;
    return bzrLinTo(h,idxCurRobot, pos, curProfile, curFd);
}

int BzrInterface::lineto(const EPos &pos, const EProfile &prof, const EFrameData &fd) {
    if (!connected())
        return -1;
    curProfile=prof;
    curFd=fd;
    return bzrLinTo(h,idxCurRobot, pos, prof, fd);
}

int BzrInterface::linesto(const EPos &pos, double targetSpd, double approx, const EFrameData &fd) {
    if (!connected())
        return -1;
    curFd=fd;
    return bzrLinSpdTo(h,idxCurRobot, pos,targetSpd,approx,fd);
}

int BzrInterface::arcto(const EPos &mid, const EPos &end) {
    if (!connected())
       return -1;
    return bzrArcTo1(h,idxCurRobot, mid, end, curProfile,curFd);
}

int BzrInterface::arcto(const EPos &mid, const EPos &end, const EProfile &prof, const EFrameData &fd) {
    if (!connected())
        return -1;
    curProfile=prof;
    curFd=fd;
    return bzrArcTo1(h,idxCurRobot, mid, end, prof,fd);
} 
 
int BzrInterface::arcto( const EPos &center, 
        const std::vector<double>& startOrientation, 
        const std::vector<double>& endOrientation,
        double radius, double startAng, double endAng ) {
    if (!connected())
       return -1;
    return bzrArcTo2( h, idxCurRobot, center, startOrientation, endOrientation,
                      radius, startAng, endAng, curProfile, curFd );
}
 
int BzrInterface::arcto( const EPos &center, 
        const std::vector<double>& startOrientation, 
        const std::vector<double>& endOrientation,
        double radius, double startAng, double endAng, 
        const EProfile& prof, const EFrameData& fd ) {
    if (!connected())
       return -1;
    curProfile = prof;
    curFd = fd;
    return bzrArcTo2(h, idxCurRobot, center, startOrientation, endOrientation,
            radius, startAng, endAng, prof, fd);
}

 int BzrInterface::arcsto( const EPos &center, 
                       const std::vector<double>& startOrientation, 
                       const std::vector<double>& endOrientation,
                       double radius, double startAng, double endAng, 
                       const double spd, double approx, const EFrameData& fd ) {
     if (!connected())
        return -1;
     curFd = fd;
     return bzrArcSpdTo(h, idxCurRobot, center, startOrientation, endOrientation,
             radius, startAng, endAng, spd,approx,fd);
 }
 
 int BzrInterface::ptpto(const EPos &pos) {
     if (!connected())
        return -1;
     return bzrPtpTo(h, idxCurRobot, pos, curProfile,curFd);
 }
 
int BzrInterface::ptpto(const EPos &pos, const EProfile &prof, const EFrameData &fd) {
    if (!connected())
       return -1;
    curProfile=prof;
    curFd=fd;
    return bzrPtpTo(h, idxCurRobot, pos, prof,fd);
}

int BzrInterface::calibrateRobot(int joint) {
    if(!connected()){
        return -1;
    }
    return bzrCalibRobot(h, idxCurRobot, joint);
}

int BzrInterface::waitEOM(double timeout) {
    if (!connected()) {
        return -1;
    }
    return bzrWaitEOM(h, idxCurRobot, timeout);
}

int BzrInterface::restart() {
    if(!connected()){
        return -1;
    }
    return bzrRestart(h);
}

int BzrInterface::destroy(){
    if(!connected()){
        return -1;
    }
    return bzrDestroy(h);
}