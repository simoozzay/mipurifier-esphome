#include "mipurifier.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "switch/mipurifier_switch.h"
#include "select/mipurifier_select.h"
#include "fan/mipurifier_fan.h"

namespace esphome {
namespace mipurifier {

static const char *const TAG = "mipurifier";

void MiPurifierComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up MiPurifier...");
  
  // Initialize buffers
  memset(recv_buffer_, 0, max_line_length);
  memset(send_buffer_, 0, max_line_length);
  
  // Register service for sending custom commands
  register_service(&MiPurifierComponent::send_command, "send_command", {"command"});
  
  // Get initial state & settings
  strcpy(send_buffer_, "down get_properties 2 2 2 5 5 1 6 1 7 1 10 10");
}

void MiPurifierComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "MiPurifier:");
  if (this->airquality_sensor_) {
    LOG_SENSOR("  ", "Air Quality Sensor", this->airquality_sensor_);
  }
  if (this->humidity_sensor_) {
    LOG_SENSOR("  ", "Humidity Sensor", this->humidity_sensor_);
  }
  if (this->temperature_sensor_) {
    LOG_SENSOR("  ", "Temperature Sensor", this->temperature_sensor_);
  }
  if (this->filterlife_sensor_) {
    LOG_SENSOR("  ", "Filter Life Sensor", this->filterlife_sensor_);
  }
}

void MiPurifierComponent::loop() {
  while (available()) {
    if (readline_(read(), recv_buffer_, max_line_length) > 0) {
      char *cmd = strtok(recv_buffer_, " ");
      if (strcmp(cmd, "net") == 0) {
        write_str("local");
      } else if (strcmp(cmd, "time") == 0) {
        write_str("0");
      } else if (strcmp(cmd, "get_down") == 0) {
        // Send command from send_buffer
        if (strlen(send_buffer_) > 0) {
          write_str(send_buffer_);
          send_buffer_[0] = '\0';
          ESP_LOGD(TAG, "Sent send_buffer");
        } else if (millis() - last_heartbeat_ > 60000) {
          // Send mysterious heartbeat message
          write_str("down set_properties 13 9 60");
          last_heartbeat_ = millis();
          ESP_LOGD(TAG, "Sent heartbeat");
        } else if (millis() - last_query_ > 60000) {
          // Force sensor update
          write_str("down get_properties 3 6 3 7 3 8 4 3");
          last_query_ = millis();
          ESP_LOGD(TAG, "Sent query string");
        } else {
          write_str("down none");
        }
      } else if (strcmp(cmd, "properties_changed") == 0) {
        ESP_LOGD(TAG, "Parsing properties_changed message");
        char *id1 = strtok(nullptr, " ");
        char *id2 = strtok(nullptr, " ");
        if (id1 && id2) {
          char id[10];
          strcpy(id, id1);
          strcat(id, id2);
          char *val = strtok(nullptr, " ");
          if (val) {
            update_property_(id, val);
          }
        }
        write_str("ok");
      } else if (strcmp(cmd, "result") == 0) {
        // Loop over all properties and update
        ESP_LOGD(TAG, "Parsing result message");
        char *id1, *id2, *val;
        while (true) {
          if (!(id1 = strtok(nullptr, " "))) break;
          if (!(id2 = strtok(nullptr, " "))) break;
          char id[10];
          strcpy(id, id1);
          strcat(id, id2);
          strtok(nullptr, " "); // skip 0
          if (!(val = strtok(nullptr, " "))) break;
          update_property_(id, val);
        }
        write_str("ok");
      } else {
        // Just acknowledge any other message
        write_str("ok");
      }
    }
  }
}

int MiPurifierComponent::readline_(int readch, char *buffer, int len) {
  static int pos = 0;
  int rpos;
  
  if (readch > 0) {
    switch (readch) {
      case '\r': // Return on CR
        rpos = pos;
        pos = 0;  // Reset position index ready for next time
        return rpos;
      default:
        if (pos < len-1) {
          buffer[pos++] = readch;
          buffer[pos] = 0;
        }
    }
  }
  // No end of line has been found, so return -1.
  return -1;
}

void MiPurifierComponent::turn_on() {
  strcpy(send_buffer_, "down set_properties 2 2 true");
}

void MiPurifierComponent::turn_off() {
  strcpy(send_buffer_, "down set_properties 2 2 false");
}

void MiPurifierComponent::enable_beeper() {
  strcpy(send_buffer_, "down set_properties 5 1 true");
}

void MiPurifierComponent::disable_beeper() {
  strcpy(send_buffer_, "down set_properties 5 1 false");
}

void MiPurifierComponent::lock() {
  strcpy(send_buffer_, "down set_properties 7 1 true");
}

void MiPurifierComponent::unlock() {
  strcpy(send_buffer_, "down set_properties 7 1 false");
}

void MiPurifierComponent::set_mode(const std::string &mode) {
  // 0: auto, 1: sleep, 2: manual, 3: low, 4: med, 5: high
  if (mode == "auto") {
    strcpy(send_buffer_, "down set_properties 2 5 0");
  } else if (mode == "night") {
    strcpy(send_buffer_, "down set_properties 2 5 1");
  } else if (mode == "manual") {
    strcpy(send_buffer_, "down set_properties 2 5 2");
  } else if (mode == "low") {
    strcpy(send_buffer_, "down set_properties 2 4 1");
  } else if (mode == "medium") {
    strcpy(send_buffer_, "down set_properties 2 4 2");
  } else if (mode == "high") {
    strcpy(send_buffer_, "down set_properties 2 4 3");
  }
}

void MiPurifierComponent::set_brightness(const std::string &brightness) {
  if (brightness == "off") {
    strcpy(send_buffer_, "down set_properties 6 1 2");
  } else if (brightness == "low") {
    strcpy(send_buffer_, "down set_properties 6 1 1");
  } else if (brightness == "high") {
    strcpy(send_buffer_, "down set_properties 6 1 0");
  }
}

void MiPurifierComponent::set_manualspeed(int speed) {
  snprintf(send_buffer_, max_line_length, "down set_properties 10 10 %i", speed);
}

void MiPurifierComponent::send_command(const std::string &command) {
  strcpy(send_buffer_, command.c_str());
}

void MiPurifierComponent::update_property_(const char *id, const char *val) {
  if (strcmp(id, "36") == 0) {
    if (airquality_sensor_) {
      airquality_sensor_->publish_state(atof(val));
    }
  } else if (strcmp(id, "37") == 0) {
    if (humidity_sensor_) {
      humidity_sensor_->publish_state(atof(val));
    }
  } else if (strcmp(id, "38") == 0) {
    if (temperature_sensor_) {
      temperature_sensor_->publish_state(atof(val));
    }
  } else if (strcmp(id, "43") == 0) {
    if (filterlife_sensor_) {
      filterlife_sensor_->publish_state(atof(val));
    }
  } else if (strcmp(id, "22") == 0) {
    // power (on, off)
    if (fan_) {
      if (strcmp(val, "true") == 0) {
        auto call = fan_->turn_on();
        call.perform();
      } else {
        auto call = fan_->turn_off();
        call.perform();
      }
    }
  } else if (strcmp(id, "25") == 0) {
    // mode (auto, night, manual, preset)
    is_preset_ = false;
    if (fan_) {
      auto call = fan_->make_call();
      switch (atoi(val)) {
        case 0: 
          call.set_preset_mode("auto");
          break;
        case 1:
          call.set_preset_mode("night");
          break;
        case 2:
          call.set_preset_mode("manual");
          break;
        case 3:
          is_preset_ = true;
          break;
      }
      call.perform();
    }
  } else if (strcmp(id, "24") == 0) {
    // preset (low, medium, high)
    if (is_preset_ && fan_) {
      auto call = fan_->make_call();
      switch (atoi(val)) {
        case 1:
          call.set_preset_mode("low");
          break;
        case 2:
          call.set_preset_mode("medium");
          break;
        case 3:
          call.set_preset_mode("high");
          break;
      }
      call.perform();
    }
  } else if (strcmp(id, "51") == 0) {
    // beeper (on, off)
    if (beeper_switch_) {
      beeper_switch_->publish_state(strcmp(val, "true") == 0);
    }
  } else if (strcmp(id, "71") == 0) {
    // lock (on, off)
    if (lock_switch_) {
      lock_switch_->publish_state(strcmp(val, "true") == 0);
    }
  } else if (strcmp(id, "61") == 0) {
    // display brightness (off, low, high)
    if (brightness_select_) {
      switch (atoi(val)) {
        case 0:
          brightness_select_->publish_state("high");
          break;
        case 1:
          brightness_select_->publish_state("low");
          break;
        case 2:
          brightness_select_->publish_state("off");
          break;
      }
    }
  } else if (strcmp(id, "1010") == 0) {
    // manual speed
    if (fan_) {
      // Note: This would require accessing the fan's speed property
      // which may need additional implementation depending on fan component
    }
  }
}

}  // namespace mipurifier
}  // namespace esphome
