import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_HUMIDITY,
    CONF_TEMPERATURE,
    DEVICE_CLASS_HUMIDITY,
    DEVICE_CLASS_PM25,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    UNIT_MICROGRAM_PER_CUBIC_METER,
    UNIT_PERCENT,
)
from . import mipurifier_ns, MiPurifierComponent, CONF_MIPURIFIER_ID

DEPENDENCIES = ["mipurifier"]

CONF_AIR_QUALITY = "air_quality"
CONF_FILTER_LIFE = "filter_life"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_MIPURIFIER_ID): cv.use_id(MiPurifierComponent),
    cv.Optional(CONF_AIR_QUALITY): sensor.sensor_schema(
        unit_of_measurement=UNIT_MICROGRAM_PER_CUBIC_METER,
        device_class=DEVICE_CLASS_PM25,
        state_class=STATE_CLASS_MEASUREMENT,
        accuracy_decimals=0,
    ),
    cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
        unit_of_measurement=UNIT_PERCENT,
        device_class=DEVICE_CLASS_HUMIDITY,
        state_class=STATE_CLASS_MEASUREMENT,
        accuracy_decimals=0,
    ),
    cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
        accuracy_decimals=1,
    ),
    cv.Optional(CONF_FILTER_LIFE): sensor.sensor_schema(
        unit_of_measurement=UNIT_PERCENT,
        icon="mdi:air-filter",
        accuracy_decimals=0,
    ),
})

async def to_code(config):
    parent = await cg.get_variable(config[CONF_MIPURIFIER_ID])
    
    if CONF_AIR_QUALITY in config:
        sens = await sensor.new_sensor(config[CONF_AIR_QUALITY])
        cg.add(parent.set_airquality_sensor(sens))
    
    if CONF_HUMIDITY in config:
        sens = await sensor.new_sensor(config[CONF_HUMIDITY])
        cg.add(parent.set_humidity_sensor(sens))
    
    if CONF_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(parent.set_temperature_sensor(sens))
    
    if CONF_FILTER_LIFE in config:
        sens = await sensor.new_sensor(config[CONF_FILTER_LIFE])
        cg.add(parent.set_filterlife_sensor(sens))
