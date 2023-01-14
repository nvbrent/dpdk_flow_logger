#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dlfcn.h>

#define RTE_EAL_SO "librte_eal.so"
#define RTE_NET_SO "librte_net.so"
#define RTE_ETHDEF_SO "librte_ethdev.so"

int (*p_rte_eal_init)(int argc, char **argv);
uint16_t (*p_rte_eth_dev_count_avail)(void);

uint16_t rte_eth_dev_count_avail(void)
{
    uint16_t res = (*p_rte_eth_dev_count_avail)();
    printf("flow logger: %s returned %d\n", __FUNCTION__, res);
    return res;
}

int rte_eal_init(int argc, char **argv)
{
    printf("flow logger: %s called with args:\n", __FUNCTION__);
    for (int i=0; i<argc; i++) {
        printf("flow logger: [%d] = %s\n", i, argv[i]);
    }
    int res = (*p_rte_eal_init)(argc, argv);
    printf("flow logger: rte_eal_init returned %d\n", res);
    return res;
}

//////////////////////////////////////////////////////////////////////

void __attribute__ ((constructor)) load_wrappers(void);

void * log_dlopen(const char *file, int mode)
{
    void * handle = dlopen(file, mode);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }
    return handle;
}

void * log_dlsym(void *handle, const char *name)
{
    dlerror();
    void * res = dlsym(handle, name);
    char * error = dlerror();
    if (error) {
        fprintf(stderr, "%s\n", error);
        exit(1);
    }
    return res;
}

void load_wrappers(void)
{
    void * handle_ethdev = log_dlopen(RTE_ETHDEF_SO, RTLD_LAZY);
    void * handle_eal = log_dlopen(RTE_EAL_SO, RTLD_LAZY);

    p_rte_eth_dev_count_avail = log_dlsym(handle_ethdev, "rte_eth_dev_count_avail");
    p_rte_eal_init = log_dlsym(handle_eal, "rte_eal_init");
}
