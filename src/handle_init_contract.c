#include "dodo_plugin.h"

static int find_selector(uint32_t selector, const uint32_t *selectors, size_t n, selector_t *out) {
    for (selector_t i = 0; i < n; i++) {
        if (selector == selectors[i]) {
            *out = i;
            return 0;
        }
    }
    return -1;
}

// Called once to init.
void handle_init_contract(void *parameters) {
    // Cast the msg to the type of structure we expect (here, ethPluginInitContract_t).
    ethPluginInitContract_t *msg = (ethPluginInitContract_t *) parameters;

    // Make sure we are running a compatible version.
    if (msg->interfaceVersion != ETH_PLUGIN_INTERFACE_VERSION_LATEST) {
        // If not the case, return the `UNAVAILABLE` status.
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }

    // Double check that the `context_t` struct is not bigger than the maximum size (defined by
    // `msg->pluginContextLength`).
    if (msg->pluginContextLength < sizeof(context_t)) {
        PRINTF("Plugin parameters structure is bigger than allowed size\n");
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    context_t *context = (context_t *) msg->pluginContext;

    // Initialize the context (to 0).
    memset(context, 0, sizeof(*context));

    uint32_t selector = U4BE(msg->selector, 0);
    if (find_selector(selector, DODO_SELECTORS, NUM_SELECTORS, &context->selectorIndex)) {
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }

    // Set `next_param` to be the first field we expect to parse.
    switch (context->selectorIndex) {
        case SWAP_V2_PROXY_EXTERNAL_SWAP:
        case SWAP_V2_PROXY_DODO_SWAP_V2_TOKEN_TO_TOKEN:
        case SWAP_V2_PROXY_DODO_SWAP_V2_TOKEN_TO_ETH:
        case SWAP_DODO_ROUTE_PROXY_MIX_SWAP:
            context->next_param = FROM_TOKEN;
            break;
        case SWAP_DODO_ROUTE_PROXY_DODO_MUTLI_SWAP:
        case SWAP_WETH9_WITHDRAW:
        case SWAP_WETH9_DEPOSIT:
            context->next_param = FROM_TOKEN_AMOUNT;
            break;
        case SWAP_V2_PROXY_DODO_SWAP_V2_ETH_TO_TOKEN:
            context->next_param = TO_TOKEN;
            break;
        default:
            PRINTF("Missing selectorIndex: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    // Return valid status.
    msg->result = ETH_PLUGIN_RESULT_OK;
}
