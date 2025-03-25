import esphome.codegen as cg
from esphome.components.api import CONF_ENCRYPTION
from esphome.components.packet_transport import (
    CONF_PING_PONG_ENABLE,
    PacketTransport,
    new_packet_transport,
    transport_schema,
)
from esphome.const import CONF_BINARY_SENSORS, CONF_SENSORS
from esphome.cpp_types import PollingComponent

from .. import SX127X_DEVICE_SCHEMA, register_sx127x_client, sx127x_ns

SX127xTransport = sx127x_ns.class_("SX127xTransport", PacketTransport, PollingComponent)  

CONFIG_SCHEMA = transport_schema(SX127xTransport).extend(SX127X_DEVICE_SCHEMA)


async def to_code(config):
    var, _ = await new_packet_transport(config)
    await register_sx127x_client(var, config)
