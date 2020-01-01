#ifndef ABAC_THEM_H
#define ABAC_THEM_H

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

int match_ops(char **req_ops, size_t req_ops_len, const char **p_ops, size_t p_ops_len);
int match_attr(req_attr ra, attr pa);
int match_attrs(req_attr *r_attrs, size_t ra_len, attr *p_attrs, size_t pa_len);
int authorize(request req, struct policy *ps, size_t ps_len);

request new_request(size_t ua_len, size_t oa_len, size_t ca_len, size_t op_len);
req_attr new_attr_str(char *name, char *value);
req_attr new_attr_num(char *name, float num);

#endif
