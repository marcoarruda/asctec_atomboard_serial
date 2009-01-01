 // ROS lib
#include "ros/ros.h"

// String libs
#include "std_msgs/String.h"
#include "std_msgs/UInt16.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int16MultiArray.h"
#include <sstream>
#include <iostream>
#include <string>

// Serial libs
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h>  // File control definitions
#include <termios.h> // POSIX terminal control definitionss

// Local libs
#include "inc/serial.h"
#include "inc/treat.h"

int fd;

int main(int argc, char **argv)
{
  // ROS variables
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<std_msgs::Int16MultiArray>("chatter", 10);
  ros::Rate loop_rate(1);
  std_msgs::UInt16 count;
  count.data = 0;

  fd = openSerial("/dev/ttyUSB1");
  confSerial(fd);

  std_msgs::String toPublish;
  std::stringstream ss_toPublish;

  while (ros::ok())
  {
    ss_toPublish.str(std::string());
    //ROS_INFO("loop [%d]", count.data);

    // read serial
    readSerial(fd, ss_toPublish);
    toPublish.data = ss_toPublish.str();
    //ROS_INFO("%s", toPublish.data.c_str());

    // validate

    // convert
    std_msgs::Int16MultiArray array;
    treatConvert(toPublish, array);

    // publish
    chatter_pub.publish(array);
    ros::spinOnce();
    loop_rate.sleep();
    count.data++;
  }

  return 0;
}
