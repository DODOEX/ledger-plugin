#include <string.h>
#include "eth_internals.h"

// `0xeeeee` as a dummy address to represent ETH in Swap.
const uint8_t ETH_ADDRESS[ADDRESS_LENGTH] = {0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee,
                                                      0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee,
                                                      0xee, 0xee, 0xee, 0xee, 0xee, 0xee};

// Used to indicate that the beneficiary should be the sender.
const uint8_t NULL_ETH_ADDRESS[ADDRESS_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// Returns 1 if corresponding address is the 1inch address for the chain token (ETH, BNB, MATIC,
// etc.. are 0xeeeee...).
#define ADDRESS_IS_NETWORK_TOKEN(_addr)                      \
    (!memcmp(_addr, ETH_ADDRESS, ADDRESS_LENGTH) || \
     !memcmp(_addr, NULL_ETH_ADDRESS, ADDRESS_LENGTH))
