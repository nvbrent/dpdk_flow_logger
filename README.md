# dpdk_flow_logger

This library is designed to sit between a DPDK application and the DPDK libraries, logging calls of interest to a log file before passing the arguments to the true DPDK libraries.

Currently, the only supported log format is JSON. In the future, testpmd format is planned.

## Building

Prerequisites:
* meson
* json-c
* dpdk

Note that dpdk_flow_logger should be built against the same release of DPDK which will be linked at runtime.

```bash
meson build
ninja -C build install
```

## Usage

Usage is simple; prefix the DPDK program with the `LD_PRELOAD` flag as follows, or perform an `export LD_PRELOAD` to make the setting persist.

```bash
LD_PRELOAD=libdpdk_flow_logger.so my-dpdk-program <my-dpdk-args>
```

Example output:
```json
[
{
  "function": "rte_eal_init",
  "arguments": {
    "argc": 3,
    "argv": [
      "\/root\/repos\/dpdk_flow_logger\/build\/test\/test_logger",
      "-a0:00.0",
      "-c0x1"
    ]
  },
  "return": 2
},
{
  "function": "rte_eth_dev_count_avail",
  "arguments": {
  },
  "return": 0
},
{
  "function": "rte_flow_create",
  "arguments": {
    "attr": {
      "group": 2,
      "priority": 3,
      "ingress": true,
      "transfer": true
    },
    "pattern": [
      {
        "type": "VOID"
      },
      {
        "type": "ETH",
        "spec": {
          "src": "11:22:33:44:55:66",
          "type": 33024
        },
        "last": {
          "src": "11:22:ff:ff:ff:ff"
        },
        "mask": {
          "src": "ff:ff:00:00:00:00"
        }
      },
      {
        "type": "END"
      }
    ]
  },
  "return": 0
},
{}
]
```
