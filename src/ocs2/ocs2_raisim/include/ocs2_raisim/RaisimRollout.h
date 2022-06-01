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

#include <ocs2_oc/rollout/RolloutBase.h>
#include <ocs2_raisim/RaisimRolloutSettings.h>
#include <raisim/World.hpp>

#ifdef USE_RAISIM_VISUALIZER
#include <raisim/OgreVis.hpp>
#endif

namespace ocs2 {

/**
 * This rollout class uses the Raisim physics simulator for integrating the system dynamics
 *
 * @tparam STATE_DIM: Dimension of the state space.
 * @tparam INPUT_DIM: Dimension of the control input space.
 */
template <size_t STATE_DIM, size_t INPUT_DIM>
class RaisimRollout final : public RolloutBase<STATE_DIM, INPUT_DIM> {
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  using Base = RolloutBase<STATE_DIM, INPUT_DIM>;

  using typename Base::controller_t;
  using typename Base::input_vector_array_t;
  using typename Base::input_vector_t;
  using typename Base::scalar_array_t;
  using typename Base::scalar_t;
  using typename Base::size_array_t;
  using typename Base::state_vector_array_t;
  using typename Base::state_vector_t;
  using typename Base::time_interval_array_t;
  using typename Base::time_interval_t;

  using state_to_raisim_gen_coord_gen_vel_t =
      std::function<std::pair<Eigen::VectorXd, Eigen::VectorXd>(const state_vector_t&, const input_vector_t&)>;
  using raisim_gen_coord_gen_vel_to_state_t = std::function<state_vector_t(const Eigen::VectorXd&, const Eigen::VectorXd&)>;
  using input_to_raisim_generalized_force_t =
      std::function<Eigen::VectorXd(double, const input_vector_t&, const state_vector_t&, const Eigen::VectorXd&, const Eigen::VectorXd&)>;
  using input_to_raisim_pd_targets_t = std::function<std::pair<Eigen::VectorXd, Eigen::VectorXd>(
      double, const input_vector_t&, const state_vector_t&, const Eigen::VectorXd&, const Eigen::VectorXd&)>;
  using data_extraction_callback_t = std::function<void(double, const raisim::ArticulatedSystem&)>;

  /**
   * @brief Constructor
   * @param[in] path: Absolute file path to the *.urdf description or the urdf string (xml document)
   * @param[in] stateToRaisimGenCoordGenVel: Transformation function that converts ocs2 state to generalized coordinate and generalized
   * velocity used by Raisim
   * @param[in] raisimGenCoordGenVelToState: Transformation function that converts Raisim generalized coordinates and velocities to ocs2
   * state
   * @param[in] inputToRaisimGeneralizedForce: Tranformation function that converts ocs2 control input to Raisim generalized force
   * @param[in] dataExtractionCallback: Optional callback function to extract user-defined information from the simulation at each timestep
   * @param[in] rolloutSettings: The rollout settings.
   * @param[in] inputToRaisimPdTargets: Transformation function that converts Raisim generalized positions and velocities to Raisim PD
   * targets (i.e., joint positions and velocities). This argument is only required if controlMode is not FORCE_AND_TORQUE.
   *
   * @note The function handles stateToRaisimGenCoordGenVel, raisimGenCoordGenVelToState, inputToRaisimGeneralizedForce,
   * dataExtractionCallback, inputToRaisimPdTargets must be thread safe, i.e., multiple rollout instances might execute them in parallel
   */
  RaisimRollout(std::string urdf, state_to_raisim_gen_coord_gen_vel_t stateToRaisimGenCoordGenVel,
                raisim_gen_coord_gen_vel_to_state_t raisimGenCoordGenVelToState,
                input_to_raisim_generalized_force_t inputToRaisimGeneralizedForce,
                data_extraction_callback_t dataExtractionCallback = nullptr,
                RaisimRolloutSettings raisimRolloutSettings = RaisimRolloutSettings(),
                input_to_raisim_pd_targets_t inputToRaisimPdTargets = nullptr);

  //! Copy constructor
  RaisimRollout(const RaisimRollout& other);

  RaisimRollout<STATE_DIM, INPUT_DIM>* clone() const override { return new RaisimRollout(*this); }

  /**
   * @brief Replaces the default flat ground plane with a generated terrain in Raisim
   * @param properties: Terrain properties
   * @return Pointer to the created terrain instance
   */
  raisim::HeightMap* generateTerrain(raisim::TerrainProperties properties = raisim::TerrainProperties());

  /**
   * @brief Replaces the default flat ground plane with the given heightMap
   * @param[in] heightMap The new terrain
   */
  void setTerrain(const raisim::HeightMap& heightMap);

  /**
   * @brief Replaces the default flat ground plane with the given heightMap described by a png file
   * @param[in] pngFileName Path to the png file which is loaded as the new terrain
   * @param[in] centerX World X-coordinate corresponding to the map center
   * @param[in] centerY World Y-coordinate corresponding to the map center
   * @param[in] xSize Actual size of the map in X-direction [m]
   * @param[in] ySize Actual size of the map in Y-direction [m]
   * @param[in] heightScale Distance [m] that a single level of greyscale saturation [0 - 255] corresponds to
   * @param[in] heightOffset Height offset of the zero level [m]
   */
  void setTerrain(const std::string& pngFileName, double centerX, double centerY, double xSize, double ySize, double heightScale,
                  double heightOffset);

  /**
   * @brief Returns the heightMap, which can be read for terrain information
   * @return Pointer to the class heightMap_ member variable
   */
  const raisim::HeightMap* getTerrain() const;

  /**
   * @brief Save and apply P and D gain values. They only take effect if the controlMode is not FORCE_AND_TORQUE
   * @param[in] pGain: Proportional (position) gains (dim == degrees of freedom)
   * @param[in] dGain: Derivative (velocity) gains (dim == degrees of freedom)
   */
  void setPdGains(const Eigen::VectorXd& pGain, const Eigen::VectorXd& dGain);

 protected:
  state_vector_t runImpl(time_interval_array_t timeIntervalArray, const state_vector_t& initState, controller_t* controller,
                         scalar_array_t& timeTrajectory, size_array_t& postEventIndicesStock, state_vector_array_t& stateTrajectory,
                         input_vector_array_t& inputTrajectory) override;

 private:
  /**
   * @brief Internal helper method to step the simulation forward for a given time interval and save results
   * @note Assumes the state of the simulation has already been set
   * @param[in] timeInterval: Times between which the integration is performed
   * @param[in] controller: Pointer to controller that will be used during the rollout
   * @param[out] timeTrajectory: Vector to which times will be appended
   * @param[out] stateTrajectory: Vector to which states will be appended
   * @param[out] inputTrajectory: Vector to which inputs will be appended
   */
  void runSimulation(const time_interval_t& timeInterval, controller_t* controller, scalar_array_t& timeTrajectory,
                     state_vector_array_t& stateTrajectory, input_vector_array_t& inputTrajectory);

  //! Helper method to remove the ground plane from simulation
  void deleteGroundPlane();

 public:
  RaisimRolloutSettings raisimRolloutSettings_;

 private:
  // Save some constructor/function arguments required for copy constructor / cloning
  std::string urdf_;

  // Handles to Raisim objects
  raisim::World world_;
  raisim::Ground* ground_ = nullptr;
  raisim::HeightMap* heightMap_ = nullptr;
  raisim::ArticulatedSystem* system_ = nullptr;

#ifdef USE_RAISIM_VISUALIZER
  // Handles to Raisim visualization objects
  std::vector<raisim::GraphicObject>* systemVisual_;
  std::vector<raisim::GraphicObject>* groundVisual_;
#endif

  // Robot-specific conversion function handles
  state_to_raisim_gen_coord_gen_vel_t stateToRaisimGenCoordGenVel_;
  raisim_gen_coord_gen_vel_to_state_t raisimGenCoordGenVelToState_;
  input_to_raisim_generalized_force_t inputToRaisimGeneralizedForce_;
  data_extraction_callback_t dataExtractionCallback_;
  input_to_raisim_pd_targets_t inputToRaisimPdTargets_;
};

}  // namespace ocs2

#include "implementation/RaisimRollout.h"
