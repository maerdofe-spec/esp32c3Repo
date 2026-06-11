#include "floatManager.h"

void floatManager::init() {
  currentMillis = millis();
}

void floatManager::handleCurrentState() {
  switch (currentState) {
    case IDLE: idle(); break;
    case DIVE: toDepth(TARGET_DEPTH_DEEP); break;
    case HOVER1: hover(TARGET_DEPTH_DEEP); break;
    case ASCEND: toDepth(TARGET_DEPTH_SHALLOW); break;
    case HOVER2: hover(TARGET_DEPTH_SHALLOW); break;
    case RECOVERY: recovery(); break;
  }
}