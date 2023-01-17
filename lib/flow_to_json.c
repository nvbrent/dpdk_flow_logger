#include <arpa/inet.h>

#include "flow_to_json.h"

struct json_object *
json_object_new_flow_attr(const struct rte_flow_attr *attr)
{
    struct json_object * json_attr = json_object_new_object();
    OPTIONAL_INT(attr, json_attr, group, );
    OPTIONAL_INT(attr, json_attr, priority, );
    OPTIONAL_FLAG(attr, json_attr, ingress);
    OPTIONAL_FLAG(attr, json_attr, egress);
    OPTIONAL_FLAG(attr, json_attr, transfer);
    OPTIONAL_INT(attr, json_attr, transfer_mode, );
    OPTIONAL_INT(attr, json_attr, hint_num_of_rules_log, );
    return json_attr;
}

struct json_object *
json_object_new_flow_indir_action_conf(const struct rte_flow_indir_action_conf *attr)
{
    struct json_object * json_attr = json_object_new_object();
    OPTIONAL_FLAG(attr, json_attr, ingress);
    OPTIONAL_FLAG(attr, json_attr, egress);
    OPTIONAL_FLAG(attr, json_attr, transfer);
    return json_attr;
}

struct json_object *
json_object_new_flow_pattern_template_attr(const struct rte_flow_pattern_template_attr *attr)
{
    struct json_object * json_attr = json_object_new_object();
    OPTIONAL_FLAG(attr, json_attr, relaxed_matching);
    OPTIONAL_FLAG(attr, json_attr, ingress);
    OPTIONAL_FLAG(attr, json_attr, egress);
    OPTIONAL_FLAG(attr, json_attr, transfer);
    return json_attr;
}

struct json_object *
json_object_new_flow_actions_template_attr(const struct rte_flow_actions_template_attr *attr)
{
    struct json_object * json_attr = json_object_new_object();
    OPTIONAL_FLAG(attr, json_attr, ingress);
    OPTIONAL_FLAG(attr, json_attr, egress);
    OPTIONAL_FLAG(attr, json_attr, transfer);
    return json_attr;
}

struct json_object *
json_object_new_flow_template_table_attr(const struct rte_flow_template_table_attr *attr)
{
    struct json_object * json_attr = json_object_new_object();
    json_object_object_add(json_attr, "flow_attr", json_object_new_flow_attr(&attr->flow_attr));
    OPTIONAL_INT(attr, json_attr, nb_flows, );
    json_object_object_add(json_attr, "insertion_type", json_object_new_string(rte_flow_table_insertion_type_name(attr->insertion_type)));
    json_object_object_add(json_attr, "hash_func", json_object_new_string(rte_flow_table_hash_func_name(attr->hash_func)));
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

const char *
flow_action_type_name(enum rte_flow_action_type type)
{
    switch (type)
    {
#define HANDLE_CASE(s) case RTE_FLOW_ACTION_TYPE_ ## s: return #s
    HANDLE_CASE(END);
    HANDLE_CASE(VOID);
    HANDLE_CASE(PASSTHRU);
    HANDLE_CASE(JUMP);
    HANDLE_CASE(MARK);
    HANDLE_CASE(FLAG);
    HANDLE_CASE(QUEUE);
    HANDLE_CASE(DROP);
    HANDLE_CASE(COUNT);
    HANDLE_CASE(RSS);
    HANDLE_CASE(PF);
    HANDLE_CASE(VF);
    HANDLE_CASE(PHY_PORT);
    HANDLE_CASE(PORT_ID);
    HANDLE_CASE(METER);
    HANDLE_CASE(SECURITY);
    HANDLE_CASE(OF_SET_MPLS_TTL);
    HANDLE_CASE(OF_DEC_MPLS_TTL);
    HANDLE_CASE(OF_SET_NW_TTL);
    HANDLE_CASE(OF_DEC_NW_TTL);
    HANDLE_CASE(OF_COPY_TTL_OUT);
    HANDLE_CASE(OF_COPY_TTL_IN);
    HANDLE_CASE(OF_POP_VLAN);
    HANDLE_CASE(OF_PUSH_VLAN);
    HANDLE_CASE(OF_SET_VLAN_VID);
    HANDLE_CASE(OF_SET_VLAN_PCP);
    HANDLE_CASE(OF_POP_MPLS);
    HANDLE_CASE(OF_PUSH_MPLS);
    HANDLE_CASE(VXLAN_ENCAP);
    HANDLE_CASE(VXLAN_DECAP);
    HANDLE_CASE(NVGRE_ENCAP);
    HANDLE_CASE(NVGRE_DECAP);
    HANDLE_CASE(RAW_ENCAP);
    HANDLE_CASE(RAW_DECAP);
    HANDLE_CASE(SET_IPV4_SRC);
    HANDLE_CASE(SET_IPV4_DST);
    HANDLE_CASE(SET_IPV6_SRC);
    HANDLE_CASE(SET_IPV6_DST);
    HANDLE_CASE(SET_TP_SRC);
    HANDLE_CASE(SET_TP_DST);
    HANDLE_CASE(MAC_SWAP);
    HANDLE_CASE(DEC_TTL);
    HANDLE_CASE(SET_TTL);
    HANDLE_CASE(SET_MAC_SRC);
    HANDLE_CASE(SET_MAC_DST);
    HANDLE_CASE(INC_TCP_SEQ);
    HANDLE_CASE(DEC_TCP_SEQ);
    HANDLE_CASE(INC_TCP_ACK);
    HANDLE_CASE(DEC_TCP_ACK);
    HANDLE_CASE(SET_TAG);
    HANDLE_CASE(SET_META);
    HANDLE_CASE(SET_IPV4_DSCP);
    HANDLE_CASE(SET_IPV6_DSCP);
    HANDLE_CASE(AGE);
    HANDLE_CASE(SAMPLE);
    HANDLE_CASE(SHARED);
    HANDLE_CASE(DEC_IPV4_TTL);
    HANDLE_CASE(SET_IPV4_TTL);
    HANDLE_CASE(DEC_IPV6_HOP);
    HANDLE_CASE(SET_IPV6_HOP);
    HANDLE_CASE(SET_UDP_TP_SRC);
    HANDLE_CASE(SET_UDP_TP_DST);
    HANDLE_CASE(SET_TCP_TP_SRC);
    HANDLE_CASE(SET_TCP_TP_DST);
    HANDLE_CASE(SFT);
    HANDLE_CASE(MODIFY_FIELD);
    HANDLE_CASE(CONNTRACK);
    HANDLE_CASE(METER_COLOR);
    HANDLE_CASE(INDIRECT);
    HANDLE_CASE(PORT_REPRESENTOR);
    HANDLE_CASE(REPRESENTED_PORT);
    HANDLE_CASE(METER_MARK);
    HANDLE_CASE(SEND_TO_KERNEL);
    HANDLE_CASE(QUOTA);
    default: return "UNKNOWN";
#undef HANDLE_CASE
    }
}

const char *
rte_flow_table_insertion_type_name(enum rte_flow_table_insertion_type type)
{
    switch (type)
    {
#define HANDLE_CASE(s) case RTE_FLOW_TABLE_INSERTION_TYPE_ ## s: return #s
    HANDLE_CASE(PATTERN);
    HANDLE_CASE(INDEX);
    default: return "UNKNOWN";
#undef HANDLE_CASE
    }
}

const char *
rte_flow_table_hash_func_name(enum rte_flow_table_hash_func type)
{
    switch (type)
    {
#define HANDLE_CASE(s) case RTE_FLOW_TABLE_HASH_FUNC_ ## s: return #s
    HANDLE_CASE(DEFAULT);
    HANDLE_CASE(LINEAR);
    HANDLE_CASE(CRC32);
    HANDLE_CASE(CRC16);
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

struct json_object *
json_object_new_flow_action_jump(const struct rte_flow_action_jump *jump)
{
    struct json_object * json_jump = json_object_new_object();
    OPTIONAL_INT(jump, json_jump, group, );
    return json_jump;
}

struct json_object *
json_object_new_flow_action_mark(const struct rte_flow_action_mark *mark)
{
    struct json_object * json_mark = json_object_new_object();
    OPTIONAL_INT(mark, json_mark, id, );
    return json_mark;
}

struct json_object *
json_object_new_flow_action_queue(const struct rte_flow_action_queue * queue)
{
    struct json_object * json_queue = json_object_new_object();
    OPTIONAL_INT(queue, json_queue, index, );
    return json_queue;
}

struct json_object *
json_object_new_flow_action_count(const struct rte_flow_action_count * count)
{
    struct json_object * json_count = json_object_new_object();
    OPTIONAL_FLAG(count, json_count, shared);
    OPTIONAL_INT(count, json_count, id, );
    return json_count;
}

struct json_object *
json_object_new_flow_action_rss(const struct rte_flow_action_rss * rss)
{
    struct json_object * json_rss = json_object_new_object();
    OPTIONAL_INT(rss, json_rss, func, );
    OPTIONAL_INT(rss, json_rss, level, );
    OPTIONAL_INT(rss, json_rss, types, );
    OPTIONAL_INT(rss, json_rss, key_len, );
    OPTIONAL_INT(rss, json_rss, queue_num, );

    if (rss->queue) {
        struct json_object * json_queue = json_object_new_array();
        for (uint32_t i=0; i<rss->queue_num; i++)
            json_object_array_add(json_queue, json_object_new_int(rss->queue[i]));
        json_object_object_add(json_rss, "queue", json_queue);
    }

    return json_rss;
}

struct json_object *
json_object_new_flow_action_conf(const struct rte_flow_action *action)
{
    switch (action->type)
    {
    case RTE_FLOW_ACTION_TYPE_JUMP: return json_object_new_flow_action_jump(action->conf);
    case RTE_FLOW_ACTION_TYPE_MARK: return json_object_new_flow_action_mark(action->conf);
    case RTE_FLOW_ACTION_TYPE_QUEUE: return json_object_new_flow_action_queue(action->conf);
    case RTE_FLOW_ACTION_TYPE_COUNT: return json_object_new_flow_action_count(action->conf);
    case RTE_FLOW_ACTION_TYPE_RSS: return json_object_new_flow_action_rss(action->conf);
    default: break;
    }

    return json_object_new_object();
}

struct json_object *
json_object_new_flow_action(const struct rte_flow_action *action)
{
    struct json_object * json_action = json_object_new_object();

    json_object_object_add(json_action, "type", json_object_new_string(flow_action_type_name(action->type)));
    
    if (action->conf)
        json_object_object_add(json_action, "conf", json_object_new_flow_action_conf(action));

    return json_action;
}

struct json_object *
json_object_new_flow_actions_array(const struct rte_flow_action *actions)
{
    struct json_object * json_action = json_object_new_array();
    if (!actions)
        return json_action;

    while (true)
    {
        json_object_array_add(json_action, json_object_new_flow_action(actions));
        if (actions->type == RTE_FLOW_ACTION_TYPE_END)
            break;
        ++actions;
    }

    return json_action;
}

struct json_object *
json_object_new_flow_port_attr(const struct rte_flow_port_attr *attr)
{
    struct json_object * json_attr = json_object_new_object();
    
    OPTIONAL_INT(attr, json_attr, nb_counters, );
    OPTIONAL_INT(attr, json_attr, nb_aging_objects, );
    OPTIONAL_INT(attr, json_attr, nb_meters, );
    OPTIONAL_INT(attr, json_attr, nb_meter_profiles, );
    OPTIONAL_INT(attr, json_attr, nb_meter_policies, );
    OPTIONAL_INT(attr, json_attr, nb_cts, );
    OPTIONAL_INT(attr, json_attr, host_port_id, );
    OPTIONAL_INT(attr, json_attr, nb_quotas, );
    OPTIONAL_INT(attr, json_attr, flags, );

    return json_attr;
}


struct json_object *
json_object_new_flow_queue_attr_list(uint16_t nb_queue, const struct rte_flow_queue_attr *attr[])
{
    struct json_object * json_attr = json_object_new_array();

    for (uint32_t i=0; i<nb_queue; i++)
        json_object_array_add(json_attr, json_object_new_uint64(attr[i]->size));

    return json_attr;
}
