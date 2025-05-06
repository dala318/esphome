import esphome.codegen as cg
from esphome.components.lora import LoRa, lora_schema, new_lora
import esphome.config_validation as cv
from esphome.cpp_types import Component

from .. import CONF_SX126X_ID, SX126x, SX126xListener, sx126x_modes, sx126x_ns

SX126xLoRa = sx126x_ns.class_("SX126xLoRa", LoRa, Component, SX126xListener)


def validate_lora(config):
    radio_id = str(config[CONF_SX126X_ID])
    mode = sx126x_modes.get(radio_id)
    if mode is not None and mode != "LORA":
        raise cv.Invalid(
            f"SX126x comonent {radio_id} is not configured in modulation LORA"
        )
    return config


CONFIG_SCHEMA = (
    lora_schema(SX126xLoRa)
    .extend(
        {
            cv.GenerateID(CONF_SX126X_ID): cv.use_id(SX126x),
        }
    )
    .add_extra(validate_lora)
)


async def to_code(config):
    # Call the validate_lora function here as well, config validation is none in the
    # order as defined in config.yaml, so if lora is configured before sx126x, the
    # sx126x mode will not be set yet.
    validate_lora(config)

    var = await new_lora(config)
    sx126x = await cg.get_variable(config[CONF_SX126X_ID])
    cg.add(var.set_parent(sx126x))
    cg.add(sx126x.register_listener(var))
