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

#ifndef DOUBLETRAIT_OCS2_H_
#define DOUBLETRAIT_OCS2_H_

#include <Eigen/Core>

namespace iit {
namespace rbd {

class DoubleTrait
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	typedef double Scalar;

	inline static Scalar sin(const Scalar& x)  { return std::sin(x); }
	inline static Scalar cos(const Scalar& x)  { return std::cos(x); }
	inline static Scalar tan(const Scalar& x)  { return std::tan(x); }
	inline static Scalar sinh(const Scalar& x) { return std::sinh(x); }
	inline static Scalar cosh(const Scalar& x) { return std::cosh(x); }
	inline static Scalar tanh(const Scalar& x) { return std::tanh(x); }
	inline static Scalar exp(const Scalar& x)  { return std::exp(x); }
	inline static Scalar fabs(const Scalar& x) { return std::fabs(x); }

	// Solves a linear system of equations using an LU factorization
	template <int Rows, int Cols>
	inline static Eigen::Matrix<Scalar, Cols, 1> solve(
			const Eigen::Matrix<Scalar, Rows, Cols>& A,
			const Eigen::Matrix<Scalar, Rows, 1>& b) {

		return A.inverse()*b;
	}

};

} // namespace rbd
} // namespace iit

#endif /* DOUBLETRAIT_OCS2_H_ */

