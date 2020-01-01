#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include "abac_them.h"
#include "util.h"

int main()
{
	size_t ps_len;
	policy *ps = load_policies("policies.json", &ps_len);
	print_policies(ps, ps_len);

	request r = new_request(1, 1, 0, 1);
	r.user_attrs[0] = new_attr_num("Day", 30.0);
	r.object_attrs[0] = new_attr_str("ServiceType\0", "Led\0");
	r.operations[0] = "discover";

	printf("\n%d\n", authorize(r, ps, ps_len));

	return 0;
}

