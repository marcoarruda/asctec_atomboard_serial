// Serial libs
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h>  // File control definitions
#include <termios.h> // POSIX terminal control definitionss

int openSerial(char *portName) {
  int fd = open(portName, O_CREAT | O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK, S_IRUSR|S_IWUSR);
  if(!fd)
  {
    ROS_INFO("Sooo sorry! there was a problem opennig the serial port!");
    ROS_INFO("Are you sure you have access to ttyS1?");
    return -1;
  }
  struct termios port_settings; // structure to store the port settings in
  cfsetispeed(&port_settings, B9600);    // set baud rates
  port_settings.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
  port_settings.c_iflag = IGNPAR;
  port_settings.c_oflag = 0;
  port_settings.c_lflag = 0;
  tcsetattr(fd, TCSANOW, &port_settings); // apply the settings to the port
  return fd;
}

