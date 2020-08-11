#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef MBED_MAJOR_VERSION
#include "mbed.h"
Serial pc(P0_25, P0_8);
#endif

// definitions
#include "abac_them.h"

// constructors

attr_v2 new_attr_integer(char *name, int value)
{
	attr_v2 at;
	at.data_type = abac_integer;
	at.name = name;
	at.integer = value;
	return at;
}

attr_v2 new_attr_real(char *name, float value)
{
	attr_v2 at;
	at.data_type = abac_real;
	at.name = name;
	at.real = value;
	return at;
}

attr_v2 new_attr_integer_range(char *name, int min, int max)
{
	attr_v2 at;
	at.data_type = abac_integer_range;
	at.name = name;
	at.ran.integer_min = min;
	at.ran.integer_max = max;
	return at;
}

attr_v2 new_attr_real_range(char *name, float min, float max)
{
	attr_v2 at;
	at.data_type = abac_real_range;
	at.name = name;
	at.ran.real_min = min;
	at.ran.real_max = max;
	return at;
}

attr_v2 new_attr_string(char *name, char *value)
{
	attr_v2 at;
	at.data_type = abac_string;
	at.name = name;
	at.string = value;
	return at;
}

attr_v2 new_attr_string_list(char *name, size_t len)
{
	attr_v2 at;
	at.data_type = abac_string_list;
	at.name = name;
	at.string_list = malloc(sizeof(char *) * len);
	return at;
}

attr_v2 new_attr_dictionary(char *name, attr_v2 **value, size_t len)
{
	attr_v2 at;
	at.data_type = abac_dictionary;
	at.name = name;
	at.inner_list_len = len;
	at.inner_attrs = value;
	return at;
}

attr_v2 **new_attr_list(size_t len)
{
	attr_v2 **list = (attr_v2**) malloc(sizeof(attr_v2 *) * len);
	return list;
}

char **new_operations_list(size_t len)
{
	char **list = (char**) malloc(sizeof(char *) * len);
	return list;
}

// graph

node new_graph_node(char *value)
{
	node n;
	n.value = value;
	n.next = NULL;
	return n;
}

void create_directed_edge(node *a, node *b)
{
	a->next = b;
}

graph new_graph(size_t len)
{
	graph g;
	g.len = len;
	g.list = malloc(sizeof(node *) * len);
	return g;
}

int is_in(node k, node *list, size_t v_len)
{
	for (int i = 0; i < v_len; ++i)
		if (strcmp(k.value, list[i].value) == 0)
			return 1;
	return 0;
}

void print_node_list(node *list, size_t len, char *desc)
{
	printf("%s: ", desc);
	for (int i = 0; i < len; ++i)
		printf("%s ", list[i].value);
	printf("\n");
}

node *find_ancestors_dfs(graph g, node n, size_t *v_len)
{
	node *visited = malloc(sizeof(node) * g.len); // allocate max number of nodes for simplicity
	node **stack = malloc(sizeof(node *) * g.len);
	node k;
	size_t s_head = 0, the_len = 0;
	stack[s_head++] = &n;
	while (s_head > 0) {
		k = *stack[--s_head];
		if (!is_in(k, visited, the_len)) {
			visited[the_len++] = k;
			// walk over k.next and add to stack
			while (k.next) {
				stack[s_head++] = k.next;
				k = *k.next;
			}
		}
	}

	*v_len = the_len;
	return visited;
}

void expand_attrs(rule *req, graph g)
{
	size_t v_len = 0;
	node n;
	for (int i = 0; i < req->users_len; ++i)
	{
		for (int j = 0; j < g.len; ++j)
			if (strcmp(req->users[i]->string, g.list[j]->value) == 0)
				n = *g.list[j];
		node *visited = find_ancestors_dfs(g, n, &v_len);
		show_visited(visited, v_len);
		req->users[i]->inner_list_len = v_len;
		req->users[i]->data_type = abac_string_list;

		attr_v2 at = new_attr_string_list(req->users[i]->name, v_len);
		for (int i = 0; i < v_len; ++i)
			at.string_list[i] = visited[i].value;
		req->users[i] = &at;
	}
	printf("ok\n");
}

// authorization

int is_subset(char **ro, size_t ro_len, char **po, size_t po_len)
{
	for (int i = 0; i < ro_len; i++) {
		int ok = 0;
		for (int j = 0; j < po_len; j++)
			if (strcmp(ro[i], po[j]) == 0)
				ok = 1;
		if (!ok)
			return 0;
	}
	return 1;
}

int match_attr_v2(attr_v2 ra, attr_v2 pa)
{
	if (strcmp(ra.name, pa.name) != 0)
		return 0;

	switch(pa.data_type) {
	case abac_integer:
		if (ra.integer == pa.integer)
			return 1;
		break;
	case abac_real:
		if (ra.real == pa.real)
			return 1;
		break;
	case abac_integer_range:
		if (ra.integer >= pa.ran.integer_min && ra.integer <= pa.ran.integer_max)
			return 1;
		break;
	case abac_real_range:
		if (ra.real >= pa.ran.real_min && ra.real <= pa.ran.real_max)
			return 1;
		break;
	case abac_string:
		if (strcmp(ra.string, pa.string) == 0)
			return 1;
		break;
	case abac_dictionary:
		return match_attrs_v2(ra.inner_attrs, ra.inner_list_len, pa.inner_attrs, pa.inner_list_len);
	}
	return 0;
}

int match_attrs_v2(attr_v2 **ras, size_t ras_len, attr_v2 **pas, size_t pas_len)
{
	int any_r;
	for (int i = 0; i < pas_len; i++)
	{
		any_r = 0;
		for (int j = 0; j < ras_len; j++)
			if (match_attr_v2(*(ras[i]), *(pas[i])))
				any_r = 1;
		if (!any_r)
			return 0;
	}
	if (any_r)
		return 1;
	else
		return 0;
}

int match_permission(rule r, rule perm)
{
	return 
		is_subset(r.operations, r.operations_len, perm.operations, perm.operations_len) &&
		match_attrs_v2(r.users, r.users_len, perm.users, perm.users_len) && 
		match_attrs_v2(r.objects, r.objects_len, perm.objects, perm.objects_len) && 
		match_attrs_v2(r.contexts, r.contexts_len, perm.contexts, perm.contexts_len);
}

int authorize_permissions(rule req, rule *perms, size_t p_len)
{
	for (int i = 0; i < p_len; i++)
		if (match_permission(req, perms[i]))
			return 1;
	return 0;
}

int authorize_permissions_expand(rule req, rule *perms, size_t p_len, graph g)
{
	expand_attrs(&req, g);
	for (int i = 0; i < p_len; i++)
		if (match_permission(req, perms[i]))
			return 1;
	return 0;
}

// debug

void show_attr_v2(attr_v2 at)
{
	switch(at.data_type) {
	case abac_integer:
		printf("%s: %d\n", at.name, at.integer);
		break;
	case abac_real:
		printf("%s: %.2f\n", at.name, at.real);
		break;
	case abac_integer_range:
		printf("%s: %d..%d\n", at.name, at.ran.integer_min, at.ran.integer_max);
		break;
	case abac_real_range:
		printf("%s: %.2f..%.2f\n", at.name, at.ran.real_min, at.ran.real_max);
		break;
	case abac_string:
		printf("%s: %s\n", at.name, at.string);
		break;
	case abac_string_list:
		printf("%s: ", at.name);
		for (int i = 0; i < at.inner_list_len; ++i)
			printf("%s ", at.string_list[i]);
		printf("\n");
		break;
	case abac_dictionary:
		printf("[%s:\n", at.name);
		for (int i = 0; i < at.inner_list_len; i++)
			show_attr_v2(*(at.inner_attrs[i]));
		printf("]\n");
		break;
	}
}

void show_operations(char **ops, size_t len)
{
	for (int i = 0; i < len; i++)
		printf("%s ", ops[i]);
	printf("\n");
}

void show_rule(rule r, char *desc)
{
	printf("\n>%s\n", desc);
	printf("#users:\n");
	for (int i = 0; i < r.users_len; i++)
		show_attr_v2(*(r.users[i]));
	printf("#objects:\n");
	for (int i = 0; i < r.objects_len; i++)
		show_attr_v2(*(r.objects[i]));
	printf("#contexts:\n");
	for (int i = 0; i < r.contexts_len; i++)
		show_attr_v2(*(r.contexts[i]));
	printf("#operations:\n");
	show_operations(r.operations, r.operations_len);
}

void show_visited(node *visited, size_t v_len)
{
	printf("visited: ");
	for (int i = 0; i < v_len; ++i)
	{
		printf("%s ", visited[i].value);
	}
	printf("\n");
}
