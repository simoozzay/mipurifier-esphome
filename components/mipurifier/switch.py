import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import ENTITY_CATEGORY_CONFIG
from . import mipurifier_ns, MiPurifierComponent, CONF_MIPURIFIER_ID

DEPENDENCIES = ["mipurifier"]

CONF_BEEPER = "beeper"
CONF_LOCK = "lock"

MiPurifierBeeperSwitch = mipurifier_ns.class_("MiPurifierBeeperSwitch", switch.Switch, cg.Parented.template(MiPurifierComponent))
MiPurifierLockSwitch = mipurifier_ns.class_("MiPurifierLockSwitch", switch.Switch, cg.Parented.template(MiPurifierComponent))

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_MIPURIFIER_ID): cv.use_id(MiPurifierComponent),
    cv.Optional(CONF_BEEPER): switch.switch_schema(
        MiPurifierBeeperSwitch,
        icon="mdi:volume-high",
        entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_LOCK): switch.switch_schema(
        MiPurifierLockSwitch,
        icon="mdi:lock",
        entity_category=ENTITY_CATEGORY_CONFIG,
    ),
})

async def to_code(config):
    parent = await cg.get_variable(config[CONF_MIPURIFIER_ID])
    
    if CONF_BEEPER in config:
        var = await switch.new_switch(config[CONF_BEEPER])
        await cg.register_parented(var, config[CONF_MIPURIFIER_ID])
        cg.add(parent.set_beeper_switch(var))
    
    if CONF_LOCK in config:
        var = await switch.new_switch(config[CONF_LOCK])
        await cg.register_parented(var, config[CONF_MIPURIFIER_ID])
        cg.add(parent.set_lock_switch(var))
