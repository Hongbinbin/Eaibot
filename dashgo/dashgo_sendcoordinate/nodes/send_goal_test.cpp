/* THIS IS A SUBSCRIBER NODE */

#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <iostream>
#include <string>
#include <std_msgs/String.h>

#define GOHOME "HOME"
#define GOGOAL1 "GOAL1"
#define GOGOAL2 "GOAL2"
#define GOGOAL3 "GOAL3"
#define GOGOAL4 "GOAL4"
#define TAKEMEAL "TAKE"
using namespace std;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

bool Callback_flag = false;
string arrive_position = "";
string msg_str = "";

typedef struct _POSE
{
  double X;
  double Y;
  double Z;
  double or_x;
  double or_y;
  double or_z;
  double or_w;
} POSE;


// Set coordinates of the space for N1
POSE home ={0.150877491216, -0.0653026237985, 0.138, 0.0, 0.0, -0.00121532079559, 0.999999261497};
POSE takemeal = {0.277803149202, -0.443939687916, 0.138, 0.0, 0.0, -0.691625729087, 0.722256083993};
/* TMTS */
POSE goal1 = {0.914020121098, 0.54086202383, 0.0, 0.0, 0.0, -0.710723441559, 0.703471527227};
POSE goal2 = {0.968357205391, -0.24762737751, 0.0, 0.0, 0.0, 0.704834806617, 0.709371479114};
POSE goal3 = {1.40087115765, -0.53991484642, 0.0, 0.0, 0.0, 0.134545641846, 0.990907397419};
POSE goal4 = {0.936185240746, 0.195249557495, 0.0, 0.0, 0.0, 0.713097393693, 0.70106498066};


string setGoal(POSE pose)
{
     //tell the action client that we want to spin a thread by default  
    MoveBaseClient ac("move_base", true);  
      
    //wait for the action server to come up  
    while(!ac.waitForServer(ros::Duration(10.0)))
    {  
        ROS_WARN("Waiting for the move_base action server to come up.\n");  
    }  
    //ac.waitForServer(ros::Duration(60.0));
  
    move_base_msgs::MoveBaseGoal goal;  
      
    //we'll send a goal to the robot to move 1 meter forward  
    goal.target_pose.header.frame_id = "map";  
    goal.target_pose.header.stamp = ros::Time::now();  
   
    goal.target_pose.pose.position.x = pose.X; 
    goal.target_pose.pose.position.y = pose.Y;  
    goal.target_pose.pose.position.z = pose.Z;   
    goal.target_pose.pose.orientation.x = pose.or_x;
    goal.target_pose.pose.orientation.y = pose.or_y;
    goal.target_pose.pose.orientation.z = pose.or_z;
    goal.target_pose.pose.orientation.w = pose.or_w;   
    
    ROS_INFO_STREAM("Sending goal...\n");  
    
    ac.sendGoal(goal);  
      
    ac.waitForResult();  
      
    if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){  
       ROS_INFO("it is successful\n");   
       return "t";
    }else{  
       ROS_ERROR("The base failed move to goal!!!\n");
       return "u";
    }   
}

void poseCallback(const std_msgs::String::ConstPtr &msg)
{
    ROS_INFO_STREAM("Topic is Subscriber\n");
    std::cout<<"Get topic text: " << msg->data << std::endl;
    
    Callback_flag = true;
    msg_str = msg->data;
}

int main(int argc, char* argv[]){
    ros::init(argc, argv, "send_goal_test");
    ros::NodeHandle nh;

    // pose_topic_pub: to control the robot arm in advance, therefore move to the coordinate.
    ros::Subscriber sub_destination = nh.subscribe("destination_topic", 10, poseCallback); //poseCallback is the call back function name.
    // setHome(pub_initialpose);
    ROS_INFO_STREAM("Finished set a initial position.\n");
    std_msgs::String msg;

   // go to the (known) coordinate

    while(ros::ok())
    {
       if(Callback_flag == true)
        {
            Callback_flag = false;

            if(msg_str == GOHOME)
            {
                msg_str = "GO HOME NOW....\n";
                arrive_position = setGoal(home);
            }else  if(msg_str == GOGOAL1)
            {
                msg_str = "GET TO GOAL 1 NOW....\n";
                arrive_position = setGoal(goal1);

            }else  if(msg_str == GOGOAL2)
            {
                msg_str = "GET TO GOAL 2 NOW....\n";
                arrive_position = setGoal(goal2);
            }else  if(msg_str == GOGOAL3)
            {
                msg_str = "GET TO GOAL 3 NOW....\n";
                arrive_position = setGoal(goal3);
            }else  if(msg_str == GOGOAL4)
            {
                msg_str = "GET TO GOAL 4 NOW....\n";
                arrive_position = setGoal(goal4);
            }else  if(msg_str == TAKEMEAL)
            {
                msg_str = "";
                arrive_position = setGoal(takemeal);
            }
            else
            {
                msg_str = "Wrong input\n";
            }
        }
        ros::spinOnce();
    }

    return 0;
}