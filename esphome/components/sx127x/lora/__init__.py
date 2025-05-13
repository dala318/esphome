import esphome.codegen as cg
from esphome.components.lora import LoRa, lora_schema, new_lora
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.cpp_types import Component

from .. import (
    CONF_MODULATION,
    CONF_SX127X_ID,
    CONFIG_SCHEMA as SX127X_CONFIG_SCHEMA,
    MOD,
    SX127x,
    SX127xListener,
    sx127x_modes,
    sx127x_ns,
    to_code as sx127x_to_code,
)

SX127xLoRa = sx127x_ns.class_("SX127xLoRa", LoRa, Component, SX127xListener)


def validate_lora(config):
    radio_id = str(config[CONF_SX127X_ID])
    mode = sx127x_modes.get(radio_id)
    if mode is not None and mode != "LORA":
        raise cv.Invalid(
            f"SX127x comonent {radio_id} is not configured in modulation LORA"
        )
    return config


REFERENCE_CONFIG_SCHEMA = (
    lora_schema(SX127xLoRa)
    .extend(
        {
            cv.GenerateID(CONF_SX127X_ID): cv.use_id(SX127x),
        }
    )
    .add_extra(validate_lora)
)

INCLUDE_CONFIG_SCHEMA = SX127X_CONFIG_SCHEMA.extend(lora_schema(SX127xLoRa)).extend(
    {
        cv.GenerateID(CONF_SX127X_ID): cv.declare_id(SX127x),
        cv.Optional(CONF_MODULATION, default="LORA"): cv.enum({"LORA": MOD["LORA"]}),
    }
)

CONFIG_SCHEMA = cv.Any(REFERENCE_CONFIG_SCHEMA, INCLUDE_CONFIG_SCHEMA)


async def to_code(config):
    # LoRa Config with ref to other SX127x is expected to have been provided
    if CONF_MODULATION not in config:
        # Call the validate_lora function here as well, config validation is done in the
        # order as defined in config.yaml, so if lora is configured before sx127x, the
        # sx127x mode will not be set yet.
        validate_lora(config)

        var = await new_lora(config)
        sx127x = await cg.get_variable(config[CONF_SX127X_ID])
        cg.add(var.set_parent(sx127x))
        cg.add(sx127x.register_listener(var))

    # LoRa Config with full SX127x module config is provided
    else:
        # Create the sx127x component
        sx127x_config = config.copy()
        sx127x_config[CONF_ID] = config[CONF_SX127X_ID]
        await sx127x_to_code(sx127x_config)

        # Create the lora component
        var = await new_lora(config)
        sx127x = await cg.get_variable(config[CONF_SX127X_ID])
        cg.add(var.set_parent(sx127x))
        cg.add(sx127x.register_listener(var))
