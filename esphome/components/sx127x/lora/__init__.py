from esphome.components.lora import LoRaComponent, lora_schema, new_lora
from esphome.cpp_types import Component

from .. import SX127X_SCHEMA, register_sx127x_client, sx127x_ns

SX127xLoRa = sx127x_ns.class_("SX127xLoRa", LoRaComponent, Component)

CONFIG_SCHEMA = lora_schema(SX127xLoRa).extend(SX127X_SCHEMA)


async def to_code(config):
    var = await new_lora(config)
    await register_sx127x_client(var, config)
