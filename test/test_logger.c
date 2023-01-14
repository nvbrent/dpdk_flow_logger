#include <rte_eal.h>
#include <rte_ethdev.h>

int main(int argc, char * argv[])
{
    int res = 0;

    printf("test: calling rte_eal_init...\n");
    res = rte_eal_init(argc, argv);
    printf("test: rte_eal_init returned %d\n", res);

    printf("test: calling rte_eth_dev_count_avail...\n");
    res = rte_eth_dev_count_avail();
    printf("test: rte_eth_dev_count_avail returned %d\n", res);
    return 0;
}
