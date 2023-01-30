#include "dodo_plugin.h"

// Sets the first screen to display.
void handle_query_contract_id(void *parameters) {
    ethQueryContractID_t *msg = (ethQueryContractID_t *) parameters;
    const context_t *context = (const context_t *) msg->pluginContext;
    // msg->name will be the upper sentence displayed on the screen.
    // msg->version will be the lower sentence displayed on the screen.

    // For the first screen, display the plugin name.
    strlcpy(msg->name, PLUGIN_NAME, msg->nameLength);

    // EDIT THIS: Adapt the cases by modifying the strings you pass to `strlcpy`.
    switch (context->selectorIndex) {
        case SWAP_V2_PROXY_EXTERNAL_SWAP:
        case SWAP_V2_PROXY_DODO_SWAP_V2_TOKEN_TO_TOKEN:
        case SWAP_V2_PROXY_DODO_SWAP_V2_TOKEN_TO_ETH:
        case SWAP_V2_PROXY_DODO_SWAP_V2_ETH_TO_TOKEN:
        case SWAP_DODO_ROUTE_PROXY_MIX_SWAP:
        case SWAP_DODO_ROUTE_PROXY_DODO_MUTLI_SWAP:
        case SWAP_WETH9_WITHDRAW:
        case SWAP_WETH9_DEPOSIT:
            strlcpy(msg->version, "Swap", msg->versionLength);
            msg->result = ETH_PLUGIN_RESULT_OK;
            break;

        default:
            PRINTF("Selector index: %d not supported\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}