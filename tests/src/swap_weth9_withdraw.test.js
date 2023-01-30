import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx, nano_models } from "./test.fixture";
import { ethers } from "ethers";
import { parseEther, parseUnits } from "ethers/lib/utils";

const contractAddr = "0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2";
const pluginName = "swap";
const abi_path = `../${pluginName}/abis/` + contractAddr + ".json";
const abi = require(abi_path);

nano_models.forEach(function (model) {
  test(
    "[Nano " + model.letter + "] Swap weth9 withdraw",
    zemu(model, async (sim, eth) => {
      const contract = new ethers.Contract(contractAddr, abi);

      const fromTokenAmount = parseUnits("5000000000000000", "wei");

      // EDIT THIS: adapt the signature to your method
      // signature: swapExactETHForTokens(uint amountOutMin, address[] calldata path, address to, uint deadline)
      // EDIT THIS: don't call `swapExactETHForTokens` but your own method and adapt the arguments.
      const { data } = await contract.populateTransaction.withdraw(
        fromTokenAmount
      );

      // Get the generic transaction template
      let unsignedTx = genericTx;
      // Modify `to` to make it interact with the contract
      unsignedTx.to = contractAddr;
      // Modify the attached data
      unsignedTx.data = data;
      // EDIT THIS: get rid of this if you don't wish to modify the `value` field.
      // Modify the number of ETH sent
      unsignedTx.value = parseEther("0");

      // Create serializedTx and remove the "0x" prefix
      const serializedTx = ethers.utils
        .serializeTransaction(unsignedTx)
        .slice(2);

      const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

      const right_clicks = 5;

      // Wait for the application to actually load and parse the transaction
      await waitForAppScreen(sim);
      // Navigate the display by pressing the right button 10 times, then pressing both buttons to accept the transaction.
      // EDIT THIS: modify `10` to fix the number of screens you are expecting to navigate through.
      await sim.navigateAndCompareSnapshots(
        ".",
        model.name + "_swap_weth9_withdraw",
        [right_clicks, 0]
      );

      await tx;
    })
  );
});
