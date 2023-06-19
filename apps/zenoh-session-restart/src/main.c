//
// Copyright (c) 2022 ZettaScale Technology
//
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// http://www.eclipse.org/legal/epl-2.0, or the Apache License, Version 2.0
// which is available at https://www.apache.org/licenses/LICENSE-2.0.
//
// SPDX-License-Identifier: EPL-2.0 OR Apache-2.0
//
// Contributors:
//   ZettaScale Zenoh Team, <zenoh@zettascale.tech>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <zenoh-pico.h>

#define CLIENT_OR_PEER 0  // 0: Client mode; 1: Peer mode
#if CLIENT_OR_PEER == 0
#define MODE "client"
#define PEER ""  // If empty, it will scout
#elif CLIENT_OR_PEER == 1
#define MODE "peer"
#define PEER "udp/224.0.0.225:7447#iface=en0"
#else
#error "Unknown Zenoh operation mode. Check CLIENT_OR_PEER value."
#endif

#define KEYEXPR "demo/example/zenoh-pico-pub"
#define VALUE "[STSTM32]{nucleo-F767ZI} Pub from Zenoh-Pico!"


#include <zephyr/sys/sys_heap.h>
extern struct sys_heap _system_heap;

void print_sys_heap()
{
    int err;
    struct sys_memory_stats stats;

    err = sys_heap_runtime_stats_get(&_system_heap, &stats);
    if (err) {
        printf("Failed to read kernel system heap statistics (err %d)\n", err);
    }

    printf("free:           %zu\t", stats.free_bytes);
    printf("allocated:      %zu\t", stats.allocated_bytes);
    printf("max. allocated: %zu\n", stats.max_allocated_bytes);
}

int main(int argc, char **argv) {

    // print_sys_heap();
    // const int array_size = 700;
    // int* ptr_array[array_size];
    // for (int i = 0; i < array_size; i++)
    // {
    //     ptr_array[i] = (int*)k_malloc(sizeof(int));
    // }
    // //print_sys_heap();

    // for (int i = 0; i < array_size; i++)
    // {
    //     k_free(ptr_array[i]);
    //     ptr_array[i] = NULL;
    // }

    // sys_heap_print_info(&_system_heap, false);

    while (true) {
        sleep(5);

        //print_sys_heap();
        // sys_heap_print_info(&_system_heap, false);

        // Initialize Zenoh Session and other parameters
        z_owned_config_t config = z_config_default();
        zp_config_insert(z_loan(config), Z_CONFIG_MODE_KEY, z_string_make(MODE));
        if (strcmp(PEER, "") != 0) {
            zp_config_insert(z_loan(config), Z_CONFIG_PEER_KEY, z_string_make(PEER));
        }

        // Open Zenoh session
        printf("Opening Zenoh Session...");
        z_owned_session_t s = z_open(z_move(config));
        if (!z_check(s)) {
            printf("Unable to open session!\n");
            exit(-1);
        }
        printf("OK\n");
        //print_sys_heap();
        // sys_heap_print_info(&_system_heap, false);
        bool session_is_valid = true;

        printf("Starting tasks...");
        // Start the receive and the session lease loop for zenoh-pico
        zp_start_read_task(z_loan(s), NULL);
        zp_start_lease_task(z_loan(s), NULL);
        printf("OK\n");

        //print_sys_heap();
        // sys_heap_print_info(&_system_heap, false);

        printf("Declaring publisher for '%s'...", KEYEXPR);
        z_owned_publisher_t pub = z_declare_publisher(z_loan(s), z_keyexpr(KEYEXPR), NULL);
        if (!z_check(pub)) {
            printf("Unable to declare publisher for key expression!\n");
            exit(-1);
        }
        printf("OK\n");

        char buf[256];
        for (int idx = 0; session_is_valid; ++idx) {
            sleep(1);
            sprintf(buf, "[%4d] %s", idx, VALUE);
            printf("Putting Data ('%s': '%s')...\n", KEYEXPR, buf);
            int put_res = z_publisher_put(z_loan(pub), (const uint8_t *)buf, strlen(buf), NULL);
            if (put_res != _Z_RES_OK) {
                printf("Error publishing message (%d)\n", put_res);
                session_is_valid = false;
            }
            if(!z_check(s)) {
                printf("Zenoh session has become invalid\n");
            }
        }

        z_undeclare_publisher(z_move(pub));

        printf("Stopping tasks...");
        // Stop the receive and the session lease loop for zenoh-pico
        zp_stop_read_task(z_loan(s));
        zp_stop_lease_task(z_loan(s));
        printf("OK\n");

        //print_sys_heap();
        // sys_heap_print_info(&_system_heap, false);

        printf("Closing Zenoh Session...");
        z_close(z_move(s));
        printf("OK!\n");

        //print_sys_heap();
        sys_heap_print_info(&_system_heap, false);

    }

    return 0;
}
