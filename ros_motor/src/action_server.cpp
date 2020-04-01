#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <actionlib/server/simple_action_server.h> 
#include "ros_motor/motorAction.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>

class action_class {

	private:
  	ros::NodeHandle nh_;
  	actionlib::SimpleActionServer<ros_motor::motorAction> action_server; 
	ros_motor::motorFeedback feedback; 
	ros_motor::motorResult result;
	ros_motor::motorGoal goal;

  	std::string action_name;
  	float xc;
  	

	public:
		action_class(std::string name) :
  	  action_server(nh_, name, boost::bind(&action_class::executeCB, this, _1), false),
  	  action_name(name) {
				action_server.registerPreemptCallback(boost::bind(&action_class::preemptCB, this));
				action_server.start();
  		}
	
		void preemptCB(){ 
			ROS_WARN("%s got preempted!", action_name.c_str());
			result.final_position = xc;
			action_server.setPreempted(result,"I got Preempted"); 
  	}
  
		void executeCB(const ros_motor::motorGoalConstPtr &goal) {
			if(!action_server.isActive() || action_server.isPreemptRequested()) return;
			ros::Rate rate(10);
			ROS_INFO("%s is processing the goal %f", action_name.c_str(), goal->fin_pos);
			float x_in = (goal->max_vel)*0.1;
			for(xc = goal->init_pos ; xc <= goal->fin_pos; xc = xc+x_in){
				//Check for ros
				if (!ros::ok()) {
					result.final_position  = xc;
					action_server.setAborted(result,"I failed !");
					ROS_INFO("%s Shutting down",action_name.c_str());
					break;
				}
		
				if(!action_server.isActive() || action_server.isPreemptRequested()){
					return;
				}	

				if(goal->fin_pos <= xc) {
					ROS_INFO("%s Succeeded at getting to goal %f", action_name.c_str(), goal->fin_pos);
					result.final_position = xc;
					action_server.setSucceeded(result);
				}
				else {
					feedback.curr_pos = xc;
					ROS_INFO("Current position %f / %f",feedback.curr_pos,goal->fin_pos);
					action_server.publishFeedback(feedback);
					if((goal->fin_pos)- xc <x_in){
						x_in = goal->fin_pos -xc;
					}
			}
		rate.sleep();
		}	
  }
};

int main(int argc, char** argv)
{

  ros::init(argc, argv, "motor_action");
  ROS_INFO("Starting motor Action Server");
  action_class motor_action_obj(ros::this_node::getName());
  ros::spin();
  return 0;
}
