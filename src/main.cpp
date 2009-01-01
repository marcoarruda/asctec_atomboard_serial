#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/UInt16.h"

#include <sstream>


// Serial Libs
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h>  // File control definitions
#include <termios.h> // POSIX terminal control definitionss

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
  fd = open("/dev/ttyUSB1", O_CREAT | O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK, S_IRUSR|S_IWUSR);
  if(!fd)
  {
    ROS_INFO("Sooo sorry! there was a problem opennig the serial port!");
    ROS_INFO("Are you sure you have access to ttyS1?");
    return 1;
  }
  struct termios port_settings; // structure to store the port settings in
  cfsetispeed(&port_settings, B9600);    // set baud rates
  port_settings.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
  port_settings.c_iflag = IGNPAR;
  port_settings.c_oflag = 0;
  port_settings.c_lflag = 0;
  tcsetattr(fd, TCSANOW, &port_settings); // apply the settings to the port

  std_msgs::String msg;
  std::stringstream ss;
  int end_count;
  char mensagem[60];
  int mensagem_count;

  while (ros::ok())
  {
    end_count = 0;
    mensagem_count = 0;
    ROS_INFO("loop [%d]", count.data);
    for(int i=0;i<60;i++) {
      mensagem[i] = 0x00;
    }

    while(end_count < 2) {
      result = read(fd, &data, 1);
      if(result > 0) {
        if (data == ']') {
          end_count++;
        }
        //ROS_INFO("result: [%d]; data: [%c]", result, data);
        mensagem[mensagem_count] = data;
        mensagem_count++;
      } else {
        //ROS_INFO("result: [%d]", result);
        break;
      }
    }
    ROS_INFO("mensagem: [%.60s];", &mensagem[0]);
    std_msgs::String toPublish;
    std::stringstream ss_toPublish;
    int char_index = 0;
    char a = mensagem[char_index];
    while(a != 0x00) {
      ss_toPublish << a;
    }
    ROS_INFO();
    toPublish.data = ss_toPublish.str();
    ROS_INFO("%s", toPublish.data.c_str());

    chatter_pub.publish(toPublish);
    ros::spinOnce();
    loop_rate.sleep();
    count.data++;
  }

  return 0;
}
