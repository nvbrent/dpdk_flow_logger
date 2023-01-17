#pragma once

#include <rte_flow.h>
#include <rte_ethdev.h>

#ifndef FLOW_LOGGER_DECL_NO_EXTERN // only defined in one source file!
#define FLOW_LOGGER_EXTERN extern
#else
#define FLOW_LOGGER_EXTERN
#endif

#define DECL_PFUNC(func) FLOW_LOGGER_EXTERN typeof(func) * p_ ## func

DECL_PFUNC(rte_eal_init);
DECL_PFUNC(rte_eth_dev_count_avail);

DECL_PFUNC(rte_flow_create);
DECL_PFUNC(rte_flow_destroy);
DECL_PFUNC(rte_flow_flush);
DECL_PFUNC(rte_flow_action_handle_create);
DECL_PFUNC(rte_flow_action_handle_destroy);
DECL_PFUNC(rte_flow_action_handle_update);
DECL_PFUNC(rte_flow_configure);
