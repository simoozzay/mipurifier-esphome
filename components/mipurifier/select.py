import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
from esphome.const import ENTITY_CATEGORY_CONFIG
from . import mipurifier_ns, MiPurifierComponent, CONF_MIPURIFIER_ID

DEPENDENCIES = ["mipurifier"]

CONF_BRIGHTNESS = "brightness"

MiPurifierBrightnessSelect = mipurifier_ns.class_("MiPurifierBrightnessSelect", select.Select, cg.Parented.template(MiPurifierComponent))

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_MIPURIFIER_ID): cv.use_id(MiPurifierComponent),
    cv.Optional(CONF_BRIGHTNESS): select.select_schema(
        MiPurifierBrightnessSelect,
        icon="mdi:brightness-6",
        entity_category=ENTITY_CATEGORY_CONFIG,
    ),
})

async def to_code(config):
    parent = await cg.get_variable(config[CONF_MIPURIFIER_ID])
    
    if CONF_BRIGHTNESS in config:
        var = await select.new_select(config[CONF_BRIGHTNESS], options=["off", "low", "high"])
        await cg.register_parented(var, config[CONF_MIPURIFIER_ID])
        cg.add(parent.set_brightness_select(var))
