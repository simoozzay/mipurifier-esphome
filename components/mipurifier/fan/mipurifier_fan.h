#pragma once

#include "esphome/components/fan/fan.h"
#include "esphome/core/component.h"
#include "../mipurifier.h"

namespace esphome {
namespace mipurifier {

class MiPurifierFan : public fan::Fan, public Parented<MiPurifierComponent> {
 public:
  MiPurifierFan() = default;
  
  void setup() override;
  fan::FanTraits get_traits() override;

 protected:
  void control(const fan::FanCall &call) override;
};

}  // namespace mipurifier
}  // namespace esphome
