#include <stdio.h>
#include <ros/ros.h>
#include <semaphore.h>
#include <fcntl.h>

int main(int argc, char **argv) {

    printf("%d\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    ros::init(argc, argv, "rosStatusChecker");
    
    sem_t *sem = sem_open("CRrosStatusCheckerSync", O_RDWR);
    if (sem == SEM_FAILED) {
        printf("Fatal error: daemon not exist!\n");
        ros::shutdown();
        system("rosnode kill -a");
    }

    sem_post(sem);

    while (ros::ok()) {
        ros::spinOnce();
    }

    return 0;
}