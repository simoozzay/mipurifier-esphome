# ESPHome MiPurifier External Component Migration

This document explains the migration from the old custom component pattern to the new ESPHome external component pattern.

## What Changed

The code has been restructured to follow ESPHome's modern external component architecture:

### Old Structure
- Single `mipurifier.h` file with everything
- `custom_component` in YAML
- `custom` sensor platform
- Template-based switches and selects

### New Structure
```
components/
  mipurifier/
    __init__.py              # Main component definition
    mipurifier.h/.cpp        # Core component implementation
    sensor.py                # Sensor platform definition
    switch.py                # Switch platform definition
    select.py                # Select platform definition  
    fan.py                   # Fan platform definition
    switch/
      mipurifier_switch.h/.cpp
    select/
      mipurifier_select.h/.cpp
    fan/
      mipurifier_fan.h/.cpp
```

## Key Improvements

1. **Proper Component Architecture**: Each entity type (sensor, switch, select, fan) is now a proper ESPHome platform
2. **Better Code Organization**: Code is split into logical modules
3. **Type Safety**: Proper C++ classes with inheritance
4. **ESPHome Integration**: Full integration with ESPHome's config validation and code generation
5. **Future Compatibility**: Follows current ESPHome best practices

## Configuration Changes

### Old Configuration (`mipurifier.yaml`)
```yaml
custom_component:
  - lambda: |-
      auto mipurifier = new MiPurifier(id(uart_bus));
      App.register_component(mipurifier);
      return {mipurifier};

sensor:
  - platform: custom
    lambda: |-
      auto c = static_cast<MiPurifier *>(mipurifier);
      return {c->airquality_sensor, ...};

switch:
  - platform: template
    turn_on_action:
      - lambda: |-
          auto c = static_cast<MiPurifier *>(mipurifier);
          c->enable_beeper();
```

### New Configuration (`mipurifier_new.yaml`)
```yaml
external_components:
  - source:
      type: local
      path: components

mipurifier:
  id: mipurifier_component
  uart_id: uart_bus

sensor:
  - platform: mipurifier
    mipurifier_id: mipurifier_component
    air_quality:
      name: "Air quality (PM2.5)"

switch:
  - platform: mipurifier
    mipurifier_id: mipurifier_component
    beeper:
      name: "Beeper"
```

## Migration Steps

1. **Copy the new components folder** to your ESPHome configuration directory
2. **Update your YAML file** to use the new configuration format (see `mipurifier_new.yaml`)
3. **Remove the old `mipurifier.h` file** from your includes
4. **Test the configuration** before deploying

## Features Preserved

All original functionality is preserved:
- ✅ Fan control (on/off, speed, preset modes)
- ✅ Environmental sensors (PM2.5, humidity, temperature, filter life)
- ✅ Configuration switches (beeper, lock)
- ✅ Display brightness control
- ✅ Custom command service (`esphome.mipurifier_send_command`)
- ✅ MIoT protocol implementation
- ✅ Heartbeat and query mechanisms

## Benefits of the New Approach

1. **Maintainability**: Easier to maintain and extend
2. **Debugging**: Better error messages and debugging support
3. **Documentation**: Self-documenting through ESPHome's schema validation
4. **IDE Support**: Better IDE support with proper C++ structure
5. **Community**: Follows ESPHome community standards
6. **Future-proof**: Compatible with future ESPHome versions

## Service Usage

The custom command service is still available:
```yaml
service: esphome.mipurifier_send_command
data:
  command: "down set_properties 5 1 false"
```

## Troubleshooting

If you encounter issues:

1. **Check ESPHome logs** for detailed error messages
2. **Verify YAML syntax** using ESPHome's validation
3. **Ensure all file paths** are correct in your configuration
4. **Check component dependencies** are properly loaded

## Technical Notes

- Uses modern ESPHome APIs (`setup_priority::AFTER_WIFI` instead of `esphome::setup_priority::AFTER_WIFI`)
- Proper namespace organization (`esphome::mipurifier`)
- Memory-safe pointer handling with proper null checks
- Follows ESPHome coding standards and conventions
