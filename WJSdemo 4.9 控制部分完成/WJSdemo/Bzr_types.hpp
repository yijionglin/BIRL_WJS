/***************************************************************************
  tag: Rock Chen  Wednesday April 29 2015,  Bzr_types.hpp

                        Bzr_types.hpp -  description
                           -------------------
    begin                : April 29 2015
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
 *		$Id: Bzr_types.hpp$
 *		$Name:  $
 *  
 ****************************************************************************/

#ifndef BZR_TYPES_HPP
#define	BZR_TYPES_HPP

#define ERR_MSG_PRINT          999       //! Esp. messages from print() of Lua code directly sending back to clients/IDEs.
#define ERR_MSG_HWALM        -9900       //! Alarm messages from Hardware Related Device 
#define ERR_MSG_HWDEV        -9999       //! Error messages from Hardware Related Device 
#define ERR_MSG_KDL          -11         //! Error messages from KDL functions   
#define ERR_MSG_RECONNECT   -99999       //! Error re-connecting, since broken connection

// Mode
#define MODE_MANUAL     0
#define MODE_RECOVERY   1
#define MODE_FREE       2
#define MODE_AUTO       3

// Send Event (Simulation)
#define SEVT_ESTOP          0
#define SEVT_MOTION_STOP    1
#define SEVT_MODE_SWITCH    2 
#define SEVT_ERR_RESET      3
#define SEVT_SERVO_ON       4
#define SEVT_PRGM_START     5
#define SEVT_PRGM_PAUSE     6
#define SEVT_PRGM_CANCEL    7
#define SEVT_PRGM_STOP      8
#define SEVT_MODE_SWITCH2   9 

// System status
#define ST_COUNT    5   //! number of different status
#define ST_EMO      0   //! ESTOP or MOTION_STOP
#define ST_ERR      1   //! Error reset
#define ST_MODE     2   //! Mode changed
#define ST_SERVO    3   //! Servo-on or off
#define ST_PROG     4   //! Program status

// Mode status
#define ST_MODE_MAN         0
#define ST_MODE_AUTO        1
#define ST_MODE_RECOVERY    2
#define ST_MODE_FREE        3
#define ST_MODE_JUMP_BASE   20  //safety jumped
#define ST_MODE_MAN_J       (ST_MODE_MAN+ST_MODE_JUMP_BASE)
#define ST_MODE_AUTO_J      (ST_MODE_AUTO+ST_MODE_JUMP_BASE)
#define ST_MODE_RECOVERY_J  (ST_MODE_RECOVERY+ST_MODE_JUMP_BASE)
#define ST_MODE_FREE_J      (ST_MODE_FREE+ST_MODE_JUMP_BASE)

// Program status
#define ST_PROG_STOP    0
#define ST_PROG_RUN     1
#define ST_PROG_PAUSE   2
#define ST_PROG_CANCEL  3
#define ST_PROG_UNKNOWN 4

struct EPos {
    double x;
    double y;
    double z;
    double A;
    double B;
    double C;
    long S;
    long T;
    double E_A1;
    double E_A2;
    double E_A3;
};

struct EAxis {
    double A1; 
    double A2; 
    double A3; 
    double A4; 
    double A5; 
    double A6; 
    double A7; 
    double A8; 
    double E_A1;
    double E_A2;
    double E_A3;
};

struct EFrame {
    double x,y,z;
    double A,B,C;
};

struct EFrameData {
    int baseNO;  //base frame ID
    int toolNO;  //tool frame ID
    int IPO_MODE;   //1/2/3: base/tool/SA
    int Torque_state;   //For torque mode use only
    double Circ_alpha;  //the angle of motion for circular motion
};

struct EProfile {
    int Type;     //1/2/3/4: trap/spline/traphalf/scurve
    double MaxVel;  //percentage w.r.t. maximal speed
    double MaxAcc;  //percentage w.r.t. maximal acc
    double MaxJerk; //percentage w.r.t. maximal jerk
    double AppDist;  //approximate distance
    double AccTime; //acceleration time
    double DecTime; //deceleration time  (Reserved)
};

// a data structure for an arc movement, this is for another way of describing arc movement
struct EArc {
    EPos center;
    std::vector<double> startOrientation;
    std::vector<double> endOrientation;
    double radius;
    double startAngle;
    double endAngle;
};


struct EETeachData {
    std::vector<std::string> eaxis_var;
    std::vector<std::string> epos_var;
    std::vector<std::string> fdata_var;
    std::vector<std::string> profile_var;
    std::vector<std::string> baseNames;
    std::vector<std::string> toolNames;
    std::vector<EAxis> eaxis;
    std::vector<EPos> epos;
    std::vector<EFrame> baseFrame;
    std::vector<EFrame> toolFrame;
    std::vector<EFrameData> fdata;
    std::vector<EProfile> profile;
};

struct ETeachData{
    std::vector<std::string> robotlist;
    std::vector<EETeachData> rdata;
};

struct RobotInfo{
    std::string name;
    int nJoints;
};

#define JNT_TYPE_REVOLUTE     1
#define JNT_TYPE_PRISMATIC    2
struct JointInfo{
    std::string name;
    int jntType;
};

//Message levels.
#ifndef ESIGNAL_LEVEL 
#define ESIGNAL_LEVEL
enum ESignalLevel { ENever = 0, EFatal, ECritical, EError, EWarning, EInfo, EDebug, EPrint=999 };
#endif

#endif	/* BZR_TYPES_HPP */

