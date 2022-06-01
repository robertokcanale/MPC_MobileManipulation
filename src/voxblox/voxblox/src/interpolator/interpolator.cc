//
// Created by johannes on 18.12.19.
//
#include <voxblox/interpolator/interpolator.h>

using namespace voxblox;

template <>
bool Interpolator<EsdfCachingVoxel>::getInterpolatedDistanceGradient(
    const Point& pos, FloatingPoint* distance, Point* gradient) const {
  typename Layer<EsdfCachingVoxel>::BlockType::ConstPtr block_ptr =
      layer_->getBlockPtrByCoordinates(pos);
  if (block_ptr == nullptr) {
    return false;
  }
  VoxelIndex voxel_index =
      block_ptr->computeTruncatedVoxelIndexFromCoordinates(pos);
  Point voxel_pos = block_ptr->computeCoordinatesFromVoxelIndex(voxel_index);

  const EsdfCachingVoxel* voxel_ptr = block_ptr->getVoxelPtrByCoordinates(pos);

  if (voxel_ptr) {
    float_t voxelSizeInv = block_ptr->voxel_size_inv();
    Point offset = (pos - voxel_pos);
    *distance = voxel_ptr->distance + voxel_ptr->gradient.dot(offset);
    *gradient = voxel_ptr->gradient;
    return true;
  }
  return false;
}

template <>
bool Interpolator<EsdfCachingVoxel>::getInterpolatedDistanceGradientFromHessian(
    const Point& pos, FloatingPoint* distance, Point* gradient) const {
  typename Layer<EsdfCachingVoxel>::BlockType::ConstPtr block_ptr =
      layer_->getBlockPtrByCoordinates(pos);
  if (block_ptr == nullptr) {
    return false;
  }
  VoxelIndex voxel_index =
      block_ptr->computeTruncatedVoxelIndexFromCoordinates(pos);
  Point voxel_pos = block_ptr->computeCoordinatesFromVoxelIndex(voxel_index);

  const EsdfCachingVoxel* voxel_ptr = block_ptr->getVoxelPtrByCoordinates(pos);

  if (voxel_ptr) {
    float_t voxelSizeInv = block_ptr->voxel_size_inv();
    Point offset = (pos - voxel_pos);
    *distance = voxel_ptr->distance + voxel_ptr->gradient.dot(offset) +
                0.5 * offset.dot(voxel_ptr->hessian * offset);
    *gradient = voxel_ptr->gradient + voxel_ptr->hessian * offset;
    return true;
  }
  return false;
}

template <>
bool Interpolator<EsdfCachingVoxel>::getInterpolatedGradient(
    const Point& pos, Point* grad) const {
  typename Layer<EsdfCachingVoxel>::BlockType::ConstPtr block_ptr =
      layer_->getBlockPtrByCoordinates(pos);
  if (block_ptr == nullptr) {
    return false;
  }
  VoxelIndex voxel_index =
      block_ptr->computeTruncatedVoxelIndexFromCoordinates(pos);
  Point voxel_pos = block_ptr->computeCoordinatesFromVoxelIndex(voxel_index);

  const EsdfCachingVoxel* voxel_ptr = block_ptr->getVoxelPtrByCoordinates(pos);

  if (voxel_ptr) {
    float_t voxelSizeInv = block_ptr->voxel_size_inv();
    Point offset = (pos - voxel_pos);
    *grad = voxel_ptr->gradient + voxel_ptr->hessian * offset;
    return true;
  }
  return false;
}

template <>
bool Interpolator<EsdfCachingVoxel>::getInterpolatedDistance(
    const Point& pos, FloatingPoint* distance) const {
  typename Layer<EsdfCachingVoxel>::BlockType::ConstPtr block_ptr =
      layer_->getBlockPtrByCoordinates(pos);
  if (block_ptr == nullptr) {
    return false;
  }
  VoxelIndex voxel_index =
      block_ptr->computeTruncatedVoxelIndexFromCoordinates(pos);
  Point voxel_pos = block_ptr->computeCoordinatesFromVoxelIndex(voxel_index);

  const EsdfCachingVoxel* voxel_ptr = block_ptr->getVoxelPtrByCoordinates(pos);

  if (voxel_ptr) {
    float_t voxelSizeInv = block_ptr->voxel_size_inv();
    Point offset = (pos - voxel_pos);
    *distance = voxel_ptr->distance + voxel_ptr->gradient.dot(offset);
    return true;
  }
  return false;
}