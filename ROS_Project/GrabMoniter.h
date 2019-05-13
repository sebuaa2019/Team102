#ifndef _GET_OBJ_
#define _GET_OBJ_

static void GrabSwitch(bool inActive)
static void PassSwitch(bool inActive)
void GrabResultCallback(const std_msgs::String::ConstPtr& res)
void PassResultCallback(const std_msgs::String::ConstPtr& res)
void GrabObj()
void ComeBackWithObj()
void PassObj()

#endif
