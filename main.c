#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

struct range {
	float min;
	float max;
} range;

struct attr {
	const char *data_type;
	const char *name;
	union {
		const char *str;
		float num;
	};
} attr;

struct policy {
	struct attr **user_attrs;
	struct attr **object_attrs;
	struct attr **context_attrs;
	const char **operations;
} policy;

struct attr *create_attr(json_t *attr_json) {
	json_t *t = json_array_get(attr_json, 0);
	json_t *n = json_array_get(attr_json, 1);
	json_t *v = json_array_get(attr_json, 2);
	printf("%s %s %s\n", json_string_value(t), json_string_value(n), json_string_value(v));

	struct attr *at = malloc(sizeof(attr));
	at->data_type = json_string_value(t);
	at->name = json_string_value(n);
	if (strcmp(at->data_type, "string") == 0) {
		at->str = json_string_value(v);
		printf("%s %s %s\n", at->data_type, at->name, at->str);
	} else if (strcmp(at->data_type, "number") == 0) {
		at->num = json_real_value(v);
		printf("%s %s %f\n", at->data_type, at->name, at->num);
	}

	return at;
}

struct attr **create_attrs(json_t *attrs_json) {
	int j;
	struct attr **attrs = malloc(sizeof(attr) * json_array_size(attrs_json));
	for (j = 0; j < json_array_size(attrs_json); j++) {
		json_t *attr_json = json_array_get(attrs_json, j);
		attrs[j] = create_attr(attr_json);
	}

	return attrs;
}

char *load_policies() {
	FILE *f = fopen("policies.json", "rb");
	char *policies_buf = 0;
	long length;


	if (!f) {
		fprintf(stderr, "error: could not open file");
		exit(1);
	}
	fseek(f, 0, SEEK_END);
	length = ftell(f);
	fseek(f, 0, SEEK_SET);
	policies_buf = malloc(length);
	if (policies_buf)
		fread(policies_buf, 1, length, f);
	fclose(f);
	printf("%s\n", policies_buf);

	return policies_buf;
}

json_t *convert_to_json(char *policies_buf) {
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

struct policy ***create_policies(json_t *root) {
	int i, j;
	struct policy ***policies = malloc(sizeof(policy) * json_array_size(root));
	for (i = 0; i < json_array_size(root); i++) {
		json_t *policy_json, *object_attrs_json;
		policy_json = json_array_get(root, i);

		struct attr **user_attrs = create_attrs(json_object_get(policy_json, "user_attrs"));
		struct attr **object_attrs = create_attrs(json_object_get(policy_json, "object_attrs"));
		struct attr **context_attrs = create_attrs(json_object_get(policy_json, "context_attrs"));
		printf("%s\n", object_attrs[0]->data_type);

		json_t *operations_json = json_object_get(policy_json, "operations");
		const char **operations = malloc(sizeof(const char *) * json_array_size(operations_json));
		for (j = 0; j < json_array_size(operations_json); j++) {
			json_t *operation_json = json_array_get(operations_json, j);
			operations[j] = json_string_value(operation_json);
		}

		(*policies[i])->user_attrs = user_attrs;
		(*policies[i])->object_attrs = object_attrs;
		(*policies[i])->context_attrs = context_attrs;
		(*policies[i])->operations = operations;
	}
	printf("%s\n", (*policies[0])->object_attrs[0]->data_type);
	printf("%s\n", (*policies[0])->operations[0]);

	return policies;
}

int main() {
	char *policies_buf = load_policies();
	json_t *root = convert_to_json(policies_buf);
	struct policy ***policies = create_policies(root);

	return 0;
}
