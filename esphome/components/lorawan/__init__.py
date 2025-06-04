"""ESPHome LoRa component."""

import logging
from typing import Any

import esphome.codegen as cg
from esphome.components import sensor
from esphome.components.lora import CONF_LORA_ID, LORA_SCHEMA
import esphome.config_validation as cv
from esphome.const import (
    CONF_BATTERY_LEVEL,
    CONF_ID,
    CONF_MAX_VALUE,
    CONF_MIN_VALUE,
    CONF_SENSOR,
)

CODEOWNERS = ["@dala318"]
DEPENDENCIES = ["lora"]

lorawan_ns = cg.esphome_ns.namespace("lorawan")
LoRaWAN = lorawan_ns.class_("LoRaWAN", cg.Component)

MULTI_CONF = False  # Static object pointer in callbacks prevent multiple instances

DOMAIN = "lorawan"

CONF_LORAWAN_ID = "lorawan_id"

CONF_APP_KEY = "app_key"
CONF_DEV_EUI = "dev_eui"
CONF_JOIN_EUI = "join_eui"
CONF_GEN_APP_KEY = "gen_app_key"
CONF_PERIODICAL_UPLINK_DELAY = "periodical_uplink_delay"
CONF_AFTER_JOIN_DELAY = "after_join_delay"

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
            cv.Required(CONF_JOIN_EUI): cv.All(
                cv.ensure_list(cv.hex_uint8_t), cv.Length(min=8, max=8)
            ),
            cv.Optional(CONF_GEN_APP_KEY): cv.All(
                cv.ensure_list(cv.hex_uint8_t), cv.Length(min=16, max=16)
            ),
            cv.Optional(
                CONF_PERIODICAL_UPLINK_DELAY, default="60s"
            ): cv.positive_time_period_seconds,
            cv.Optional(
                CONF_AFTER_JOIN_DELAY, default="60s"
            ): cv.positive_time_period_seconds,
            cv.Optional(CONF_BATTERY_LEVEL): cv.Any(
                cv.use_id(sensor.Sensor),
                cv.Schema(
                    {
                        cv.Required(CONF_SENSOR): cv.use_id(sensor.Sensor),
                        cv.Optional(CONF_MIN_VALUE): cv.float_,
                        cv.Optional(CONF_MAX_VALUE): cv.float_,
                    }
                ),
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(LORA_SCHEMA)
)


async def to_code(config: dict[str, Any]):
    cg.add_library(
        "SWL2001",
        None,
        # "https://github.com/dala318/SWL2001#libraty",  # Add library definition to build from platform.io
        "https://github.com/dala318/SWL2001#transparent",  # Add transparent radio option on top of library definition
        # Below specific commits to force library update since ref to branch not always is recognizing new content
        # "https://github.com/dala318/SWL2001#afb8974a883d4415cd3d7fdd57c146433eb8d2f7",  # Without transparent
        # "https://github.com/dala318/SWL2001#b70d59783f74e4c814cca5edf86209c912e8bd1d",
    )
    cg.add_build_flag("-DTRANSPARENT_RADIO")

    # smtc_modem_core/radio_planner/src/radio_planner_hook_id_defs.h
    cg.add_build_flag("-DNUMBER_OF_STACKS=1")

    # smtc_modem_core/lr1mac/src/smtc_real/src/smtc_real_defs.h
    # "RP_VERSION (LoRaWAN Regional Parameter version) must be defined: RP2_101 or RP2_103"
    cg.add_build_flag("-DRP2_101")

    # Copy of section from main makefil in library
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
    cg.add(var.set_join_eui(config[CONF_JOIN_EUI]))
    if CONF_GEN_APP_KEY in config:
        cg.add(var.set_gen_app_key(config[CONF_GEN_APP_KEY]))
    cg.add(
        var.set_periodicity(
            config[CONF_PERIODICAL_UPLINK_DELAY], config[CONF_AFTER_JOIN_DELAY]
        )
    )
    if battery_config := config.get(CONF_BATTERY_LEVEL):
        if isinstance(battery_config, dict) and CONF_SENSOR in battery_config:
            battery_sensor = await cg.get_variable(battery_config[CONF_SENSOR])
            cg.add(var.set_battery_level_sensor(battery_sensor))
            cg.add(
                var.set_battery_level_min_max(
                    battery_config.get(CONF_MIN_VALUE, 0),
                    battery_config.get(CONF_MAX_VALUE, 100),
                )
            )
        else:
            battery_sensor = await cg.get_variable(battery_config)
            cg.add(var.set_battery_level_sensor(battery_sensor))
            cg.add(var.set_battery_level_min_max(0, 100))
