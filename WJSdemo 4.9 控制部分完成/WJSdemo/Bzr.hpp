/***************************************************************************
  tag: Rock Chen  Wednesday March 25 2015,  Bzr.hpp

                        Bzr.hpp -  description
                           -------------------
    begin                : March 25 2015
    copyright            : (C) 2015 Rock Chen
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
 *		$Id: Bzr.hpp$
 *		$Name:  $
 *  
 ****************************************************************************/

/**
 * \file Bzr.hpp
 *
 * This is the C/C++ API reference of Bzr clients.
 *
 *
**/

#ifndef BZROS_BZR_HPP
#define BZROS_BZR_HPP

#include <string>
#include <vector>
#include "Bzr_types.hpp"

#if defined _WIN32 || defined __CYGWIN__
#  define BZR_DLLIMPORT __declspec(dllimport)
#  define BZR_DLLEXPORT __declspec(dllexport)
#  define BZR_DLLLOCAL
#else 
#  if __GNUC__ >= 4
#   define BZR_DLLIMPORT __attribute__ ((visibility("default")))
#   define BZR_DLLEXPORT __attribute__ ((visibility("default")))
#   define BZR_DLLLOCAL  __attribute__ ((visibility("hidden")))
#  else
#   define BZR_DLLIMPORT
#   define BZR_DLLEXPORT
#   define BZR_DLLLOCAL
#  endif
#endif

#ifdef BZR_STATIC
#  define BZR_DLLAPI
#  define BZR_LOCAL
#else
#  if defined( BZR_EXPORTS )
#    define BZR_API BZR_DLLEXPORT
#  else
#    define BZR_API BZR_DLLIMPORT
#  endif
#  define BZR_LOCAL BZR_DLLLOCAL
#endif

typedef void* HBZR;

enum JogSpace {
    ID_CART_WORLD=1,
    ID_CART_TOOL=2,
    ID_CART_BASE=3,
    ID_JOINT=4
};

enum JogJoint{
    ID_X_A1=1,
    ID_Y_A2=2,
    ID_Z_A3=3,
    ID_A_A4=4,
    ID_B_A5=5,
    ID_C_A6=6
};

enum PathTriggerType{
    TIME_FRACTION=0,  // [0..1]
    DIST_FRACTION=1, // [0..1]
    TIME_DELAY=2,  // in second
    DIST_DELAY=3   // in mm
};


typedef int (*FoundController)(void *userdata, const char *ip, const char *sn, const char *host);

typedef int (*SystemStatus)(void *userdata, int index, int value);
typedef int (*ProgressStatus)(void *userdata, int value);  //if return !=0; will stop the progress.
typedef int (*ErrorMessage)(void *userdata, int code, const char*msg);
typedef int (*UpdateCart)(void *userdata, int robot_index, const EPos*);
typedef int (*UpdateJnt)(void *userdata, int robot_index, const EAxis*);
typedef int (*UpdateSpd)(void *userdata, int robot_index, double value);
typedef int (*DigitalIO)(void *userdata, const char *name, int value);
typedef int (*AnalogyIO)(void *userdata, const char *name, double value);
typedef int (*MotionStatus)(void *userdata, int state);


typedef struct _EventCallbacks_ {
    void    *userdata;    //callback to user with the same 'userdata'
    SystemStatus status;  //The current status of system
    ProgressStatus prog;  //Progress for long time API calls, e.g. bzrConnect()
    ErrorMessage err;   //Error code and error message when happen
    ErrorMessage debug;
    UpdateCart cart;    //The current position of robot
    UpdateJnt jnt;      //The current position of robot
    UpdateSpd spd;      //The current speed (percentage) of robot
    DigitalIO sin;      //Safety Inputs, when value is changed.
    DigitalIO sout;     //Safety Outputs, when value is changed.
    DigitalIO din;      //Digital Inputs, when value is changed.
    DigitalIO dout;     //Digital Outputs, when value is changed.
    DigitalIO softio;   //Soft I/O, when value is changed.
    AnalogyIO ain;      //Analogy Inputs, when value is changed.
    AnalogyIO aout;     //Analogy Outputs, when value is changed.
    MotionStatus mstatus; // motion status
}EventCallbacks;

enum IOChannels {
    nSI = 0,  //Safety Inputs
    nSO = 1,  //Safety Outputs
    nDI = 2,  //Digital Inputs
    nDO = 3,  //Digital Outputs
    nSf = 4,  //Soft I/O
    nAI = 5,  //Analogy Inputs
    nAO = 6   //Analogy Outputs
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the SDK
 * @param lib Path to the libraries (default on Windows: "C:/orocos/lib/orocos" or others: "/usr/local/lib/orocos");
 * @param cb Refer to structure 'EventCallbacks';
 * @return The handle or NULL when error.
 */
BZR_API HBZR bzrCreate(const char *lib, EventCallbacks cb);

/**
 * @brief Destroy the SDK, after disconnect and cleanup everything
 * @param h Handle of SDK;
 * @return 0 when no error.
 */
BZR_API int bzrDestroy(HBZR h);

/**
 * @brief Stop or Restart remote 'ip' services.
 * @param h Handle of SDK;
 * @param ip The remote IP-address;
 * @param waitSeconds The waiting time to connect, -1 for infinitely;
 * @param flag: exit(2,3,0) or restart(1,4); Keep-the-last-mode(0) or To-Config-mode(1,2) or To-Running-mode(3,4);
 * @return 0 when no error.
 */
BZR_API int bzrStop(HBZR h, const char *ip, int waitSeconds, int flag);

/**
 * @brief Scan controllers in this local network, to get the remote controller's IP-address 'ip'.
 * @param h Handle of SDK;
 * @param timeoutSec The waiting time (in seconds) to scan the local network, e.g. 2.0s;
 * @param cb The callback function pointer to receive IP-address of controllers;
 * @return 0 when connecting successfully.
 */
BZR_API int bzrScanController(HBZR h, double timeoutSec, FoundController cb);

/**
 * @brief Connect to remote with IP-address 'ip'.
 * @param h Handle of SDK;
 * @param ip The remote IP-address;
 * @param waitSeconds The waiting time to connect, -1 for infinitely;
 * @param waitSecondsReconnect The waiting time to reconnect & call error-message-callback;
 * @return 0 when connecting successfully.
 */
BZR_API int bzrConnect(HBZR h, const char *ip, int waitSeconds, double waitSecondsReconnect);

/**
 * @brief Start to pull data from remote, and callback with data. Otherwise, no callback is called.
 * @param h Handle of SDK;
 * @param bStart True for starting callback
 * @return 0 when no error.
 */
BZR_API int bzrStart(HBZR h, bool bStart);

/**
 * @brief Get the number of robots connected with the controller.
 * @param h Handle of SDK;
 * @return The number of robots.
 */
BZR_API int bzrRobotsNum(HBZR h);

/**
 * @brief Set the current robot base and tool frame no .
 * @param h Handle of SDK;
 * @param index ID of the robot
 * @param _baseNo, the number of current base frame
 * @param _toolNo, the number of current tool frame
 * @return The number of robots.
 */
BZR_API int bzrSetBaseTool(HBZR h, int index, int _baseNo, int _toolNo);

/**
 * @brief Get the current setting speed of the robot.
 * @param h Handle of SDK;
 * @param index ID of the robot;
 * @param speed [OUTPUT] The current speed;
 * @return 0 when no error.
 */
BZR_API int bzrRobotGetSpeed(HBZR h, int index, int &speed);

/**
 * @brief Set the speed of the robot.
 * @param h Handle of SDK;
 * @param index ID of the robot;
 * @param speed The new speed to set to the robot;
 * @return 0 when no error.
 */
BZR_API int bzrRobotSetSpeed(HBZR h, int index, int speed);

/**
 * @brief Home the robot.
 * @param h Handle of SDK;
 * @param index ID of the robot,0 based;
 * @param spd_pris is the homing spd for prismatic joint, if <0, the homing spd is determined by internal jogging spd;
 * @param spd_revolute is the homing spd for revolute joint, if <0, the homing spd is determine by internal jogging spd;
 * @return 0 when no error.
 */
BZR_API int bzrRobotHome(HBZR h, int index,double spd_pris, double spd_revolute);

/**
 * @brief Get Robot max vel, max jerk, and max acc (which is minimal among all cartesian axes).
 * @param h Handle of SDK;
 * @param index ID of the robot,0 based;
 * @param max_vel, max cartesian vel
 * @param max_acc, max cartesian acc
 * @param max_jerk, max cartesian jerk
 * @return 0 when no error.
 */
BZR_API int bzrGetRobCartProfLimits(HBZR h, int index,double & max_vel, double &max_acc, double &max_jerk);


/**
 * @brief Home the robot.
 * @param h Handle of SDK;
 * @param index ID of the robot, 0 based;
 * @param index ID of robot axis, 0 based;
 * @param spd_pris is the homing spd for prismatic joint, if <0, the homing spd is determined by internal jogging spd
 * @param spd_revolute is the homing spd for revolute joint, if <0, the homing spd is determined by internal jogging spd
 * @return 0 when no error.
 */
BZR_API int bzrRobotHomeAxis(HBZR h, int index, int axisNo, double spd_pris, double spd_revolute);


/**
 * @brief Starting to jog robot in 'space' and 'joint'.
 * @param h Handle of SDK;
 * @param index ID of the robot;
 * @param space Refer to 'enum JogSpace';
 * @param joint Refer to 'enum JogJoint', positive/negative means jog direction;
 * @param length For step mode. It is continuous mode, when <= -1;
 * @return 0 when no error.
 */
BZR_API int bzrRobotStartJog(HBZR h, int index, int space, int joint, double length);

/**
 * @brief Ending to jog robot.
 * @param h Handle of SDK;
 * @param index ID of the robot;
 * @return 0 when no error.
 */
BZR_API int bzrRobotEndJog(HBZR h, int index);

/**
 * @brief Teach robot Base frame.
 * @param h Handle of SDK;
 * @param index ID of the robot;
 * @param method Must 1 now;
 * @param idBase The index of Base frame [0..7];
 * @param idPoint The ID of point for the current location [0..2];
 * @return 0 when no error.
 */
BZR_API int bzrRobotTeachBase(HBZR h, int index, int method, int idBase, int idPoint, EFrame &frame);

/**
 * @brief Teach robot Tool frame.
 * @param h Handle of SDK;
 * @param index ID of the robot;
 * @param method Must 1 now;
 * @param idTool The index of Tool frame [0..7];
 * @param idPoint The ID of point for the current location [0..5];
 * @return 0 when no error.
 */
BZR_API int bzrRobotTeachTool(HBZR h, int index, int method, int idTool, int idPoint, EFrame &frame);

/**
 * @brief Load & read the current configured user thread information from remote.
 * @param h Handle of SDK;
 * @param num_thread Return the number of user thread;
 * @param names Return the user thread names of each user thread
 * @return 0 when no error.
 */
BZR_API int bzrConfigDataRead(HBZR h, int &num_thread, std::vector<std::string> &names);

/**
 * @brief Save the configured user thread information to remote.
 * @param h Handle of SDK;
 * @param num_thread The number of user thread;
 * @param names The user thread names of each user thread
 * @return 0 when no error.
 */
BZR_API int bzrConfigDataSave(HBZR h, int num_thread, const std::vector<std::string> &names);

/**
 * @brief Load & read the current teach-data from remote.
 * @param h Handle of SDK;
 * @param data Return the teach-data;
 * @return 0 when no error.
 */
BZR_API int bzrTeachDataRead(HBZR h, ETeachData &data);

/**
 * @brief Save the teach-data to remote.
 * @param h Handle of SDK;
 * @param data The teach-data;
 * @return 0 when no error.
 */
BZR_API int bzrTeachDataSave(HBZR h, const ETeachData &data);

/**
 * @brief List all user program filenames from remote.
 * @param h Handle of SDK;
 * @param names Return the user program filenames;
 * @return 0 when no error.
 */
BZR_API int bzrUserProgNamesRead(HBZR h, std::vector<std::string> &names);

/**
 * @brief Remove the user program from remote.
 * @param h Handle of SDK;
 * @param fname The user program filename to be removed;
 * @return 0 when no error.
 */
BZR_API int bzrUserProgDelete(HBZR h, const char *fname);

/**
 * @brief Rename the user program from remote.
 * @param h Handle of SDK;
 * @param orgFileName The original user program filename to be renamed;
 * @param newFileName The new user program filename;
 * @return 0 when no error.
 */
BZR_API int bzrUserProgRename(HBZR h, const char *orgFileName, const char *newFileName);

/**
 * @brief Load & read the user program from remote.
 * @param h Handle of SDK;
 * @param fname The user program filename to be loaded;
 * @param prog Return the loaded user program in a 'string';
 * @return 0 when no error.
 */
BZR_API int bzrUserProgRead(HBZR h, const char *fname, std::string &prog);

/**
 * @brief Save the user program to remote.
 * @param h Handle of SDK;
 * @param fname The user program filename to be written or overwritten;
 * @param prog The user program in a 'string';
 * @return 0 when no error.
 */
BZR_API int bzrUserProgSave(HBZR h, const char *fname, const std::string &prog);

/**
 * @brief Change the current operation mode (one of manual modes). 
 *      To switch between auto and manual mode, use safety IO. 
 * @param h Handle of SDK;
 * @param value The operation mode: MODE_MANUAL, MODE_RECOVERY, MODE_FREE;
 * @return 0 when no error.
 */
BZR_API int bzrOperationModeSet(HBZR h, int value);

/**
 * @brief For Simulation only, before safety IO device is ready.
 * @param h Handle of SDK;
 * @param sInputs
 * @param switched
 * Enable Drive: <4,1>
 * Acknowledge All: <3,1>
 * Auto or Manual: <2,1/0>
 * Start/stop program/trajectory in auto mode: <5,1/0>
 * Pause/resume program/trajectory in auto mode: <6, 0/1>
 * Cancel program/trajectory: <7,0>
 * Jump motion stop: <1,1>
 * Jump estop: <0,1>
 * @return 0 when no error
 */
BZR_API int bzrSetSafetyIO(HBZR h, int sInputs, bool switched);

/**
 * @brief For Simulation only, before safety IO device is ready.
 * @param h Handle of SDK;
 * @param jumped
 * @return 0 when no error
 */
BZR_API int bzrJumpSafeIn(HBZR h, bool jumped);

/**
 * @brief Get the total number of IO channels.
 * @param h Handle of SDK;
 * @param name Refer to 'enum IOChannels';
 * @return The number of each type of IO.
 */
BZR_API int bzrIOChannels(HBZR h, IOChannels name);

/**
 * @brief Calibrate robot joint.
 * @param h Handle of SDK.
 * @param Name of the robot being calibrated.
 * @param Joint number of the robot (1 based).
 * @return 0 when no errors, 1 when finish coupled, >1 when calibration partly, <0 when errors
 */
BZR_API int bzrCalibRobot(HBZR h, int index, int joint);

BZR_API int bzrPathTrigger(HBZR h, int index, const std::vector<std::string> ports, 
                                            double ratio, int trigType, bool action);
/**
 * @brief Enable/Disable script loading. To use lin/ptp/arc command, script loading must be disabled first.
 * @param h Handle of SDK.
 * @param Script loading is disabled when false and enabled when true.
 * @return 0 when no errors, <0 when errors
 */
BZR_API int bzrEnableScriptLoading(HBZR h, bool enable);

/**
 * @brief robot linear motion command
 * @param h Handle of SDK.
 * @param pos target position
 * @param prof motion profile for this command
 * @param fd the frame data for this command
 * @return 0 when no errors, <0 when errors
 */
BZR_API int bzrLinTo(HBZR h, int index, const EPos &pos, const EProfile &prof, const EFrameData &fd);

/**
 * @brief robot linear motion with target speed command
 * @param h Handle of SDK.
 * @param pos target position
 * @param targetSpd target speed of motion in mm/s
 * @param approx the approx ratio (0-1), 1 means no approximation, usually this number should be between 0.51 to 0.99  
 * @param fd the frame data for this command
 * @return 0 when no errors, 1 when finish coupled, >1 when calibration partly, <0 when errors
 */
BZR_API int bzrLinSpdTo(HBZR h, int index,const EPos &pos, const double targetSpd, const double approx, const EFrameData &fd);

        
/**
 * @brief robot PTP motion command
 * @param h Handle of SDK.
 * @param pos target position
 * @param prof motion profile for this command
 * @param fd the frame data for this command
 * @return 0 when no errors, <0 when errors
 */
BZR_API int bzrPtpTo(HBZR h, int index, const EPos &pos, const EProfile &prof, const EFrameData &fd);

/**
 * @brief robot ARC motion command (based on three non-colinear points)
 * @param h Handle of SDK.
 * @param pos_mid, the mid target position (recall the first point is the previous target point
 * @param pos_end, the end target position 
 * @param prof motion profile for this command
 * @param fd the frame data for this command
 * @return 0 when no errors, <0 when errors
 */
BZR_API int bzrArcTo1(HBZR h, int index, const EPos &pos_mid, const EPos &pos_end, const EProfile &prof, const EFrameData &fd);

/**
 * @brief robot ARC motion command (based on center of the arc, the radius, the start and end orientation vector, plus the startAng, and endAng w.r.t. center)
 * @param h Handle of SDK.
 * @param center, the center of arc
 * @param startOrientation, endOrientation, the start and end orientation of robot along the arc 
 * @param radius, the radius of the arc
 * @param startAng, endAng, the start and end angle of the arc ends points w.r.t. the center
 * @param fd the frame data, w.r.t which EPOS is defined
 * @param profile, the profile (how fast) of this move
 * @return 0 when no errors, <0 when errors
 */
BZR_API int bzrArcTo2( HBZR h, int index, const EPos &center, 
         const std::vector<double>& startOrientation, 
         const std::vector<double>& endOrientation,
         double radius, double startAng, double endAng, 
         const EProfile &profile, const EFrameData &fd );


/**
 * @brief robot ARC motion command (based on center of the arc, the radius, the start and end orientation vector, plus the startAng, and endAng w.r.t. center)
 * @param h Handle of SDK.
 * @param center, the center of arc
 * @param startOrientation, endOrientation, the start and end orientation of robot along the arc 
 * @param radius, the radius of the arc
 * @param startAng, endAng, the start and end angle of the arc ends points w.r.t. the center
 * @param fd the frame data, w.r.t which EPOS is defined
 * @param spd, the target spd at target pos
 * @param approx, (0-1), the approximate ratio for smoothing with the next motion command
 * @return 0 when no errors, 1 when finish coupled, >1 when calibration partly, <0 when errors
 */
BZR_API int bzrArcSpdTo( HBZR h, int index, const EPos &center, 
         const std::vector<double>& startOrientation, 
         const std::vector<double>& endOrientation,
         double radius, double startAng, double endAng, 
         const double spd, const double approx, const EFrameData &fd );


/**
 * @brief Get Robot information.
 * @param h Handle of SDK.
 * @param Robot index.
 * @param RobotInfo structure to be updated.
 * @return 0 when no error
 */
BZR_API int bzrRobotInfo(HBZR h, int index, RobotInfo& robotInfo);

/**
 * @brief Get Robot information.
 * @param h Handle of SDK.
 * @param Robot index.
 * @param Joint number of the robot (1 based).
 * @param JointInfo structure to be updated.
 * @return 0 when no error
 */
BZR_API int bzrJointInfo(HBZR h, int index, int joint, JointInfo& jntInfo);

/**
 * @brief Restart controller in run mode.
 * @param h Handle of SDK.
 * @return 0 when no error
 */
BZR_API int bzrRestart(HBZR h);

/**
 * @brief Reboot the whole controller system.
 * @param h Handle of SDK.
 * @return 0 when no error
 */
BZR_API int bzrRebootSystem(HBZR h);

/**
 * @brief Shutdown/power-off the whole controller system.
 * @param h Handle of SDK.
 * @return 0 when no error
 */
BZR_API int bzrShutdownSystem(HBZR h);

/**
 * @brief Reset servo drive alarm(s).
 * @param h Handle of SDK.
 * @return 0 when no error
 */
BZR_API int bzrResetAlarm(HBZR h);

/**
 * @brief Get the value of a given digital input.
 * @param h Handle of SDK.
 * @param _io_number, the logical channel number of the target digital input
 * @param val, 1 means on, 0 means off
 * @return 0 when no error
 */
BZR_API int bzrGetDI(HBZR h,int _io_number,int & val);


/**
 * @brief Get the value of a given analog input.
 * @param h Handle of SDK.
 * @param _io_number, the logical channel number of the target analog input
 * @param val (double), the value of the analog input
 * @return 0 when no error
 */
BZR_API int bzrGetAI(HBZR h,int _io_number,double & val);

/**
 * @brief Set the value of a given digital output.
 * @param h Handle of SDK.
 * @param _io_number, the logical channel number of the target digital output
 * @param val, 1 means on, 0 means off
 * @return 0 when no error
 */
BZR_API int bzrSetDO(HBZR h,int _io_number,int val);


/**
 * @brief Set the value of a given analog Output.
 * @param h Handle of SDK.
 * @param _io_number, the logical channel number of the target analog output
 * @param val (double), the value of the analog output
 * @return 0 when no error
 */
BZR_API int bzrSetAO(HBZR h,int _io_number,double val);

/**
 * @brief Wait until motion is over or timeout
 * @param h Handle of SDK.
 * @param Robot index.
 * @param timeout (double), timeout value for the wait
 * @return 0 when no error
 */
BZR_API int bzrWaitEOM(HBZR h, int index, double timeout);


#ifdef __cplusplus
}
#endif
    
#endif