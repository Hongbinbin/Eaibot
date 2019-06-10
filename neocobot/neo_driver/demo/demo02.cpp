#include <ros/ros.h>
#include <string>

#include "neo_msgs/EndPose.h"
#include "neo_msgs/JointAngles.h"
#include "neo_msgs/JointCurrent.h"
#include "neo_msgs/JointVelocity.h"

#include "neo_msgs/SetEOATAction.h"
#include "neo_msgs/MoveToPose.h"

int main(int argc, char **argv) 
{
    ros::init(argc, argv, "demo02");
    ros::NodeHandle handle;
    ros::ServiceClient gripper_client = handle.serviceClient<neo_msgs::SetEOATAction>("neo_set_EOAT_action");
	ros::ServiceClient client1 = handle.serviceClient<neo_msgs::MoveToPose>("neo_move_to_pose");
/*
	ros::ServiceClient client2 = handle.serviceClient<neo_msgs::JointAngles>("neo_move_to_pose");
	ros::ServiceClient client3 = handle.serviceClient<neo_msgs::JointCurrent>("neo_move_to_pose");
	ros::ServiceClient client4 = handle.serviceClient<neo_msgs::JointVelocity>("neo_move_to_pose");
*/	

 // Not working
 //   neo_msgs::EndPose pose1;
//  neo_msgs::JointAngles pose1;
/*
    pose1.x = 301.789276;
    pose1.y = 0.000000;
    pose1.z = 642.094849;
    pose1.Rx = -180.000000;
    pose1.Ry = 0.000308;
    pose1.Rz = 0.000000;
*/
/*
    pose1.angles[1] = -0.000000;
    pose1.angles[2] = -10.000015;
    pose1.angles[3] = 70.000000;
    pose1.angles[4] = 0.000000;
    pose1.angles[5] = 29.999706;
    pose1.angles[6] = 0.000000;
*/
	
    neo_msgs::MoveToPose MoveToPose_msg;
    neo_msgs::SetEOATAction SetEOATAction_msg;

   // Pose 1
    MoveToPose_msg.request.pose.x = 301.789276;
    MoveToPose_msg.request.pose.y = 0.000000;
    MoveToPose_msg.request.pose.z = 642.094849;
    MoveToPose_msg.request.pose.Rx = -180.000000;
    MoveToPose_msg.request.pose.Ry = 0.000308;
    MoveToPose_msg.request.pose.Rz = 0.000308;

/*
    // Pose 2
    MoveToPose_msg.request.pose.x = 218.767761;
    MoveToPose_msg.request.pose.y = 0.000000;
    MoveToPose_msg.request.pose.z = 581.681030;
    MoveToPose_msg.request.pose.Rx = -180.000000;
    MoveToPose_msg.request.pose.Ry = -2.000107;
    MoveToPose_msg.request.pose.Rz = 0.000000;
*/
/*
MoveToPose_msg.request.pose.x = 218.767761;
MoveToPose_msg.request.pose.y = 0.000000;
MoveToPose_msg.request.pose.z = 581.681030;
MoveToPose_msg.request.pose.Rx = -180.000000;
MoveToPose_msg.request.pose.Ry = -2.000107;
MoveToPose_msg.request.pose.Rz = 0.000000;
client1.call(MoveToPose_msg);
SetEOATAction_msg.request.name = "Gripper";
SetEOATAction_msg.request.action = "open";
gripper_client.call(SetEOATAction_msg);
printf("Gripper opened\n");

MoveToPose_msg.request.pose.x = 386.593994;
MoveToPose_msg.request.pose.y = 0.000000;
MoveToPose_msg.request.pose.z = 551.861206;
MoveToPose_msg.request.pose.Rx = -180.000000;
MoveToPose_msg.request.pose.Ry = 0.000989;
MoveToPose_msg.request.pose.Rz = 0.000000;
client1.call(MoveToPose_msg);

SetEOATAction_msg.request.name = "Gripper";
SetEOATAction_msg.request.action = "close";
gripper_client.call(SetEOATAction_msg);
 printf("Gripper closed\n");

MoveToPose_msg.request.pose.x = 218.767761;
MoveToPose_msg.request.pose.y = 0.000000;
MoveToPose_msg.request.pose.z = 581.681030;
MoveToPose_msg.request.pose.Rx = -180.000000;
MoveToPose_msg.request.pose.Ry = -2.000107;
MoveToPose_msg.request.pose.Rz = 0.000000;
client1.call(MoveToPose_msg);
*/


//  MoveToPose_msg.request.pose.push_back(pose1);
/*   MoveToPose_msg.request.points.push_back(pose2);
    MoveToPose_msg.request.points.push_back(pose3);
    MoveToPose_msg.request.points.push_back(pose4);
*/
    MoveToPose_msg.request.velocity = 10.0;
    MoveToPose_msg.request.acceleration = 50.0;
    MoveToPose_msg.request.relative = false; // It MUST be false, otherwise won't move!
    MoveToPose_msg.request.block = true; 
    // If the block value is true, it will do robot.wait_for_motors (cehck out at neo_driver.cpp)
    // All the movements will move step-by-step, unless could decide when the gripper is opened/closed.

    client1.call(MoveToPose_msg);
/*    if(client1.call(MoveToPose_msg))
    {
        printf("MoveToPose_msg call succeed\n");
        printf("Event: %d\n", MoveToPose_msg.response.event);
    }
    else
    {
        printf("MoveToPose_msg call failed!!!!!\n");
    }
*/

    // Gripper move
    SetEOATAction_msg.request.name = "Gripper";
    SetEOATAction_msg.request.action = "open";
    gripper_client.call(SetEOATAction_msg);
    printf("Gripper opened\n");
    SetEOATAction_msg.request.name = "Gripper";
    SetEOATAction_msg.request.action = "close";
    gripper_client.call(SetEOATAction_msg);
    printf("Gripper closed\n");

    return 0;

}
