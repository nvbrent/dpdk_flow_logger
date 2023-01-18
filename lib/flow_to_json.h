#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <json-c/json.h>
#include <rte_flow.h>

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
json_object_new_flow_attr(const struct rte_flow_attr *attr);

struct json_object *
json_object_new_flow_indir_action_conf(const struct rte_flow_indir_action_conf *attr);

struct json_object *
json_object_new_flow_pattern_template_attr(const struct rte_flow_pattern_template_attr *attr);

struct json_object *
json_object_new_flow_actions_template_attr(const struct rte_flow_actions_template_attr *attr);

struct json_object *
json_object_new_flow_template_table_attr(const struct rte_flow_template_table_attr *attr);

struct json_object *
json_object_new_flow_op_attr(const struct rte_flow_op_attr *op_attr);

const char *
rte_flow_table_insertion_type_name(enum rte_flow_table_insertion_type type);

const char *
rte_flow_table_hash_func_name(enum rte_flow_table_hash_func type);

const char *
flow_item_type_name(enum rte_flow_item_type type);

const char *
flow_action_type_name(enum rte_flow_action_type type);

bool addr_bits_set(const uint8_t * addr_bytes, int n_addr_bytes);

bool ether_addr_bits_set(const struct rte_ether_addr *addr);

void ether_addr_format(const struct rte_ether_addr *addr, char *addr_str);

bool ipv6_addr_bits_set(const uint8_t addr[]);

struct json_object * 
json_object_new_flow_item_eth(const struct rte_flow_item_eth *eth);

struct json_object *
json_object_new_flow_item_vlan(const struct rte_flow_item_vlan *vlan);

struct json_object *
json_object_new_flow_item_ipv4(const struct rte_flow_item_ipv4 * ipv4);

struct json_object *
json_object_new_flow_item_ipv6(const struct rte_flow_item_ipv6 * ipv6);

struct json_object *
json_object_new_flow_item_vxlan(const struct rte_flow_item_vxlan * vxlan);

struct json_object *
json_object_new_flow_item_tcp(const struct rte_flow_item_tcp * tcp);

struct json_object *
json_object_new_flow_item_udp(const struct rte_flow_item_udp * udp);

struct json_object *
json_object_new_flow_item_spec(enum rte_flow_item_type type, const void *p);

struct json_object *
json_object_new_flow_item(const struct rte_flow_item *item);

struct json_object *
json_object_new_flow_item_array(const struct rte_flow_item *pattern);

struct json_object *
json_object_new_flow_action_jump(const struct rte_flow_action_jump *jump);

struct json_object *
json_object_new_flow_action_mark(const struct rte_flow_action_mark *mark);

struct json_object *
json_object_new_flow_action_queue(const struct rte_flow_action_queue * queue);

struct json_object *
json_object_new_flow_action_count(const struct rte_flow_action_count * count);

struct json_object *
json_object_new_flow_action_rss(const struct rte_flow_action_rss * rss);

struct json_object *
json_object_new_flow_action_conf(const struct rte_flow_action *action);

struct json_object *
json_object_new_flow_action(const struct rte_flow_action *action);

struct json_object *
json_object_new_flow_actions_array(const struct rte_flow_action *actions);

struct json_object *
json_object_new_flow_port_attr(const struct rte_flow_port_attr *attr);

struct json_object *
json_object_new_flow_queue_attr_list(uint16_t nb_queue, const struct rte_flow_queue_attr *attr[]);

struct json_object *
json_object_new_flow_op_result(const struct rte_flow_op_result *res);

struct json_object *
json_object_new_flow_op_result_list(uint16_t n_res, const struct rte_flow_op_result res[]);
