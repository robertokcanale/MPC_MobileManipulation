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

namespace ocs2 {

/******************************************************************************************************/
/******************************************************************************************************/
/******************************************************************************************************/
template <typename SCALAR_T>
TargetTrajectories_Joystick_Interface<SCALAR_T>::TargetTrajectories_Joystick_Interface(
    int argc, char* argv[], const std::string& robotName /*= "robot"*/, const size_t targetCommandSize /*= 0*/,
    const scalar_array_t& targetCommandLimits /*= scalar_array_t()*/)
    : BASE(argc, argv, robotName), targetCommandSize_(targetCommandSize), targetCommandLimits_(targetCommandLimits) {
  if (targetCommandLimits.size() != targetCommandSize) throw std::runtime_error("Target command limits are not set properly");
}

/******************************************************************************************************/
/******************************************************************************************************/
/******************************************************************************************************/
template <typename SCALAR_T>
size_t& TargetTrajectories_Joystick_Interface<SCALAR_T>::targetCommandSize() {
  return targetCommandSize_;
}

/******************************************************************************************************/
/******************************************************************************************************/
/******************************************************************************************************/
template <typename SCALAR_T>
void TargetTrajectories_Joystick_Interface<SCALAR_T>::publishTargetTrajectoriesFromDesiredState(
    cost_desired_trajectories_t costDesiredTrajectories) {
  // publish cost desired trajectories
  BASE::publishTargetTrajectories(costDesiredTrajectories);
}
}  // namespace ocs2
