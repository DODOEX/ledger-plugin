import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx, nano_models } from './test.fixture';
import { ethers } from "ethers";
import { parseEther, parseUnits} from "ethers/lib/utils";

const contractAddr = "0xa2398842f37465f89540430bdc00219fa9e4d28a";
const pluginName = "swap";
const abi_path = `../${pluginName}/abis/` + contractAddr + '.json';
const abi = require(abi_path);

nano_models.forEach(function(model) {
  test('[Nano ' + model.letter + '] Swap Dodo Route Proxy Mix Swap', zemu(model, async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, abi);
  const fromToken = '0xdAC17F958D2ee523a2206206994597C13D831ec7';
  const toToken = '0x6B3595068778DD592e39A122f4f5a5cF09C90fE2';
  const fromTokenAmount = parseUnits("1343560512", 'wei');
  const minReturnAmount = parseUnits("28471151959593036279", 'wei');
  const mixAdapters = [
    '0xAC716E87b0853C0712674e8E3a8435a489F276b4',
    '0xAC716E87b0853C0712674e8E3a8435a489F276b4',
    '0xAC716E87b0853C0712674e8E3a8435a489F276b4'
  ];
  const mixPairs = [
    '0x857b8daf090B1b10BDA546C4b4fe163223Dc6694',
    '0x8ad599c3A0ff1De082011EFDDc58f1908eb6e6D8',
    '0x813b22032E94667CC0F854Cc7241FC5A309c45bc'
  ];
  const assetTo = [
    '0xAC716E87b0853C0712674e8E3a8435a489F276b4',
    '0xAC716E87b0853C0712674e8E3a8435a489F276b4',
    '0xAC716E87b0853C0712674e8E3a8435a489F276b4',
    '0x2d491A089FA87393480185312F01ec775d8bCAB8',
  ]
  const directions = "3";
  const moreInfos = [
    '0x000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000060000000000000000000000000e4b5936dce1820f84509c89cce0f28c87988bad8000000000000000000000000c02aaa39b223fe8d0a0e5c4f27ead9083c756cc20000000000000000000000000000000000000000000000000000000000002710',
    '0x000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000060000000000000000000000000c02aaa39b223fe8d0a0e5c4f27ead9083c756cc2000000000000000000000000a0b86991c6218b36c1d19d4a2e9eb0ce3606eb480000000000000000000000000000000000000000000000000000000000000bb8',
    '0x000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000060000000000000000000000000a0b86991c6218b36c1d19d4a2e9eb0ce3606eb48000000000000000000000000f4d2888d29d722226fafa5d9b24f9164c092421e0000000000000000000000000000000000000000000000000000000000000bb8'
  ]
  const deadLine = Number(1657096387);

  const {data} = await contract.populateTransaction.mixSwap(fromToken, toToken, fromTokenAmount, minReturnAmount, mixAdapters, mixPairs, assetTo, directions, moreInfos, deadLine);

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
  await sim.navigateAndCompareSnapshots('.', model.name + '_swap_dodo_route_proxy_mix_swap', [right_clicks, 0]);

  await tx;
  }));
});

