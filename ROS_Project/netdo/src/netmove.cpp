#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <SXXCoreTask/CoreTask.h>
#include <netprotocol.hpp>

ros::Publisher vel_pub;

static void retrieveData(void *info) {
    void *data;
    int length;
    ((NetProtocol*)info)->retrieveData_noblock(&data, length);
    printf("recv%s\n", data);

    for (int i = 0; i < length; i++) {
        char a = ((char*)data)[i];

        switch(a) {
            case 'h':
            {
                geometry_msgs::Twist vel_cmd;

                vel_cmd.linear.x = 0.1;
                vel_cmd.linear.y = 0;
                vel_cmd.linear.z = 0;

                vel_cmd.angular.x = 0;
                vel_cmd.angular.y = 0;
                vel_cmd.angular.z = 0;
                vel_pub.publish(vel_cmd);
                break;
            }
            case 'q':
            {
                geometry_msgs::Twist vel_cmd;

                vel_cmd.linear.x = 0;
                vel_cmd.linear.y = 0.1;
                vel_cmd.linear.z = 0;

                vel_cmd.angular.x = 0;
                vel_cmd.angular.y = 0;
                vel_cmd.angular.z = 0;
                vel_pub.publish(vel_cmd);
                break;
            }
            case 'w':
            {
                geometry_msgs::Twist vel_cmd;

                    vel_cmd.linear.x = -0.1;
                    vel_cmd.linear.y = 0;
                    vel_cmd.linear.z = 0;

                    vel_cmd.angular.x = 0;
                    vel_cmd.angular.y = 0;
                    vel_cmd.angular.z = 0;
                    vel_pub.publish(vel_cmd);
                break;
            }
            case 'z':
            {
                geometry_msgs::Twist vel_cmd;

                vel_cmd.linear.x = 0;
                vel_cmd.linear.y = -0.1;
                vel_cmd.linear.z = 0;

                vel_cmd.angular.x = 0;
                vel_cmd.angular.y = 0;
                vel_cmd.angular.z = 0;
                vel_pub.publish(vel_cmd);
                break;
            }
            case 's':
            {
                geometry_msgs::Twist vel_cmd;

                vel_cmd.linear.x = 0;
                vel_cmd.linear.y = -0.1;
                vel_cmd.linear.z = 0;

                vel_cmd.angular.x = 0;
                vel_cmd.angular.y = 0;
                vel_cmd.angular.z = 0;
                vel_pub.publish(vel_cmd);
                break;
            }
        }
        
        ros::spinOnce();
    }

    free(data);
}

int main(int argc, char **argv) {

    SCT_queue_t queue = SCT_get_global_queue(SCT_QUEUE_PRIORITY_DEFAULT, 0);

    ros::init(argc, argv, "netmove");
    ros::NodeHandle n;
    vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

NetProtocol p;

    p.connectServer();
    printf("%d\n", p.sock);
    //fcntl(p.sock, F_SETFL, O_NONBLOCK);

    SCT_source_t server_source = SCT_source_create(SCT_SOURCE_TYPE_READ, p.sock, 0, queue);
    SCT_set_context(server_source, (void*)&p);
    SCT_source_set_event_handler_f(server_source, retrieveData);
    SCT_resume(server_source);

    SCT_main();
    return 0;
}