#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abac_them.h"

void _test_v2();

int main()
{
	_test_v2();
}

void _test_v2()
{
	attr_v2 year = new_attr_integer("year", 2020);
	show_attr_v2(year);
	attr_v2 reputation = new_attr_real("reputation", 4.5);
	show_attr_v2(reputation);
	attr_v2 hour_range = new_attr_integer_range("hour", 8, 18);
	show_attr_v2(hour_range);
	attr_v2 id_alice = new_attr_string("id", "alice");
	show_attr_v2(id_alice);

	attr_v2 householdId = new_attr_string("id", "home-1");
	attr_v2 **at_list = new_attr_list(1);
	at_list[0] = &householdId;
	attr_v2 household = new_attr_dictionary("household", at_list, 1);
	show_attr_v2(household);

	// creating a permission rule
	rule perm;
	perm.users = new_attr_list(1);
	perm.users_len = 1;
	perm.users[0] = &id_alice;

	attr_v2 owner_alice = new_attr_string("owner", "alice");
	perm.objects = new_attr_list(1);
	perm.objects_len = 1;
	perm.objects[0] = &owner_alice;

	perm.contexts_len = 0;

	perm.operations = new_operations_list(4);
	perm.operations_len = 4;
	perm.operations[0] = "create";
	perm.operations[1] = "read";
	perm.operations[2] = "update";
	perm.operations[3] = "delete";

	show_rule(perm, "p1\0");

	// creating a request
	rule req;
	req.users = new_attr_list(1);
	req.users_len = 1;
	req.users[0] = &id_alice;

	req.objects = new_attr_list(1);
	req.objects_len = 1;
	req.objects[0] = &owner_alice;

	req.contexts_len = 0;

	req.operations = new_operations_list(1);
	req.operations_len = 1;
	req.operations[0] = "read";

	show_rule(req, "request\0");

	// data_type = string
	if (!match_attr_v2(id_alice, id_alice))
		printf("fail match_attr_v2 string id_alice\n");

	// data_type = integer
	if (!match_attr_v2(year, year))
		printf("fail match_attr_v2 integer year\n");

	// data_type = real
	if (!match_attr_v2(reputation, reputation))
		printf("fail match_attr_v2 real reputation\n");

	// data_type = integer_range
	attr_v2 hour = new_attr_integer("hour", 12);
	if (!match_attr_v2(hour, hour_range))
		printf("fail match_attr_v2 range hour\n");

	// data_type = dictionary
	if (!match_attr_v2(household, household))
		printf("fail match_attr_v2 dictionary household\n");

	at_list = new_attr_list(1);
	at_list[0] = &id_alice;
	attr_v2 household_wrong = new_attr_dictionary("household", at_list, 1);
	if (match_attr_v2(household_wrong, household))
		printf("fail match_attr_v2 dictionary wrong\n");

	// match many attributes
	if (!match_attrs_v2(req.users, req.users_len, perm.users, perm.users_len))
		printf("fail match_attrs_v2 users\n");
	if (match_attrs_v2(req.users, req.users_len, perm.objects, perm.objects_len))
		printf("fail match_attrs_v2 users\n");

	// match permission
	if (!match_permission(req, perm))
		printf("fail authorize_permission 1\n");

	// authorize permissions
	rule *perms = (rule *) malloc(sizeof(rule) * 1);
	perms[0] = perm;
	if (!authorize_permissions(req, perms, 1))
		printf("fail authorize_permission 1\n");
	req.operations[0] = "wrong";
	if (authorize_permissions(req, perms, 1))
		printf("fail authorize_permission 1 wrong\n");
}
