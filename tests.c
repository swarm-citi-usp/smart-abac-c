#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include "abac_them.h"
#include "util.h"

// unit tests

void _test_create_policies()
{
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

	// print_policies(ps, ps_len);

	//printf("test ok\n");
}

void _test_match_ops()
{
	char **ro = malloc(sizeof(char *) * 1);
	ro[0] = "read";

	const char **po = malloc(sizeof(char *) * 2);
	po[0] = "read";
	po[1] = "update";

	if (!match_ops(ro, 1, po, 2))
		printf(">>>> fail 1");

	po[0] = "";
	if (match_ops(ro, 1, po, 2))
		printf(">>>> fail 2");
}

void _test_match_attrs()
{
	size_t ra_len = 1;
	req_attr *r_attrs = malloc(sizeof(req_attr) * ra_len);
	r_attrs[0].name = "Service";
	r_attrs[0].len = 1;
	r_attrs[0].strs = malloc(sizeof(char *) * r_attrs[0].len);
	r_attrs[0].strs[0] = "Camera";

	size_t pa_len = 2;
	attr *p_attrs = malloc(sizeof(attr) * pa_len);
	p_attrs[0].data_type = "string";
	p_attrs[0].name = "Service";
	p_attrs[0].str = "Camera";

	p_attrs[1].data_type = "string";
	p_attrs[1].name = "Service";
	p_attrs[1].str = "Detection";

	if (!match_attrs(r_attrs, ra_len, p_attrs, pa_len))
		printf(">>>> fail 1\n");

	p_attrs[0].str = "foobar";
	if (match_attrs(r_attrs, ra_len, p_attrs, pa_len))
		printf(">>>> fail 2\n");

	if (!match_attrs(r_attrs, 0, p_attrs, pa_len)) // no request attrs
		printf(">>>> fail 3\n");

	r_attrs[0].name = "Day";
	r_attrs[0].num = 30.0;

	p_attrs[1].data_type = "number";
	p_attrs[1].name = "Day";
	p_attrs[1].num = 30.0;

	if (!match_attrs(r_attrs, ra_len, p_attrs, pa_len))
		printf(">>>> fail 4\n");

	p_attrs[1].data_type = "range";
	p_attrs[1].name = "Day";
	p_attrs[1].ran.min = 25.0;
	p_attrs[1].ran.max = 31.0;

	if (!match_attrs(r_attrs, ra_len, p_attrs, pa_len))
		printf(">>>> fail 5\n");
}

void _test_authorize()
{
	char *policies_buf = read_policies("policies.json");
	json_t *root = convert_to_json(policies_buf);
	size_t ps_len = json_array_size(root);
	struct policy *policies = create_policies(root, ps_len);

	struct request areq;
	areq.user_attrs_len = 0;
	areq.context_attrs_len = 0;
	areq.object_attrs_len = 1;
	areq.object_attrs = malloc(sizeof(struct req_attr *) * areq.object_attrs_len);
	areq.object_attrs[0].name = "ServiceType";
	areq.object_attrs[0].len = 1;
	areq.object_attrs[0].strs = malloc(sizeof(char *) * areq.object_attrs[0].len);
	areq.object_attrs[0].strs[0] = "Led";
	areq.operations_len = 1;
	areq.operations = malloc(sizeof(char *) * areq.operations_len);
	areq.operations[0] = "discover";

	if (!authorize(areq, policies, ps_len))
		printf(">>>> fail 1\n");

	areq.operations[0] = "read";
	if (authorize(areq, policies, ps_len))
		printf(">>>> fail 2\n");
}

void _tests_paper();
void _tests_new_attrs_v2();

int main()
{
	// _test_create_policies();
	// _test_match_ops();
	// _test_match_attrs();
	// _test_authorize();

	_tests_new_attrs_v2();
}

void _tests_new_attrs_v2()
{
	attr_v2 year = new_attr_integer("year", 2020);
	show_attr_v2(year);
	attr_v2 reputation = new_attr_real("reputation", 4.5);
	show_attr_v2(reputation);
	attr_v2 hour = new_attr_integer_range("hour", 8, 18);
	show_attr_v2(hour);
	attr_v2 id = new_attr_string("id", "alice");
	show_attr_v2(id);

	attr_v2 houlseholdId = new_attr_string("id", "home-1");
	attr_v2 **at_list = new_attr_list(1);
	at_list[0] = &houlseholdId;
	attr_v2 houlsehold = new_attr_dictionary("houlsehold", at_list, 1);
	show_attr_v2(houlsehold);
}

void _tests_paper()
{
	attr at;
	at.data_type = malloc(sizeof(char) * 7);
	at.data_type = "string\0";
	at.name = malloc(sizeof(char) * 8);
	at.name = "id\0";
	at.str = malloc(sizeof(char) * 7);
	at.str = "alice\0";

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

}