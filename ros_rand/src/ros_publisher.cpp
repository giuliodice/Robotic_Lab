#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include "ros_rand/custom.h"

#include <time.h>
#include <iostream>
#include <cstdlib>


using namespace std;

class ROS_PUB {

	public:
		ROS_PUB();
		void generate_rand ();
	private:
		ros::NodeHandle _nh;
		ros::Publisher _topic_pub;		
};


ROS_PUB::ROS_PUB(){
  generate_rand();
}

void ROS_PUB::generate_rand() {
	int hz=1;
	ros::Rate loop_rate(hz);
	ros_rand::custom msg;
	
		while (ros::ok()) {
		msg.num1 = rand()%200;
		msg.num2 = rand()%200;
		msg.sum =0.0;
		_topic_pub = _nh.advertise<ros_rand::custom>("/rand",10);
		_topic_pub.publish(msg);
		ROS_INFO("ESEMPIO: %.2f ---- %.2f ", msg.num1, msg.num2);
		loop_rate.sleep();
		}

}

int main( int argc, char** argv ) {
	ros::init(argc, argv, "ros_publisher_1");
	ROS_PUB pub;
	//ros::spin();
	return 0;
}
