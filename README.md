
# ITCH Limit Orderbook

## Benchmarks

These benchmarks were taken on a (4) core Intel(R) Core(TM) i5-9300H CPU @ 2.40GHz with isolcpus on cores 2 and 3. The linux kernel is v6.10.11-200.fc40.x86_64 and compiled with gcc version 14.2.1.

| Total Messages | Symbols Count | Mean (ns) | 99% (ns) | 
| -------------- | ------------- | --------- | -------- |
| 368,366,634    | 1             | 44.5      | 68       |
| 368,366,634    | 8695          | 84.6      | 205      |

## Installing

To build the executable, run the commands below.

```
    $ mkdir build && cd build
    $ cmake ..
    $ make
```

## Dependencies

This repository contains a .devcontainer directory. The .devcontainer has all the required dependencies and can be run inside Docker with the Dev Containers VSCode extension.

#### User Install Required

- [Boost IOStream](https://github.com/google/googletest) 


## License

This software is distributed under the GNU license. Please read [LICENSE](https://github.com/drogalis/ITCH-Limit-Orderbook/blob/main/LICENSE) for information on the software availability and distribution.

