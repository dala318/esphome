import esphome.codegen as cg
from esphome.components.lora import LoRa, lora_schema, new_lora
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.cpp_types import Component

from .. import (
    CONF_MODULATION,
    CONF_SX126X_ID,
    CONFIG_SCHEMA as SX126X_CONFIG_SCHEMA,
    MOD,
    SX126x,
    SX126xListener,
    sx126x_modes,
    sx126x_ns,
    to_code as sx126x_to_code,
)

SX126xLoRa = sx126x_ns.class_("SX126xLoRa", LoRa, Component, SX126xListener)


def validate_lora(config):
    radio_id = str(config[CONF_SX126X_ID])
    mode = sx126x_modes.get(radio_id)
    if mode is not None and mode != "LORA":
        raise cv.Invalid(
            f"SX126x comonent {radio_id} is not configured in modulation LORA"
        )
    return config


REFERENCE_CONFIG_SCHEMA = (
    lora_schema(SX126xLoRa)
    .extend(
        {
            cv.GenerateID(CONF_SX126X_ID): cv.use_id(SX126x),
        }
    )
    .add_extra(validate_lora)
)


INCLUDE_CONFIG_SCHEMA = SX126X_CONFIG_SCHEMA.extend(lora_schema(SX126xLoRa)).extend(
    {
        cv.GenerateID(CONF_SX126X_ID): cv.declare_id(SX126x),
        cv.Optional(CONF_MODULATION, default="LORA"): cv.enum({"LORA": MOD["LORA"]}),
    }
)

CONFIG_SCHEMA = cv.Any(REFERENCE_CONFIG_SCHEMA, INCLUDE_CONFIG_SCHEMA)


async def to_code(config):
    # LoRa Config with ref to other SX126x is expected to have been provided
    if CONF_MODULATION not in config:
        # Call the validate_lora function here as well, config validation is done in the
        # order as defined in config.yaml, so if lora is configured before sx126x, the
        # sx126x mode will not be set yet.
        validate_lora(config)

        var = await new_lora(config)
        sx126x = await cg.get_variable(config[CONF_SX126X_ID])
        cg.add(var.set_parent(sx126x))
        cg.add(sx126x.register_listener(var))

    # LoRa Config with full SX126x module config is provided
    else:
        # Create the sx126x component
        sx126x_config = config.copy()
        sx126x_config[CONF_ID] = config[CONF_SX126X_ID]
        await sx126x_to_code(sx126x_config)

        # Create the lora component
        var = await new_lora(config)
        sx126x = await cg.get_variable(config[CONF_SX126X_ID])
        cg.add(var.set_parent(sx126x))
        cg.add(sx126x.register_listener(var))
