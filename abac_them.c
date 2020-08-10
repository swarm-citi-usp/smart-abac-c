#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abac_them.h"

#define DEBUG 0

#if DEBUG
#include "util.h"
#endif

// req_ops \in p_ops
int match_ops(char **req_ops, size_t req_ops_len, const char **p_ops, size_t p_ops_len)
{
	int i, j;
	for (i = 0; i < req_ops_len; i++) {
		int ok = 0;
		for (j = 0; j < p_ops_len; j++) {
			if (strcmp(req_ops[i], p_ops[j]) == 0) {
#if DEBUG
				printf("match: %s = %s\n", req_ops[i], p_ops[j]);
#endif
				ok = 1;
			}
		}
		if (!ok)
			return 0;
	}
	return 1;
}

int match_attr(req_attr ra, attr pa)
{
	if (strcmp(ra.name, pa.name) != 0)
		return 0;

	if (strcmp(pa.data_type, "string") == 0) {
		int i, ok = 0;
		for (i = 0; i < ra.len; i++) {
			if (strcmp(ra.strs[i], pa.str) == 0) {
				return 1;
			}
		}
	} else if (strcmp(pa.data_type, "number") == 0) {
		return ra.num == pa.num;
	} else if (strcmp(pa.data_type, "range") == 0) {
		return pa.ran.min <= ra.num && ra.num <= pa.ran.max;
	}
	return 0;
}

int match_attrs(req_attr *r_attrs, size_t ra_len, attr *p_attrs, size_t pa_len)
{
	if (pa_len == 0)
		return 1;

	int i, j;
	for (i = 0; i < ra_len; i++) {
		int ok = 0;
		for (j = 0; j < pa_len; j++) {
			if (match_attr(r_attrs[i], p_attrs[j])) {
#if DEBUG
				printf("match: ");
				print_req_attr(r_attrs[i], p_attrs[j].data_type);
				printf(" = ");
				print_attr(p_attrs[j]);
				printf("\n");
#endif
				ok = 1;
			}
		}
		if (!ok)
			return 0;
	}
	return 1;
}

int authorize(request r, struct policy *ps, size_t ps_len)
{
	//print_policies(ps, ps_len);
	int i, j;
	for (i = 0; i < ps_len; i++) {
		int ok =
			match_ops(r.operations, r.operations_len, ps[i].operations, ps[i].operations_len) &&
			match_attrs(r.user_attrs, r.user_attrs_len, ps[i].user_attrs, ps[i].user_attrs_len) &&
			match_attrs(r.object_attrs, r.object_attrs_len, ps[i].object_attrs, ps[i].object_attrs_len) &&
			match_attrs(r.context_attrs, r.context_attrs_len, ps[i].context_attrs, ps[i].context_attrs_len);

		if (ok)
			return 1;
	}
	return 0;
}

// create requests

request new_request(size_t ua_len, size_t oa_len, size_t ca_len, size_t op_len)
{
	request r;
	r.user_attrs_len = ua_len;
	r.user_attrs = malloc(sizeof(req_attr) * r.user_attrs_len);
	r.object_attrs_len = oa_len;
	r.object_attrs = malloc(sizeof(req_attr) * r.object_attrs_len);
	r.context_attrs_len = ca_len;
	r.context_attrs = malloc(sizeof(req_attr) * r.context_attrs_len);
	r.operations_len = 1;
	r.operations = malloc(sizeof(char *) * r.operations_len);

	return r;
}

req_attr new_attr_str(char *name, char *value)
{
	req_attr at;
	at.name = name;
	at.len = 1;
	at.strs = malloc(sizeof(char *) * at.len);
	at.strs[0] = value;
	return at;
}

req_attr new_attr_num(char *name, float num)
{
	req_attr at;
	at.name = name;
	at.num = num;
	return at;
}

// create attrs_v2

attr_v2 new_attr_integer(char *name, int value)
{
	attr_v2 at;
	at.data_type = integer;
	at.name = name;
	at.integer = value;
	return at;
}

attr_v2 new_attr_real(char *name, float value)
{
	attr_v2 at;
	at.data_type = real;
	at.name = name;
	at.real = value;
	return at;
}

attr_v2 new_attr_integer_range(char *name, int min, int max)
{
	attr_v2 at;
	at.data_type = integer_range;
	at.name = name;
	at.ran.integer_min = min;
	at.ran.integer_max = max;
	return at;
}

attr_v2 new_attr_real_range(char *name, float min, float max)
{
	attr_v2 at;
	at.data_type = real_range;
	at.name = name;
	at.ran.real_min = min;
	at.ran.real_max = max;
	return at;
}

attr_v2 new_attr_string(char *name, char *value)
{
	attr_v2 at;
	at.data_type = string;
	at.name = name;
	at.string = value;
	return at;
}

attr_v2 **new_attr_list(size_t len)
{
	attr_v2 **list = malloc(sizeof(attr_v2 *) * len);
	return list;
}

attr_v2 new_attr_dictionary(char *name, attr_v2 **value, size_t len)
{
	attr_v2 at;
	at.data_type = dictionary;
	at.name = name;
	at.inner_list_len = len;
	at.inner_attrs = value;
	return at;
}

// debug attrs_v2
void show_attr_v2(attr_v2 at)
{
	switch(at.data_type) {
	case integer:
		printf("%s: %d\n", at.name, at.integer);
		break;
	case real:
		printf("%s: %.2f\n", at.name, at.real);
		break;
	case integer_range:
		printf("%s: %d..%d\n", at.name, at.ran.integer_min, at.ran.integer_max);
		break;
	case real_range:
		printf("%s: %.2f..%.2f\n", at.name, at.ran.real_min, at.ran.real_max);
		break;
	case string:
		printf("%s: %s\n", at.name, at.string);
		break;
	case dictionary:
		printf("\n%s:\n", at.name);
		int i = 0;
		for (i = 0; i < at.inner_list_len; i++)
			show_attr_v2(*(at.inner_attrs[i]));
		printf("\n");
		break;
	}
}