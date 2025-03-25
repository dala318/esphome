#pragma once

#include "../sx127x.h"
#include "esphome/core/component.h"
#include "esphome/components/packet_transport/packet_transport.h"
// #include "esphome/components/network/ip_address.h"
// #if defined(USE_SOCKET_IMPL_BSD_SOCKETS) || defined(USE_SOCKET_IMPL_LWIP_SOCKETS)
// #include "esphome/components/socket/socket.h"
// #endif
// #ifdef USE_SOCKET_IMPL_LWIP_TCP
// #include <WiFiUdp.h>
// #endif
#include <vector>

namespace esphome {
namespace sx127x {

class SX127xTransport : public packet_transport::PacketTransport, public Parented<SX127x> {
 public:
  void setup() override;
  void update() override;

  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }

 protected:
  void send_packet(std::vector<uint8_t> &buf) const override;
  bool should_send() override;
  size_t get_max_packet_size() override { return 256; }  // ToDo: Set relevant value
};

}  // namespace sx127x
}  // namespace esphome