#include "ros/ros.h"
#include "service_fib/service_fib.h"
#include <iostream>
#include <sstream>

using namespace std;

int fib(int n) 
{ 
    if (n <= 1) 
        return n; 
    return fib(n-1) + fib(n-2); 
} 

string print_array (vector<int32_t> v){
    stringstream ss;
    ss<<"[ ";
    for (int i=0; i<v.size(); i++){
        ss<<v[i]<<" ";
    }
    ss<<"]";
    return ss.str();
}

   bool service_callback(service_fib::service_fib::Request &req, service_fib::service_fib::Response & res){
      
      ROS_INFO("Request: indexx=%ld, len=%ld", (long int)req.index, (long int)req.len);
    
	int index=req.index;
  	int len=req.len;
  	vector<int32_t> seq (req.len);
  
  	for(int i=0; i<=len; i++){
      seq[i]=fib(index);
      index++;
      }

  string ss = print_array(seq);
  res.seq = seq;
  
  ROS_INFO("sending back response: %s", ss.c_str());
  return true;
      //ROS_INFO("sending back response: [%ld]", (long int)res.seq);
     return true;
   }
   
   int main(int argc, char **argv)
   {
     ros::init(argc, argv, "fib_server");
     ros::NodeHandle n;
  
     ros::ServiceServer service = n.advertiseService("service_fib", service_callback);
     ROS_INFO("Ready to add two ints.");
     ros::spin();
   
     return 0;
   }
