#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

// Structs

struct range {
	float min;
	float max;
} range;

typedef struct attr {
	const char *data_type;
	const char *name;
	union {
		const char *str;
		float num;
		struct range ran;
	};
} attr;

typedef struct policy {
	struct attr *user_attrs;
	struct attr *object_attrs;
	struct attr *context_attrs;
	const char **operations;
	size_t user_attrs_len;
	size_t object_attrs_len;
	size_t context_attrs_len;
	size_t operations_len;
} policy;

void print_attrs(attr *attrs, size_t len, char *label) {
	int j;
	printf("%s: ", label);
	for (j = 0; j < len; j++) {
		printf("%s ", attrs[j].data_type);
		printf("%s ", attrs[j].name);
		printf("%s", attrs[j].str);
	}
	printf(" | \n");
}

void print_policies(policy *ps, size_t ps_len) {
	int i, j;
	for (i = 0; i < ps_len; i++) {
		print_attrs(ps[i].user_attrs, ps[i].user_attrs_len, "u\0");
		print_attrs(ps[i].object_attrs, ps[i].object_attrs_len, "o\0");
		print_attrs(ps[i].context_attrs, ps[i].context_attrs_len, "c\0");
		for (j = 0; j < ps[i].operations_len; j++)
			printf("%s ", ps[i].operations[j]);
		printf("\n====\n");
	}
}

void _test_create_policies() {
	attr at;
	at.data_type = malloc(sizeof(char) * 7);
	at.data_type = "string\0";
	at.name = malloc(sizeof(char) * 8);
	at.name = "Service\0";
	at.str = malloc(sizeof(char) * 7);
	at.str = "Camera\0";

	policy p;
	p.user_attrs_len = 1;
	p.user_attrs = malloc(sizeof(attr) * p.user_attrs_len);
	p.user_attrs[0] = at;
	p.object_attrs_len = 1;
	p.object_attrs = malloc(sizeof(attr) * 1);
	p.object_attrs[0] = at;
	p.context_attrs_len = 1;
	p.context_attrs = malloc(sizeof(attr) * p.context_attrs_len);
	p.context_attrs[0] = at;
	p.operations_len = 1;
	p.operations = malloc(sizeof(char *) * p.operations_len);
	p.operations[0] = malloc(sizeof(char) * 8);
	p.operations[0] = "discover\0";

	int ps_len = 2, i, j;
	policy *ps = malloc(sizeof(policy) * ps_len);
	ps[0] = p;
	ps[1] = p;

	print_policies(ps, ps_len);

	printf("test ok\n");
}

typedef struct req_attr {
	char *name;
	size_t len;
	union {
		char **strs;
		float num;
	};
} req_attr;

typedef struct request {
	struct req_attr *user_attrs;
	struct req_attr *object_attrs;
	struct req_attr *context_attrs;
	char **operations;
	size_t user_attrs_len;
	size_t object_attrs_len;
	size_t context_attrs_len;
	size_t operations_len;
} request;

// helper functions

struct attr create_attr(json_t *attr_json) {
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
	}

	return at;
}

struct attr *create_attrs(json_t *attrs_json) {
	int j;
	struct attr *attrs = malloc(sizeof(struct attr) * json_array_size(attrs_json));
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
	//printf("load_policies: %s\n", policies_buf);

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

struct policy *create_policies(json_t *root, size_t policies_len) {
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

// PDP functions

// req_ops \in p_ops
int match_ops(char **req_ops, size_t req_ops_len, const char **p_ops, size_t p_ops_len) {
	int i, j;
	for (i = 0; i < req_ops_len; i++) {
		int ok = 0;
		for (j = 0; j < p_ops_len; j++) {
			if (strcmp(req_ops[i], p_ops[j]) == 0) {
				printf("eq: %s %s\n", req_ops[i], p_ops[j]);
				ok = 1;
			}
		}
		if (!ok)
			return 0;
	}
	return 1;
}

void _test_match_ops() {
	char **ro = malloc(sizeof(char *) * 1);
	ro[0] = "read";

	const char **po = malloc(sizeof(char *) * 2);
	po[0] = "read";
	po[1] = "update";

	if (!match_ops(ro, 1, po, 2))
		printf("fail 1");

	po[0] = "";
	if (match_ops(ro, 1, po, 2))
		printf("fail 2");
}

int match_attrs(req_attr *req_attrs, attr *p_attrs) {
	return 1;
}

int authorize(request req, struct policy *ps, size_t ps_len) {
	printf("authorize: %s\n", req.object_attrs[0].name);
	print_policies(ps, ps_len);
	int i, j;
	for (i = 0; i < ps_len; i++) {
		int ok =
			match_ops(req.operations, req.operations_len, ps[i].operations, ps[i].operations_len) &&
			match_attrs(req.user_attrs, ps[i].user_attrs) &&
			match_attrs(req.user_attrs, ps[i].user_attrs) &&
			match_attrs(req.user_attrs, ps[i].user_attrs);

		if (ok)
			return 1;
	}
	return 0;
}

int main() {
	// _test_create_policies();
	// _test_match_ops();
	// return 0;

	char *policies_buf = load_policies();
	json_t *root = convert_to_json(policies_buf);
	size_t ps_len = json_array_size(root);
	struct policy *policies = create_policies(root, ps_len);

	struct request areq;
	areq.object_attrs = malloc(sizeof(struct req_attr *) * 1);
	areq.object_attrs[0].name = "ServiceType";
	areq.object_attrs[0].len = 1;
	areq.object_attrs[0].strs = malloc(sizeof(char *) * areq.object_attrs[0].len);
	areq.object_attrs[0].strs[0] = "Led";
	areq.operations_len = 1;
	areq.operations = malloc(sizeof(char *) * areq.operations_len);
	areq.operations[0] = "discover";

	if (authorize(areq, policies, ps_len))
		printf("allowed\n");
	else
		printf("denied\n");

	return 0;
}

