#ifndef PDP_H
#define PDP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/*int match_ops(char **req_ops, size_t req_ops_len, const char **p_ops, size_t p_ops_len);
int match_attr(req_attr ra, attr pa);
int match_attrs(req_attr *r_attrs, size_t ra_len, attr *p_attrs, size_t pa_len);
int authorize(request req, struct policy *ps, size_t ps_len);*/

#define DEBUG 0

#if DEBUG
#include "util.h"
#endif

// req_ops \in p_ops
int match_ops(char **req_ops, size_t req_ops_len, const char **p_ops, size_t p_ops_len) {
	int i, j;
	for (i = 0; i < req_ops_len; i++) {
		int ok = 0;
		for (j = 0; j < p_ops_len; j++) {
			if (strcmp(req_ops[i], p_ops[j]) == 0) {
				printf("match: %s = %s\n", req_ops[i], p_ops[j]);
				ok = 1;
			}
		}
		if (!ok)
			return 0;
	}
	return 1;
}

int match_attr(req_attr ra, attr pa) {
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

int match_attrs(req_attr *r_attrs, size_t ra_len, attr *p_attrs, size_t pa_len) {
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

int authorize(request req, struct policy *ps, size_t ps_len) {
	//print_policies(ps, ps_len);
	int i, j;
	for (i = 0; i < ps_len; i++) {
		int ok =
			match_ops(req.operations, req.operations_len, ps[i].operations, ps[i].operations_len) &&
			match_attrs(req.user_attrs, req.user_attrs_len, ps[i].user_attrs, ps[i].user_attrs_len) &&
			match_attrs(req.object_attrs, req.object_attrs_len, ps[i].object_attrs, ps[i].object_attrs_len) &&
			match_attrs(req.context_attrs, req.context_attrs_len, ps[i].context_attrs, ps[i].context_attrs_len);

		if (ok)
			return 1;
	}
	return 0;
}

#endif

