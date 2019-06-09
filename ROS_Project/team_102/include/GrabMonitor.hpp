#ifndef __GRAB_MONITER_HPP__
#define __GRAB_MONITER_HPP__

class Monitor;

class GrabbedItems {
	int position[3];
	int weight;
public:
	GrabbedItems(int x, int y, int z, int weight){};
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

class Request {
	int src[3];
	int dst[3];
	enum {
		STEERING,
		GRAB,
	} requestType;
	GrabbedItems item;
public:
    Request(int *src, int *dst, int type);
	static bool speech(char *fp);

	bool readSpeechPattern(int p);
	int *getSrc();
	int *getDst();
	bool hasitm();
};

void GrabResultCallback(const std_msgs::String::ConstPtr& res);
void PassResultCallback(const std_msgs::String::ConstPtr& res);
void GrabObj();
void PassObj();

#endif
