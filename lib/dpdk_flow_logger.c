#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dlfcn.h>
#include <arpa/inet.h>

#include <json-c/json.h>

#include "wrapper_decls.h"

FILE * f_log;

int json_out_flag = JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY ;

const char * default_filename()
{
    return "rte_flow.json";
}

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
flow_item_type_name(enum rte_flow_item_type type)
{
    switch (type)
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

bool addr_bits_set(const uint8_t * addr_bytes, int n_addr_bytes)
{
    for (int i=0; i<n_addr_bytes; i++)
        if (addr_bytes[i])
            return true;
    return false;
}

bool ether_addr_bits_set(const struct rte_ether_addr *addr)
{
    return addr_bits_set(addr->addr_bytes, RTE_ETHER_ADDR_LEN);
}

void ether_addr_format(const struct rte_ether_addr *addr, char *addr_str)
{
    sprintf(addr_str, "%02x:%02x:%02x:%02x:%02x:%02x",
        addr->addr_bytes[0], addr->addr_bytes[1],
        addr->addr_bytes[2], addr->addr_bytes[3],
        addr->addr_bytes[4], addr->addr_bytes[5]);
}

bool ipv6_addr_bits_set(const uint8_t addr[])
{
    return addr_bits_set(addr, 16);
}

#define OPTIONAL_MAC(flow_item, json_item, field) \
    if (ether_addr_bits_set(&flow_item->field)) { \
        char addr[32]; \
        ether_addr_format(&flow_item->field, addr); \
        json_object_object_add(json_item, #field, json_object_new_string(addr)); \
    }

#define OPTIONAL_IPV4(flow_item, json_item, field) \
    if (flow_item->field) { \
        char addr[INET_ADDRSTRLEN]; \
        inet_ntop(AF_INET, &flow_item->field, addr, INET_ADDRSTRLEN); \
        json_object_object_add(json_item, #field, json_object_new_string(addr)); \
    }

#define OPTIONAL_IPV6(flow_item, json_item, field) \
    if (ipv6_addr_bits_set(flow_item->field)) { \
        char addr[INET6_ADDRSTRLEN]; \
        inet_ntop(AF_INET6, &flow_item->field, addr, INET6_ADDRSTRLEN); \
        json_object_object_add(json_item, #field, json_object_new_string(addr)); \
    }

#define OPTIONAL_INT(flow_item, json_item, field, ENDIAN_OP) \
    if (flow_item->field) \
        json_object_object_add(json_item, #field, json_object_new_int(ENDIAN_OP(flow_item->field)))

#define OPTIONAL_FLAG(flow_item, json_item, field) \
    if (flow_item->field) \
        json_object_object_add(json_item, #field, json_object_new_boolean(flow_item->field))

struct json_object * 
json_object_new_flow_item_eth(const struct rte_flow_item_eth *eth)
{
    struct json_object * json_eth = json_object_new_object();

    OPTIONAL_MAC(eth, json_eth, dst);
    OPTIONAL_MAC(eth, json_eth, src);
    OPTIONAL_INT(eth, json_eth, type, RTE_BE16);
    OPTIONAL_FLAG(eth, json_eth, has_vlan);

    return json_eth;
}

struct json_object *
json_object_new_flow_item_vlan(const struct rte_flow_item_vlan *vlan)
{
    struct json_object * json_vlan = json_object_new_object();
    OPTIONAL_INT(vlan, json_vlan, tci, RTE_BE16);
    OPTIONAL_INT(vlan, json_vlan, inner_type, RTE_BE16);
    OPTIONAL_FLAG(vlan, json_vlan, has_more_vlan);

    return json_vlan;
}

struct json_object *
json_object_new_flow_item_ipv4(const struct rte_flow_item_ipv4 * ipv4)
{
    const struct rte_ipv4_hdr *hdr = &ipv4->hdr;
    struct json_object * json_ipv4 = json_object_new_object();
    
    struct json_object * json_hdr = json_object_new_object();
    OPTIONAL_INT(hdr, json_hdr, ihl, );
    OPTIONAL_INT(hdr, json_hdr, version, );
    OPTIONAL_INT(hdr, json_hdr, type_of_service, );
    OPTIONAL_INT(hdr, json_hdr, total_length, RTE_BE16);
    OPTIONAL_INT(hdr, json_hdr, packet_id, RTE_BE16);
    OPTIONAL_INT(hdr, json_hdr, fragment_offset, RTE_BE16);
    OPTIONAL_INT(hdr, json_hdr, time_to_live, );
    OPTIONAL_INT(hdr, json_hdr, next_proto_id, );
    OPTIONAL_INT(hdr, json_hdr, hdr_checksum, RTE_BE16);
    OPTIONAL_IPV4(hdr, json_hdr, src_addr);
    OPTIONAL_IPV4(hdr, json_hdr, dst_addr);
    json_object_object_add(json_ipv4, "hdr", json_hdr);
    
    return json_ipv4;
}

struct json_object *
json_object_new_flow_item_ipv6(const struct rte_flow_item_ipv6 * ipv6)
{
    const struct rte_ipv6_hdr *hdr = &ipv6->hdr;
    struct json_object * json_ipv6 = json_object_new_object();
    struct json_object * json_hdr = json_object_new_object();


    OPTIONAL_INT(hdr, json_hdr, vtc_flow, RTE_BE32);
    OPTIONAL_INT(hdr, json_hdr, payload_len, RTE_BE16);
    OPTIONAL_INT(hdr, json_hdr, proto, );
    OPTIONAL_INT(hdr, json_hdr, hop_limits, );
    OPTIONAL_IPV6(hdr, json_hdr, src_addr);
    OPTIONAL_IPV6(hdr, json_hdr, dst_addr);
    json_object_object_add(json_ipv6, "hdr", json_hdr);

    OPTIONAL_FLAG(ipv6, json_hdr, has_hop_ext);
    OPTIONAL_FLAG(ipv6, json_hdr, has_route_ext);
    OPTIONAL_FLAG(ipv6, json_hdr, has_frag_ext);
    OPTIONAL_FLAG(ipv6, json_hdr, has_auth_ext);
    OPTIONAL_FLAG(ipv6, json_hdr, has_esp_ext);
    OPTIONAL_FLAG(ipv6, json_hdr, has_dest_ext);
    OPTIONAL_FLAG(ipv6, json_hdr, has_mobil_ext);
    OPTIONAL_FLAG(ipv6, json_hdr, has_hip_ext);
    OPTIONAL_FLAG(ipv6, json_hdr, has_shim6_ext);

    return json_ipv6;
}

struct json_object *
json_object_new_flow_item_vxlan(const struct rte_flow_item_vxlan * vxlan)
{
    struct json_object * json_vxlan = json_object_new_object();

    OPTIONAL_INT(vxlan, json_vxlan, flags, );

    rte_be32_t vni = (vxlan->vni[0] << 16) | (vxlan->vni[1] << 8) | vxlan->vni[2];
    if (vni)
        json_object_object_add(json_vxlan, "vni", json_object_new_int(vni));

    return json_vxlan;
}

struct json_object *
json_object_new_flow_item_tcp(const struct rte_flow_item_tcp * tcp)
{
    const struct rte_tcp_hdr *hdr = &tcp->hdr;
    struct json_object * json_tcp = json_object_new_object();
    struct json_object * json_hdr = json_object_new_object();

    OPTIONAL_INT(hdr, json_hdr, src_port, RTE_BE16);
    OPTIONAL_INT(hdr, json_hdr, dst_port, RTE_BE16);
    OPTIONAL_INT(hdr, json_hdr, sent_seq, RTE_BE32);
    OPTIONAL_INT(hdr, json_hdr, recv_ack, RTE_BE32);
    OPTIONAL_INT(hdr, json_hdr, dt_off, );
    OPTIONAL_FLAG(hdr, json_hdr, fin);
    OPTIONAL_FLAG(hdr, json_hdr, syn);
    OPTIONAL_FLAG(hdr, json_hdr, rst);
    OPTIONAL_FLAG(hdr, json_hdr, psh);
    OPTIONAL_FLAG(hdr, json_hdr, ack);
    OPTIONAL_FLAG(hdr, json_hdr, urg);
    OPTIONAL_FLAG(hdr, json_hdr, ecne);
    OPTIONAL_FLAG(hdr, json_hdr, cwr);
    OPTIONAL_INT(hdr, json_hdr, rx_win, RTE_BE16);
    OPTIONAL_INT(hdr, json_hdr, cksum, RTE_BE16);
    OPTIONAL_INT(hdr, json_hdr, tcp_urp, RTE_BE16);
    json_object_object_add(json_tcp, "hdr", json_hdr);

    return json_tcp;
}

struct json_object *
json_object_new_flow_item_udp(const struct rte_flow_item_udp * udp)
{
    const struct rte_udp_hdr *hdr = &udp->hdr;
    struct json_object * json_udp = json_object_new_object();
    struct json_object * json_hdr = json_object_new_object();

    OPTIONAL_INT(hdr, json_hdr, src_port, RTE_BE16);
    OPTIONAL_INT(hdr, json_hdr, dst_port, RTE_BE16);
    OPTIONAL_INT(hdr, json_hdr, dgram_len, RTE_BE16);
    OPTIONAL_INT(hdr, json_hdr, dgram_cksum, RTE_BE16);
    json_object_object_add(json_udp, "hdr", json_hdr);

    return json_udp;
}

struct json_object *
json_object_new_flow_item_spec(enum rte_flow_item_type type, const void *p)
{
    switch (type)
    {
    case RTE_FLOW_ITEM_TYPE_ETH: return json_object_new_flow_item_eth(p);
    case RTE_FLOW_ITEM_TYPE_VLAN: return json_object_new_flow_item_vlan(p);
    case RTE_FLOW_ITEM_TYPE_IPV4: return json_object_new_flow_item_ipv4(p);
    case RTE_FLOW_ITEM_TYPE_IPV6: return json_object_new_flow_item_ipv6(p);
    case RTE_FLOW_ITEM_TYPE_VXLAN: return json_object_new_flow_item_vxlan(p);
    case RTE_FLOW_ITEM_TYPE_TCP: return json_object_new_flow_item_tcp(p);
    case RTE_FLOW_ITEM_TYPE_UDP: return json_object_new_flow_item_udp(p);
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

    if (item->spec)
        json_object_object_add(json_item, "spec", json_object_new_flow_item_spec(item->type, item->spec));

    if (item->last)
        json_object_object_add(json_item, "last", json_object_new_flow_item_spec(item->type, item->last));

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
