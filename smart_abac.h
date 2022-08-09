/*
 * Copyright (C) 2022 Geovane Fedrecheski <geonnave@gmail.com>
 *               2022 Universidade de São Paulo
 *               2022 LSI-TEC
 *
 * This file is part of the SwarmOS project, and it is subject to 
 * the terms and conditions of the GNU Lesser General Public License v2.1. 
 * See the file LICENSE in the top level directory for more details.
 */

#ifndef SMART_ABAC_H
#define SMART_ABAC_H

enum abac_types {abac_integer, abac_real, abac_integer_range, abac_real_range, abac_string, abac_string_list, abac_dictionary};

typedef struct range_v2 {
	union {
		float real_min;
		int integer_min;
	};
	union {
		float real_max;
		int integer_max;
	};
} range_v2;

typedef struct attr_v2 {
	enum abac_types data_type;
	char *name;
	size_t inner_list_len; // used for either strings or inner_attrs
	union {
		char *string;
		char **string_list;
		int integer;
		float real;
		range_v2 ran;
		struct attr_v2 **inner_attrs;
	};
} attr_v2;

typedef struct rule {
	attr_v2 **users;
	attr_v2 **objects;
	attr_v2 **contexts;
	char **operations;
	size_t users_len;
	size_t objects_len;
	size_t contexts_len;
	size_t operations_len;
} rule;

// typedef struct policy_v2 {
// 	char *id;
// 	char *version;
// 	char *name;
// 	rule permissions;
// } policy_v2;

typedef struct node {
	char *value;
	struct node *next;
} node;

typedef struct graph {
	size_t len;
	node **list;
} graph;

attr_v2 new_attr_integer(char *name, int value);
attr_v2 new_attr_real(char *name, float value);
attr_v2 new_attr_integer_range(char *name, int min, int max);
attr_v2 new_attr_real_range(char *name, float min, float max);
attr_v2 new_attr_string(char *name, char *value);
attr_v2 new_attr_string_list(char *name, size_t len);
attr_v2 new_attr_dictionary(char *name, attr_v2 **value, size_t len);
attr_v2 **new_attr_list(size_t len);
char **new_operations_list(size_t len);

int match_attrs_v2(attr_v2 **ras, size_t ras_len, attr_v2 **pas, size_t pas_len);
int match_attr_v2(attr_v2 ra, attr_v2 pa);
int match_permission(rule r, rule perm);
int authorize_permissions(rule req, rule *perms, size_t len);
int is_subset(char **ro, size_t ro_len, char **po, size_t po_len);

void show_attr_v2(attr_v2 at);
void show_operations(char **ops, size_t len);
void show_rule(rule r, char *desc);
void show_node_list(node *list, size_t len, char *desc);
void show_visited(node *visited, size_t v_len);

node new_graph_node(char *value);
void create_directed_edge(node *, node *);
graph new_graph(size_t len);

node *find_ancestors_dfs(graph g, node n, size_t *len);
int is_in(node k, node *list, size_t v_len);
void expand_attrs(rule *, graph);
int authorize_permissions_expand(rule req, rule *perms, size_t p_len, graph g);

#endif
