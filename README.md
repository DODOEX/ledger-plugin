# Ledger DODO Plugin

This is a plugin for the Ethereum application which helps parsing and displaying relevant information when signing a DODO transaction.

## Prerequisite

Clone submodules

```Bash
git submodule update --init --recursive
```

## Documentation

Need more information about the interface, the architecture, or general stuff about ethereum plugins? You can find more about them in the [ethereum-app documentation](https://github.com/LedgerHQ/app-ethereum/blob/master/doc/ethapp_plugins.asc).

## Smart Contracts

Smart contracts covered by this plugin are:

| Network | Smart Contract                                                                                                                | Name           |
| ------- | ----------------------------------------------------------------------------------------------------------------------------- | -------------- |
| Mainnet | [0xa356867fdcea8e71aeaf87805808803806231fdc](https://etherscan.io/address/0xa356867fdcea8e71aeaf87805808803806231fdc#code)    | DODOV2Proxy02  |
|         | [0xa2398842f37465f89540430bdc00219fa9e4d28a](https://etherscan.io/address/0xa2398842f37465f89540430bdc00219fa9e4d28a#code)    | DODORouteProxy |
|         | [0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2](https://etherscan.io/address/0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2#code)    | WETH9          |
| BSC     | [0x8F8Dd7DB1bDA5eD3da8C9daf3bfa471c12d58486](https://bscscan.com/address/0x8f8dd7db1bda5ed3da8c9daf3bfa471c12d58486#code)     | DODOV2Proxy02  |
|         | [0x6B3D817814eABc984d51896b1015C0b89E9737Ca](https://bscscan.com/address/0x6b3d817814eabc984d51896b1015c0b89e9737ca#code)     | DODORouteProxy |
|         | [0xbb4CdB9CBd36B01bD1cBaEBF2De08d9173bc095c](https://bscscan.com/address/0xbb4CdB9CBd36B01bD1cBaEBF2De08d9173bc095c#code)     | WETH9          |
| Polygon | [0xa222e6a71D1A1Dd5F279805fbe38d5329C1d0e70](https://polygonscan.com/address/0xa222e6a71D1A1Dd5F279805fbe38d5329C1d0e70#code) | DODOV2Proxy02  |
|         | [0x2fA4334cfD7c56a0E7Ca02BD81455205FcBDc5E9](https://polygonscan.com/address/0x2fa4334cfd7c56a0e7ca02bd81455205fcbdc5e9#code) | DODORouteProxy |
|         | [0x0d500B1d8E8eF31E21C99d1Db9A6444d3ADf1270](https://etherscan.io/address/0x0d500B1d8E8eF31E21C99d1Db9A6444d3ADf1270#code)    | WETH9          |

## Methods

| Contract name  | ID         | Name                   |
| -------------- | ---------- | ---------------------- |
| DODOV2Proxy02  | 0x54bacd13 | externalSwap           |
|                | 0xf87dc1b7 | dodoSwapV2TokenToToken |
|                | 0x1e6d24c2 | dodoSwapV2TokenToETH   |
|                | 0x5028bb95 | dodoSwapV2ETHToToken   |
| DODORouteProxy | 0x7617b389 | mixSwap                |
|                | 0x81791788 | dodoMutliSwap          |
| WETH9          | 0x2e1a7d4d | withdraw               |
|                | 0xd0e30db0 | deposit                |
