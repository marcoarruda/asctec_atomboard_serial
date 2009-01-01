 // ROS lib
#include "ros/ros.h"

// String libs
#include "std_msgs/String.h"
#include "std_msgs/UInt16.h"
#include <sstream>

// Serial libs
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h>  // File control definitions
#include <termios.h> // POSIX terminal control definitionss

// Local libs
#include "inc/serial.h"

int fd, result = -1;
unsigned char data;

int main(int argc, char **argv)
{
  // ROS variables
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 10);
  ros::Rate loop_rate(1);
  std_msgs::UInt16 count;
  count.data = 0;

  fd = openSerial("/dev/ttyUSB1");

  std_msgs::String msg;
  std::stringstream ss;
  int end_count;
  std_msgs::String toPublish;
  std::stringstream ss_toPublish;

  while (ros::ok())
  {
    end_count = 0;
    ss_toPublish.str(std::string());
    ROS_INFO("loop [%d]", count.data);


    while(end_count < 2) {
      result = read(fd, &data, 1);
      if(result > 0) {
        if (data == ']') {
          end_count++;
        }
        //ROS_INFO("result: [%d]; data: [%c]", result, data);
        ss_toPublish << data;
      } else {
        //ROS_INFO("result: [%d]", result);
        break;
      }
    }
    const char* a = ss_toPublish.str().c_str();
    ROS_INFO("mensagem: [%.60s];", &a[0]);
    toPublish.data = ss_toPublish.str();
    ROS_INFO("%s", toPublish.data.c_str());

    chatter_pub.publish(toPublish);
    ros::spinOnce();
    loop_rate.sleep();
    count.data++;
  }

  return 0;
}
