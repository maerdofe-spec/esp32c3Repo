#include "dataRecorder.h"

dataRecorder::dataRecorder()
    : sampleCount_(0), uploadPending_(false) {}

void dataRecorder::record(float depth, unsigned long nowMs) {
  if (sampleCount_ < kMaxSamples) {
    samples_[sampleCount_].timeMs = nowMs;
    samples_[sampleCount_].depth = depth;
    ++sampleCount_;
  }
}

void dataRecorder::clear() {
  sampleCount_ = 0;
  uploadPending_ = false;
}
