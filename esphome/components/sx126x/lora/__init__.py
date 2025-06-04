import esphome.codegen as cg
from esphome.components.lora import LoRa, lora_schema, new_lora
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.cpp_types import Component
import esphome.final_validate as fv

from .. import (
    CONF_MODULATION,
    CONF_SX126X_ID,
    CONFIG_SCHEMA as SX126X_CONFIG_SCHEMA,
    MOD,
    SX126x,
    SX126xListener,
    sx126x_ns,
    to_code as sx126x_to_code,
)

SX126xLoRa = sx126x_ns.class_("SX126xLoRa", LoRa, Component, SX126xListener)


REFERENCE_CONFIG_SCHEMA = lora_schema(SX126xLoRa).extend(
    {
        cv.GenerateID(CONF_SX126X_ID): cv.use_id(SX126x),
    }
)


INCLUDE_CONFIG_SCHEMA = SX126X_CONFIG_SCHEMA.extend(lora_schema(SX126xLoRa)).extend(
    {
        cv.GenerateID(CONF_SX126X_ID): cv.declare_id(SX126x),
        cv.Optional(CONF_MODULATION, default="LORA"): cv.enum({"LORA": MOD["LORA"]}),
    }
)

CONFIG_SCHEMA = cv.Any(REFERENCE_CONFIG_SCHEMA, INCLUDE_CONFIG_SCHEMA)


def _final_validate(config):
    full_config = fv.full_config.get()
    sx_path = full_config.get_path_for_id(config[CONF_SX126X_ID])[:-1]
    sx_config = full_config.get_config_for_path(sx_path)

    mode = sx_config.get(CONF_MODULATION)
    if mode is not None and mode != "LORA":
        raise cv.Invalid(
            f"SX126x comonent {config[CONF_SX126X_ID]} is not configured in modulation LORA"
        )


FINAL_VALIDATE_SCHEMA = _final_validate


async def to_code(config):
    # LoRa Config with ref to other SX126x is expected to have been provided
    if CONF_MODULATION not in config:
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
