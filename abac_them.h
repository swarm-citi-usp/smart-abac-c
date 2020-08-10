#ifndef ABAC_THEM_H
#define ABAC_THEM_H

enum abac_types {integer, real, integer_range, real_range, string, dictionary};

struct range {
	float min;
	float max;
} range;

struct range_v2 {
	union {
		float real_min;
		int integer_min;
	};
	union {
		float real_max;
		int integer_max;
	};
} range_v2;

typedef struct attr {
	const char *data_type;
	const char *name;
	union {
		const char *str;
		float num;
		struct range ran;
	};
} attr;

typedef struct attr_v2 {
	enum abac_types data_type;
	const char *name;
	size_t inner_list_len; // used for either strings or inner_attrs
	union {
		const char *string;
		char **strings;
		int integer;
		float real;
		struct range_v2 ran;
		struct attr_v2 **inner_attrs;
	};
} attr_v2;


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

attr_v2 new_attr_integer(char *name, int value);
attr_v2 new_attr_real(char *name, float value);
attr_v2 new_attr_integer_range(char *name, int min, int max);
attr_v2 new_attr_real_range(char *name, float min, float max);
attr_v2 new_attr_string(char *name, char *value);
attr_v2 new_attr_dictionary(char *name, attr_v2 **value, size_t len);
attr_v2 **new_attr_list(size_t len);
void show_attr_v2(attr_v2 at);

#endif
