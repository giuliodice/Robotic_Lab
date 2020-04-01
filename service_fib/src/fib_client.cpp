#include "ros/ros.h"
#include "service_fib/service_fib.h"
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "fib_client");
  
  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<service_fib::service_fib>("service_fib");
  while (ros::ok())
  {
  service_fib::service_fib srv;
  cout<<"Insert index: ";
  cin>>srv.request.index;
  cout<<"Insert len: ";
  cin>>srv.request.len;

  if (client.call(srv))
  {
    ROS_INFO("Request index= %ld len= %ld ", srv.request.index, srv.request.len);
  }
  else
  {
    ROS_ERROR("ERROR");
    return 1;
  }
  ros::spinOnce();
}
  return 0;
}
