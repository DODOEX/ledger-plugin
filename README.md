# Ledger Dapp Plugin

## contracts

| chain   | contract                                   | name           |
|---------|--------------------------------------------|----------------|
| mainnet | [0xa356867fdcea8e71aeaf87805808803806231fdc](https://etherscan.io/address/0xa356867fdcea8e71aeaf87805808803806231fdc#code)    | DODOV2Proxy02  |
|         | [0xa2398842f37465f89540430bdc00219fa9e4d28a](https://etherscan.io/address/0xa2398842f37465f89540430bdc00219fa9e4d28a#code)    | DODORouteProxy |
|         | [0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2](https://etherscan.io/address/0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2#code)    | WETH9 |
| bsc     | [0x8F8Dd7DB1bDA5eD3da8C9daf3bfa471c12d58486](https://bscscan.com/address/0x8f8dd7db1bda5ed3da8c9daf3bfa471c12d58486#code)     | DODOV2Proxy02  |
|         | [0x6B3D817814eABc984d51896b1015C0b89E9737Ca](https://bscscan.com/address/0x6b3d817814eabc984d51896b1015c0b89e9737ca#code)     | DODORouteProxy |
|         | [0xbb4CdB9CBd36B01bD1cBaEBF2De08d9173bc095c](https://etherscan.io/address/0xbb4CdB9CBd36B01bD1cBaEBF2De08d9173bc095c#code)    | WETH9 |
| polygon | [0xa222e6a71D1A1Dd5F279805fbe38d5329C1d0e70](https://polygonscan.com/address/0xa222e6a71D1A1Dd5F279805fbe38d5329C1d0e70#code) | DODOV2Proxy02  |
|         | [0x2fA4334cfD7c56a0E7Ca02BD81455205FcBDc5E9](https://polygonscan.com/address/0x2fa4334cfd7c56a0e7ca02bd81455205fcbdc5e9#code) | DODORouteProxy |
|         | [0x0d500B1d8E8eF31E21C99d1Db9A6444d3ADf1270](https://etherscan.io/address/0x0d500B1d8E8eF31E21C99d1Db9A6444d3ADf1270#code)    | WETH9 |

## methods

| contract name  | id         | name                   |
|----------------|------------|------------------------|
| DODOV2Proxy02  | 0x54bacd13 | externalSwap           |
|                | 0xf87dc1b7 | dodoSwapV2TokenToToken |
|                | 0x1e6d24c2 | dodoSwapV2TokenToETH   |
|                | 0x5028bb95 | dodoSwapV2ETHToToken   |
| DODORouteProxy | 0x7617b389 | mixSwap                |
|                | 0x81791788 | dodoMutliSwap          |
| WETH9          | 0x2e1a7d4d | withdraw               |
|                | 0xd0e30db0 | deposit                |

## 概述

插件库依赖 `app-ethereum` 和 `plugin-tools` 项目，这里用子模块管理。插件在 `app-plugin` 下，是 clone [app-plugin-boilerplate](https://github.com/LedgerHQ/app-plugin-boilerplate) 修改而成

更新子模块

```Bash
git submodule update --remote
```

## 环境搭建

1. 安装 `docker` 及 `docker-compose`

2. 进入 `docker` 环境

```Bash
cd plugin-tools
./start.sh
```
如果收到此错误：
ERROR: Couldn't connect to Docker daemon at http+docker://localhost - is it running?
这意味着您：
- 安装失败docker。
- 需要执行 `sudo ./start.sh`

如果执行成功，则会进入 docker 环境。通过 `volume` 和本地共享文件目录

3. 编译 `app-ethereum` 及 `app-plugin`

```Bash
# docker 环境
cd app-ethereum
make
```

## 增加合约适配

可以参考以往的提交记录，及[Ledger 文档](https://developers.ledger.com/docs/dapp/nano-plugin/init-contract/) 配合查阅
如：
- [增加 method](https://github.com/DODOEX/ledger-plugin/commit/d1cf7a19ad6edd02afe89d0b917f98429ab4d63f)
- [增加合约](https://github.com/DODOEX/ledger-plugin/commit/ca1bec69eec0d3185b0e4f8223912708b2fa715f)

有一些合约比较特殊，入参是一个数组，处理 `handle_provide_parameter` 就会比较特殊。比如 `doodMutliSwap` 其中的一条 [tx](https://etherscan.io/tx/0xfc54a176a7aeca39f9dfa0914c4a7b8b0f5aed82e775ec5e54657c05c5ee7e9d).

```
# 入参
# fromToken 在 midToken[0]
# toToken 在 midToken[3]
（uint256 fromTokenAmount, uint256 minReturnAmount, uint256[] totalWeight, uint256[] splitNumber, address[] midToken, address[] assetFrom, bytes[] sequence, uint256 deadLine）
```
`Input Data` 有 31 位数组，以下是对应关系

- [0] => fromTokenAmount
- [1] => minReturnAmount
- [2] => totalWeight 的偏移量
- [3] => splitNumber 的偏移量
- [4] => midToken 的偏移量 => 0x1a0 => 转换十进制为 416
- [5] => assetFrom 的偏移量
- [6] => sequence 的偏移量
- [7] => deadLine
- [8] => fromTokenAmount
- ...
- [13] => midToken 的数组长度 => 4
- [14] => midToken[0]
- [15] => midToken[1]
- [16] => midToken[2]
- [17] => midToken[3]
- ...

[4] 的位置是十六进制 1a0，转换成十进制为 416。每行的长度为 32,即 [0] => 0, [1] => 32。也就是偏移 13 次，即 [13]。所以 `midToken` 数组是从 [13] 开始的，依次为 `[len, value, ...]`。

其他数组数据不重要，就不做解析了。需要的数据就来自 [0]、[1]、[14]、[17]。

0、1 可以直接解析，而 [14、17] 可能是动态的，所以跳转到 [4]，根据实际的偏移量来跳转到 `midToken` 的位置.
```C
// msg->parameterOffset 是当前的偏移量
// SELECTOR_SIZE 是方法名的 size
// PARAMETER_LENGTH 为每行的 size（32）
// 这里从 [1] 跳到 [4] ，所以用 PARAMETER_LENGTH * 3
context->offset = msg->parameterOffset - SELECTOR_SIZE + PARAMETER_LENGTH * 3;
```
得到了 `midToken` 的偏移量，就可以得到 `fromToken` 和 `toToken` 了.

可以参考 [doodMutliSwap 的提交记录](https://github.com/DODOEX/ledger-plugin/commit/65fbae9d63fa40637ba066b9a220b536660045f2)，及 [Ledger 关于参数分析的文档](https://developers.ledger.com/docs/dapp/nano-plugin/provide-parameter/)

## 打包

进 `docker` 环境执行打包

```Bash
cd app-plugin/tests
./build_local_test_elfs.sh
```

## 单元测试

```Bash
# 非 docker 环境
cd app-plugin/tests
yarn
yarn test
```

## tips

- `tokenLookup[12]` 指定 token 有可能查询不到，会显示为 Unknown token
- 要注意修改测试文件中的 `right_clicks` 变量，这里指的是右键几次，会到 Accept 的界面。

## 参考文档

[ledger 文档](https://developers.ledger.com/docs/dapp/process/)