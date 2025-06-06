#include "mipurifier_switch.h"

namespace esphome {
namespace mipurifier {

void MiPurifierBeeperSwitch::write_state(bool state) {
  if (state) {
    parent_->enable_beeper();
  } else {
    parent_->disable_beeper();
  }
  publish_state(state);
}

void MiPurifierLockSwitch::write_state(bool state) {
  if (state) {
    parent_->lock();
  } else {
    parent_->unlock();
  }
  publish_state(state);
}

}  // namespace mipurifier
}  // namespace esphome
