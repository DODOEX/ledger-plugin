#include "dodo_plugin.h"

// List of selectors supported by this plugin.
static const uint32_t SWAP_V2_PROXY_EXTERNAL_SWAP_SELECTOR = 0x54bacd13;
static const uint32_t SWAP_V2_PROXY_DODO_SWAP_V2_TOKEN_TO_TOKEN_SELECTOR = 0xf87dc1b7;
static const uint32_t SWAP_V2_PROXY_DODO_SWAP_V2_TOKEN_TO_ETH_SELECTOR = 0x1e6d24c2;
static const uint32_t SWAP_V2_PROXY_DODO_SWAP_V2_ETH_TO_TOKEN_SELECTOR = 0x5028bb95;
static const uint32_t SWAP_DODO_ROUTE_PROXY_MIX_SWAP_SELECTOR = 0x7617b389;
static const uint32_t SWAP_DODO_ROUTE_PROXY_DODO_MUTLI_SWAP_SELECTOR = 0x81791788;
static const uint32_t SWAP_WETH9_WITHDRAW_SELECTOR = 0x2e1a7d4d;
static const uint32_t SWAP_WETH9_DEPOSIT_SELECTOR = 0xd0e30db0;

// Array of all the different boilerplate selectors. Make sure this follows the same order as the
// enum defined in `dodo_plugin.h`
const uint32_t DODO_SELECTORS[NUM_SELECTORS] = {
    SWAP_V2_PROXY_EXTERNAL_SWAP_SELECTOR,
    SWAP_V2_PROXY_DODO_SWAP_V2_TOKEN_TO_TOKEN_SELECTOR,
    SWAP_V2_PROXY_DODO_SWAP_V2_TOKEN_TO_ETH_SELECTOR,
    SWAP_V2_PROXY_DODO_SWAP_V2_ETH_TO_TOKEN_SELECTOR,
    SWAP_DODO_ROUTE_PROXY_MIX_SWAP_SELECTOR,
    SWAP_DODO_ROUTE_PROXY_DODO_MUTLI_SWAP_SELECTOR,
    SWAP_WETH9_WITHDRAW_SELECTOR,
    SWAP_WETH9_DEPOSIT_SELECTOR,
};

const uint8_t ETH_ADDRESS[ADDRESS_LENGTH] = {0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee,
                                             0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee,
                                             0xee, 0xee, 0xee, 0xee, 0xee, 0xee};

// Used to indicate that the beneficiary should be the sender.
const uint8_t NULL_ETH_ADDRESS[ADDRESS_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00};