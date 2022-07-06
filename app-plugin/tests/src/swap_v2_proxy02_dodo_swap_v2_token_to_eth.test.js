import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx, nano_models } from './test.fixture';
import { ethers } from "ethers";
import { parseEther, parseUnits} from "ethers/lib/utils";

const contractAddr = "0xa356867fdcea8e71aeaf87805808803806231fdc";
const pluginName = "swap";
const abi_path = `../${pluginName}/abis/` + contractAddr + '.json';
const abi = require(abi_path);

nano_models.forEach(function(model) {
  test('[Nano ' + model.letter + '] Swap V2 Proxy02 Dodo Swap V2 Token To ETH', zemu(model, async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, abi);

  const fromToken = '0xdAC17F958D2ee523a2206206994597C13D831ec7';
  const fromTokenAmount = parseUnits("1343560512", 'wei');
  const minReturnAmount = parseUnits("28471151959593036279", 'wei');
  const directions = "1";
  const isIncentive = false;
  const deadLine = Number(1656942502);
  const dodoPairs = ['0x3058EF90929cb8180174D74C507176ccA6835D73'];

  const {data} = await contract.populateTransaction.dodoSwapV2TokenToETH(fromToken, fromTokenAmount, minReturnAmount, dodoPairs, directions, isIncentive, deadLine);

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
  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);

  const tx = eth.signTransaction(
    "44'/60'/0'/0",
    serializedTx
  );

  const right_clicks = model.letter === 'S' ? 7 : 5;

  // Wait for the application to actually load and parse the transaction
  await waitForAppScreen(sim);
  // Navigate the display by pressing the right button 10 times, then pressing both buttons to accept the transaction.
  // EDIT THIS: modify `10` to fix the number of screens you are expecting to navigate through.
  await sim.navigateAndCompareSnapshots('.', model.name + '_swap_v2_proxy02_dodo_swap_v2_token_to_eth', [right_clicks, 0]);

  await tx;
  }));
});

