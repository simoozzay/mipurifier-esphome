import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

DEPENDENCIES = ["uart", "api"]
AUTO_LOAD = ["sensor", "switch", "select", "fan"]

CONF_MIPURIFIER_ID = "mipurifier_id"

mipurifier_ns = cg.esphome_ns.namespace("mipurifier")
MiPurifierComponent = mipurifier_ns.class_("MiPurifierComponent", cg.Component, uart.UARTDevice, cg.CustomAPIDevice)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(MiPurifierComponent),
}).extend(cv.COMPONENT_SCHEMA).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
