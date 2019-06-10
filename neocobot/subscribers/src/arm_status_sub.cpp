#include "ros/ros.h"
#include "../../neo_driver/include/neo_driver/neo_driver.h"

// There are 4 callback functions that will get called when a new info has arrived on the topics.

void anglesCallback(const neo_msgs::JointAngles::ConstPtr& msg)
{
	printf("Angles: [");
	for(int i=0; i<6; i++)
	{
		if(i==5)printf ("%f", msg->angles[i]);
		else printf ("%f, ", msg->angles[i]);
	}
	printf("]\n"); 
//	printf("Angles: [%f,%f,%f,%f,%f,%f]\n", msg->angles[0], msg->angles[1], msg->angles[2], msg->angles[3], msg->angles[4], msg->angles[5]);
//	ROS_INFO("Angles: [%f]", msg->angles[0]); // This is not working
}

void velocityCallback(const neo_msgs::JointVelocity::ConstPtr& msg)
{
	printf("Velocity: [");
	for(int i=0; i<6; i++)
	{
		if(i==5) printf ("%f", msg->velocity[i]);
		else printf ("%f, ", msg->velocity[i]);
	} 
	printf("]\n");
//	printf("Velocity: [%f]\n", msg->velocity[0]);
}

void currentCallback(const neo_msgs::JointCurrent::ConstPtr& msg)
{
	printf("Current: [");
	for(int i=0; i<6; i++)
	{
		if(i==5) printf ("%f", msg->current[i]);
		else printf ("%f, ", msg->current[i]);
	}
	printf("]\n");
	
//	printf("Current: [%f]\n", msg->current[0]);
}

void poseCallback(const neo_msgs::EndPose::ConstPtr& msg)
{
  printf("Pose:\n x: %f\n y: %f\n z: %f\n Rx: %f\n Ry: %f\n Rz: %f \n", msg->x, msg->y, msg->z, msg->Rx, msg->Ry, msg->Rz);
	printf("========== For copy and paste: ========== \n");
	printf("MoveToPose_msg.request.pose.x = %f;\n", msg->x);
	printf("MoveToPose_msg.request.pose.y = %f;\n", msg->y);
	printf("MoveToPose_msg.request.pose.z = %f;\n", msg->z);
	printf("MoveToPose_msg.request.pose.Rx = %f;\n", msg->Rx);
	printf("MoveToPose_msg.request.pose.Ry = %f;\n", msg->Ry);
	printf("MoveToPose_msg.request.pose.Rz = %f;\n", msg->Rz);
  printf("==============================\n");
}


int main(int argc, char **argv)
{
  /*
  Initialize ROS. This allows ROS to do name remapping through the command line -- not important now.
  This is also where we specify thename of our node "arm_status_sub". 
  The name must be unique in a running system!
  */ 
	ros::init(argc, argv, "arm_status_sub");
  /*
  Create a handle to this process' node. This first NodeHandle created will actually do 
  the initialization of the node, and the last one detructed will cleanup any 
  resources the node was using.
  */
	ros::NodeHandle n;
  /*
  4 Topics here. Subscribe to the each topic with the master. 
  ROS will call the callback function whenever a new message arrives. 
  The 2nd argument is the queue size,in case we're not able to process msgs fast enough. 
  If the queue reaches 1 message, we will start throwing away old msgs as new one arrives.
  */
	ros::Subscriber sub_ang = n.subscribe("neo_angles", 1, anglesCallback);
	ros::Subscriber sub_vel = n.subscribe("neo_velocity", 1, velocityCallback);
	ros::Subscriber sub_cur = n.subscribe("neo_current", 1, currentCallback);
	ros::Subscriber sub_pos = n.subscribe("neo_pose", 1, poseCallback);
 
 /*
  Enters a loop, calling msg callbacks as fast as possible.
  ros::spin() will exit once ros::ok() returns false, which means ros::shutdown() has been called,
  either by the default CTRL+C handeler, the master telling us to shutdown, or it being called manually.
  */
	ros::spin();

	return 0;
}
