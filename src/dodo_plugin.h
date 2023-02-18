#pragma once

#include "eth_internals.h"
#include "eth_plugin_interface.h"
#include <string.h>

// Number of selectors defined in this plugin. Should match the enum `selector_t`.
// EDIT THIS: Put in the number of selectors your plugin is going to support.
#define NUM_SELECTORS 8

// Name of the plugin.
// EDIT THIS: Replace with your plugin name.
#define PLUGIN_NAME "DODO"

// Enumeration of the different selectors possible.
// Should follow the exact same order as the array declared in main.c
// EDIT THIS: Change the naming (`selector_t`), and add your selector names.
typedef enum {
    SWAP_V2_PROXY_EXTERNAL_SWAP = 0,
    SWAP_V2_PROXY_DODO_SWAP_V2_TOKEN_TO_TOKEN,
    SWAP_V2_PROXY_DODO_SWAP_V2_TOKEN_TO_ETH,
    SWAP_V2_PROXY_DODO_SWAP_V2_ETH_TO_TOKEN,
    SWAP_DODO_ROUTE_PROXY_MIX_SWAP,
    SWAP_DODO_ROUTE_PROXY_DODO_MUTLI_SWAP,
    SWAP_WETH9_WITHDRAW,
    SWAP_WETH9_DEPOSIT,
} selector_t;

// Enumeration used to parse the smart contract data.
// EDIT THIS: Adapt the parameter names here.
typedef enum {
    FROM_TOKEN = 0,
    TO_TOKEN,
    APPROVE_TARGET,
    SWAP_TARGET,
    FROM_TOKEN_AMOUNT,
    MIN_RETURN_AMOUNT,
    CALL_DATA_CONCAT,
    IS_INCENTIVE,
    MID_TOKEN_OFFSET,
    MID_TOKEN_LENGTH,
    UNEXPECTED_PARAMETER,
    NONE,
} parameter;

extern const uint32_t DODO_SELECTORS[NUM_SELECTORS];

// `0xeeeee` as a dummy address to represent ETH in Swap.
extern const uint8_t ETH_ADDRESS[ADDRESS_LENGTH];

// Used to indicate that the beneficiary should be the sender.
extern const uint8_t NULL_ETH_ADDRESS[ADDRESS_LENGTH];

#define ADDRESS_IS_NETWORK_TOKEN(_addr)             \
    (!memcmp(_addr, ETH_ADDRESS, ADDRESS_LENGTH) || \
     !memcmp(_addr, NULL_ETH_ADDRESS, ADDRESS_LENGTH))

// Shared global memory with Ethereum app. Must be at most 5 * 32 bytes.
// EDIT THIS: This struct is used by your plugin to save the parameters you parse. You
// will need to adapt this struct to your plugin.
typedef struct context_t {
    // For display.
    uint8_t amount_pay[INT256_LENGTH];
    uint8_t amount_received[INT256_LENGTH];
    uint8_t token_received[ADDRESS_LENGTH];
    uint8_t token_pay[ADDRESS_LENGTH];
    char ticker_received[MAX_TICKER_LEN];
    uint8_t decimals_received;
    uint8_t token_found_received;
    char ticker_pay[MAX_TICKER_LEN];
    uint8_t decimals_pay;
    uint8_t token_found_pay;
    bool eth_amount_pay;  // true if the amount is in ETH

    // For parsing data.
    uint8_t next_param;  // Set to be the next param we expect to parse.
    uint16_t offset;     // Offset at which the array or struct starts.
    bool go_to_offset;   // If set, will force the parsing to iterate through parameters until
                         // `offset` is reached.
    bool valid;

    // For both parsing and display.
    selector_t selectorIndex;
} context_t;

// Piece of code that will check that the above structure is not bigger than 5 * 32. Do not remove
// this check.
_Static_assert(sizeof(context_t) <= 5 * 32, "Structure of parameters too big.");

void handle_provide_parameter(void *parameters);
void handle_query_contract_ui(void *parameters);
void handle_init_contract(void *parameters);
void handle_finalize(void *parameters);
void handle_provide_token(void *parameters);
void handle_query_contract_id(void *parameters);
