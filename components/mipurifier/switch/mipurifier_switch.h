#pragma once

#include "esphome/components/switch/switch.h"
#include "esphome/core/component.h"
#include "../mipurifier.h"

namespace esphome {
namespace mipurifier {

class MiPurifierBeeperSwitch : public switch_::Switch, public Parented<MiPurifierComponent> {
 public:
  MiPurifierBeeperSwitch() = default;

 protected:
  void write_state(bool state) override;
};

class MiPurifierLockSwitch : public switch_::Switch, public Parented<MiPurifierComponent> {
 public:
  MiPurifierLockSwitch() = default;

 protected:
  void write_state(bool state) override;
};

}  // namespace mipurifier
}  // namespace esphome
