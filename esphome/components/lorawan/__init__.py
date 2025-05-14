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

MULTI_CONF = False  # Static object pointer in callbacks prevent multiple instances

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
    cg.add_library(
        "SWL2001",
        None,
        # "https://github.com/dala318/SWL2001#libraty",  # Better, but don't force update check on every build
        # "https://github.com/dala318/SWL2001#afb8974a883d4415cd3d7fdd57c146433eb8d2f7",  # Without transparent
        # "https://github.com/dala318/SWL2001#69ae582225c7dc795c2f46f30c074926caf0b61a",  # Before adding scrFilters
        "https://github.com/dala318/SWL2001#b70d59783f74e4c814cca5edf86209c912e8bd1d",
    )
    cg.add_build_flag("-DTRANSPARENT_RADIO")

    # smtc_modem_core/radio_planner/src/radio_planner_hook_id_defs.h
    cg.add_build_flag("-DNUMBER_OF_STACKS=1")

    # smtc_modem_core/lr1mac/src/smtc_real/src/smtc_real_defs.h
    # "RP_VERSION (LoRaWAN Regional Parameter version) must be defined: RP2_101 or RP2_103"
    cg.add_build_flag("-DRP2_101")

    """
    $(call echo_help, " * MODEM_APP=xxx                   : choose which modem application to build:(default is PERIODICAL_UPLINK)")
    $(call echo_help, " *                                  - PERIODICAL_UPLINK")
    $(call echo_help, " *                                  - HW_MODEM")
    $(call echo_help, " *                                  - PORTING_TESTS")
    $(call echo_help, " *                                  - LCTT_CERTIF")
    $(call echo_help, " * REGION=xxx                      : choose which region should be compiled (default: all)")
    $(call echo_help, " *                                  - AS_923")
    $(call echo_help, " *                                  - AU_915")
    $(call echo_help, " *                                  - CN_470")
    $(call echo_help, " *                                  - CN_470_RP_1_0")
    $(call echo_help, " *                                  - EU_868")
    $(call echo_help, " *                                  - IN_865")
    $(call echo_help, " *                                  - KR_920")
    $(call echo_help, " *                                  - RU_864")
    $(call echo_help, " *                                  - US_915")
    $(call echo_help, " *                                  - WW_2G4 (to be used only for lr1120 and sx128x targets)")
    $(call echo_help, " * CRYPTO=xxx                      : choose which crypto should be compiled (default: SOFT)")
    $(call echo_help, " *                                  - SOFT")
    $(call echo_help, " *                                  - LR11XX (only for lr1110 and lr1120 targets)")
    $(call echo_help, " *                                  - LR11XX_WITH_CREDENTIALS (only for lr1110 and lr1120 targets)")
    $(call echo_help, " * LBM_TRACE=yes/no                : choose to enable or disable modem trace print (default: trace is ON)")
    $(call echo_help, " * APP_TRACE=yes/no                : choose to enable or disable application trace print (default: trace is ON)")
    $(call echo_help, " * ALLOW_RELAY_TX=yes/no           : choose to enable or disable RelayTx (default: no)")
    $(call echo_help, " * ALLOW_RELAY_RX=yes/no           : choose to enable or disable RelayRx (default: no)")
    $(call echo_help, "-------------------- Optional makefile parameters --------------------------")
    $(call echo_help, " * MULTITHREAD=no                  : Disable multithreaded build")
    $(call echo_help, " * VERBOSE=yes                     : Increase build verbosity")
    $(call echo_help, " * SIZE=yes                        : Display size for all objects")
    $(call echo_help, " * DEBUG=yes                       : Compile library and application with debug symbols")
    """

    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    lora_var = await cg.get_variable(config[CONF_LORA_ID])
    cg.add(var.set_parent(lora_var))

    cg.add(var.set_app_key(config[CONF_APP_KEY]))
    cg.add(var.set_dev_eui(config[CONF_DEV_EUI]))
    cg.add(var.set_app_eui(config[CONF_APP_EUI]))
