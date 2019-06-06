#ifndef __GRAB_MONITER_HPP__
#define __GRAB_MONITER_HPP__

class Monitor;

class GrabbedItems {
	int position[3];
	int weight;
public:
	GrabbedItems(int x, int y, int z, int weight);
	static void getPosition();
};

class GrabMonitor : Monitor {
	static int MAX_GRAB_SIZE;
public:
	static void move();
	static void stop();
	static void status();

    static void GrabObj();
    static void PassObj();
};

void GrabResultCallback(const std_msgs::String::ConstPtr& res);
void PassResultCallback(const std_msgs::String::ConstPtr& res);
void GrabObj();
void PassObj();

#endif
