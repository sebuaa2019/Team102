#include <stdio.h>
#include <ros/ros.h>
#include <semaphore.h>
#include <fcntl.h>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

int main(int argc, char **argv) {

    printf("%d\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    ros::init(argc, argv, "rosStatusChecker");
    ros::NodeHandle n;
    ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    
    sem_t *sem = sem_open("CRrosStatusCheckerSync", O_RDWR);
    if (sem == SEM_FAILED) {
        printf("Fatal error: daemon not exist!\n");
        ros::shutdown();
        system("rosnode kill -a");
    }
    sem_t *sem1 = sem_open("CRACKSync", O_RDWR | O_CREAT, 0600, 0);
    if (sem1 == SEM_FAILED) {
        printf("Fatal error: semaphore create failed.\n");
        ros::shutdown();
        system("rosnode kill -a");
    }

    sleep(3);
    sem_post(sem);

    sem_wait(sem1);

    while (ros::ok()) {
        geometry_msgs::Twist vel_cmd;
        printf("Check node: stop\n");

        vel_cmd.linear.x = 0;
        vel_cmd.linear.y = 0;
        vel_cmd.linear.z = 0;
        vel_cmd.angular.x = 0;
        vel_cmd.angular.y = 0;
        vel_cmd.angular.z = 0;
        vel_pub.publish(vel_cmd);
        ros::spinOnce();
    }

    return 0;
}