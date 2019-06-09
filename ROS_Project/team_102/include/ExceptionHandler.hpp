#ifndef __EXCEPTIONHANDLER_HPP__
#define __EXCEPTIONHANDLER_HPP__

#define WAYPOINT_NOT_EXIST_EXP  		0
#define FAIL_TO_WAYPOINT_EXP    		1
#define MASTER_NOT_EXIST_EXP    		2
#define FAIL_TO_MASTER_EXP      		3
#define FOLLOW_START_EXP        		4
#define FOLLOW_STOP_EXP         		5
#define VOICE_NOT_GET_TRANSFORM_EXP 	6

class ExceptionHandler {
public:
	enum {
		OVERFLOW_EXCEPTION,
		TURN_SIDE_EXCEPTION,
		NO_PATH_EXCEPTION,
	};
	static ExceptionHandler exception(int type);
    void MoveExpHandler(int expNum);
};

#endif // EXCEPTIONHANDLER_H
