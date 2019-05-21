#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>


int main(int argc, char **argv) {
    ros::init(argc, argv, "netmove");
    ros::NodeHandle n;
    ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    
    int port = 13245;
    struct sockaddr_in cliAddr;
    struct sockaddr_in servAddr;
    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = htons(0);

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    bind(sock, (struct sockaddr*)&cliAddr, sizeof(struct sockaddr_in));

    servAddr.sin_family = AF_INET;
    struct in_addr sin_addr;
    inet_aton("10.136.117.172", &sin_addr);
    memcpy(&servAddr.sin_addr, &sin_addr, sizeof(struct in_addr));

    servAddr.sin_port = htons(port);
    socklen_t serLen = sizeof(servAddr);

    connect(sock, (struct sockaddr*)&servAddr, serLen);

    while (ros::ok()) {
        char a;
        recv(sock, &a, 1, 0);
        printf("received: %c\n", a);

        geometry_msgs::Twist vel_cmd;

        vel_cmd.linear.x = 0.1;
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