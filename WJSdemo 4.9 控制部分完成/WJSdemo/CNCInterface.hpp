/***************************************************************************
  tag: Leon Guanfeng Liu  Friday October 21 2016,  cncAPI.hpp

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
 *		$Id: cncAPI.hpp$
 *		$Name:  $
 *  
 ****************************************************************************/

/**
 * \file cncAPI.hpp
 *
 * This is the C/C++ API reference for 2D CNC applications.
 *
 *
**/

#ifndef BZROS_CNCAPI_HPP
#define BZROS_CNCAPI_HPP

#include <string>
#include <vector>
//#include <Bzr_types.hpp>
#include "Bzr.hpp"
#include "BzrInterface.hpp"
#define mm2m 0.001
#define deg2rad 0.01745329251994329576923690768488

namespace CNCControl {
class BZR_API CNCInterface: public BzrInterface 
{
protected:
    int tableFrameNo; // the base table Frame No;
    EFrame tableFrame; //the frame value for table frame
    int doWaterValve; //digital output to control open and close of water gun
    int doWaterPressure; //digital output to control water pressure
    int cncIndex; //the ID of cnc in a multi-robot system
    EPos tableOrigin; //the original points in 3-point calibration method of table frame
    EAxis cncAxis; //cnc machine current axis data
public:  
    CNCInterface(const char *libpath = NULL);
    CNCInterface(int _cncIndex,int _tableFrameNo, int _doWaterValve, int _doWaterPressure);
	EPos cncPos; //cnc machine current pos data

    virtual int onUpdateCart(int robIndex, const EPos *d);
    virtual int onUpdateJnt(int robIndex, const EAxis *d);
    virtual int onProgressStatusChanged(int value);
//    virtual int onSoftIOChanged(const char *name, int value);
//    virtual int onDigitalOutChanged(const char *name, int value);

/**
 * @brief get the robot index in 
 * @param _index the id of the cnc machine
 * @return 0 when no error. <0 for error codes
 */
    int getCncIndex(int &_index);
    
    
/**
 * @brief jogging robot along x axis
 * @param x, jogging distance, unit determined by setUnits(BUnits rotate, BUnits linear) function in parent class
 * @param conti, a boolean if ture, means jogging continously (in this case, x>0 means jogging positive, x<0 means jogging negative), if conti=false, then the jogging value and direction is determined by x 
 * @return 0 when no error. <0 for error codes
 */
int jogRobotXaxis(double x, bool conti);

/**
 * @brief jogging robot along y axis
 * @param y, jogging distance, unit determined by setUnits(BUnits rotate, BUnits linear) function in parent class
 * @param conti, a boolean if ture, means jogging continously, (in this case, y>0 means jogging positive, y<0 means jogging negative) else, jogging upto distance y 
 * @return 0 when no error. <0 for error codes
 */
int jogRobotYaxis(double y, bool conti);

/**
 * @brief jogging robot along z axis
 * @param z, jogging distance, unit determined by setUnits(BUnits rotate, BUnits linear) function in parent class
 * @param conti, a boolean if ture, means jogging continously,(in this case, z>0 means jogging positive, z<0 means jogging negative) else, jogging upto distance z 
 * @return 0 when no error. <0 for error codes
 */
int jogRobotZaxis(double z, bool conti);


/**
 * @brief jogging robot rotating about Z axis
 * @param A, jogging angle around Z-axis, unit determined by setUnits(BUnits rotate, BUnits linear) function in parent class
 * @param conti, a boolean if ture, means jogging continously, (in this case, A>0 means jogging positive, A<0 means jogging negative) , else, jogging upto distance A
 * @return 0 when no error.<0 for error codes
 */
int jogRobotAaxis(double A, bool conti);

/**
 * @brief jogging robot rotating about Y axis
 * @param B, jogging angle around Y-axis, unit determined by setUnits(BUnits rotate, BUnits linear) function in parent class
 * @param conti, a boolean if ture, means jogging continously, (in this case, B>0 means jogging positive, B<0 means jogging negative) , else, jogging upto distance B
 * @return 0 when no error. <0 for error codes
 */
int jogRobotBaxis(double B, bool conti);


/**
 * @brief jogging robot rotating about X axis
 * @param C, jogging angle around X-axis, unit determined by setUnits(BUnits rotate, BUnits linear) function in parent class
 * @param conti, a boolean if ture, means jogging continously, (in this case, C>0 means jogging positive, C<0 means jogging negative) , else, jogging upto distance C
 * @return 0 when no error. <0 for error codes
 */
int jogRobotCaxis(double C, bool conti);

/**
 * @brief Ending to jog robot for continuous jogging case.
 * @return 0 when no error.
 */
int endRobotJog();

/* 
 * @brief set the table frame number (base frame), should be in [0,...,7]
 * @return 0 when no error
 */
int setTableFrameNo(int frame_no);

/* 
 * @brief get the table frame number (a.k.a base frame n), should be in [0,...,7]
 * @return 0 when no error
 */
int getTableFrameNo(int &frame_no);


/**
 * @brief Calibration of table frame 
 * @param calibStep, in (1,2,3) for the calibration step;
 *        Step 1: align tool with table origin(P1). 
 *        Step 2: align tool to x positive(P2).
 *        Step 3: align tool to y positive(P3).
 * @return 0 when no error.
 */
int calibTable(int calibStep);

/**
 * @brief Get the calibration data of the table (or marble) 
 * @param x, the x value of the calibration frame
 * @param y, the y value of the calibration frame
 * @param z, the z value of the calibration frame
 * @param A, the yaw angle (or ZYX Euler angle A) of the calibration frame
 * @param B, the pitch angle (or ZYX Euler angle B) of the calibration frame
 * @param C, the roll angle (or ZYX Euler angle C) of the calibration frame
 * @return 0 when no error.
 */
int getTableFrame(double & x, double &y, double &z, double &A, double &B, double &C);

/**
 * @brief robot delay command
 * @param ms, the delaying time (in ms)
 * @return 0 when no error.
 */
int delay(int ms);

/**
 * @brief set working frame 
 * @param Frame data to be set as working frame
 * @return 0 when no error.
 */
int setCurrentFrame(const EFrameData &fd);

/**
 * @brief Move robot in straight line from current position to (x, y, z) 
 *         coordinates.
 * @param Destination X coordinate.
 * @param Destination Y coordinate.
 * @param Destination Z coordinate.
 * @param The desired speed of motion for this straight line segment.
 * @return 0 when no error.
 */
int line(double x, double y, double z, double v);

/**
 * @brief robot arc motion command 
 * @param double Angle_start, the start angle of the arc
 * @param double Angle_add, the amount of angle that this arc travels
 * @param double r, the radius of the arc
 * @param double Arccent_x, the x-coordinate of the arc center
 * @param double Arccent_y, the y-coordinate of the arc center
 * @param double v, the desired motion speed for this arc segment
 * @return 0 when no error.
 */
int arc(double Angle_start, double Angle_add, double r, double Arccent_x, double Arccent_y, double v);
};
}
    
#endif