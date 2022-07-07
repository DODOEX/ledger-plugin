# Ledger Dapp 插件

## 概述

插件库依赖 `app-ethereum` 和 `plugin-tools` 项目，这里用子模块管理。插件在 `app-plugin` 下，是 clone [app-plugin-boilerplate](https://github.com/LedgerHQ/app-plugin-boilerplate) 修改而成

更新子模块

```Bash
git submodule update --remote
```

## 开发

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
得到了 `midToken` 的偏移量，就可以得到 `fromToken` 和 `toToken` 了

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

- `tokenLookup[12]` 指定 token 有可能查询不到，要注意指定常用的 token

## 参考文档

[ledger 文档](https://developers.ledger.com/docs/dapp/process/)