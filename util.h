#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include "abac_them.h"

// print, debug

void print_attr(attr a)
{
	printf("%s ", a.data_type);
	printf("%s ", a.name);
	if (strcmp(a.data_type, "string") == 0)
		printf("%s", a.str);
	else if (strcmp(a.data_type, "number") == 0)
		printf("%.2f", a.num);
	else if (strcmp(a.data_type, "range") == 0)
		printf("(%.2f, %.2f)", a.ran.min, a.ran.max);
}

void print_attrs(attr *attrs, size_t len, char *label)
{
	int j;
	printf("%s: ", label);
	for (j = 0; j < len; j++) {
		print_attr(attrs[j]);
	}
	printf(" | \n");
}

void print_policies(policy *ps, size_t ps_len)
{
	int i, j;
	printf("\nPolicies:\n");
	for (i = 0; i < ps_len; i++) {
		print_attrs(ps[i].user_attrs, ps[i].user_attrs_len, "u\0");
		print_attrs(ps[i].object_attrs, ps[i].object_attrs_len, "o\0");
		print_attrs(ps[i].context_attrs, ps[i].context_attrs_len, "c\0");
		printf("o: ");
		for (j = 0; j < ps[i].operations_len; j++)
			printf("%s ", ps[i].operations[j]);
		printf("\n====\n");
	}
}

void print_req_attr(req_attr a, const char *data_type)
{
	printf("%s [", a.name);
	int i;

	if (strcmp(data_type, "string") == 0)
		for (i = 0; i < a.len; i++)
			printf("%s ", a.strs[i]);
	else if (strcmp(data_type, "number") == 0 || strcmp(data_type, "range") == 0)
		printf("%.2f", a.num);

	printf("] ");
}

void print_request(request r)
{
	printf("\nRequest:\n");
	// TODO: the 'polymorphism' with the union and lack of data type in req_attr is
	// a problem, as there is no way to known the type of the data in the req_attr
	printf("\n====\n");
}

// file loading and json parsing functions

char *read_policies(char *filename)
{
	FILE *f = fopen(filename, "rb");
	char *policies_buf = 0;
	long length;


	if (!f) {
		fprintf(stderr, "error: could not open file\n");
		exit(1);
	}
	fseek(f, 0, SEEK_END);
	length = ftell(f);
	fseek(f, 0, SEEK_SET);
	policies_buf = malloc(length);
	if (policies_buf)
		fread(policies_buf, 1, length, f);
	fclose(f);
	//printf("read_policies: %s\n", policies_buf);

	return policies_buf;
}

json_t *convert_to_json(char *policies_buf)
{
	json_t *root;
	json_error_t error;

	root = json_loads(policies_buf, 0, &error);

	if (!root) {
		fprintf(stderr, "error: root error\n");
		exit(1);
	}

	if (!json_is_array(root)) {
		fprintf(stderr, "error: root should be array\n");
		exit(1);
	}

	return root;
}

struct attr create_attr(json_t *attr_json)
{
	json_t *t = json_array_get(attr_json, 0);
	json_t *n = json_array_get(attr_json, 1);
	json_t *v = json_array_get(attr_json, 2);

	struct attr at;
	at.data_type = json_string_value(t);
	at.name = json_string_value(n);
	if (strcmp(at.data_type, "string") == 0) {
		at.str = json_string_value(v);
		//printf("create_attr: %s %s %s\n", at.data_type, at.name, at.str);
	} else if (strcmp(at.data_type, "number") == 0) {
		at.num = json_real_value(v);
		//printf("create_attr: %s %s %f\n", at.data_type, at.name, at.num);
	} else if (strcmp(at.data_type, "range") == 0) {
		at.ran.min = json_real_value(json_object_get(v, "min"));
		at.ran.max = json_real_value(json_object_get(v, "max"));
		//printf("%s %.2f %.2f\n", at.name, at.ran.min, at.ran.max);
	}

	return at;
}

struct attr *create_attrs(json_t *attrs_json)
{
	int j;
	struct attr *attrs = malloc(sizeof(struct attr) * json_array_size(attrs_json));
	for (j = 0; j < json_array_size(attrs_json); j++) {
		json_t *attr_json = json_array_get(attrs_json, j);
		attrs[j] = create_attr(attr_json);
	}

	return attrs;
}

struct policy *create_policies(json_t *root, size_t policies_len)
{
	int i, j;
	struct policy *policies = malloc(sizeof(struct policy) * policies_len);
	for (i = 0; i < json_array_size(root); i++) {
		json_t *policy_json;
		policy_json = json_array_get(root, i);

		json_t *user_attrs_json = json_object_get(policy_json, "user_attrs");
		policies[i].user_attrs = create_attrs(user_attrs_json);
		policies[i].user_attrs_len = json_array_size(user_attrs_json);

		json_t *object_attrs_json = json_object_get(policy_json, "object_attrs");
		policies[i].object_attrs = create_attrs(object_attrs_json);
		policies[i].object_attrs_len = json_array_size(object_attrs_json);

		json_t *context_attrs_json = json_object_get(policy_json, "context_attrs");
		policies[i].context_attrs = create_attrs(context_attrs_json);
		policies[i].context_attrs_len = json_array_size(context_attrs_json);

		json_t *operations_json = json_object_get(policy_json, "operations");
		policies[i].operations_len = json_array_size(operations_json);
		policies[i].operations = malloc(sizeof(const char *) * policies[i].operations_len);
		for (j = 0; j < policies[i].operations_len; j++) {
			json_t *operation_json = json_array_get(operations_json, j);
			policies[i].operations[j] = json_string_value(operation_json);
		}
	}
	//printf("create_policies: %s\n", policies[0].operations[0]);
	//printf("create_policies: %s\n", policies[0].object_attrs[0].data_type);

	return policies;
}

policy *load_policies(char *filename, size_t *ps_len)
{
	char *policies_buf = read_policies(filename);
	json_t *root = convert_to_json(policies_buf);
	*ps_len = json_array_size(root);
	return create_policies(root, *ps_len);
}

#endif
