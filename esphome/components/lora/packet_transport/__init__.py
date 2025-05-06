from esphome.components.packet_transport import (
    PacketTransport,
    new_packet_transport,
    transport_schema,
)
from esphome.cpp_types import PollingComponent

from .. import LORA_SCHEMA, lora_ns, register_lora_client

LoRaTransport = lora_ns.class_("LoRaTransport", PacketTransport, PollingComponent)

CONFIG_SCHEMA = transport_schema(LoRaTransport).extend(LORA_SCHEMA)


async def to_code(config):
    var, _ = await new_packet_transport(config)
    await register_lora_client(var, config)
