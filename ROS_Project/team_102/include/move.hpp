#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

class Monitor;

class SteeringMonitor : Monitor {
	double velcity[3];
	double acceleration[3];
	int position[3];
	PathAgent pathagent;
public:
	int *getPosition();

    static void moveto();
    static void moveback();
    static void cancelMove();
};

class PathAgent {
public:
    void nextPosition();
	void shortestPath();
};

void moveto();
void moveback();
void cancelMove();

#endif // MOVE_H_INCLUDED
