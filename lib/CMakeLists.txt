# SPDX-License-Identifier: Apache-2.0


# TODO: This is not ideal. We'd prefer to have a Kconfig and a CMakeLists.txt in Zenoh-Pico. That
# way integrates better with the Zephyr library scheme. But that'll do for now.
if(CONFIG_ZENOH_PICO_LIB)

add_definitions(-DWITH_ZEPHYR=1)
add_definitions(-DZENOH_ZEPHYR=1)

file(GLOB sources
  "zenoh-pico/src/*.c"
  "zenoh-pico/src/api/*.c"
  "zenoh-pico/src/net/*.c"
  "zenoh-pico/src/collections/*.c"
  "zenoh-pico/src/link/*.c"
  "zenoh-pico/src/link/config/*.c"
  "zenoh-pico/src/link/unicast/*.c"
  "zenoh-pico/src/link/multicast/*.c"
  "zenoh-pico/src/protocol/*.c"
  "zenoh-pico/src/protocol/keyexpr/*.c"
  "zenoh-pico/src/session/*.c"
  "zenoh-pico/src/system/*.c"
  "zenoh-pico/src/system/zephyr/*.c"
  "zenoh-pico/src/transport/*.c"
  "zenoh-pico/src/transport/common/*.c"
  "zenoh-pico/src/transport/unicast/*.c"
  "zenoh-pico/src/transport/unicast/link/*.c"
  "zenoh-pico/src/transport/unicast/link/task/*.c"
  "zenoh-pico/src/transport/multicast/*.c"
  "zenoh-pico/src/transport/multicast/link/*.c"
  "zenoh-pico/src/transport/multicast/link/task/*.c"
  "zenoh-pico/src/utils/*.c"
)

zephyr_library_named(zenoh-pico)

zephyr_library_sources(${sources})

zephyr_library_include_directories(zenoh-pico/include)

endif()