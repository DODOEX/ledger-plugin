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
  test('[Nano ' + model.letter + '] Swap Dodo Route Proxy Dodo Mutli Swap', zemu(model, async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, abi);
  const fromToken = '0xdAC17F958D2ee523a2206206994597C13D831ec7';
  const toToken = '0x6B3595068778DD592e39A122f4f5a5cF09C90fE2';
  const fromTokenAmount = parseUnits("1343560512", 'wei');
  const minReturnAmount = parseUnits("28471151959593036279", 'wei');
  const totalWeight = [20];
  const splitNumber = [0, 1];
  const midToken = [
    fromToken,
    fromToken,
    toToken,
    toToken,
  ];
  const assetFrom = [
    '0x6f5d8A4479855bF0222EE8f7C1DFFfdD113f7Ca7',
    '0x1Dc662D3D7De14a57CD369e3a9E774f8F80d4214'
  ];
  const sequence = [
    '0x000000000000000000000000ba12222222228d8ba445958a75a0704d566bf2c80000000000000000000000006f5d8a4479855bf0222ee8f7c1dfffdd113f7ca7000000000000000000000000000000000000000000000000000000000002280100000000000000000000000000000000000000000000000000000000000000800000000000000000000000000000000000000000000000000000000000000060a6f548df93de924d73be7d25dc02554c6bd66db500020000000000000000000e000000000000000000000000c02aaa39b223fe8d0a0e5c4f27ead9083c756cc20000000000000000000000002260fac5e5542a773aa44fbcfedf7c193bc2c599'
  ];
  const deadLine = Number(1657096387);

  const {data} = await contract.populateTransaction.dodoMutliSwap(fromTokenAmount, minReturnAmount, totalWeight, splitNumber, midToken, assetFrom, sequence, deadLine);

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
  await sim.navigateAndCompareSnapshots('.', model.name + '_swap_dodo_route_proxy_dodo_mutli_swap', [right_clicks, 0]);

  await tx;
  }));
});

