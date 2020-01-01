#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include "abac_them.h"
#include "util.h"

void simple()
{
	size_t ps_len;
	policy *ps = load_policies("policies.json", &ps_len);
	print_policies(ps, ps_len);

	request r = new_request(1, 1, 0, 1);
	r.user_attrs[0] = new_attr_num("Day", 30.0);
	r.object_attrs[0] = new_attr_str("ServiceType\0", "Led\0");
	r.operations[0] = "discover";

	printf("\n%d\n", authorize(r, ps, ps_len));
}

void bench()
{
#include <time.h>
	clock_t t;

	size_t ps_len;
	policy *ps = load_policies("2000-100.json", &ps_len);
	// print_policies(ps, ps_len);

	request r = new_request(2, 1, 0, 1);
	r.user_attrs[0] = new_attr_num("swarm:Age\0", 25.0);
	r.user_attrs[1] = new_attr_str("swarm:Type\0", "swarm:Person\0");
	r.object_attrs[0] = new_attr_str("swarm:Type\0", "swarm:AirConditioner\0");
	r.operations[0] = "read\0";

	t = clock();
	int result = authorize(r, ps, ps_len);
	t = clock() - t;
	double elapsed = ((double) t) / CLOCKS_PER_SEC;

	printf("\nelapsed time is %f and result is %d\n", elapsed, result);
}

int main()
{
	bench();

	return 0;
}

