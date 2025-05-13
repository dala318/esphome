extern "C" {

// #include "lora_basics_modem_version.h"
// #include "smtc_modem_core/lorawan_manager/lorawan_join_management.h"
#include "smtc_modem_api/smtc_modem_utilities.h"
#include "smtc_modem_api/smtc_modem_api.h"
#include "smtc_modem_core/radio_planner/src/radio_planner.h"
#include "smtc_modem_core/smtc_ralf/src/ralf.h"

// void hw_modem_event_handler(void) {
//     // Handle modem events here
//     // This function is called when the modem has an event
//     // You can use it to handle events like join, send, receive, etc.
// }

void init_swl2001() {
  // Use the C library here
  // Try replicating whats in smtc_modem_core/smtc_modem.c smtc_modem_init()

  ralf_t modem_radio = {
      .ral =
          {
              .context = nullptr,
              .driver = nullptr,
          },
      .ralf_drv = nullptr,
  };
  radio_planner_t modem_radio_planner;
  rp_init(&modem_radio_planner, &modem_radio);

  // lorawan_join_add_task(0u);
  // smtc_modem_init( &hw_modem_event_handler );
}

}  // extern "C"