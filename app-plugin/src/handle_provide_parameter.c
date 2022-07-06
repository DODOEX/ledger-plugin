#include "boilerplate_plugin.h"

// EDIT THIS: Remove this function and write your own handlers!
static void handle_swap_v2_proxy_external_swap(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
    //    if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
           return;
    //    }
    //    context->go_to_offset = false;
    }
    switch (context->next_param) {
        case FROM_TOKEN:
            copy_address(context->token_pay, msg->parameter, sizeof(context->token_pay));
            context->next_param = TO_TOKEN;
            break;
        case TO_TOKEN:
            copy_address(context->token_received, msg->parameter, sizeof(context->token_received));
            context->next_param = APPROVE_TARGET;
            break;
        case APPROVE_TARGET:
            context->next_param = SWAP_TARGET;
            break;
        case SWAP_TARGET:
            context->next_param = FROM_TOKEN_AMOUNT;
            break;
        case FROM_TOKEN_AMOUNT:
            copy_parameter(context->amount_pay,
                            msg->parameter,
                            sizeof(context->amount_pay));
            context->next_param = MIN_RETURN_AMOUNT;
            break;
        case MIN_RETURN_AMOUNT:
            copy_parameter(context->amount_received,
                            msg->parameter,
                            sizeof(context->amount_received));
            context->go_to_offset = true;
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_swap_v2_proxy_dodo_swap_v2_token_to_token(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        return;
    }
    switch (context->next_param) {
        case FROM_TOKEN:
            copy_address(context->token_pay, msg->parameter, sizeof(context->token_pay));
            context->next_param = TO_TOKEN;
            break;
        case TO_TOKEN:
            copy_address(context->token_received, msg->parameter, sizeof(context->token_received));
            context->next_param = FROM_TOKEN_AMOUNT;
            break;
        case FROM_TOKEN_AMOUNT:
            copy_parameter(context->amount_pay,
                            msg->parameter,
                            sizeof(context->amount_pay));
            context->next_param = MIN_RETURN_AMOUNT;
            break;
        case MIN_RETURN_AMOUNT:
            copy_parameter(context->amount_received,
                            msg->parameter,
                            sizeof(context->amount_received));
            context->go_to_offset = true;
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

void handle_provide_parameter(void *parameters) {
    ethPluginProvideParameter_t *msg = (ethPluginProvideParameter_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;
    // We use `%.*H`: it's a utility function to print bytes. You first give
    // the number of bytes you wish to print (in this case, `PARAMETER_LENGTH`) and then
    // the address (here `msg->parameter`).
    PRINTF("plugin provide parameter: offset %d\nBytes: %.*H\n",
           msg->parameterOffset,
           PARAMETER_LENGTH,
           msg->parameter);

    msg->result = ETH_PLUGIN_RESULT_OK;

    // EDIT THIS: adapt the cases and the names of the functions.
    switch (context->selectorIndex) {
        case SWAP_V2_PROXY_EXTERNAL_SWAP:
            handle_swap_v2_proxy_external_swap(msg, context);
            break;
        case SWAP_V2_PROXY_DODO_SWAP_V2_TOKEN_TO_TOKEN:
            handle_swap_v2_proxy_dodo_swap_v2_token_to_token(msg, context);
            break;
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
