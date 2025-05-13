"""ESPHome LoRa component."""

import logging

import esphome.codegen as cg
from esphome.components.lora import CONF_LORA_ID, LORA_SCHEMA
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@dala318"]
DEPENDENCIES = ["lora"]

lorawan_ns = cg.esphome_ns.namespace("lorawan")
LoRaWAN = lorawan_ns.class_("LoRaWAN", cg.Component)

# IS_PLATFORM_COMPONENT = True

DOMAIN = "lorawan"

CONF_LORAWAN_ID = "lorawan_id"

CONF_APP_KEY = "app_key"
CONF_DEV_EUI = "dev_eui"
CONF_APP_EUI = "app_eui"

_LOGGER = logging.getLogger(__name__)


CONFIG_SCHEMA = (
    cv.Schema(
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
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(LORA_SCHEMA)
)


async def to_code(config):
    # cg.add_library("SWL2001", None, "https://github.com/dala318/SWL2001#library")
    cg.add_library(
        "SWL2001",
        None,
        "https://github.com/dala318/SWL2001#d254d39ed39380b0022961f58bfc541749be7853",
    )
    # smtc_modem_core/radio_planner/src/radio_planner_hook_id_defs.h
    cg.add_build_flag("-DNUMBER_OF_STACKS=1")

    # From SWL2001 lbm_lib/smtc_modem_core/lr1mac/src/smtc_real/src/smtc_real_defs.h
    # "RP_VERSION (LoRaWAN Regional Parameter version) must be defined: RP2_101 or RP2_103"
    cg.add_build_flag("-DRP2_101")
    # Likely also one of the following should be defined for some of the reginal settings
    # REGION_EU_868, REGION_AS_923, REGION_US_915, REGION_AU_915, REGION_WW2G4, REGION_CN_470,
    # REGION_IN_865, REGION_KR_920, REGION_RU_864, REGION_CN_470_RP_1_0

    # lbm_lib/smtc_modem_core/geolocation_services/mw_gnss_almanac.c
    # lbm_lib/smtc_modem_core/radio_planner/src/radio_planner_hook_id_defs.h
    # cg.add_build_flag("-DRP_HOOK_ID_REDEFINE")  # Can't use...

    # Temporary defines for getting forward for now
    # cg.add_build_flag("-DSX127X")

    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    lora_var = await cg.get_variable(config[CONF_LORA_ID])
    cg.add(var.set_parent(lora_var))

    cg.add(var.set_app_key(config[CONF_APP_KEY]))
    cg.add(var.set_dev_eui(config[CONF_DEV_EUI]))
    cg.add(var.set_app_eui(config[CONF_APP_EUI]))
