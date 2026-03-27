#ifndef SENTRY_VISION__TARGET_COMBINE_HPP_
#define SENTRY_VISION__TARGET_COMBINE_HPP_

#include <vector>
#include "sentry_vision/types.hpp"
#include "sentry_vision/multi_camera_buffer.hpp"
#include "sentry_vision/coordinate_transformer.hpp"

namespace sentry_vision
{

class ParamManager;

class TargetCombine
{
public:
    explicit TargetCombine(const ParamManager & params);

    /// 从多相机缓存结果 -> 融合后的目标列表
    void fuse(const std::vector<CameraDetectionStamped> & inputs,
              std::vector<TargetState> & targets);

private:
    /// 把所有相机的检测转到车体系
    void transformAll(const std::vector<CameraDetectionStamped> & inputs,
                      std::vector<TargetState> & candidates);

    /// 对同一目标去重合并
    void mergeDuplicates(std::vector<TargetState> & candidates);

    /// 按置信度排序并截断
    void selectTopTargets(std::vector<TargetState> & targets, int max_num = 10);

    CoordinateTransformer transformer_;
    double duplicate_distance_thresh_;

    uint16_t next_id_ = 0;
};

}  // namespace sentry_vision

#endif  // SENTRY_VISION__TARGET_COMBINE_HPP_
