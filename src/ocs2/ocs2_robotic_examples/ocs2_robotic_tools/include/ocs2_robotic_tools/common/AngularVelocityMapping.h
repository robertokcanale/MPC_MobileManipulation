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

#include <Eigen/Core>
#include <array>
#include <cmath>

namespace ocs2 {

/******************************************************************************************************/
/******************************************************************************************************/
/******************************************************************************************************/
/**
 * @brief Computes the matrix which transforms derivatives of angular velocities in the body frame to euler angles derivatives
 * WARNING: matrix is singular when rotation around y axis is +/- 90 degrees
 *
 * @param[in] eulerAngles: euler angles in xyz convention
 * @return M: matrix that does the transformation
 */
template <typename SCALAR_T>
inline Eigen::Matrix<SCALAR_T, 3, 3> AngularVelocitiesToEulerAngleDerivativesMatrix(const Eigen::Matrix<SCALAR_T, 3, 1>& eulerAngles) {
  Eigen::Matrix<SCALAR_T, 3, 3> M;
  SCALAR_T sinPsi = sin(eulerAngles(2));
  SCALAR_T cosPsi = cos(eulerAngles(2));
  SCALAR_T sinTheta = sin(eulerAngles(1));
  SCALAR_T cosTheta = cos(eulerAngles(1));

  M << cosPsi / cosTheta, -sinPsi / cosTheta, 0, sinPsi, cosPsi, 0, -cosPsi * sinTheta / cosTheta, sinTheta * sinPsi / cosTheta, 1;

  return M;
}

/******************************************************************************************************/
/******************************************************************************************************/
/******************************************************************************************************/
/**
 * @brief Convert derivatives of ZYX euler angles to angular velocity
 * @param[in] The current orientation (ZYX euler angles)
 * @param[in] The derivatives of the ZYX euler angles
 * @return The angular velocity in world frame
 */
template <typename SCALAR_T>
inline Eigen::Matrix<SCALAR_T, 3, 1> eulerAngleZyxDerivativesToAngularVelocityInWorld(
    const Eigen::Matrix<SCALAR_T, 3, 1>& eulerAngles, const Eigen::Matrix<SCALAR_T, 3, 1>& eulerAnglesTimeDerivative) {
  const double cyaw = cos(eulerAngles(0));
  const double cpitch = cos(eulerAngles(1));

  const double syaw = sin(eulerAngles(0));
  const double spitch = sin(eulerAngles(1));

  Eigen::Matrix3d transform;
  transform << 0, -syaw, cpitch * cyaw,  // clang-format off
               0,  cyaw, cpitch * syaw,
               1,     0,       -spitch;  // clang-format on

  return transform * eulerAnglesTimeDerivative;
}

/******************************************************************************************************/
/******************************************************************************************************/
/******************************************************************************************************/
template <typename SCALAR_T>
inline Eigen::Matrix<SCALAR_T, 3, 1> angularVelocityInWorldToEulerAngleZyxDerivatives(
    const Eigen::Matrix<SCALAR_T, 3, 1>& eulerAngles, const Eigen::Matrix<SCALAR_T, 3, 1>& omega_world_base_inWorld) {
  const double cyaw = cos(eulerAngles(0));
  const double cpitch = cos(eulerAngles(1));

  const double syaw = sin(eulerAngles(0));
  const double spitch = sin(eulerAngles(1));

  assert(abs(cpitch) > 1e-8);  // test for singularity in debug mode

  Eigen::Matrix<SCALAR_T, 3, 3> transform;
  transform << cyaw * spitch / cpitch, spitch * syaw / cpitch, 1,  // clang-format off
                                -syaw,                   cyaw, 0,
                        cyaw / cpitch,            syaw/cpitch, 0;  // clang-format on

  return transform * omega_world_base_inWorld;
}

/******************************************************************************************************/
/******************************************************************************************************/
/******************************************************************************************************/

/**
 * to map local angular velocity \omega_W expressed in body coordinates, to changes in Euler Angles expressed in an inertial frame q_I
 * we have to map them via \dot{q}_I = H \omega_W, where H is the matrix defined in kindr getMappingFromLocalAngularVelocityToDiff.
 * You can see the kindr cheat sheet to figure out how to build this matrix. The following code computes the Jacobian of \dot{q}_I
 * with respect to \q_I and \omega_W. Thus the lower part of the Jacobian is H and the upper part is dH/dq_I \omega_W. We include
 * both parts for more efficient computation. The following code is computed using auto-diff.
 * @param eulerAnglesXyz
 * @param angularVelocity
 * @return
 */
template <typename SCALAR_T>
inline Eigen::Matrix<SCALAR_T, 6, 3> JacobianOfAngularVelocityMapping(const Eigen::Matrix<SCALAR_T, 3, 1>& eulerAnglesXyz,
                                                                      const Eigen::Matrix<SCALAR_T, 3, 1>& angularVelocity) {
  using std::cos;
  using std::sin;

  Eigen::Matrix<SCALAR_T, 6, 1> xAD;
  xAD << eulerAnglesXyz, angularVelocity;
  const SCALAR_T* x = xAD.data();

  std::array<SCALAR_T, 10> v;

  Eigen::Matrix<SCALAR_T, 6, 3> jac;
  SCALAR_T* y = jac.data();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "cppcoreguidelines-pro-bounds-pointer-arithmetic"
  y[9] = sin(x[2]);
  y[10] = cos(x[2]);
  v[0] = cos(x[1]);
  v[1] = 1 / v[0];
  y[3] = v[1] * y[10];
  v[2] = sin(x[1]);
  y[1] = 0 - (0 - (0 - x[4] * y[9] + x[3] * y[10]) * 1 / v[0] * v[1]) * v[2];
  v[3] = sin(x[2]);
  v[4] = 0 - v[1];
  y[4] = v[4] * y[9];
  v[5] = y[10];
  y[2] = 0 - x[3] * v[1] * v[3] + x[4] * v[4] * v[5];
  y[8] = 0 - x[4] * v[3] + x[3] * v[5];
  v[6] = v[1] * y[9];
  v[7] = v[4] * y[10];
  v[8] = v[2];
  y[15] = v[7] * v[8];
  y[16] = v[6] * v[8];
  v[9] = x[4] * v[8];
  v[8] = x[3] * v[8];
  y[13] = (x[4] * v[6] + x[3] * v[7]) * v[0] - (0 - (v[9] * y[9] - v[8] * y[10]) * 1 / v[0] * v[1]) * v[2];
  y[14] = 0 - v[8] * v[4] * v[3] + v[9] * v[1] * v[5];
  // dependent variables without operations
  y[0] = 0;
  y[5] = 0;
  y[6] = 0;
  y[7] = 0;
  y[11] = 0;
  y[12] = 0;
  y[17] = 1;
#pragma clang diagnostic pop

  return jac;
}

}  // namespace ocs2
