#include "mipurifier_fan.h"
#include "esphome/core/log.h"

namespace esphome {
namespace mipurifier {

static const char *const TAG = "mipurifier.fan";

void MiPurifierFan::setup() {
  // Set initial state
  this->state = false;
}

fan::FanTraits MiPurifierFan::get_traits() {
  auto traits = fan::FanTraits();
  traits.set_supports_speed(true);
  traits.set_speed_count(15);
  traits.set_supports_preset_modes(true);
  traits.set_supported_preset_modes({"auto", "night", "low", "medium", "high", "manual"});
  return traits;
}

void MiPurifierFan::control(const fan::FanCall &call) {
  if (call.get_state().has_value()) {
    if (call.get_state().value()) {
      parent_->turn_on();
      this->state = true;
    } else {
      parent_->turn_off();
      this->state = false;
    }
  }
  
  if (call.get_preset_mode().has_value()) {
    const std::string &preset = call.get_preset_mode().value();
    parent_->set_mode(preset);
    this->preset_mode = preset;
  }
  
  if (call.get_speed().has_value()) {
    int speed = call.get_speed().value();
    parent_->set_manualspeed(speed - 1);
    this->speed = speed;
    // Switch to manual mode when speed is set
    parent_->set_mode("manual");
    this->preset_mode = "manual";
  }
  
  this->publish_state();
}

}  // namespace mipurifier
}  // namespace esphome
