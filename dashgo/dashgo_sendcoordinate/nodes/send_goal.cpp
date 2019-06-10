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

/*******************************默認amcl初始點******************************************/
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

/*Bill*/
/*
POSE home = {0.06387, -0.01085, 0.0,  0.0, 0.0, 0.0134, 0.99991};
POSE goal1 = {7.3468708992, 1.44528746605, 0.0,  0.0, 0.0, 0.718360373375, 0.695671167984};
POSE goal2 = {4.88023710251, -1.22698187828, 0.0,  0.0, 0.0, 0.735213247403, 0.677835880463};
POSE goal3 = {1.40267, -2.5057, 0.0,  0.0, 0.0, 0.6996249, 0.71451};
POSE goal4 = {1.29298079014, -2.62258481979, 0.0,  0.0, 0.0, 0.716425198821, 0.69766391227};
POSE takemeal = {1.2915109396, 0.00272600352764, 0.0,  0.0, 0.0, -0.68652640805, 0.727104869362};
*/

/*Hilary Test*/
/*
POSE home ={0.119964838028, -0.0647741556168, 0.0, 0.0, 0.0, 0.00447272969125, 0.999989997295};
POSE goal1 = {7.24299621582, 0.622530341148, 0.0, 0.0, 0.0, 0.697834115105, 0.716259413757};
POSE goal2 = {0.746588647366, -2.66178274155, 0.0, 0.0, 0.0, 0.633581535542, 0.773675925579};
POSE takemeal = {1.3395601511, 0.0827664211392, 0.0, 0.0, 0.0, -0.724490335702, 0.689284958108};

POSE goal3 = {1.40267, -2.5057, 0.0,  0.0, 0.0, 0.6996249, 0.71451};
POSE goal4 = {1.29298079014, -2.62258481979, 0.0,  0.0, 0.0, 0.716425198821, 0.69766391227};
*/

/* SSLAB */
/*
POSE home ={-0.186487436295, 0.170773863792, 0.0, 0.0, 0.0, 0.123017060338, 0.992404556049};
POSE goal1 = {1.46793222427, -1.14611554146, 0.0, 0.0, 0.0, 0.28039329478, 0.959885201596};
POSE goal2 = {0.968357205391, -0.24762737751, 0.0, 0.0, 0.0, 0.704834806617, 0.709371479114};
POSE takemeal = {1.40087115765, -0.53991484642, 0.0, 0.0, 0.0, 0.134545641846, 0.990907397419};
POSE goal3 = {1.40087115765, -0.53991484642, 0.0, 0.0, 0.0, 0.134545641846, 0.990907397419};
POSE goal4 = {0.936185240746, 0.195249557495, 0.0, 0.0, 0.0, 0.713097393693, 0.70106498066};
*/


// Set coordinates of the space for N1
POSE home ={0.150877491216, -0.0653026237985, 0.138, 0.0, 0.0, -0.00121532079559, 0.999999261497};
POSE takemeal = {0.277803149202, -0.443939687916, 0.138, 0.0, 0.0, -0.691625729087, 0.722256083993};
/* TMTS */
POSE goal1 = {0.914020121098, 0.54086202383, 0.0, 0.0, 0.0, -0.710723441559, 0.703471527227};
POSE goal2 = {0.968357205391, -0.24762737751, 0.0, 0.0, 0.0, 0.704834806617, 0.709371479114};
POSE goal3 = {1.40087115765, -0.53991484642, 0.0, 0.0, 0.0, 0.134545641846, 0.990907397419};
POSE goal4 = {0.936185240746, 0.195249557495, 0.0, 0.0, 0.0, 0.713097393693, 0.70106498066};


//void setHome( ros::Publisher pub)
//{
//    geometry_msgs::PoseWithCovarianceStamped msg_poseinit;
//    msg_poseinit.header.frame_id = "map";
//    msg_poseinit.header.stamp = ros::Time::now();
//    msg_poseinit.pose.pose.position.x = 0.06387;
//    msg_poseinit.pose.pose.position.y = -0.01085;
//    msg_poseinit.pose.pose.position.z = 0.0;
//    msg_poseinit.pose.pose.orientation.x = 0.0;
//    msg_poseinit.pose.pose.orientation.y = 0.0;
//    msg_poseinit.pose.pose.orientation.z = 0.0134;
//    msg_poseinit.pose.pose.orientation.w = 0.99991;
//
//    pub.publish(msg_poseinit);
//    ros::Duration(1.0).sleep();
//    pub.publish(msg_poseinit);
//    ros::Duration(1.0).sleep();
//    pub.publish(msg_poseinit);
//    ros::Duration(1.0).sleep();
//}

string setGoal(POSE pose)
{
     //tell the action client that we want to spin a thread by default  
    MoveBaseClient ac("move_base", true);  
      
    //wait for the action server to come up  
    while(!ac.waitForServer(ros::Duration(10.0))){  
        ROS_WARN("Waiting for the move_base action server to come up");  
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
    
    ROS_INFO_STREAM("Sending goal");  
    
    ac.sendGoal(goal);  
      
    ac.waitForResult();  
      
    if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){  
       ROS_INFO("it is successful");   
       return "t";
    }else{  
       ROS_ERROR("The base failed  move to goal!!!");
       return "u";
    }   
}

void poseCallback(const std_msgs::String::ConstPtr &msg)
{
     ROS_INFO_STREAM("Topic is Subscriber ");
     std::cout<<"get topic text: " << msg->data << std::endl;
    
     Callback_flag = true;
     msg_str = msg->data;
}   


int main(int argc, char* argv[]){
//   ros::init(argc, argv, "send_goals_node");
    ros::init(argc, argv, "send_goal");
    ros::NodeHandle nh;
//    ros::Publisher pub_initialpose = nh.advertise<geometry_msgs::PoseWithCovarianceStamped>("/initialpose", 10);
//    ros::Publisher pub_arriveStatus = nh.advertise<std_msgs::String>("/take_picture", 10); 
//    ros::Publisher pub_move = nh.advertise<std_msgs::String>("/tools/control_move", 10); 

    // pose_topic_pub: to control the robot arm in advance, therefore move to the coordinate.
    ros::Subscriber sub = nh.subscribe("/destination_topic",10,poseCallback); //poseCallback is the call back function name.
//    setHome(pub_initialpose);
    ROS_INFO_STREAM("Finished set a initial position");
    std_msgs::String msg;

   // go to the (known) coordinate

    while(ros::ok())
    {
       if(Callback_flag == true)
       {

         Callback_flag = false;

         if(msg_str == GOHOME)
         {
            msg_str = "";
            arrive_position = setGoal(home);
            //msg.data = arrive_position; //arrived or not
            //pub_arriveStatus.publish(msg);
         }
         else  if(msg_str == GOGOAL1)
         {
            msg_str = "";
            arrive_position = setGoal(goal1);
            //msg.data = arrive_position;
            //pub_arriveStatus.publish(msg);
         }
         else  if(msg_str == GOGOAL2)
         {
            msg_str = "";
            arrive_position = setGoal(goal2);
            //msg.data = arrive_position;
	    //pub_arriveStatus.publish(msg);
         }else  if(msg_str == GOGOAL3)
         {
            msg_str = "";
            arrive_position = setGoal(goal3);
            //msg.data = arrive_position;
	    //pub_arriveStatus.publish(msg);
         }else  if(msg_str == GOGOAL4)
         {
            msg_str = "";
            arrive_position = setGoal(goal4);
            //msg.data = arrive_position;
	    //pub_arriveStatus.publish(msg);
         }else  if(msg_str == TAKEMEAL)
         {
            msg_str = "";
            arrive_position = setGoal(takemeal);
            //msg.data = "i";
	    //pub_move.publish(msg);
         }
         else
         {

         }
       }

      ros::spinOnce();
    }
  



    /*MoveBaseClient ac("move_base", true);
    printf("Waiting for the move_base action server");
    ac.waitForServer(ros::Duration(60.0));

    printf("Connected to move base server");
    
    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x = 3.48;
    goal.target_pose.pose.position.y = 0.585;
    goal.target_pose.pose.orientation.w = 1;

    ROS_INFO_STREAM("Sending goal");
    ac.sendGoal(goal);
    ac.waitForResult();
    if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
	ROS_INFO_STREAM("You have reached the goal!");
    else
	ROS_INFO_STREAM("The base failed for some reason");
    */
    return 0;
}
