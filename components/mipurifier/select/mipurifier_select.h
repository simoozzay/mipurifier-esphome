#pragma once

#include "esphome/components/select/select.h"
#include "esphome/core/component.h"
#include "../mipurifier.h"

namespace esphome {
namespace mipurifier {

class MiPurifierBrightnessSelect : public select::Select, public Parented<MiPurifierComponent> {
 public:
  MiPurifierBrightnessSelect() = default;

 protected:
  void control(const std::string &value) override;
};

}  // namespace mipurifier
}  // namespace esphome
