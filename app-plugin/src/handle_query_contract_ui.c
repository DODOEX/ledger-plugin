#include "boilerplate_plugin.h"

// EDIT THIS: You need to adapt / remove the static functions (set_send_ui, set_receive_ui ...) to
// match what you wish to display.

// Set UI for the "Send" screen.
// EDIT THIS: Adapt / remove this function to your needs.
static void set_send_ui(ethQueryContractUI_t *msg, const context_t *context) {
    strlcpy(msg->title, "Send", msg->titleLength);

    uint8_t decimals = context->decimals_pay;
    const char *ticker = context->ticker_pay;
    const uint8_t *amount = context->amount_pay;
    uint8_t amount_size = sizeof(context->amount_pay); 

    // If the token look up failed, use the default network ticker along with the default decimals.
    if (!context->token_found_pay) {
        decimals = WEI_TO_ETHER;
        ticker = msg->network_ticker;
    }
    if (context->eth_amount_pay) {
        amount = msg->pluginSharedRO->txContent->value.value;
        amount_size = msg->pluginSharedRO->txContent->value.length;
    }

    amountToString(amount, amount_size, decimals, ticker, msg->msg, msg->msgLength);
}

// Set UI for "Receive" screen.
// EDIT THIS: Adapt / remove this function to your needs.
static void set_receive_ui(ethQueryContractUI_t *msg, const context_t *context) {
    strlcpy(msg->title, "Receive Min.", msg->titleLength);

    uint8_t decimals = context->decimals_received;
    const char *ticker = context->ticker_received;

    // If the token look up failed, use the default network ticker along with the default decimals.
    if (!context->token_found_received) {
        decimals = WEI_TO_ETHER;
        ticker = msg->network_ticker;
    }

    amountToString(context->amount_received,
                   sizeof(context->amount_received),
                   decimals,
                   ticker,
                   msg->msg,
                   msg->msgLength);
}

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    // msg->title is the upper line displayed on the device.
    // msg->msg is the lower line displayed on the device.

    // Clean the display fields.
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

    msg->result = ETH_PLUGIN_RESULT_OK;

    // EDIT THIS: Adapt the cases for the screens you'd like to display.
    switch (msg->screenIndex) {
        case 0:
            set_send_ui(msg, context);
            break;
        case 1:
            set_receive_ui(msg, context);
            break;
        // Keep this
        default:
            PRINTF("Received an invalid screenIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}
