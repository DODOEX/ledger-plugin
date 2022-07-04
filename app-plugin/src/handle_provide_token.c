#include "boilerplate_plugin.h"

// EDIT THIS: Adapt this function to your needs! Remember, the information for tokens are held in
// `msg->token1` and `msg->token2`. If those pointers are `NULL`, this means the ethereum app didn't
// find any info regarding the requested tokens!
void handle_provide_token(void *parameters) {
    ethPluginProvideInfo_t *msg = (ethPluginProvideInfo_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;
    /* pay */
    if (msg->item1) {
        // The Ethereum App found the information for the requested token!
        // Store its decimals.
        context->decimals_pay = msg->item1->token.decimals;
        // Store its ticker.
        strlcpy(context->ticker_pay, (char *) msg->item1->token.ticker, sizeof(context->ticker_pay));

        // Keep track that we found the token.
        context->token_found_pay = true;
    } else {
        // The Ethereum App did not manage to find the info for the requested token.
        context->token_found_pay = false;

        // If we wanted to add a screen, say a warning screen for example, we could instruct the
        // ethereum app to add an additional screen by setting `msg->additionalScreens` here, just
        // like so:
        // msg->additionalScreens = 1;
    }
    /* received */
    if (msg->item2) {
        // The Ethereum App found the information for the requested token!
        // Store its decimals.
        context->decimals_received = msg->item2->token.decimals;
        // Store its ticker.
        strlcpy(context->ticker_received, (char *) msg->item2->token.ticker, sizeof(context->ticker_received));

        // Keep track that we found the token.
        context->token_found_received = true;
    } else {
        // The Ethereum App did not manage to find the info for the requested token.
        context->token_found_received = false;

        // If we wanted to add a screen, say a warning screen for example, we could instruct the
        // ethereum app to add an additional screen by setting `msg->additionalScreens` here, just
        // like so:
        // msg->additionalScreens = 1;
    }
   msg->result = ETH_PLUGIN_RESULT_OK;
}
