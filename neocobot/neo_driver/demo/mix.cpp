// Basic include
#include <ros/ros.h>
#include <string>
// OMNI 6 include 
#include "neo_msgs/EndPose.h"
#include "neo_msgs/JointAngles.h"
#include "neo_msgs/JointCurrent.h"
#include "neo_msgs/JointVelocity.h"
#include "neo_msgs/SetEOATAction.h"
#include "neo_msgs/MoveToPose.h"
// N1 include
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <iostream>
#include <std_msgs/String.h>

// Meaningless definitions
#define GOHOME "HOME"
#define GOHOME_2 "HOME2"
#define GOGOAL1 "GOAL1"
#define GOGOAL2 "GOAL2"
#define GOGOAL3 "GOAL3"
#define GOGOAL4 "GOAL4"
#define TAKEMEAL "TAKE"

// Collect the meal
#define COLLECT_FROM_GOAL_1_1 "COLLECT_FROM_GOAL_1_1"
#define COLLECT_FROM_GOAL_1_2 "COLLECT_FROM_GOAL_1_2"
#define COLLECT_FROM_GOAL_2_1 "COLLECT_FROM_GOAL_2_1"
#define COLLECT_FROM_GOAL_2_2 "COLLECT_FROM_GOAL_2_2"

// Take the meal
#define TAKE_TO_GOAL_1_1 "TAKE_TO_GOAL_1_1"
#define TAKE_TO_GOAL_1_2 "TAKE_TO_GOAL_1_2"
#define TAKE_TO_GOAL_2_1 "TAKE_TO_GOAL_2_1"
#define TAKE_TO_GOAL_2_2 "TAKE_TO_GOAL_2_2"


using namespace std;

// START For N1
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

// END For N1

// Set coordinates of the space for N1
POSE home = {0.198950537479, -0.0506683966777, 0.138, 0.0, 0.0, 0.0145251092119, 0.999894505037};
POSE home2 = {0.136499346146, 0.0263122121675, 0.138, 0.0, 0.0, -9.27490156775e-05, 0.999999995699};

/* FOR TEST */
POSE goal1 = {-1.60594665031, -1.61384410883, 0.138, 0.0, 0.0, -0.6645250777, 0.747265964104};
POSE goal2 = {0, 0, 0, 0, 0, 0, 0};
POSE goal3 = {-3.72713303134, 1.11758145534, 0.138, 0.0, 0.0, 0.67681465113, 0.736153467706};
POSE goal4 = {-3.29754103329, 1.8297248201, 0.138, 0.0, 0.0, 0.999998940386, 0.00145575619787};
// POSE goalx = {0, 0, 0, 0, 0, 0, 0};

/* My goals for use */
POSE goal1_1 = {-1.60594665031, -1.61384410883, 0.138, 0.0, 0.0, -0.6645250777, 0.747265964104};
POSE goal1_2 = {0, 0, 0, 0, 0, 0, 0};
POSE goal2_1 = {-3.72713303134, 1.11758145534, 0.138, 0.0, 0.0, 0.67681465113, 0.736153467706};
POSE goal2_2 = {-3.29754103329, 1.8297248201, 0.138, 0.0, 0.0, 0.999998940386, 0.00145575619787};

// Won't use this
POSE takemeal ={0, 0, 0, 0, 0, 0, 0};

// START: pre-work for N1
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
    
    printf("Get to the destination now...\n");  
    
    ac.sendGoal(goal);  
      
    ac.waitForResult();  
      
    if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){  
       ROS_INFO("ROS_ERROR: It is successful\n");
       printf("It is successful\n");   
       return "t";
    }else{  
       ROS_ERROR("ROS_ERROR: The base failed move to goal!!!\n");
       printf("The base failed move to goal!!!\n");  
       return "u";
    }   
}

void poseCallback(const std_msgs::String::ConstPtr &msg)
{
    printf("Topic is Subscriber.\n");
    std::cout<<"Get topic command text: " << msg->data << std::endl;
    
    Callback_flag = true;
    msg_str = msg->data;
}
// END: pre-work for N1


// int main(int argc, char* argv[]) // at send_goal
int main(int argc, char **argv) 
{
    ros::init(argc, argv, "mix");
    ros::NodeHandle handle;
    
// START: for N1

    // pose_topic_pub: to control the robot arm in advance, therefore move to the coordinate.
    // subscribe the pubisher. The file located at: /src/dashgo/dashgo_sendcoordinate/nodes/order_goal_test.cpp
    ros::Subscriber sub_destination = handle.subscribe("destination_topic", 10, poseCallback); //poseCallback is the call back function name.
    // setHome(pub_initialpose);
    ROS_INFO_STREAM("Finished set a initial position.\n");
    printf("Finished set a initial position.\n");
    std_msgs::String msg;

// END: for N1

// START: for OMNI6

    ros::ServiceClient gripper_client = handle.serviceClient<neo_msgs::SetEOATAction>("neo_set_EOAT_action");
	ros::ServiceClient client1 = handle.serviceClient<neo_msgs::MoveToPose>("neo_move_to_pose");

    neo_msgs::MoveToPose MoveToPose_msg;
    neo_msgs::SetEOATAction SetEOATAction_msg;

    // Set the OMNI6 status
    MoveToPose_msg.request.velocity = 10.0;
    MoveToPose_msg.request.acceleration = 50.0;
    MoveToPose_msg.request.relative = false;
    MoveToPose_msg.request.block = true; 

// END: for OMNI6


// START: Movements for N1

    while(ros::ok())
    {
       if(Callback_flag == true)
        {
            Callback_flag = false;

            if(msg_str == GOHOME)
            {
                msg_str = "Test: GO HOME NOW...\n";
                arrive_position = setGoal(home);
            }
            else  if(msg_str == GOHOME_2)
            {
                arrive_position = setGoal(home2);
                msg_str = "Test: GO HOME 2 NOW...\n";
            }
            else  if(msg_str == GOGOAL1)
            {
                arrive_position = setGoal(goal1);
                msg_str = "Test: GET TO GOAL 1 NOW....\n";

            }else  if(msg_str == GOGOAL2)
            {
                msg_str = "Test: GET TO GOAL 2 NOW....\n";
                arrive_position = setGoal(goal2);
            }else  if(msg_str == GOGOAL3)
            {
                msg_str = "Test: GET TO GOAL 3 NOW....\n";
                arrive_position = setGoal(goal3);
            }else  if(msg_str == GOGOAL4)
            {
                msg_str = "Test: GET TO GOAL 4 NOW....\n";
                arrive_position = setGoal(goal4);
            }
            else  if(msg_str == COLLECT_FROM_GOAL_1_1)                       // COLLECT FROM GOAL 1-1
            {
                // START: Movements for OMNI6
		        // START: HERE GOES THE MOVEMENTS FOR GET CUP FROM CUSTOMERS' TABLE

                // DESTINATION - COLLECT FROM WHICH TABLE
                msg_str = "COLLECT FROM GOAL 1-1 NOW....\n";
                arrive_position = setGoal(goal1_1);
                
                ros::Duration(3).sleep(); // sleep for 3 seconds
                // Pose 1 - READY POSE
		        // move_to_angles [1,2,3,4,5,6] [0,-30,95,0,27,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 218.761261;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 581.685608;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -1.998999;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 1 arrived. READY POSE. \n");

                // Gripper OPENED
                SetEOATAction_msg.request.name = "Gripper";
                SetEOATAction_msg.request.action = "open";
                gripper_client.call(SetEOATAction_msg);
                printf("Gripper opened! \n");

                // Pose 2 - MOVE FORWARD
		        // move_to_angles [1,2,3,4,5,6] [0,-10,80,0,22,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 323.096863;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 592.753113;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -2.000385;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 2 arrived. MOVE FORWARD.\n");

                // Gripper CLOSED
                SetEOATAction_msg.request.name = "Gripper";
                SetEOATAction_msg.request.action = "close";
                gripper_client.call(SetEOATAction_msg);
                printf("Gripper closed, item grabbed! \n");

                // Pose 3 - TAKE IT UP
	            // move_to_angles [1,2,3,4,5,6] [0,-10,80,0,18,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 323.112366;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 599.790527;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = 1.997350;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 3 arrived. TAKE IT UP.\n");

                // Pose 4- FOLD
		        // move_to_angles [1,2,3,4,5,6] [0,-90,105,0,77,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = -113.988853;
                MoveToPose_msg.request.pose.x = -113.988853;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 488.099701;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -2.003237;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                printf("Position 4 arrived. FOLD.\n");
                client1.call(MoveToPose_msg);

                // END: HERE GOES THE MOVEMENTS FOR GET CUP FROM CUSTOMERS' TABLE
                // GO HOME
                arrive_position = setGoal(home);

                // Pose 1 - READY POSE
                // move_to_angles [1,2,3,4,5,6] [0,-30,95,0,27,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 218.761261;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 581.685608;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -1.998999;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 1 arrived. READY POSE\n");

                // Pose 2 - MOVE FORWARD
                // move_to_angles [1,2,3,4,5,6] [0,10,60,0,20,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 423.895905;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 596.250977;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -0.004195;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 2 arrived. MOVE FORWARD.\n");

                    // Pose 2-1 - Put it down briefly on home's table
                    // move_to_angles [1,2,3,4,5,6] [0,10,60,0,21,0] 10 60 0 0
                    MoveToPose_msg.request.pose.x = 423.854065;
                    MoveToPose_msg.request.pose.y = 0.000000;
                    MoveToPose_msg.request.pose.z = 594.515808;
                    MoveToPose_msg.request.pose.Rx = -180.000000;
                    MoveToPose_msg.request.pose.Ry = -1.000027;
                    MoveToPose_msg.request.pose.Rz = 0.000000;
                    client1.call(MoveToPose_msg);

                    // move_to_angles [1,2,3,4,5,6] [0,10,60,0,22,0] 10 60 0 0
                    MoveToPose_msg.request.pose.x = 423.806641;
                    MoveToPose_msg.request.pose.y = 0.000000;
                    MoveToPose_msg.request.pose.z = 592.758301;
                    MoveToPose_msg.request.pose.Rx = -180.000000;
                    MoveToPose_msg.request.pose.Ry = -1.999460;
                    MoveToPose_msg.request.pose.Rz = 0.000000;
                    client1.call(MoveToPose_msg);

                    // move_to_angles [1,2,3,4,5,6] [0,10,60,0,23,0] 10 60 0 0
                    MoveToPose_msg.request.pose.x = 423.731262;
                    MoveToPose_msg.request.pose.y = 0.000000;
                    MoveToPose_msg.request.pose.z = 590.990906;
                    MoveToPose_msg.request.pose.Rx = -180.000000;
                    MoveToPose_msg.request.pose.Ry = -3.000571;
                    MoveToPose_msg.request.pose.Rz = 0.000000;
                    client1.call(MoveToPose_msg);

                // Gripper OPENED
                SetEOATAction_msg.request.name = "Gripper";
                SetEOATAction_msg.request.action = "open";
                gripper_client.call(SetEOATAction_msg);
                printf("Gripper opened! \n");

                // Pose 4 - Go back to READY POSE
		        // move_to_angles [1,2,3,4,5,6] [0,-30,95,0,27,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 218.761261;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 581.685608;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -1.998999;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 4 arrived. READY POSE\n");

                // Gripper CLOSED
                SetEOATAction_msg.request.name = "Gripper";
                SetEOATAction_msg.request.action = "close";
                gripper_client.call(SetEOATAction_msg);
                printf("Gripper closed, item grabbed! \n");

                // Pose 5 - FOLD
		        // move_to_angles [1,2,3,4,5,6] [0,-90,105,0,77,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = -113.988853;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 488.099701;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -2.003237;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 5 arrived. FOLD.\n");

                // END: Movements for OMNI6

                printf("NEXT COMMAND PLEASE (HOME, COLLECT_FROM_GOAL_x_y, TAKE_TO_GOAL_x_y): \n");
            }
            else  if(msg_str == COLLECT_FROM_GOAL_1_2)                       // COLLECT FROM GOAL 1-2
            {
                // START: Movements for OMNI6
		        // START: HERE GOES THE MOVEMENTS FOR GET CUP FROM CUSTOMERS' TABLE

                // DESTINATION - COLLECT FROM WHICH TABLE
                msg_str = "COLLECT FROM GOAL 1-2 NOW....\n";
                arrive_position = setGoal(goal1_2);
                
                ros::Duration(3).sleep(); // sleep for 3 seconds
                // Pose 1 - READY POSE
		        // move_to_angles [1,2,3,4,5,6] [0,-30,95,0,27,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 218.761261;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 581.685608;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -1.998999;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 1 arrived. READY POSE. \n");

                // Gripper OPENED
                SetEOATAction_msg.request.name = "Gripper";
                SetEOATAction_msg.request.action = "open";
                gripper_client.call(SetEOATAction_msg);
                printf("Gripper opened! \n");

                // Pose 2 - MOVE FORWARD
		        // move_to_angles [1,2,3,4,5,6] [0,-10,80,0,22,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 323.096863;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 592.753113;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -2.000385;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 2 arrived. MOVE FORWARD.\n");

                // Gripper CLOSED
                SetEOATAction_msg.request.name = "Gripper";
                SetEOATAction_msg.request.action = "close";
                gripper_client.call(SetEOATAction_msg);
                printf("Gripper closed, item grabbed! \n");

                // Pose 3 - TAKE IT UP
	            // move_to_angles [1,2,3,4,5,6] [0,-10,80,0,18,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 323.112366;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 599.790527;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = 1.997350;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 3 arrived. TAKE IT UP.\n");

                // Pose 4- FOLD
		        // move_to_angles [1,2,3,4,5,6] [0,-90,105,0,77,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = -113.988853;
                MoveToPose_msg.request.pose.x = -113.988853;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 488.099701;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -2.003237;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                printf("Position 4 arrived. FOLD.\n");
                client1.call(MoveToPose_msg);

                // END: HERE GOES THE MOVEMENTS FOR GET CUP FROM CUSTOMERS' TABLE
                // GO HOME
                arrive_position = setGoal(home);

                // Pose 1 - READY POSE
                // move_to_angles [1,2,3,4,5,6] [0,-30,95,0,27,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 218.761261;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 581.685608;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -1.998999;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 1 arrived. READY POSE\n");

                // Pose 2 - MOVE FORWARD
                // move_to_angles [1,2,3,4,5,6] [0,10,60,0,20,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 423.895905;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 596.250977;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -0.004195;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 2 arrived. MOVE FORWARD.\n");

                    // Pose 2-1 - Put it down briefly on home's table
                    // move_to_angles [1,2,3,4,5,6] [0,10,60,0,21,0] 10 60 0 0
                    MoveToPose_msg.request.pose.x = 423.854065;
                    MoveToPose_msg.request.pose.y = 0.000000;
                    MoveToPose_msg.request.pose.z = 594.515808;
                    MoveToPose_msg.request.pose.Rx = -180.000000;
                    MoveToPose_msg.request.pose.Ry = -1.000027;
                    MoveToPose_msg.request.pose.Rz = 0.000000;
                    client1.call(MoveToPose_msg);

                    // move_to_angles [1,2,3,4,5,6] [0,10,60,0,22,0] 10 60 0 0
                    MoveToPose_msg.request.pose.x = 423.806641;
                    MoveToPose_msg.request.pose.y = 0.000000;
                    MoveToPose_msg.request.pose.z = 592.758301;
                    MoveToPose_msg.request.pose.Rx = -180.000000;
                    MoveToPose_msg.request.pose.Ry = -1.999460;
                    MoveToPose_msg.request.pose.Rz = 0.000000;
                    client1.call(MoveToPose_msg);

                    // move_to_angles [1,2,3,4,5,6] [0,10,60,0,23,0] 10 60 0 0
                    MoveToPose_msg.request.pose.x = 423.731262;
                    MoveToPose_msg.request.pose.y = 0.000000;
                    MoveToPose_msg.request.pose.z = 590.990906;
                    MoveToPose_msg.request.pose.Rx = -180.000000;
                    MoveToPose_msg.request.pose.Ry = -3.000571;
                    MoveToPose_msg.request.pose.Rz = 0.000000;
                    client1.call(MoveToPose_msg);

                // Gripper OPENED
                SetEOATAction_msg.request.name = "Gripper";
                SetEOATAction_msg.request.action = "open";
                gripper_client.call(SetEOATAction_msg);
                printf("Gripper opened! \n");

                // Pose 4 - Go back to READY POSE
		        // move_to_angles [1,2,3,4,5,6] [0,-30,95,0,27,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 218.761261;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 581.685608;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -1.998999;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 4 arrived. READY POSE\n");

                // Gripper CLOSED
                SetEOATAction_msg.request.name = "Gripper";
                SetEOATAction_msg.request.action = "close";
                gripper_client.call(SetEOATAction_msg);
                printf("Gripper closed, item grabbed! \n");

                // Pose 5 - FOLD
		        // move_to_angles [1,2,3,4,5,6] [0,-90,105,0,77,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = -113.988853;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 488.099701;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -2.003237;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 5 arrived. FOLD.\n");

                // END: Movements for OMNI6

                printf("NEXT COMMAND PLEASE (HOME, COLLECT_FROM_GOAL_x_y, TAKE_TO_GOAL_x_y): \n");
            }
            else  if(msg_str == COLLECT_FROM_GOAL_2_1)                       // COLLECT FROM GOAL 2-1
            {
               // START: Movements for OMNI6
		        // START: HERE GOES THE MOVEMENTS FOR GET CUP FROM CUSTOMERS' TABLE

                // DESTINATION - COLLECT FROM WHICH TABLE
                msg_str = "COLLECT FROM GOAL 2-1 NOW....\n";
                arrive_position = setGoal(goal2_1);
                
                ros::Duration(3).sleep(); // sleep for 3 seconds
                // Pose 1 - READY POSE
		        // move_to_angles [1,2,3,4,5,6] [0,-30,95,0,27,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 218.761261;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 581.685608;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -1.998999;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 1 arrived. READY POSE. \n");

                // Gripper OPENED
                SetEOATAction_msg.request.name = "Gripper";
                SetEOATAction_msg.request.action = "open";
                gripper_client.call(SetEOATAction_msg);
                printf("Gripper opened! \n");

                // Pose 2 - MOVE FORWARD
		        // move_to_angles [1,2,3,4,5,6] [0,-10,80,0,22,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 323.096863;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 592.753113;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -2.000385;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 2 arrived. MOVE FORWARD.\n");

                // Gripper CLOSED
                SetEOATAction_msg.request.name = "Gripper";
                SetEOATAction_msg.request.action = "close";
                gripper_client.call(SetEOATAction_msg);
                printf("Gripper closed, item grabbed! \n");

                // Pose 3 - TAKE IT UP
	            // move_to_angles [1,2,3,4,5,6] [0,-10,80,0,18,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 323.112366;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 599.790527;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = 1.997350;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 3 arrived. TAKE IT UP.\n");

                // Pose 4- FOLD
		        // move_to_angles [1,2,3,4,5,6] [0,-90,105,0,77,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = -113.988853;
                MoveToPose_msg.request.pose.x = -113.988853;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 488.099701;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -2.003237;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                printf("Position 4 arrived. FOLD.\n");
                client1.call(MoveToPose_msg);

                // END: HERE GOES THE MOVEMENTS FOR GET CUP FROM CUSTOMERS' TABLE
                // GO HOME
                arrive_position = setGoal(home);

                // Pose 1 - READY POSE
                // move_to_angles [1,2,3,4,5,6] [0,-30,95,0,27,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 218.761261;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 581.685608;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -1.998999;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 1 arrived. READY POSE\n");

                // Pose 2 - MOVE FORWARD
                // move_to_angles [1,2,3,4,5,6] [0,10,60,0,20,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 423.895905;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 596.250977;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -0.004195;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 2 arrived. MOVE FORWARD.\n");

                    // Pose 2-1 - Put it down briefly on home's table
                    // move_to_angles [1,2,3,4,5,6] [0,10,60,0,21,0] 10 60 0 0
                    MoveToPose_msg.request.pose.x = 423.854065;
                    MoveToPose_msg.request.pose.y = 0.000000;
                    MoveToPose_msg.request.pose.z = 594.515808;
                    MoveToPose_msg.request.pose.Rx = -180.000000;
                    MoveToPose_msg.request.pose.Ry = -1.000027;
                    MoveToPose_msg.request.pose.Rz = 0.000000;
                    client1.call(MoveToPose_msg);

                    // move_to_angles [1,2,3,4,5,6] [0,10,60,0,22,0] 10 60 0 0
                    MoveToPose_msg.request.pose.x = 423.806641;
                    MoveToPose_msg.request.pose.y = 0.000000;
                    MoveToPose_msg.request.pose.z = 592.758301;
                    MoveToPose_msg.request.pose.Rx = -180.000000;
                    MoveToPose_msg.request.pose.Ry = -1.999460;
                    MoveToPose_msg.request.pose.Rz = 0.000000;
                    client1.call(MoveToPose_msg);

                    // move_to_angles [1,2,3,4,5,6] [0,10,60,0,23,0] 10 60 0 0
                    MoveToPose_msg.request.pose.x = 423.731262;
                    MoveToPose_msg.request.pose.y = 0.000000;
                    MoveToPose_msg.request.pose.z = 590.990906;
                    MoveToPose_msg.request.pose.Rx = -180.000000;
                    MoveToPose_msg.request.pose.Ry = -3.000571;
                    MoveToPose_msg.request.pose.Rz = 0.000000;
                    client1.call(MoveToPose_msg);

                // Gripper OPENED
                SetEOATAction_msg.request.name = "Gripper";
                SetEOATAction_msg.request.action = "open";
                gripper_client.call(SetEOATAction_msg);
                printf("Gripper opened! \n");

                // Pose 4 - Go back to READY POSE
		        // move_to_angles [1,2,3,4,5,6] [0,-30,95,0,27,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 218.761261;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 581.685608;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -1.998999;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 4 arrived. READY POSE\n");

                // Gripper CLOSED
                SetEOATAction_msg.request.name = "Gripper";
                SetEOATAction_msg.request.action = "close";
                gripper_client.call(SetEOATAction_msg);
                printf("Gripper closed, item grabbed! \n");

                // Pose 5 - FOLD
		        // move_to_angles [1,2,3,4,5,6] [0,-90,105,0,77,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = -113.988853;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 488.099701;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -2.003237;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 5 arrived. FOLD.\n");

                // END: Movements for OMNI6

                printf("NEXT COMMAND PLEASE (HOME, COLLECT_FROM_GOAL_x_y, TAKE_TO_GOAL_x_y): \n");
            }
            else  if(msg_str == COLLECT_FROM_GOAL_2_2)                       // COLLECT FROM GOAL 2-2
            {
                // START: Movements for OMNI6
		        // START: HERE GOES THE MOVEMENTS FOR GET CUP FROM CUSTOMERS' TABLE

                // DESTINATION - COLLECT FROM WHICH TABLE
                msg_str = "COLLECT FROM GOAL 2-2 NOW....\n";
                arrive_position = setGoal(goal2_2);
                
                ros::Duration(3).sleep(); // sleep for 3 seconds
                // Pose 1 - READY POSE
		        // move_to_angles [1,2,3,4,5,6] [0,-30,95,0,27,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 218.761261;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 581.685608;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -1.998999;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 1 arrived. READY POSE. \n");

                // Gripper OPENED
                SetEOATAction_msg.request.name = "Gripper";
                SetEOATAction_msg.request.action = "open";
                gripper_client.call(SetEOATAction_msg);
                printf("Gripper opened! \n");

                // Pose 2 - MOVE FORWARD
		        // move_to_angles [1,2,3,4,5,6] [0,-10,80,0,22,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 323.096863;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 592.753113;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -2.000385;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 2 arrived. MOVE FORWARD.\n");

                // Gripper CLOSED
                SetEOATAction_msg.request.name = "Gripper";
                SetEOATAction_msg.request.action = "close";
                gripper_client.call(SetEOATAction_msg);
                printf("Gripper closed, item grabbed! \n");

                // Pose 3 - TAKE IT UP
	            // move_to_angles [1,2,3,4,5,6] [0,-10,80,0,18,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 323.112366;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 599.790527;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = 1.997350;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 3 arrived. TAKE IT UP.\n");

                // Pose 4- FOLD
		        // move_to_angles [1,2,3,4,5,6] [0,-90,105,0,77,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = -113.988853;
                MoveToPose_msg.request.pose.x = -113.988853;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 488.099701;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -2.003237;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                printf("Position 4 arrived. FOLD.\n");
                client1.call(MoveToPose_msg);

                // END: HERE GOES THE MOVEMENTS FOR GET CUP FROM CUSTOMERS' TABLE
                // GO HOME
                arrive_position = setGoal(home);

                // Pose 1 - READY POSE
                // move_to_angles [1,2,3,4,5,6] [0,-30,95,0,27,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 218.761261;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 581.685608;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -1.998999;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 1 arrived. READY POSE\n");

                // Pose 2 - MOVE FORWARD
                // move_to_angles [1,2,3,4,5,6] [0,10,60,0,20,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 423.895905;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 596.250977;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -0.004195;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 2 arrived. MOVE FORWARD.\n");

                    // Pose 2-1 - Put it down briefly on home's table
                    // move_to_angles [1,2,3,4,5,6] [0,10,60,0,21,0] 10 60 0 0
                    MoveToPose_msg.request.pose.x = 423.854065;
                    MoveToPose_msg.request.pose.y = 0.000000;
                    MoveToPose_msg.request.pose.z = 594.515808;
                    MoveToPose_msg.request.pose.Rx = -180.000000;
                    MoveToPose_msg.request.pose.Ry = -1.000027;
                    MoveToPose_msg.request.pose.Rz = 0.000000;
                    client1.call(MoveToPose_msg);

                    // move_to_angles [1,2,3,4,5,6] [0,10,60,0,22,0] 10 60 0 0
                    MoveToPose_msg.request.pose.x = 423.806641;
                    MoveToPose_msg.request.pose.y = 0.000000;
                    MoveToPose_msg.request.pose.z = 592.758301;
                    MoveToPose_msg.request.pose.Rx = -180.000000;
                    MoveToPose_msg.request.pose.Ry = -1.999460;
                    MoveToPose_msg.request.pose.Rz = 0.000000;
                    client1.call(MoveToPose_msg);

                    // move_to_angles [1,2,3,4,5,6] [0,10,60,0,23,0] 10 60 0 0
                    MoveToPose_msg.request.pose.x = 423.731262;
                    MoveToPose_msg.request.pose.y = 0.000000;
                    MoveToPose_msg.request.pose.z = 590.990906;
                    MoveToPose_msg.request.pose.Rx = -180.000000;
                    MoveToPose_msg.request.pose.Ry = -3.000571;
                    MoveToPose_msg.request.pose.Rz = 0.000000;
                    client1.call(MoveToPose_msg);

                // Gripper OPENED
                SetEOATAction_msg.request.name = "Gripper";
                SetEOATAction_msg.request.action = "open";
                gripper_client.call(SetEOATAction_msg);
                printf("Gripper opened! \n");

                // Pose 4 - Go back to READY POSE
		        // move_to_angles [1,2,3,4,5,6] [0,-30,95,0,27,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = 218.761261;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 581.685608;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -1.998999;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 4 arrived. READY POSE\n");

                // Gripper CLOSED
                SetEOATAction_msg.request.name = "Gripper";
                SetEOATAction_msg.request.action = "close";
                gripper_client.call(SetEOATAction_msg);
                printf("Gripper closed, item grabbed! \n");

                // Pose 5 - FOLD
		        // move_to_angles [1,2,3,4,5,6] [0,-90,105,0,77,0] 10 60 0 0
                MoveToPose_msg.request.pose.x = -113.988853;
                MoveToPose_msg.request.pose.y = 0.000000;
                MoveToPose_msg.request.pose.z = 488.099701;
                MoveToPose_msg.request.pose.Rx = -180.000000;
                MoveToPose_msg.request.pose.Ry = -2.003237;
                MoveToPose_msg.request.pose.Rz = 0.000000;
                client1.call(MoveToPose_msg);
                printf("Position 5 arrived. FOLD.\n");

                // END: Movements for OMNI6

                printf("NEXT COMMAND PLEASE (HOME, COLLECT_FROM_GOAL_x_y, TAKE_TO_GOAL_x_y): \n");
            }
            else  if(msg_str == TAKE_TO_GOAL_1_1)                       // TAKE TO GOAL 1-1
            {
                
            }
            else  if(msg_str == TAKE_TO_GOAL_1_2)                       // TAKE TO GOAL 1-2
            {
                
            }
            else  if(msg_str == TAKE_TO_GOAL_2_1)                       // TAKE TO GOAL 2-1
            {

            }
            else  if(msg_str == TAKE_TO_GOAL_2_2)                       // TAKE TO GOAL 2-2
            {
                
            }
            // else  if(msg_str == TAKEMEAL)
            // {
            //     msg_str = "";
            //     arrive_position = setGoal(takemeal);
            // }
            else
            {
                msg_str = "WRONG INPUT, TYPE IT AGAIN !!!\n";
            }
        }
        ros::spinOnce();
    }

// END: Movements for N1

    return 0;

}