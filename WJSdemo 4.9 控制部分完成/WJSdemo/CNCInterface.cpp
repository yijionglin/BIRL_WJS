/***************************************************************************
  tag: Leon Guanfeng Liu  Friday October 21 2016,  cncAPI.cpp

                        cncAPI.hpp -  description
                           -------------------
    begin                : October 21 2016
    copyright            : (C) 2016 Leon Guanfeng Liu
    email                : leon.guanfeng.liu@bz-intelligentrobot.com

******************************************************************************/
 /*****************************************************************************
 *  \author
 *  	Leon Guanfeng Liu, BZ Robot INC
 *
 *  \version
 *	           BZROS_TASK V0.1
 *
 *	\par History
 *		- $log$
 *
 *	\par Release
 *		$Id: CNCInterface.cpp$
 *		$Name:  $
 *  
 ****************************************************************************/

/**
 * \file CNCInterface.cpp
 *
 * This is the C/C++ API reference for 2D CNC applications.
 *
 *
**/

#include "CNCInterface.hpp"
#include "stdio.h"

/**
 * @brief jogging robot along x axis
 * @param x, jogging distance, in mm
 * @param conti, a boolean if ture, means jogging continously, 
 * @return 0 when no error. <0 for error codes
 */
namespace CNCControl {
    
CNCInterface::CNCInterface(const char *libpath) : BzrInterface(libpath), 
                                                  cncIndex(0),tableFrameNo(1),
                                                  doWaterValve(-1),
                                                  doWaterPressure(-1)
{
    
}

CNCInterface::CNCInterface(int _cncIndex, int _tableFrameNo, int _doWaterValve, 
                    int _doWaterPressure): BzrInterface(), 
                                           cncIndex(_cncIndex), 
                                           tableFrameNo(_tableFrameNo), 
                                           doWaterValve(_doWaterValve), 
                                           doWaterPressure(_doWaterPressure)
{
    
}
    
int CNCInterface::getCncIndex(int &_index)
{
    _index=cncIndex;
    return 0;
}

//int CNCInterface::onSoftIOChanged(const char *name, int value){
//    printf("Soft IO Triggered -> %s:%d\n", name, value);
//}

//int CNCInterface::onDigitalOutChanged(const char *name, int value){
//    printf("Digital Out Triggered -> %s:%d\n", name, value);
//}

int CNCInterface::jogRobotXaxis(double x, bool conti)
{
    int axisNo = 1;
    double jogLength = x;
    if (x < 0)
    {
        axisNo = -1;
        jogLength = -x;
    }  
    if (conti)
        jogLength = -1;
    /*
    if (uLinear==UMillimeter) 
        jogLength *=mm2m;
     */ 
    return this->startJog(ID_CART_WORLD, axisNo, jogLength);
}

int CNCInterface::onProgressStatusChanged(int value){
    fprintf(stderr, "Progress : %d\n", value);
    return 0;
}

/**
 * @brief jogging robot along y axis
 * @param y, jogging distance, in mm
 * @param conti, a boolean if ture, means jogging continously, 
 * @return 0 fen no error. <0 for error codes
 */
int CNCInterface::jogRobotYaxis(double y, bool conti)
{
    int axisNo = 2;
    double jogLength = y;
    if (y < 0)
    {
        axisNo = -2;
        jogLength = -y;
    }  
    if (conti)
        jogLength = -1;
    /*
    if (uLinear==UMillimeter) 
        jogLength *=mm2m;
     */ 
    return this->startJog(ID_CART_WORLD, axisNo, jogLength);
}

/**
 * @brief jogging robot along z axis
 * @param z, jogging distance, in mm
 * @param conti, a boolean if ture, means jogging continously, 
 * @return 0 when no error. <0 for error codes
 */
int CNCInterface::jogRobotZaxis(double z, bool conti)
{
    int axisNo = 3;
    double jogLength = z;
    if (z < 0)
    {
        axisNo = -3;
        jogLength = -z;
    }  
    if (conti)
        jogLength = -1;
   
    /*
    if (uLinear==UMillimeter) 
        jogLength *=mm2m;
     */ 
    return this->startJog(ID_CART_WORLD, axisNo, jogLength);
}

/**
 * @brief jogging robot rotating about Z axis
 * @param A, jogging angle around Z-axis, in degree
 * @param conti, a boolean if ture, means jogging continously, 
 * @return 0 when no error. <0 for error codes
 */
int CNCInterface::jogRobotAaxis(double A, bool conti)
{
    int axisNo = 4;
    double jogLength = A;
    if (A < 0)
    {
        axisNo = -4;
        jogLength = -A;
    }  
    if (conti)
        jogLength = -1;
    /*
    if (uRotate==UDegree) 
        jogLength *=deg2rad;
     */ 
    return this->startJog(ID_CART_WORLD, axisNo, jogLength);
}

/**
 * @brief jogging robot rotating about Y axis
 * @param index, the id of the robot
 * @param B, jogging angle around Y-axis, in degree
 * @param conti, a boolean if ture, means jogging continously, 
 * @return 0 when no error. <0 for error codes
 */
int CNCInterface::jogRobotBaxis(double B, bool conti)
{
    int axisNo = 5;
    double jogLength = B;
    if(B < 0)
    {
        axisNo = -5;
        jogLength = -B;
    }  
    if (conti)
        jogLength = -1;
    /*
    if (uRotate==UDegree) 
        jogLength *=deg2rad;
     */ 
    return this->startJog(ID_CART_WORLD, axisNo, jogLength);
}

/**
 * @brief jogging robot rotating about X axis
 * @param index, the id of the robot
 * @param C, jogging angle around X-axis, in degree
 * @param conti, a boolean if ture, means jogging continously, 
 * @return 0 when no error. <0 for error codes
 */
int CNCInterface::jogRobotCaxis(double C, bool conti)
{
    int axisNo = 6;
    double jogLength = C;
    if (C < 0)
    {
        axisNo = -6;
        jogLength = -C;
    }  
    if (conti)
        jogLength = -1;
    /*
    if (uRotate==UDegree) 
        jogLength *=deg2rad;
     */ 
    return this->startJog(ID_CART_WORLD, axisNo, jogLength);
}

/**
 * @brief Ending to jog robot for continuous jogging case.
 * @return 0 when no error.
 */
int CNCInterface::endRobotJog()
{
    return this->endJog();
}

int CNCInterface::setTableFrameNo(int frame_no)
{
    if ((frame_no <= 0) && (frame_no > 7))
    {
        return -1;
    }
    tableFrameNo = frame_no;
	return 0;
}

int CNCInterface::getTableFrameNo(int &frame_no)
{
    frame_no = tableFrameNo;
	return 0;
}

/**
 * @brief Calibration of table frame 
 * @param calibStep, in (1,2,3) for the calibration step;
 *        Step 1: align tool with table origin(P1). 
 *        Step 2: align tool to x positive(P2).
 *        Step 3: align tool to y positive(P3).
 * @return 0 when no error.
 */
int CNCInterface::calibTable(int calibStep)
{
    if ((calibStep < 1) || (calibStep > 3))
        return -1; // calibStep out of reach
   
    int ret = 0;
    if (calibStep <= 3)
    {
        static EPos step_1_pos;
        ret=this->teachBase(1, tableFrameNo, calibStep, tableFrame);
        if (calibStep == 1){
            step_1_pos = cncPos;
        }
        if (calibStep == 3) // if done teaching
        {
            printf("X=%f, Y=%f, Z=%f, A=%f, B=%f, C=%f\n", tableFrame.x, 
                                tableFrame.y, tableFrame.z, tableFrame.A,
                                                  tableFrame.B, tableFrame.C);
            // set base and tool to taught base and current tool
            setBaseTool(tableFrameNo, curTool);
            // curFd.baseNO=tableFrameNo;
            tableOrigin = step_1_pos;
        }
    }
    return ret;
}

/**
 * @brief Get the calibration data of the table (or marble) 
 * @param index ID of the robot;
 * @param x, the x value of the calibration frame
 * @param y, the y value of the calibration frame
 * @param z, the z value of the calibration frame
 * @param A, the yaw angle (or ZYX Euler angle A) of the calibration frame
 * @param B, the pitch angle (or ZYX Euler angle B) of the calibration frame
 * @param C, the roll angle (or ZYX Euler angle C) of the calibration frame
 * @return 0 when no error.
 */
int CNCInterface::getTableFrame(double & x, double &y, double &z, double &A, 
                                                           double &B, double &C)
{
    x = this->tableFrame.x;
    y = this->tableFrame.y;
    z = this->tableFrame.z;
    A = this->tableFrame.A;
    B = this->tableFrame.B;
    C = this->tableFrame.C;
    return 0;
}

/**
 * @brief robot delay command
 * @param index ID of the robot;
 * @param ms, the delaying time (in ms)
 * @return 0 when no error.
 */
int CNCInterface::delay(int ms)
{
    return sleep(ms);
}

int CNCInterface::onUpdateCart(int robIndex, const EPos *d)
{
    if (robIndex == cncIndex)
         cncPos = *d;
	return 0;
}

int CNCInterface::onUpdateJnt(int robIndex, const EAxis *d)
{
    if (robIndex == cncIndex)
        cncAxis = *d;
	return 0;
}

int CNCInterface::setCurrentFrame(const EFrameData &fd){
    curFd = fd;
    return 0;
}

/**
 * @brief Move robot in straight line from current position to (x, y, z) 
 *         coordinates.
 * @param Destination X coordinate.
 * @param Destination Y coordinate.
 * @param Destination Z coordinate.
 * @param The desired speed of motion for this straight line segment.
 * @return 0 when no error.
 */
int CNCInterface::line(double x, double y, double z, double v)
{
//    if ((v > cur_maxvel) || (v < -cur_maxvel))
    if ((v > 1.0) || (v < 0.0))
        return -1; //out of speed limit
    EPos target = tableOrigin;
    target.x = x;
    target.y = y;
    target.z = z;
    target.A = 0;
    target.B = 0;
    target.C = 0;
    target.S = 0;
    target.T = 0;
    
    EProfile prof;
    prof.Type = 13; //speed controlled adjustable trapezoidal profile
    prof.MaxVel = v;
    prof.MaxAcc = cur_maxacc;
    prof.MaxJerk = cur_maxjerk;
    prof.AppDist = 0.9;
    prof.AccTime = 0.1;  //0.1 sec
    
    EFrameData fd = curFd;
    fd.baseNO = tableFrameNo;
    fd.IPO_MODE = 3; // this has to be there. System crashes with random value
    return lineto(target, prof, fd);
}

/**
 * @brief robot arc motion command 
 * @param The start angle of the arc in reference to positive X axis in anticlockwise direction
 * @param The amount of angle that this arc travels in anticlockwise direction.
 * @param The radius of the arc
 * @param The X-coordinate of the arc center
 * @param The Y-coordinate of the arc center
 * @param The desired motion speed for this arc segment
 * @return 0 when no error.
 */
int CNCInterface::arc(double angle_start, double angle_add, double r, 
        double arccent_x, double arccent_y, double v)
{
    if ((v > 1.0) || (v < 0.0))
        return -1; // out of limit
    EPos center;
    center.x = arccent_x;
    center.y = arccent_y;
    center.z = cncPos.z;
    center.A = 0;  // w.r.t to table frame it's 
    center.B = 0;
    center.C = 0;
    center.S = 0;  // disregard any value
    center.T = 0;  // disregard any value
    
    std::vector<double> start_orientation(3, 0.0);
    std::vector<double> end_orientation(3, 0.0);
    
    EProfile prof;
    prof.Type = 13;  // speed controlled adjustable trapezoidal profile
    prof.MaxVel = v;
    prof.MaxAcc = cur_maxacc;
    prof.MaxJerk = cur_maxjerk;
    prof.AppDist = 0.9;
    prof.AccTime = 0.1;  //0.1 sec
    
    EFrameData fd = curFd;
    fd.baseNO = tableFrameNo;
    fd.IPO_MODE = 3;
    arcto( center, start_orientation, end_orientation, r, 
           angle_start, angle_start + angle_add, prof, fd );
	return 0;
}

} //namespace

