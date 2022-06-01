//
// Created by rgrandia on 12.02.20.
//

#include "ocs2_quadrotor_example/ros_comm/QuadrotorDummyVisualization.h"

namespace ocs2 {
namespace quadrotor {

void QuadrotorDummyVisualization::update(const system_observation_t& observation, const primal_solution_t& policy,
                                         const command_data_t& command) {
  const auto& costDesiredTrajectories = command.mpcCostDesiredTrajectories_;

  // publish command transform
  const Eigen::Vector3d desiredPositionWorldToTarget = Eigen::Vector3d(costDesiredTrajectories.desiredStateTrajectory().back()(0),
                                                                       costDesiredTrajectories.desiredStateTrajectory().back()(1),
                                                                       costDesiredTrajectories.desiredStateTrajectory().back()(2));
  const Eigen::Quaterniond desiredQuaternionBaseToWorld =
      Eigen::AngleAxisd{costDesiredTrajectories.desiredStateTrajectory().back()(3), Eigen::Vector3d::UnitZ()} *
      Eigen::AngleAxisd{costDesiredTrajectories.desiredStateTrajectory().back()(4), Eigen::Vector3d::UnitY()} *
      Eigen::AngleAxisd{costDesiredTrajectories.desiredStateTrajectory().back()(5), Eigen::Vector3d::UnitX()};
  ros::Time timeMsg = ros::Time::now();
  geometry_msgs::TransformStamped command_frame_transform;
  command_frame_transform.header.stamp = timeMsg;
  command_frame_transform.header.frame_id = "odom";
  command_frame_transform.child_frame_id = "command";
  command_frame_transform.transform.translation.x = desiredPositionWorldToTarget.x();
  command_frame_transform.transform.translation.y = desiredPositionWorldToTarget.y();
  command_frame_transform.transform.translation.z = desiredPositionWorldToTarget.z();
  command_frame_transform.transform.rotation.w = desiredQuaternionBaseToWorld.w();
  command_frame_transform.transform.rotation.x = desiredQuaternionBaseToWorld.x();
  command_frame_transform.transform.rotation.y = desiredQuaternionBaseToWorld.y();
  command_frame_transform.transform.rotation.z = desiredQuaternionBaseToWorld.z();
  tfBroadcaster_.sendTransform(command_frame_transform);

  tf::Transform transform;
  transform.setOrigin(tf::Vector3(observation.state()(0), observation.state()(1), observation.state()(2)));
  tf::Quaternion q = tf::createQuaternionFromRPY(observation.state()(3), observation.state()(4), observation.state()(5));
  transform.setRotation(q);
  tfBroadcaster_.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", "base"));
}

}  // namespace quadrotor
}  // namespace ocs2
