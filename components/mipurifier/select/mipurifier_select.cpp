#include "mipurifier_select.h"

namespace esphome {
namespace mipurifier {

void MiPurifierBrightnessSelect::control(const std::string &value) {
  parent_->set_brightness(value);
  publish_state(value);
}

}  // namespace mipurifier
}  // namespace esphome
