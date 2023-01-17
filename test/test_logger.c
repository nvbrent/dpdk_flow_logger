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
    struct rte_flow_item_eth eth_spec = { .src = { "\x11\x22\x33\x44\x55\x66" }, .type = RTE_BE16(0x8100) };
    struct rte_flow_item_eth eth_last = { .src = { "\x11\x22\xFF\xFF\xFF\xFF" } };
    struct rte_flow_item_eth eth_mask = { .src = { "\xFF\xFF\x00\x00\x00\x00" } };
    struct rte_flow_item pattern[] = {
        { .type = RTE_FLOW_ITEM_TYPE_VOID },
        { .type = RTE_FLOW_ITEM_TYPE_ETH, .spec = &eth_spec, .last = &eth_last, .mask = &eth_mask },
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
