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
    struct rte_flow_item_eth eth_spec;
    struct rte_flow_item_eth eth_mask;
    struct rte_flow_item pattern[] = {
        { .type = RTE_FLOW_ITEM_TYPE_VOID },
        { .type = RTE_FLOW_ITEM_TYPE_ETH, .spec = &eth_spec, .mask = &eth_mask },
        { .type = RTE_FLOW_ITEM_TYPE_END },
    };
    struct rte_flow_action actions[] = {
        { .type = RTE_FLOW_ACTION_TYPE_JUMP, .conf = 0 },
        { .type = RTE_FLOW_ACTION_TYPE_END, .conf = 0 },
    };
    struct rte_flow_error error;
    struct rte_flow * flow = rte_flow_create(0, &attr, pattern, actions, &error);

    return 0;
}
