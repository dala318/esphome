"""ESPHome LoRa component."""

import logging

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@dala318"]

lorawan_ns = cg.esphome_ns.namespace("lorawan")
LoRaWAN = lorawan_ns.class_("LoRaWAN", cg.Component)

# IS_PLATFORM_COMPONENT = True

DOMAIN = "lorawan"

CONF_LORAWAN_ID = "lorawan_id"
CONF_LORA_ID = "lora_id"

CONF_APP_KEY = "app_key"
CONF_DEV_EUI = "dev_eui"
CONF_APP_EUI = "app_eui"
CONF_DUMMY_RADIO = "dummy_radio"

_LOGGER = logging.getLogger(__name__)


CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(LoRaWAN),
        cv.Required(CONF_APP_KEY): cv.All(
            cv.ensure_list(cv.hex_uint8_t), cv.Length(min=16, max=16)
        ),
        cv.Required(CONF_DEV_EUI): cv.All(
            cv.ensure_list(cv.hex_uint8_t), cv.Length(min=8, max=8)
        ),
        cv.Required(CONF_APP_EUI): cv.All(
            cv.ensure_list(cv.hex_uint8_t), cv.Length(min=8, max=8)
        ),
        cv.Optional(CONF_DUMMY_RADIO, default=False): cv.boolean,
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    lora_var = await cg.get_variable(config[CONF_LORA_ID])
    cg.add(var.set_parent(lora_var))

    cg.add(var.set_app_key(config[CONF_APP_KEY]))
    cg.add(var.set_dev_eui(config[CONF_DEV_EUI]))
    cg.add(var.set_app_eui(config[CONF_APP_EUI]))
    if config[CONF_DUMMY_RADIO]:
        cg.add_define("LORAWAN_INCLUDE_DUMMY_RADIO")


# LORAWAN_SCHEMA = (
#     cv.Schema(
#         {
#         }
#     ).extend(cv.COMPONENT_SCHEMA)
#     # .add_extra(validate_)
# )


# async def to_code(config):
#     var = cg.new_Pvariable(config[CONF_ID])
#     await cg.register_component(var, config)
#     return var
