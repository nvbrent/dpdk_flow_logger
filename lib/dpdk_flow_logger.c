#include <rte_flow.h>
#include <rte_ethdev.h>
#include <json-c/json.h>

#include "wrapper_decls.h"
#include "flow_to_json.h"

FILE * f_log;

int json_out_flag = JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY ;

const char * default_filename()
{
    return "rte_flow.json";
}

// Helper functions, used for every wrapper function

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

// Wrapper functions from rte_flow.h

struct rte_flow *
rte_flow_create(uint16_t port_id,
		const struct rte_flow_attr *attr,
		const struct rte_flow_item pattern[],
		const struct rte_flow_action actions[],
		struct rte_flow_error *error)
{
    struct json_object * f = json_object_new_function(__FUNCTION__);
    struct json_object * args = json_object_new_function_args(f);

    json_object_object_add(args, "port_id", json_object_new_int(port_id));
    json_object_object_add(args, "attr", json_object_new_flow_attr(attr));
    json_object_object_add(args, "pattern", json_object_new_flow_item_array(pattern));
    json_object_object_add(args, "actions", json_object_new_flow_actions_array(actions));

    struct rte_flow * res = (*p_rte_flow_create)(port_id, attr, pattern, actions, error);
    json_object_object_add(f, "return", json_object_new_uint64((intptr_t)res));
    json_object_end_function(f);
    return res;
}

int
rte_flow_destroy(uint16_t port_id,
		 struct rte_flow *flow,
		 struct rte_flow_error *error)
{
    struct json_object * f = json_object_new_function(__FUNCTION__);
    struct json_object * args = json_object_new_function_args(f);
    json_object_object_add(args, "flow", json_object_new_uint64((intptr_t)flow));

    int res = (*p_rte_flow_destroy)(port_id, flow, error);
    json_object_object_add(f, "return", json_object_new_int(res));
    json_object_end_function(f);
    return res;
}

int
rte_flow_flush(uint16_t port_id,
	       struct rte_flow_error *error)
{
    struct json_object * f = json_object_new_function(__FUNCTION__);
    struct json_object * args = json_object_new_function_args(f);

    json_object_object_add(args, "port_id", json_object_new_int(port_id));

    int res = (*p_rte_flow_flush)(port_id, error);
    json_object_object_add(f, "return", json_object_new_int(res));
    json_object_end_function(f);
    return res;
}

struct rte_flow_action_handle *
rte_flow_action_handle_create(uint16_t port_id,
			      const struct rte_flow_indir_action_conf *conf,
			      const struct rte_flow_action *action,
			      struct rte_flow_error *error)
{
    struct json_object * f = json_object_new_function(__FUNCTION__);
    struct json_object * args = json_object_new_function_args(f);

    json_object_object_add(args, "port_id", json_object_new_int(port_id));
    if (conf)
        json_object_object_add(args, "conf", json_object_new_flow_indir_action_conf(conf));
    if (action)
        json_object_object_add(args, "action", json_object_new_flow_action(action));

    struct rte_flow_action_handle * res = (*p_rte_flow_action_handle_create)(port_id, conf, action, error);
    json_object_object_add(f, "return", json_object_new_uint64((intptr_t)res));
    json_object_end_function(f);
    return res;
}

int
rte_flow_action_handle_destroy(uint16_t port_id,
			       struct rte_flow_action_handle *handle,
			       struct rte_flow_error *error)
{
    struct json_object * f = json_object_new_function(__FUNCTION__);
    struct json_object * args = json_object_new_function_args(f);

    json_object_object_add(args, "port_id", json_object_new_int(port_id));
    if (handle)
        json_object_object_add(args, "handle", json_object_new_uint64((intptr_t)handle));

    int res = (*p_rte_flow_action_handle_destroy)(port_id, handle, error);
    json_object_object_add(f, "return", json_object_new_int(res));
    json_object_end_function(f);
    return res;
}

int
rte_flow_action_handle_update(uint16_t port_id,
			      struct rte_flow_action_handle *handle,
			      const void *update,
			      struct rte_flow_error *error)
{
    struct json_object * f = json_object_new_function(__FUNCTION__);
    struct json_object * args = json_object_new_function_args(f);

    json_object_object_add(args, "port_id", json_object_new_int(port_id));
    if (handle)
        json_object_object_add(args, "handle", json_object_new_uint64((intptr_t)handle));
    if (update)
        json_object_object_add(args, "update", json_object_new_uint64((intptr_t)update));

    int res = (*p_rte_flow_action_handle_update)(port_id, handle, update, error);
    json_object_object_add(f, "return", json_object_new_int(res));
    json_object_end_function(f);
    return res;
}

int
rte_flow_configure(uint16_t port_id,
		   const struct rte_flow_port_attr *port_attr,
		   uint16_t nb_queue,
		   const struct rte_flow_queue_attr *queue_attr[],
		   struct rte_flow_error *error)
{
    struct json_object * f = json_object_new_function(__FUNCTION__);
    struct json_object * args = json_object_new_function_args(f);

    json_object_object_add(args, "port_id", json_object_new_int(port_id));
    if (port_attr)
        json_object_object_add(args, "port_attr", json_object_new_flow_port_attr(port_attr));
    
    json_object_object_add(args, "nb_queue", json_object_new_int(nb_queue));

    json_object_object_add(args, "queue_attr", json_object_new_flow_queue_attr_list(nb_queue, queue_attr));

    int res = (*p_rte_flow_configure)(port_id, port_attr, nb_queue, queue_attr, error);
    json_object_object_add(f, "return", json_object_new_int(res));
    json_object_end_function(f);
    return res;
}

struct rte_flow_pattern_template *
rte_flow_pattern_template_create(uint16_t port_id,
		const struct rte_flow_pattern_template_attr *template_attr,
		const struct rte_flow_item pattern[],
		struct rte_flow_error *error)
{
    struct json_object * f = json_object_new_function(__FUNCTION__);
    struct json_object * args = json_object_new_function_args(f);

    json_object_object_add(args, "port_id", json_object_new_int(port_id));

    if (template_attr)
        json_object_object_add(args, "template_attr", json_object_new_flow_pattern_template_attr(template_attr));
    
    json_object_object_add(args, "pattern", json_object_new_flow_item_array(pattern));

    struct rte_flow_pattern_template * res = (*p_rte_flow_pattern_template_create)(port_id, template_attr, pattern, error);
    json_object_object_add(f, "return", json_object_new_uint64((intptr_t)res));
    json_object_end_function(f);
    return res;
}

int
rte_flow_pattern_template_destroy(uint16_t port_id,
		struct rte_flow_pattern_template *pattern_template,
		struct rte_flow_error *error)
{
    struct json_object * f = json_object_new_function(__FUNCTION__);
    struct json_object * args = json_object_new_function_args(f);

    json_object_object_add(args, "port_id", json_object_new_int(port_id));

    if (pattern_template)
        json_object_object_add(args, "pattern_template", json_object_new_uint64((intptr_t)pattern_template));

    int res = (*p_rte_flow_pattern_template_destroy)(port_id, pattern_template, error);
    json_object_object_add(f, "return", json_object_new_int(res));
    json_object_end_function(f);
    return res;
}

struct rte_flow_actions_template *
rte_flow_actions_template_create(uint16_t port_id,
		const struct rte_flow_actions_template_attr *template_attr,
		const struct rte_flow_action actions[],
		const struct rte_flow_action masks[],
		struct rte_flow_error *error)
{
    struct json_object * f = json_object_new_function(__FUNCTION__);
    struct json_object * args = json_object_new_function_args(f);

    json_object_object_add(args, "port_id", json_object_new_int(port_id));

    if (template_attr)
        json_object_object_add(args, "template_attr", json_object_new_flow_actions_template_attr(template_attr));
    
    json_object_object_add(args, "actions", json_object_new_flow_actions_array(actions));
    json_object_object_add(args, "masks", json_object_new_flow_actions_array(masks));

    struct rte_flow_actions_template * res = (*p_rte_flow_actions_template_create)(port_id, template_attr, actions, masks, error);
    json_object_object_add(f, "return", json_object_new_uint64((intptr_t)res));
    json_object_end_function(f);
    return res;
}

int
rte_flow_actions_template_destroy(uint16_t port_id,
		struct rte_flow_actions_template *actions_template,
		struct rte_flow_error *error)
{
    struct json_object * f = json_object_new_function(__FUNCTION__);
    struct json_object * args = json_object_new_function_args(f);

    json_object_object_add(args, "port_id", json_object_new_int(port_id));

    if (actions_template)
        json_object_object_add(args, "actions_template", json_object_new_uint64((intptr_t)actions_template));

    int res = (*p_rte_flow_actions_template_destroy)(port_id, actions_template, error);
    json_object_object_add(f, "return", json_object_new_int(res));
    json_object_end_function(f);
    return res;
}

struct rte_flow_template_table *
rte_flow_template_table_create(uint16_t port_id,
		const struct rte_flow_template_table_attr *table_attr,
		struct rte_flow_pattern_template *pattern_templates[],
		uint8_t nb_pattern_templates,
		struct rte_flow_actions_template *actions_templates[],
		uint8_t nb_actions_templates,
		struct rte_flow_error *error)
{
    struct json_object * f = json_object_new_function(__FUNCTION__);
    struct json_object * args = json_object_new_function_args(f);

    json_object_object_add(args, "port_id", json_object_new_int(port_id));

    if (table_attr)
        json_object_object_add(args, "table_attr", json_object_new_flow_template_table_attr(table_attr));

    json_object * json_pattern_templates = json_object_new_array();
    for (uint32_t i=0; i<nb_pattern_templates; i++)
        json_object_array_add(json_pattern_templates, json_object_new_uint64((intptr_t)pattern_templates[i]));
    json_object_object_add(args, "pattern_templates", json_pattern_templates);

    json_object * json_actions_templates = json_object_new_array();
    for (uint32_t i=0; i<nb_actions_templates; i++)
        json_object_array_add(json_actions_templates, json_object_new_uint64((intptr_t)actions_templates[i]));
    json_object_object_add(args, "actions_templates", json_actions_templates);

    struct rte_flow_template_table * res = (*p_rte_flow_template_table_create)(
        port_id, table_attr, 
        pattern_templates, nb_pattern_templates, 
        actions_templates, nb_actions_templates, error);
    json_object_object_add(f, "return", json_object_new_uint64((intptr_t)res));
    json_object_end_function(f);
    return res;
}

int
rte_flow_template_table_destroy(uint16_t port_id,
		struct rte_flow_template_table *template_table,
		struct rte_flow_error *error)
{
    struct json_object * f = json_object_new_function(__FUNCTION__);
    struct json_object * args = json_object_new_function_args(f);

    json_object_object_add(args, "port_id", json_object_new_int(port_id));

    if (template_table)
        json_object_object_add(args, "template_table", json_object_new_uint64((intptr_t)template_table));

    int res = (*p_rte_flow_template_table_destroy)(port_id, template_table, error);
    json_object_object_add(f, "return", json_object_new_int(res));
    json_object_end_function(f);
    return res;
}

// Wrapper functions from rte_eal.h:

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

// Wrapper functions from rte_ethdev.h

uint16_t rte_eth_dev_count_avail(void)
{
    struct json_object * f = json_object_new_function(__FUNCTION__);
    (void)json_object_new_function_args(f);
    uint16_t res = (*p_rte_eth_dev_count_avail)();
    json_object_object_add(f, "return", json_object_new_int(res));
    json_object_end_function(f);
    return res;
}

