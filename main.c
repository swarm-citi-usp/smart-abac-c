#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

// Structs

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
		struct range ran;
	};
} attr;

struct policy {
	const char **operations;
	struct attr **user_attrs;
	struct attr **object_attrs;
	struct attr **context_attrs;
	size_t user_attrs_len;
	size_t object_attrs_len;
	size_t context_attrs_len;
	size_t operations_len; // FIXME: segfault happens when there's a 4th size_t field
} policy;

struct req_attr {
	char *name;
	union {
		char *str;
		float num;
	};
} req_attr;

struct request {
	struct req_attr **user_attrs;
	struct req_attr **object_attrs;
	struct req_attr **context_attrs;
	const char **operations;
} request;

// helper functions

struct attr *create_attr(json_t *attr_json) {
	json_t *t = json_array_get(attr_json, 0);
	json_t *n = json_array_get(attr_json, 1);
	json_t *v = json_array_get(attr_json, 2);

	struct attr *at = malloc(sizeof(struct attr *));
	at->data_type = json_string_value(t);
	at->name = json_string_value(n);
	if (strcmp(at->data_type, "string") == 0) {
		at->str = json_string_value(v);
		printf("create_attr: %s %s %s\n", at->data_type, at->name, at->str);
	} else if (strcmp(at->data_type, "number") == 0) {
		at->num = json_real_value(v);
		printf("create_attr: %s %s %f\n", at->data_type, at->name, at->num);
	}

	return at;
}

struct attr **create_attrs(json_t *attrs_json) {
	int j;
	struct attr **attrs = malloc(sizeof(struct attr *) * json_array_size(attrs_json));
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
	printf("load_policies: %s\n", policies_buf);

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

struct policy ***create_policies(json_t *root, size_t policies_len) {
	int i, j;
	struct policy ***policies = malloc(sizeof(struct policy *) * policies_len);
	for (i = 0; i < json_array_size(root); i++) {
		json_t *policy_json, *object_attrs_json;
		policy_json = json_array_get(root, i);

		struct attr **user_attrs = create_attrs(json_object_get(policy_json, "user_attrs"));
		struct attr **object_attrs = create_attrs(json_object_get(policy_json, "object_attrs"));
		struct attr **context_attrs = create_attrs(json_object_get(policy_json, "context_attrs"));
		printf("create_policies: %s\n", object_attrs[0]->data_type);

		json_t *operations_json = json_object_get(policy_json, "operations");
		const char **operations = malloc(sizeof(const char *) * json_array_size(operations_json));
		for (j = 0; j < json_array_size(operations_json); j++) {
			json_t *operation_json = json_array_get(operations_json, j);
			operations[j] = json_string_value(operation_json);
		}

		// FIXME: segfault happens here
		(*policies[i])->user_attrs = user_attrs;
		(*policies[i])->object_attrs = object_attrs;
		(*policies[i])->context_attrs = context_attrs;
		(*policies[i])->operations = operations;
	}
	printf("create_policies: %s\n", (*policies[0])->operations[0]);
	printf("create_policies: %s\n", (*policies[0])->object_attrs[0]->data_type);

	return policies;
}

// PDP functions

int authorize(struct request *req, struct policy ***policies, size_t policies_len) {
	printf("%s\n", req->object_attrs[0]->name);
	//printf("%s\n", (*policies[0])->object_attrs[0]->data_type);
	int i, j;
	for (i = 0; i < policies_len; i++) {
	}
	return 1;
}

int main() {
	char *policies_buf = load_policies();
	json_t *root = convert_to_json(policies_buf);
	size_t policies_len = json_array_size(root);
	struct policy ***policies = create_policies(root, policies_len);

	struct request *areq = malloc(sizeof(struct request *));
	areq->object_attrs = malloc(sizeof(struct req_attr *) * 1);
	areq->object_attrs[0]->name = "ServiceType";
	areq->object_attrs[0]->str = "Led";
	areq->operations = malloc(sizeof(char *) * 1);
	areq->operations[0] = "discover";

	if (authorize(areq, policies, policies_len))
		printf("allowed\n");
	else
		printf("denied\n");

	return 0;
}

