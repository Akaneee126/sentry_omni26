#include "sentry_vision/target_combine.hpp"
#include "sentry_vision/param_manager.hpp"

#include <algorithm>
#include <cmath>

namespace sentry_vision
{

TargetCombine::TargetCombine(const ParamManager & params)
: transformer_(params),
  duplicate_distance_thresh_(params.duplicate_distance_thresh())
{
}

void TargetCombine::fuse(const std::vector<CameraDetectionStamped> & inputs,
                         std::vector<TargetState> & targets)
{
    targets.clear();

    // 1. 把所有相机的检测转到车体坐标
    std::vector<TargetState> candidates;
    transformAll(inputs, candidates);

    if (candidates.empty()) {
        return;
    }

    // 2. 按距离阈值去重合并
    mergeDuplicates(candidates);

    // 3. 取置信度最高的前 N 个
    selectTopTargets(candidates, 10);

    targets = std::move(candidates);
}

void TargetCombine::transformAll(const std::vector<CameraDetectionStamped> & inputs,
                                 std::vector<TargetState> & candidates)
{
    for (const auto & cam_data : inputs) {
        for (const auto & det : cam_data.detections) {
            float x, y, z;
            if (!transformer_.pixelToBody(det, cam_data.camera_id, x, y, z)) {
                continue;
            }

            TargetState t;
            t.id = 0;  // 后面去重时再分配
            t.x = x;
            t.y = y;
            t.z = z;
            t.yaw = std::atan2(y, x);
            t.confidence = det.confidence;
            t.is_tracking = false;
            candidates.push_back(t);
        }
    }
}

void TargetCombine::mergeDuplicates(std::vector<TargetState> & candidates)
{
    // 简单贪心合并：遍历列表，如果两个目标距离小于阈值，保留置信度高的
    std::vector<bool> merged(candidates.size(), false);
    std::vector<TargetState> result;

    for (size_t i = 0; i < candidates.size(); ++i) {
        if (merged[i]) continue;

        TargetState best = candidates[i];
        int count = 1;

        for (size_t j = i + 1; j < candidates.size(); ++j) {
            if (merged[j]) continue;

            float dx = candidates[i].x - candidates[j].x;
            float dy = candidates[i].y - candidates[j].y;
            float dz = candidates[i].z - candidates[j].z;
            float dist = std::sqrt(dx * dx + dy * dy + dz * dz);

            if (dist < duplicate_distance_thresh_) {
                merged[j] = true;
                // 加权平均位置
                best.x = (best.x * count + candidates[j].x) / (count + 1);
                best.y = (best.y * count + candidates[j].y) / (count + 1);
                best.z = (best.z * count + candidates[j].z) / (count + 1);
                best.confidence = std::max(best.confidence, candidates[j].confidence);
                count++;
            }
        }

        best.id = next_id_++;
        best.yaw = std::atan2(best.y, best.x);
        result.push_back(best);
    }

    candidates = std::move(result);
}

void TargetCombine::selectTopTargets(std::vector<TargetState> & targets, int max_num)
{
    std::sort(targets.begin(), targets.end(),
              [](const TargetState & a, const TargetState & b) {
                  return a.confidence > b.confidence;
              });

    if (static_cast<int>(targets.size()) > max_num) {
        targets.resize(max_num);
    }
}

}  // namespace sentry_vision
