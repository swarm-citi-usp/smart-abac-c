#include "abac_them.h"

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

