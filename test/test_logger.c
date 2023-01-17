#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_flow.h>

int main(int argc, char * argv[])
{
    int res = 0;

    printf("test: calling rte_eal_init...\n");
    res = rte_eal_init(argc, argv);
    printf("test: rte_eal_init returned %d\n", res);

    printf("test: calling rte_eth_dev_count_avail...\n");
    res = rte_eth_dev_count_avail();
    printf("test: rte_eth_dev_count_avail returned %d\n", res);

    struct rte_flow_attr attr = {
        .group = 2,
        .priority = 3,
        .ingress = true,
        .transfer = true,
    };
    struct rte_flow_item_eth eth_spec = {
        .src = { "\x11\x22\x33\x44\x55\x66" }, 
        .dst = { "\xaa\xbb\xcc\xdd\xee\xff" },
        .type = RTE_BE16(RTE_ETHER_TYPE_VLAN),
        .has_vlan = 1,
    };
    struct rte_flow_item_eth eth_last = { 
        .src = { "\x11\x22\xFF\xFF\xFF\xFF" } 
    };
    struct rte_flow_item_eth eth_mask = { 
        .src = { "\xFF\xFF\x00\x00\x00\x00" } 
    };
    struct rte_flow_item_vlan vlan_spec = {
        .tci = RTE_BE16(123),
        .inner_type = RTE_BE16(RTE_ETHER_TYPE_IPV6),
    };
    struct rte_flow_item_ipv6 ipv6_spec = {
        .hdr = {
            .src_addr = { "\x11\x22\x33\x44\x11\x22\x33\x44\x11\x22\x33\x44\x11\x22\x33\x44"},
            .dst_addr = { "\x55\x66\x77\x88\x55\x66\x77\x88\x55\x66\x77\x88\x55\x66\x77\x88"},
        }
    };
    struct rte_flow_item_vxlan vxlan_spec = {
        .flags = 0x08,
        .vni = { 5, 6, 7 },
    };
    struct rte_flow_item_ipv4 ipv4_spec = {
        .hdr = {
            .src_addr = RTE_BE32(0xc0010101),
            .dst_addr = RTE_BE32(0xc0636363),
        }
    };
    struct rte_flow_item_tcp tcp_spec = {
        .hdr = {
            .src_port = RTE_BE16(4444),
            .dst_port = RTE_BE16(5555),
            .fin = 1,
            .syn = 1,
            .rst = 1,
        }
    };
    struct rte_flow_item_udp udp_spec = {
        .hdr = {
            .src_port = RTE_BE16(6666),
            .dst_port = RTE_BE16(7777),
        }
    };
    struct rte_flow_item pattern[] = {
        { .type = RTE_FLOW_ITEM_TYPE_VOID },
        { .type = RTE_FLOW_ITEM_TYPE_ETH, .spec = &eth_spec, .last = &eth_last, .mask = &eth_mask },
        { .type = RTE_FLOW_ITEM_TYPE_VLAN, .spec = &vlan_spec, .mask = &rte_flow_item_vlan_mask },
        { .type = RTE_FLOW_ITEM_TYPE_IPV6, .spec = &ipv6_spec, .mask = &rte_flow_item_ipv6_mask },
        { .type = RTE_FLOW_ITEM_TYPE_VXLAN, .spec = &vxlan_spec, .mask = &rte_flow_item_vxlan_mask },
        { .type = RTE_FLOW_ITEM_TYPE_IPV4, .spec = &ipv4_spec, .mask = &rte_flow_item_ipv4_mask },
        { .type = RTE_FLOW_ITEM_TYPE_TCP, .spec = &tcp_spec, .mask = &rte_flow_item_tcp_mask },
        { .type = RTE_FLOW_ITEM_TYPE_UDP, .spec = &udp_spec, .mask = &rte_flow_item_udp_mask },
        { .type = RTE_FLOW_ITEM_TYPE_END },
    };
    struct rte_flow_action actions[] = {
        { .type = RTE_FLOW_ACTION_TYPE_JUMP, .conf = 0 },
        { .type = RTE_FLOW_ACTION_TYPE_END, .conf = 0 },
    };
    struct rte_flow_error error;
    uint16_t port_id = 0;
    struct rte_flow * flow = rte_flow_create(port_id, &attr, pattern, actions, &error);
    rte_flow_destroy(port_id, flow, &error);

    return 0;
}
