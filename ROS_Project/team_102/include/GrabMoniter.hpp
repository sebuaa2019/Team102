#ifndef __GRAB_MONITER_HPP__
#define __GRAB_MONITER_HPP__

static void GrabSwitch(bool inActive); 
static void PassSwitch(bool inActive);
void GrabResultCallback(const std_msgs::String::ConstPtr& res);
void PassResultCallback(const std_msgs::String::ConstPtr& res);
void GrabObj();
void PassObj();

#endif
