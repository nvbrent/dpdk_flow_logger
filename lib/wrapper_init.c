#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dlfcn.h>

#define FLOW_LOGGER_DECL_NO_EXTERN
#include "wrapper_decls.h"

#define RTE_EAL_SO "librte_eal.so"
#define RTE_NET_SO "librte_net.so"
#define RTE_ETHDEF_SO "librte_ethdev.so"

const char * default_filename();

extern FILE * f_log;

void __attribute__ ((constructor)) init_logger(void);
void __attribute__ ((destructor)) close_logger(void);

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

#define INIT_PFUNC(func, lib_handle) p_ ## func = log_dlsym(lib_handle, #func)
    INIT_PFUNC(rte_eal_init, handle_eal);
    INIT_PFUNC(rte_eth_dev_count_avail, handle_ethdev);
    INIT_PFUNC(rte_flow_create, handle_ethdev);
    INIT_PFUNC(rte_flow_destroy, handle_ethdev);
}

void init_logger(void)
{
    load_wrappers();

    f_log = fopen(default_filename(), "w");
    fprintf(f_log, "[\n");
    fflush(f_log);
}

void close_logger(void)
{
    if (f_log) {
        fprintf(f_log, "{}\n]\n");
        fclose(f_log);
    }
}
