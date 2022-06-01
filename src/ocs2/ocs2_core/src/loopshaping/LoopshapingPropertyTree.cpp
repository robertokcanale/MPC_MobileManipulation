//
// Created by rgrandia on 14.02.20.
//

#include "ocs2_core/loopshaping/LoopshapingPropertyTree.h"

#include <boost/property_tree/info_parser.hpp>

namespace ocs2 {
namespace loopshaping_property_tree {
Filter readSISOFilter(const boost::property_tree::ptree& pt, std::string filterName, bool invert) {
  // Get Sizes
  auto numRepeats = pt.get<size_t>(filterName + ".numRepeats");
  auto numPoles = pt.get<size_t>(filterName + ".numPoles");
  auto numZeros = pt.get<size_t>(filterName + ".numZeros");
  auto DCGain = pt.get<double>(filterName + ".DCGain");
  size_t numStates = numRepeats * numPoles;
  size_t numInputs = numRepeats;
  size_t numOutputs = numRepeats;

  // Setup Filter, convention a0*s^n + a1*s^(n-1) + ... + an
  Eigen::VectorXd numerator(numZeros + 1);
  numerator.setZero();
  numerator(0) = 1.0;
  for (size_t z = 0; z < numZeros; z++) {
    auto zero = pt.get<double>(filterName + ".zeros." + "(" + std::to_string(z) + ")");
    numerator.segment(1, z + 1) -= zero * numerator.segment(0, z + 1).eval();
  }

  Eigen::VectorXd denominator(numPoles + 1);
  denominator.setZero();
  denominator(0) = 1.0;
  for (size_t p = 0; p < numPoles; p++) {
    auto pole = pt.get<double>(filterName + ".poles." + "(" + std::to_string(p) + ")");
    denominator.segment(1, p + 1) -= pole * denominator.segment(0, p + 1).eval();
  }

  // Scale
  if (DCGain > 0) {
    double currentDCGain = numerator(numZeros) / denominator(numPoles);
    if (currentDCGain < 1e-6 || currentDCGain > 1e6) {
      throw std::runtime_error("Trouble rescaling transfer function, current DCGain: " + std::to_string(currentDCGain));
    }
    double scaling = DCGain / currentDCGain;
    numerator *= scaling;
  }

  if (invert) {
    Eigen::VectorXd temp;
    temp = numerator;
    numerator = denominator;
    denominator = temp;
  }

  // Convert to state space
  Eigen::MatrixXd a, b, c, d;
  ocs2::tf2ss(numerator, denominator, a, b, c, d);

  Eigen::MatrixXd A = Eigen::MatrixXd::Zero(numStates, numStates);
  Eigen::MatrixXd B = Eigen::MatrixXd::Zero(numStates, numInputs);
  Eigen::MatrixXd C = Eigen::MatrixXd::Zero(numInputs, numStates);
  Eigen::MatrixXd D = Eigen::MatrixXd::Zero(numInputs, numInputs);
  size_t statecount = 0;
  for (size_t r = 0; r < numRepeats; r++) {
    A.block(statecount, statecount, numPoles, numPoles) = a;
    B.block(statecount, r, numPoles, 1) = b;
    C.block(r, statecount, 1, numPoles) = c;
    D.block(r, r, 1, 1) = d;
    statecount += numPoles;
  }

  return Filter(A, B, C, D);
}

Filter readMIMOFilter(const boost::property_tree::ptree& pt, std::string filterName, bool invert) {
  auto numFilters = pt.get<size_t>(filterName + ".numFilters");
  Eigen::MatrixXd A(0, 0), B(0, 0), C(0, 0), D(0, 0);
  if (numFilters > 0) {
    // Read the sisoFilters
    std::vector<Filter> sisoFilters;
    size_t numStates(0), numInputs(0), numOutputs(0);
    for (size_t i = 0; i < numFilters; ++i) {
      // Read filter
      std::string sisoFilterName = filterName + ".Filter" + std::to_string(i);
      sisoFilters.emplace_back(readSISOFilter(pt, sisoFilterName, invert));

      // Track sizes
      numStates += sisoFilters.back().getNumStates();
      numInputs += sisoFilters.back().getNumInputs();
      numOutputs += sisoFilters.back().getNumOutputs();
    }

    // Concatenate siso matrices into one MIMO filter
    A = Eigen::MatrixXd::Zero(numStates, numStates);
    B = Eigen::MatrixXd::Zero(numStates, numInputs);
    C = Eigen::MatrixXd::Zero(numOutputs, numStates);
    D = Eigen::MatrixXd::Zero(numOutputs, numInputs);
    size_t statecount(0), inputcount(0), outputcount(0);
    for (const auto& filt : sisoFilters) {
      A.block(statecount, statecount, filt.getNumStates(), filt.getNumStates()) = filt.getA();
      B.block(statecount, inputcount, filt.getNumStates(), filt.getNumInputs()) = filt.getB();
      C.block(outputcount, statecount, filt.getNumOutputs(), filt.getNumStates()) = filt.getC();
      D.block(outputcount, inputcount, filt.getNumOutputs(), filt.getNumInputs()) = filt.getD();
      statecount += filt.getNumStates();
      inputcount += filt.getNumInputs();
      outputcount += filt.getNumOutputs();
    }
  }
  return Filter(A, B, C, D);
}

std::shared_ptr<LoopshapingDefinition> load(const std::string& settingsFile) {
  // Read from settings File
  boost::property_tree::ptree pt;
  boost::property_tree::read_info(settingsFile, pt);
  Filter r_filter = loopshaping_property_tree::readMIMOFilter(pt, "r_filter");
  Filter s_filter = loopshaping_property_tree::readMIMOFilter(pt, "s_inv_filter", true);
  auto gamma = pt.get<double>("gamma");

  if (r_filter.getNumOutputs() > 0 && s_filter.getNumOutputs() > 0) {
    throw std::runtime_error("[LoopshapingDefinition] using both r and s filter not implemented");
  }

  if (r_filter.getNumOutputs() > 0) {
    return std::shared_ptr<LoopshapingDefinition>(new LoopshapingDefinition(LoopshapingType::outputpattern, r_filter, gamma));
  }
  if (s_filter.getNumOutputs() > 0) {
    auto eliminateInputs = pt.get<bool>("eliminateInputs");
    if (eliminateInputs) {
      return std::shared_ptr<LoopshapingDefinition>(new LoopshapingDefinition(LoopshapingType::eliminatepattern, s_filter, gamma));
    } else {
      return std::shared_ptr<LoopshapingDefinition>(new LoopshapingDefinition(LoopshapingType::inputpattern, s_filter, gamma));
    }
  }
}

}  // namespace loopshaping_property_tree
}  // namespace ocs2
