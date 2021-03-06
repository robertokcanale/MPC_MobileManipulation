/******************************************************************************
Copyright (c) 2017, Farbod Farshidian. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

 * Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#pragma once

#include <robot_state_publisher/robot_state_publisher.h>
#include <tf/transform_broadcaster.h>

#include <ocs2_comm_interfaces/ocs2_ros_interfaces/mrt/DummyObserver.h>

#include "ocs2_ballbot_example/definitions.h"

namespace ocs2 {
namespace ballbot {

class BallbotDummyVisualization final : public DummyObserver<ballbot::STATE_DIM_, ballbot::INPUT_DIM_> {
 public:
  explicit BallbotDummyVisualization(ros::NodeHandle& nodeHandle) { launchVisualizerNode(nodeHandle); }

  ~BallbotDummyVisualization() override = default;

  void update(const system_observation_t& observation, const primal_solution_t& policy, const command_data_t& command) override;

 private:
  void launchVisualizerNode(ros::NodeHandle& nodeHandle);

  std::unique_ptr<robot_state_publisher::RobotStatePublisher> robotStatePublisherPtr_;
  tf::TransformBroadcaster tfBroadcaster_;
};

}  // namespace ballbot
}  // namespace ocs2
