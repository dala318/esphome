"""ESPHome LoRa component."""

import logging

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_PLATFORM

CODEOWNERS = ["@dala318"]

lora_ns = cg.esphome_ns.namespace("lora")
LoRa = lora_ns.class_("LoRa", cg.Component)
LoRaListener = lora_ns.class_("LoRaListener")

IS_PLATFORM_COMPONENT = True

DOMAIN = "lora"

CONF_LORA_ID = "lora_id"


_LOGGER = logging.getLogger(__name__)


LORA_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_LORA_ID): cv.use_id(LoRa),
    }
)


LORA_COMPONENT_SCHEMA = cv.Schema(
    {
        # Place to add generic LoRa-specific config parameters
    }
).extend(cv.COMPONENT_SCHEMA)


async def register_lora_client(var, config):
    lora_var = await cg.get_variable(config[CONF_LORA_ID])
    cg.add(var.set_parent(lora_var))
    return lora_var


def lora_schema(cls):
    return LORA_COMPONENT_SCHEMA.extend({cv.GenerateID(): cv.declare_id(cls)})


async def new_lora(config):
    var = cg.new_Pvariable(config[CONF_ID])
    cg.add(var.set_platform_name(config[CONF_PLATFORM]))
    await cg.register_component(var, config)
    return var
