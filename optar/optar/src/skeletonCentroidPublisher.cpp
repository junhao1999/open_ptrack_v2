#include <ros/ros.h>
#include <opt_msgs/TrackArray.h>
#include <opt_msgs/SkeletonTrackArray.h>
#include <cv_bridge/cv_bridge.h>
#include <visualization_msgs/MarkerArray.h>
#include "utils.hpp"

#define ARCORE_CAMERA_IMAGE_REPUBLISHER_NODE_NAME "skeleton_centroid_publisher"

ros::Publisher pub;
ros::Publisher pubMarker;

void callback(const opt_msgs::SkeletonTrackArray& inMsg)
{
	ROS_INFO("received skeletontrack");

	
	opt_msgs::TrackArray msg;
	msg.header = inMsg.header;
	visualization_msgs::MarkerArray markerMsg;
	for(opt_msgs::SkeletonTrack st : inMsg.tracks)
	{
		opt_msgs::Track t;
		t.id = st.id; 
		t.x = st.x;
		t.y = st.y;
		t.height = st.height;
		t.distance = st.distance;
		t.age = st.age;
		t.confidence = st.confidence;
		t.visibility = st.visibility;
		t.stable_id = t.id;

		msg.tracks.push_back(t);

		markerMsg.markers.push_back(buildMarker(t.x,t.y,t.height, "centroid "+std::to_string(t.id), 1, 0, 0, 0, 0.2, inMsg.header.frame_id));
	}

	pub.publish(msg);
	pubMarker.publish(markerMsg);

	ROS_INFO("published track");
}


int main(int argc, char** argv)
{
	ros::init(argc, argv, ARCORE_CAMERA_IMAGE_REPUBLISHER_NODE_NAME);
	ros::NodeHandle nh;
	
	ROS_INFO_STREAM("starting "<<ARCORE_CAMERA_IMAGE_REPUBLISHER_NODE_NAME);
	ros::Subscriber sub = nh.subscribe("/tracker/skeleton_tracks", 1, callback);
	pub = nh.advertise<opt_msgs::TrackArray>("/optar/skeleton_centroids", 10);
	pubMarker = nh.advertise<visualization_msgs::MarkerArray>("/optar/skeleton_centroids_marker", 10);
	ros::spin();
}


