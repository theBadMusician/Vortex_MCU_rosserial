#include "ros/ros.h"
#include "std_msgs/UInt16.h"

#include <sstream>

int main(int argc, char **argv)
{

  ros::init(argc, argv, "talker");

  ros::NodeHandle n;

  ros::Publisher chatter_pub = n.advertise<std_msgs::UInt16>("pwm_signal", 512);

  ros::Rate loop_rate(10000);

  uint16_t count = 0;
  while (ros::ok())
  {
    std_msgs::UInt16 msg;
    msg.data = count;

    ROS_INFO("%u", msg.data);

    chatter_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
    if (count > 65535) count = 0;
  }


  return 0;
}
