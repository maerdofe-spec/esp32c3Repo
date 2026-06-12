#ifndef DATA_RECORDER_H
#define DATA_RECORDER_H

#include <stdint.h>

class dataRecorder {
  public:
    struct sample {
      unsigned long timeMs;
      float depth;
    };

    dataRecorder();
    void record(float depth, unsigned long nowMs);
    void clear();
    bool hasPendingData() const { return sampleCount_ > 0; }
    uint8_t pendingCount() const { return sampleCount_; }
    const sample *pendingData() const { return samples_; }
    bool uploadPending() const { return uploadPending_; }
    void markUploadPending() { uploadPending_ = true; }
    void clearUploadPending() { uploadPending_ = false; }

  private:
    static const uint8_t kMaxSamples = 128;
    sample samples_[kMaxSamples];
    uint8_t sampleCount_;
    bool uploadPending_;
};

#endif
