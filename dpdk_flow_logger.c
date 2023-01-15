#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dlfcn.h>

#include <json-c/json.h>

#include <rte_flow.h>
#include <rte_ethdev.h>

#define LOG_FILENAME "rte_flow.json"

#define RTE_EAL_SO "librte_eal.so"
#define RTE_NET_SO "librte_net.so"
#define RTE_ETHDEF_SO "librte_ethdev.so"

FILE * f_log;

int json_out_flag = JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY ;

#define DECL_PFUNC(func) typeof(func) * p_ ## func

DECL_PFUNC(rte_eal_init);
DECL_PFUNC(rte_eth_dev_count_avail);
DECL_PFUNC(rte_flow_create);

struct json_object * json_object_new_function(const char *func_name)
{
    struct json_object * f = json_object_new_object();
    json_object_object_add(f, "function", json_object_new_string(func_name));
    return f;
}

void json_object_end_function(struct json_object * f)
{
    json_object_to_fd(fileno(f_log), f, json_out_flag);
    json_object_put(f);
    fprintf(f_log, ",\n");
    fflush(f_log);
}

int rte_eal_init(int argc, char **argv)
{
    struct json_object * f = json_object_new_function(__FUNCTION__);
    struct json_object * args = json_object_new_object();
    json_object_object_add(f, "arguments", args);

    json_object_object_add(args, "argc", json_object_new_int(argc));
    struct json_object * json_argv = json_object_new_array();
    for (int i=0; i<argc; i++)
        json_object_array_add(json_argv, json_object_new_string(argv[i]));
    json_object_object_add(args, "argv", json_argv);

    int res = (*p_rte_eal_init)(argc, argv);
    json_object_object_add(f, "return", json_object_new_int(res));
    json_object_end_function(f);
    return res;
}

uint16_t rte_eth_dev_count_avail(void)
{
    struct json_object * f = json_object_new_function(__FUNCTION__);
    struct json_object * args = json_object_new_object();
    json_object_object_add(f, "arguments", args);
    uint16_t res = (*p_rte_eth_dev_count_avail)();
    json_object_object_add(f, "return", json_object_new_int(res));
    json_object_end_function(f);
    return res;
}

struct rte_flow *
rte_flow_create(uint16_t port_id,
		const struct rte_flow_attr *attr,
		const struct rte_flow_item pattern[],
		const struct rte_flow_action actions[],
		struct rte_flow_error *error)
{
    struct json_object * f = json_object_new_function(__FUNCTION__);
    struct json_object * args = json_object_new_object();
    json_object_object_add(f, "arguments", args);
    struct rte_flow * res = (*p_rte_flow_create)(port_id, attr, pattern, actions, error);
    json_object_object_add(f, "return", json_object_new_uint64((intptr_t)res));
    json_object_end_function(f);
    return res;
}

//////////////////////////////////////////////////////////////////////

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

    p_rte_eal_init = log_dlsym(handle_eal, "rte_eal_init");
    p_rte_eth_dev_count_avail = log_dlsym(handle_ethdev, "rte_eth_dev_count_avail");
    p_rte_flow_create = log_dlsym(handle_ethdev, "rte_flow_create");
    p_rte_flow_create = log_dlsym(handle_ethdev, "rte_flow_create");
}

void init_logger(void)
{
    load_wrappers();

    f_log = fopen(LOG_FILENAME, "w");
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
