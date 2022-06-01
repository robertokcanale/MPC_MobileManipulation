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

#include <ocs2_core/Dimensions.h>

namespace ocs2 {

/**
 * This class is an interface to a NLP cost.
 */
class NLP_Cost {
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  using DIMENSIONS = Dimensions<0, 0>;
  using scalar_t = typename DIMENSIONS::scalar_t;
  using scalar_array_t = typename DIMENSIONS::scalar_array_t;
  using dynamic_vector_t = typename DIMENSIONS::dynamic_vector_t;
  using dynamic_matrix_t = typename DIMENSIONS::dynamic_matrix_t;

  /**
   * Default constructor.
   */
  NLP_Cost() = default;

  /**
   * Default destructor.
   */
  virtual ~NLP_Cost() = default;

  /**
   * Sets the current parameter vector.
   *
   * @param [in] x: The value of parameter vector.
   * @return id: It returns a number which identifies the cached data.
   */
  virtual size_t setCurrentParameter(const dynamic_vector_t& x) = 0;

  /**
   * Gets the cost value.
   *
   * @param [in] id: The ID of the cached data.
   * @param [out] f: The value of the cost.
   * @return status: whether the cost computation was successful.
   */
  virtual bool getCost(size_t id, scalar_t& f) = 0;

  /**
   * Gets the gradient of the cost w.r.t. parameter vector.
   *
   * @param [in] id: The ID of the cached data.
   * @param [out] g: The gradient of the cost.
   */
  virtual void getCostDerivative(size_t id, dynamic_vector_t& g) = 0;

  /**
   * Gets the Hessian of the cost w.r.t. parameter vector.
   *
   * @param [in] id: The ID of the cached data.
   * @param [out] H: The Hessian of the cost.
   */
  virtual void getCostSecondDerivative(size_t id, dynamic_matrix_t& H) = 0;

  /**
   * Clears the cache.
   */
  virtual void clearCache() = 0;
};

}  // namespace ocs2
