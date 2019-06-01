#include "SXXCoreTask/CoreTask.hpp"
#include <iostream>
#include <semaphore.h>
#include <fcntl.h>
#include <spawn.h>
#include <cstddef>
#include <assert.h>
#include <unistd.h>
#include "ctools.hpp"
#include <signal.h>

//ros launch and monitor status
//fork node and monitor palpitate
//server connection 

#define SERVER_IP "10.136.82.54"
#define SERVER_PORT 13245

#define ROS_VERSION "indigo"

int createSubProcess(int read_pipe, int write_pipe) {
    //std::string homePath = getenv("HOME");
    //std::string executablePath = homePath + "/catkin_ws/";
    int child_pid;
    std::string readPortArg = "--read-port=" + std::to_string(read_pipe);
    std::string writePortArg = "--write-port=" + std::to_string(write_pipe);
    child_pid = fork();
    if (child_pid > 0) {
        return child_pid;
    } else if (child_pid == 0) {
        //exit(0);
        execl("/bin/bash", "bash", (std::string("/opt/ros/") + ROS_VERSION + "/bin/rosrun").c_str(), "team_102", "main", readPortArg.c_str(), writePortArg.c_str(), NULL);
    } else {
        printf("Failed to create new process.\n");
    }
    return -1;
}

void loop(int argc, char **argv, char **envs) {
    SCT::Queue qchecker = SCT::Queue("com.sxx.ros_status_checker", SCT::Queue::SERIAL);
    SCT::Queue qnetwork = SCT::Queue("com.sxx.server_connection", SCT::Queue::SERIAL);
    SCT::Queue qpipe = SCT::Queue("com.sxx.pipe", SCT::Queue::SERIAL);
    SCT::Queue qserver = SCT::Queue("com.sxx.server", SCT::Queue::SERIAL);

    sem_t *s = sem_open("CRrosStatusCheckerSync", O_RDWR | O_CREAT, 0600, 0);
    sem_t exitSem;
    sem_t *ss;
    sem_init(&exitSem, 0, 0);
    if (s == NULL) {
        printf("Fatal error: failed to create semaphore.\n");
        sem_close(s);
        sem_unlink("CRrosStatusCheckerSync");
        return;
    }

    int filedes[2];
    int ret = pipe(filedes);
    if (ret == -1) {
        printf("Fatal error: failed to create pipe.\n");
            sem_destroy(&exitSem);
    sem_close(s);
    sem_unlink("CRrosStatusCheckerSync");
        return;
    }
    int pipe_read_daemon_to_node = filedes[0];
    int pipe_write_daemon_to_node = filedes[1];
    int sub_node_pid;
    
    ret = pipe(filedes);
    if (ret == -1) {
        printf("Fatal error: failed to create pipe.\n");
            sem_destroy(&exitSem);
    sem_close(s);
    sem_unlink("CRrosStatusCheckerSync");
    close(pipe_write_daemon_to_node);
        return;
    }
    int pipe_read_node_to_daemon = filedes[0];
    int pipe_write_node_to_daemon = filedes[1];

printf("1\n");
    SCT::Source nodePalpitateSource = SCT::Source(SCT::Source::Type::READ, pipe_read_node_to_daemon, 0, qpipe);
    SCT::Source nodePalpitateTimeout = SCT::Source(SCT::Source::Type::TIMER, 0, 0, qpipe);
    {

        uint64_t lastPalpitateTime = 0;
        
        nodePalpitateSource.setEventHandler([&sub_node_pid, &ss, &lastPalpitateTime, pipe_read_node_to_daemon ,&exitSem] {
            lastPalpitateTime = CRHighPrecisionTime();
            void *pdata = NULL;
            int length;
            int ret = retrieveData(pipe_read_node_to_daemon, pdata, length);
            
            if (ret < 0) {
                //exception here
            }
		printf("daemon receive: %s\n", pdata);

            if (strncmp(static_cast<const char*>(pdata), "heart", length) == 0) {
                //printf("receive heart packet.\n");
            } else if (strncmp(static_cast<const char *>(pdata), "killall", length) == 0) {
                kill(sub_node_pid, SIGSTOP);
                sem_post(ss);
                sleep(1);
		sleep(10);
                sem_post(&exitSem);
            }

            if (pdata != NULL) {
                free(pdata);
            }
        });
printf("1\n");

        nodePalpitateTimeout.setTimer(SCT::Time::NOW, NSEC_PER_MSEC * 500, 0);
        nodePalpitateTimeout.setEventHandler([&lastPalpitateTime, &sub_node_pid, &exitSem] {
            uint64_t current_time = CRHighPrecisionTime();
            if (lastPalpitateTime == 0) {
                return;
            }
            if (current_time - lastPalpitateTime > NSEC_PER_MSEC * 100000) {
                //timeout
                printf("Timeout\n");
                kill(sub_node_pid, SIGSTOP);
                system("rosnode kill -a");
                sem_post(&exitSem);
            }
        }); 
    }
printf("1\n");
    

//    qnetwork.sync([&qserver]{
//        int sock;
//        int ret = connectServer(sock, SERVER_IP, SERVER_PORT);

 //       if (ret == -1) {
 ///           printf("Server not available.\n");
 //           return;
 //       }
 //   });

printf("1\n");
    qchecker.sync([&ss, s, &exitSem, envs]{

        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        pid_t child_pid;
        int ret;

        child_pid = fork();
        if (child_pid == 0) {
            int nullfd = open("/dev/null", O_RDWR);
            //dup2(nullfd, 1);
            //dup2(nullfd, 2);
            execl("/usr/bin/python", "python", (std::string("/opt/ros/") + ROS_VERSION + "/bin/roslaunch").c_str(), "team_102", "team102.launch", NULL);
        }
        ts.tv_sec += 10;
        ret = sem_timedwait(s, &ts);
        if (ret == -1) {
            printf("There maybe something wrong while starting ros.\n");
            sem_post(&exitSem);
            return;
        }
        printf("ros started.\n");
        ss = sem_open("CRACKSync", O_RDWR);
        if (ss == SEM_FAILED) {
            printf("There are some issues while starting ros.\n");
            sem_post(&exitSem);
            return;
        }
    });

printf("1\n");
    sub_node_pid = createSubProcess(pipe_read_daemon_to_node, pipe_write_node_to_daemon);
    nodePalpitateSource.resume();
    nodePalpitateTimeout.resume();
    

    sem_wait(&exitSem);
    sem_destroy(&exitSem);
    sem_close(s);
    sem_unlink("CRrosStatusCheckerSync");
    close(pipe_write_daemon_to_node);
    close(pipe_read_node_to_daemon);
}


int main(int argc, char **argv, char **envs) {

    while (1) {
        loop(argc, argv, envs);
        printf("daemon restart\n");
    }
    return 0;
}
