import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import fan
from . import mipurifier_ns, MiPurifierComponent, CONF_MIPURIFIER_ID

DEPENDENCIES = ["mipurifier"]

MiPurifierFan = mipurifier_ns.class_("MiPurifierFan", fan.Fan, cg.Parented.template(MiPurifierComponent))

CONFIG_SCHEMA = fan.FAN_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(MiPurifierFan),
    cv.GenerateID(CONF_MIPURIFIER_ID): cv.use_id(MiPurifierComponent),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[cv.GenerateID()])
    await cg.register_component(var, config)
    await fan.register_fan(var, config)
    
    parent = await cg.get_variable(config[CONF_MIPURIFIER_ID])
    cg.add(var.set_parent(parent))
    cg.add(parent.set_fan(var))
