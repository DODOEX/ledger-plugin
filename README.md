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

## 参考文档

[ledger 文档](https://developers.ledger.com/docs/dapp/process/)