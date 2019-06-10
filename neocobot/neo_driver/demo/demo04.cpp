// Just for test specific coodinate!

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
    ros::init(argc, argv, "demo04");
    ros::NodeHandle handle;
    ros::ServiceClient gripper_client = handle.serviceClient<neo_msgs::SetEOATAction>("neo_set_EOAT_action");
	ros::ServiceClient client1 = handle.serviceClient<neo_msgs::MoveToPose>("neo_move_to_pose");

    neo_msgs::MoveToPose MoveToPose_msg;
    neo_msgs::SetEOATAction SetEOATAction_msg;

    MoveToPose_msg.request.velocity = 10.0;
    MoveToPose_msg.request.acceleration = 50.0;
    MoveToPose_msg.request.relative = false;
    MoveToPose_msg.request.block = true; 

//    Pose 1 - Initial
//     MoveToPose_msg.request.pose.x = 301.789276;
//     MoveToPose_msg.request.pose.y = 0.000000;
//     MoveToPose_msg.request.pose.z = 642.094849;
//     MoveToPose_msg.request.pose.Rx = -180.000000;
//     MoveToPose_msg.request.pose.Ry = 0.000308;
//     MoveToPose_msg.request.pose.Rz = 0.000308;
//     client1.call(MoveToPose_msg);
//     printf("Position 1 arrived.\n");

//     Pose 2 - Initial stand-by
//     [0,-30,95,0,27,0]
//     MoveToPose_msg.request.pose.x = 218.767761;
//     MoveToPose_msg.request.pose.y = 0.000000;
//     MoveToPose_msg.request.pose.z = 581.681030;
//     MoveToPose_msg.request.pose.Rx = -180.000000;
//     MoveToPose_msg.request.pose.Ry = -2.000107;
//     MoveToPose_msg.request.pose.Rz = 0.000000;
//     client1.call(MoveToPose_msg);
//     printf("Position 2 arrived.\n");

//     SetEOATAction_msg.request.name = "Gripper";
//     SetEOATAction_msg.request.action = "open";
//     gripper_client.call(SetEOATAction_msg);
//     printf("Gripper opened! \n");

    // Pose 3 - Go front
    // [0,0,80,0,10,0]
    MoveToPose_msg.request.pose.x = 386.593994;
    MoveToPose_msg.request.pose.y = 0.000000;
    MoveToPose_msg.request.pose.z = 551.861206;
    MoveToPose_msg.request.pose.Rx = -180.000000;
    MoveToPose_msg.request.pose.Ry = 0.000989;
    MoveToPose_msg.request.pose.Rz = 0.000000;
    client1.call(MoveToPose_msg);
    printf("Position 3 arrived.\n");

    // SetEOATAction_msg.request.name = "Gripper";
    // SetEOATAction_msg.request.action = "close";
    // gripper_client.call(SetEOATAction_msg);
    // printf("Gripper closed, item grabbed! \n");

    // ros::Duration(3).sleep(); // sleep for 3 seconds

    // // Pose 4 - Go back (Initial stand-by)
    // // [0,-30,95,0,27,0]
    // MoveToPose_msg.request.pose.x = 218.767761;
    // MoveToPose_msg.request.pose.y = 0.000000;
    // MoveToPose_msg.request.pose.z = 581.681030;
    // MoveToPose_msg.request.pose.Rx = -180.000000;
    // MoveToPose_msg.request.pose.Ry = -2.000107;
    // MoveToPose_msg.request.pose.Rz = 0.000000;
    // client1.call(MoveToPose_msg);
    // printf("Position 4 arrived.\n");

    // // Pose 5 - Be folded to secure the cup
    // // [0,-70,105,0,55,0]
    // MoveToPose_msg.request.pose.x = -5.173515;
    // MoveToPose_msg.request.pose.y = 0.000000;
    // MoveToPose_msg.request.pose.z = 548.239563;
    // MoveToPose_msg.request.pose.Rx = -180.000000;
    // MoveToPose_msg.request.pose.Ry = -0.000226;
    // MoveToPose_msg.request.pose.Rz = 0.000000;
    // client1.call(MoveToPose_msg);

    // printf("Position 5 arrived, stand by.\n");
    
    return 0;

}
