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

struct json_object * json_object_new_function_args(struct json_object * f)
{
    struct json_object * args = json_object_new_object();
    json_object_object_add(f, "arguments", args);
    return args;
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
    struct json_object * args = json_object_new_function_args(f);

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
    (void)json_object_new_function_args(f);
    uint16_t res = (*p_rte_eth_dev_count_avail)();
    json_object_object_add(f, "return", json_object_new_int(res));
    json_object_end_function(f);
    return res;
}

struct json_object *
json_object_new_flow_attr(const struct rte_flow_attr *attr)
{
    struct json_object * json_attr = json_object_new_object();
    if (attr->group)
        json_object_object_add(json_attr, "group", json_object_new_int(attr->group));
    if (attr->priority)
        json_object_object_add(json_attr, "priority", json_object_new_int(attr->priority));
    if (attr->ingress)
        json_object_object_add(json_attr, "ingress", json_object_new_boolean(attr->ingress));
    if (attr->egress)
        json_object_object_add(json_attr, "egress", json_object_new_boolean(attr->egress));
    if (attr->transfer)
        json_object_object_add(json_attr, "transfer", json_object_new_boolean(attr->transfer));
    if (attr->transfer_mode)
        json_object_object_add(json_attr, "transfer_mode", json_object_new_int(attr->transfer_mode));
    if (attr->hint_num_of_rules_log)
        json_object_object_add(json_attr, "hint_num_of_rules_log", json_object_new_int(attr->hint_num_of_rules_log));
    return json_attr;
}

const char *
flow_item_type_name(enum rte_flow_item_type t)
{
    switch (t)
    {
#define HANDLE_CASE(s) case RTE_FLOW_ITEM_TYPE_ ## s: return #s
    HANDLE_CASE(END);
    HANDLE_CASE(VOID);
    HANDLE_CASE(INVERT);
    HANDLE_CASE(ANY);
    HANDLE_CASE(PF);
    HANDLE_CASE(VF);
    HANDLE_CASE(PHY_PORT);
    HANDLE_CASE(PORT_ID);
    HANDLE_CASE(RAW);
    HANDLE_CASE(ETH);
    HANDLE_CASE(VLAN);
    HANDLE_CASE(IPV4);
    HANDLE_CASE(IPV6);
    HANDLE_CASE(ICMP);
    HANDLE_CASE(UDP);
    HANDLE_CASE(TCP);
    HANDLE_CASE(SCTP);
    HANDLE_CASE(VXLAN);
    HANDLE_CASE(E_TAG);
    HANDLE_CASE(NVGRE);
    HANDLE_CASE(MPLS);
    HANDLE_CASE(GRE);
    HANDLE_CASE(FUZZY);
    HANDLE_CASE(GTP);
    HANDLE_CASE(GTPC);
    HANDLE_CASE(GTPU);
    HANDLE_CASE(ESP);
    HANDLE_CASE(GENEVE);
    HANDLE_CASE(VXLAN_GPE);
    HANDLE_CASE(ARP_ETH_IPV4);
    HANDLE_CASE(IPV6_EXT);
    HANDLE_CASE(ICMP6);
    HANDLE_CASE(ICMP6_ND_NS);
    HANDLE_CASE(ICMP6_ND_NA);
    HANDLE_CASE(ICMP6_ND_OPT);
    HANDLE_CASE(ICMP6_ND_OPT_SLA_ETH);
    HANDLE_CASE(ICMP6_ND_OPT_TLA_ETH);
    HANDLE_CASE(MARK);
    HANDLE_CASE(META);
    HANDLE_CASE(GRE_KEY);
    HANDLE_CASE(GTP_PSC);
    HANDLE_CASE(PPPOES);
    HANDLE_CASE(PPPOED);
    HANDLE_CASE(PPPOE_PROTO_ID);
    HANDLE_CASE(NSH);
    HANDLE_CASE(IGMP);
    HANDLE_CASE(AH);
    HANDLE_CASE(HIGIG2);
    HANDLE_CASE(TAG);
    HANDLE_CASE(L2TPV3OIP);
    HANDLE_CASE(PFCP);
    HANDLE_CASE(ECPRI);
    HANDLE_CASE(IPV6_FRAG_EXT);
    HANDLE_CASE(GENEVE_OPT);
    HANDLE_CASE(SFT);
    HANDLE_CASE(INTEGRITY);
    HANDLE_CASE(CONNTRACK);
    HANDLE_CASE(FLEX);
    HANDLE_CASE(GRE_OPTION);
    HANDLE_CASE(PORT_REPRESENTOR);
    HANDLE_CASE(REPRESENTED_PORT);
    HANDLE_CASE(METER_COLOR);
    HANDLE_CASE(QUOTA);
    HANDLE_CASE(ICMP6_ECHO_REQUEST);
    HANDLE_CASE(ICMP6_ECHO_REPLY);
    default: return "UNKNOWN";
#undef HANDLE_CASE
    }
}

bool ether_addr_bits_set(const struct rte_ether_addr *addr)
{
    for (int i=0; i<RTE_ETHER_ADDR_LEN; i++)
        if (addr->addr_bytes[i])
            return true;
    return false;
}

void ether_addr_format(const struct rte_ether_addr *addr, char *addr_str)
{
    sprintf(addr_str, "%02x:%02x:%02x:%02x:%02x:%02x",
        addr->addr_bytes[0], addr->addr_bytes[1],
        addr->addr_bytes[2], addr->addr_bytes[3],
        addr->addr_bytes[4], addr->addr_bytes[5]);
}

struct json_object * 
json_object_new_flow_item_eth(const struct rte_flow_item_eth *eth)
{
    struct json_object * json_eth = json_object_new_object();

    char mac_addr[32];
    if (ether_addr_bits_set(&eth->dst)) {
        ether_addr_format(&eth->dst, mac_addr);
        json_object_object_add(json_eth, "dst", json_object_new_string(mac_addr));
    }
    if (ether_addr_bits_set(&eth->src)) {
        ether_addr_format(&eth->src, mac_addr);
        json_object_object_add(json_eth, "src", json_object_new_string(mac_addr));
    }
    if (eth->type)
        json_object_object_add(json_eth, "type", json_object_new_int(RTE_BE16(eth->type)));
    if (eth->has_vlan)
        json_object_object_add(json_eth, "has_vlan", json_object_new_boolean(eth->has_vlan));

    return json_eth;
}

struct json_object *
json_object_new_flow_item_spec(enum rte_flow_item_type t, const void *p)
{
    switch (t)
    {
    case RTE_FLOW_ITEM_TYPE_ETH: return json_object_new_flow_item_eth(p);
    default:
        break;
    }
    return json_object_new_object();
}

struct json_object *
json_object_new_flow_item(const struct rte_flow_item *item)
{
    struct json_object * json_item = json_object_new_object();
    if (!item)
        return json_item;

    json_object_object_add(json_item, "type", json_object_new_string(flow_item_type_name(item->type)));

    // TODO: spec
    if (item->spec)
        json_object_object_add(json_item, "spec", json_object_new_flow_item_spec(item->type, item->spec));
    // TODO: last
    if (item->last)
        json_object_object_add(json_item, "last", json_object_new_flow_item_spec(item->type, item->last));
    // TODO: mask
    if (item->mask)
        json_object_object_add(json_item, "mask", json_object_new_flow_item_spec(item->type, item->mask));
    return json_item;
}

struct json_object *
json_object_new_flow_item_array(const struct rte_flow_item *pattern)
{
    struct json_object * json_pattern = json_object_new_array();
    if (!pattern)
        return json_pattern;

    while (true)
    {
        json_object_array_add(json_pattern, json_object_new_flow_item(pattern));
        if (pattern->type == RTE_FLOW_ITEM_TYPE_END)
            break;
        ++pattern;
    }

    return json_pattern;
}

struct rte_flow *
rte_flow_create(uint16_t port_id,
		const struct rte_flow_attr *attr,
		const struct rte_flow_item pattern[],
		const struct rte_flow_action actions[],
		struct rte_flow_error *error)
{
    struct json_object * f = json_object_new_function(__FUNCTION__);
    struct json_object * args = json_object_new_function_args(f);

    json_object_object_add(args, "attr", json_object_new_flow_attr(attr));
    json_object_object_add(args, "pattern", json_object_new_flow_item_array(pattern));

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
