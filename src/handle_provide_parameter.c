#include "dodo_plugin.h"

static void handle_swap_v2_proxy_external_swap(ethPluginProvideParameter_t *msg,
                                               context_t *context) {
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
            copy_parameter(context->amount_pay, msg->parameter, sizeof(context->amount_pay));
            context->next_param = MIN_RETURN_AMOUNT;
            break;
        case MIN_RETURN_AMOUNT:
            copy_parameter(context->amount_received,
                           msg->parameter,
                           sizeof(context->amount_received));
            context->next_param = NONE;
            break;
        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_swap_v2_proxy_dodo_swap_v2_token_to_token(ethPluginProvideParameter_t *msg,
                                                             context_t *context) {
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
            copy_parameter(context->amount_pay, msg->parameter, sizeof(context->amount_pay));
            context->next_param = MIN_RETURN_AMOUNT;
            break;
        case MIN_RETURN_AMOUNT:
            copy_parameter(context->amount_received,
                           msg->parameter,
                           sizeof(context->amount_received));
            context->next_param = NONE;
            break;
        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_swap_v2_proxy_dodo_swap_v2_token_to_eth(ethPluginProvideParameter_t *msg,
                                                           context_t *context) {
    switch (context->next_param) {
        case FROM_TOKEN:
            copy_address(context->token_pay, msg->parameter, sizeof(context->token_pay));
            context->next_param = FROM_TOKEN_AMOUNT;
            break;
        case FROM_TOKEN_AMOUNT:
            copy_parameter(context->amount_pay, msg->parameter, sizeof(context->amount_pay));
            context->next_param = MIN_RETURN_AMOUNT;
            break;
        case MIN_RETURN_AMOUNT:
            copy_parameter(context->amount_received,
                           msg->parameter,
                           sizeof(context->amount_received));
            context->next_param = NONE;
            break;
        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_swap_v2_proxy_dodo_swap_v2_eth_to_token(ethPluginProvideParameter_t *msg,
                                                           context_t *context) {
    switch (context->next_param) {
        case TO_TOKEN:
            context->eth_amount_pay = true;
            copy_address(context->token_received, msg->parameter, sizeof(context->token_received));
            context->next_param = MIN_RETURN_AMOUNT;
            break;
        case MIN_RETURN_AMOUNT:
            copy_parameter(context->amount_received,
                           msg->parameter,
                           sizeof(context->amount_received));
            context->next_param = NONE;
            break;
        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_swap_dodo_route_proxy_mix_swap(ethPluginProvideParameter_t *msg,
                                                  context_t *context) {
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
            copy_parameter(context->amount_pay, msg->parameter, sizeof(context->amount_pay));
            context->next_param = MIN_RETURN_AMOUNT;
            break;
        case MIN_RETURN_AMOUNT:
            copy_parameter(context->amount_received,
                           msg->parameter,
                           sizeof(context->amount_received));
            context->next_param = NONE;
            break;
        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_swap_dodo_route_proxy_dodo_mutli_swap(ethPluginProvideParameter_t *msg,
                                                         context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }
    switch (context->next_param) {
        case FROM_TOKEN_AMOUNT:
            copy_parameter(context->amount_pay, msg->parameter, sizeof(context->amount_pay));
            context->next_param = MIN_RETURN_AMOUNT;
            break;
        case MIN_RETURN_AMOUNT:
            copy_parameter(context->amount_received,
                           msg->parameter,
                           sizeof(context->amount_received));
            context->next_param = PATH_OFFSET;
            context->offset = msg->parameterOffset - SELECTOR_SIZE + PARAMETER_LENGTH * 3;
            context->go_to_offset = true;
            break;
        case PATH_OFFSET:
            context->offset = U2BE(msg->parameter, PARAMETER_LENGTH - 2);
            context->next_param = PATH_LENGTH;
            context->go_to_offset = true;
            break;
        case PATH_LENGTH:
            context->next_param = FROM_TOKEN;
            break;
        case FROM_TOKEN:
            copy_address(context->token_pay, msg->parameter, sizeof(context->token_pay));
            context->offset = msg->parameterOffset - SELECTOR_SIZE + PARAMETER_LENGTH * 3;
            context->next_param = TO_TOKEN;
            context->go_to_offset = true;
            break;
        case TO_TOKEN:
            copy_address(context->token_received, msg->parameter, sizeof(context->token_received));
            context->next_param = NONE;
            break;
        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
static void handle_swap_weth9_withdraw(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case FROM_TOKEN_AMOUNT:
            copy_parameter(context->amount_pay, msg->parameter, sizeof(context->amount_pay));
            copy_parameter(context->amount_received,
                           msg->parameter,
                           sizeof(context->amount_received));
            break;

        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
static void handle_swap_weth9_deposit(ethPluginProvideParameter_t *msg, context_t *context) {
    msg->result = ETH_PLUGIN_RESULT_ERROR;
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

    if (context->next_param == NONE) {
        return;
    }

    // EDIT THIS: adapt the cases and the names of the functions.
    switch (context->selectorIndex) {
        case SWAP_V2_PROXY_EXTERNAL_SWAP:
            handle_swap_v2_proxy_external_swap(msg, context);
            break;
        case SWAP_V2_PROXY_DODO_SWAP_V2_TOKEN_TO_TOKEN:
            handle_swap_v2_proxy_dodo_swap_v2_token_to_token(msg, context);
            break;
        case SWAP_V2_PROXY_DODO_SWAP_V2_TOKEN_TO_ETH:
            handle_swap_v2_proxy_dodo_swap_v2_token_to_eth(msg, context);
            break;
        case SWAP_V2_PROXY_DODO_SWAP_V2_ETH_TO_TOKEN:
            handle_swap_v2_proxy_dodo_swap_v2_eth_to_token(msg, context);
            break;
        case SWAP_DODO_ROUTE_PROXY_MIX_SWAP:
            handle_swap_dodo_route_proxy_mix_swap(msg, context);
            break;
        case SWAP_DODO_ROUTE_PROXY_DODO_MUTLI_SWAP:
            handle_swap_dodo_route_proxy_dodo_mutli_swap(msg, context);
            break;
        case SWAP_WETH9_WITHDRAW:
            handle_swap_weth9_withdraw(msg, context);
            break;
        case SWAP_WETH9_DEPOSIT:
            handle_swap_weth9_deposit(msg, context);
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
