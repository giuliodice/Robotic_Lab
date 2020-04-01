#include "ros/ros.h"
#include <iostream>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include "ros_motor/motorAction.h"
#include <stdlib.h>

int main (int argc, char **argv) {

	ros::init(argc, argv, "motor_action_client");
  if(argc != 5){ //se il numero di argomenti è diverso da 3 
		ROS_INFO("%d",argc);
		ROS_WARN("Usage: motor_action_client <init_pos> <fin_pos> <max_vel> <time_to_preempt_in_sec>");
		return 1;
	}
  ros_motor::motorGoal goal;
  goal.init_pos = atof(argv[1]); 
  goal.fin_pos  = atof(argv[2]);
  goal.max_vel  = atof(argv[3]);

  if((goal.init_pos <0) || (goal.fin_pos < 0) || (goal.max_vel < 0) || (argv[4] < 0)){
   ROS_WARN("Usage: All values must be positive");
  }

  // create the action client
  // true causes the client to spin its own thread
  actionlib::SimpleActionClient<ros_motor::motorAction> action_client("motor_action", true);

  ROS_INFO("Waiting for action server to start.");

  // wait for the action server to start
  action_client.waitForServer(); //will wait for infinite time

  ROS_INFO("Action server started, sending goals.");

  // send a goal to the action
  
 
  ROS_INFO("Sending Goal [xi=%f]  [xf=%f] [vm=%f] and Preempt time of [%f]",goal.init_pos, goal.fin_pos, goal.max_vel, atoi(argv[4]));
  action_client.sendGoal(goal);

 
  bool finished_before_timeout = action_client.waitForResult(ros::Duration(atoi(argv[4]))); //il secondo argomento ci dice quanto tempo vogliamo aspettare


  if (finished_before_timeout) {
    actionlib::SimpleClientGoalState state = action_client.getState();
    ROS_INFO("Action finished: %s",state.toString().c_str());
	action_client.cancelGoal();

  }
  else
    ROS_INFO("Action did not finish before the time out.");

  //exit
  
  return 0;
}
