#ifndef EXCEPTIONHANDLER_H
#define EXCEPTIONHANDLER_H

#define WAYPOINT_NOT_EXIST_EXP  0
#define FAIL_TO_WAYPOINT_EXP    1
#define MASTER_NOT_EXIST_EXP    2
#define FAIL_TO_MASTER_EXP      3
#define FOLLOW_START_EXP        4
#define FOLLOW_STOP_EXP         5

class ExceptionHandler
{
    public:
        ExceptionHandler();
        void MoveExpHandler(int expNum);

    protected:

    private:
};

#endif // EXCEPTIONHANDLER_H
