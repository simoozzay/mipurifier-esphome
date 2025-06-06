#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/automation.h"
#include "esphome/core/helpers.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/select/select.h"
#include "esphome/components/fan/fan.h"
#include "esphome/components/api/custom_api_device.h"

namespace esphome {
namespace mipurifier {

class MiPurifierComponent : public Component, public uart::UARTDevice, public api::CustomAPIDevice {
 public:
  static const int max_line_length = 80;

  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }

  // Device control methods
  void turn_on();
  void turn_off();
  void enable_beeper();
  void disable_beeper();
  void lock();
  void unlock();
  void set_mode(const std::string &mode);
  void set_brightness(const std::string &brightness);
  void set_manualspeed(int speed);
  void send_command(const std::string &command);

  // Sensor setters
  void set_airquality_sensor(sensor::Sensor *sensor) { airquality_sensor_ = sensor; }
  void set_humidity_sensor(sensor::Sensor *sensor) { humidity_sensor_ = sensor; }
  void set_temperature_sensor(sensor::Sensor *sensor) { temperature_sensor_ = sensor; }
  void set_filterlife_sensor(sensor::Sensor *sensor) { filterlife_sensor_ = sensor; }

  // Switch setters
  void set_beeper_switch(switch_::Switch *switch_obj) { beeper_switch_ = switch_obj; }
  void set_lock_switch(switch_::Switch *switch_obj) { lock_switch_ = switch_obj; }

  // Select setters
  void set_brightness_select(select::Select *select) { brightness_select_ = select; }

  // Fan setter
  void set_fan(fan::Fan *fan) { fan_ = fan; }

 protected:
  char recv_buffer_[max_line_length];
  char send_buffer_[max_line_length];
  bool is_preset_{false};
  uint32_t last_heartbeat_{0};
  uint32_t last_query_{0};

  // Components
  sensor::Sensor *airquality_sensor_{nullptr};
  sensor::Sensor *humidity_sensor_{nullptr};
  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *filterlife_sensor_{nullptr};
  switch_::Switch *beeper_switch_{nullptr};
  switch_::Switch *lock_switch_{nullptr};
  select::Select *brightness_select_{nullptr};
  fan::Fan *fan_{nullptr};

  // Helper methods
  int readline_(int readch, char *buffer, int len);
  void update_property_(const char *id, const char *val);
};

}  // namespace mipurifier
}  // namespace esphome
