#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abac_them.h"

void _test_v2();
void _test_paper();

int main()
{
	// _test_v2();
	_test_paper();
}

void _test_paper()
{
	attr_v2 **at_list;

	attr_v2 id_alice = new_attr_string("id", "alice");
	attr_v2 id_camera1 = new_attr_string("id", "camera1");
	attr_v2 id_lamp1 = new_attr_string("id", "lamp1");
	attr_v2 id_some_device_x = new_attr_string("id", "some-device-x");
	attr_v2 owner_alice = new_attr_string("owner", "alice");
	attr_v2 year_2020 = new_attr_integer("year", 2020);
	attr_v2 month_6 = new_attr_integer("month", 6);
	attr_v2 day_30 = new_attr_integer("day", 30);
	attr_v2 hour_17 = new_attr_integer("hour", 17);
	attr_v2 luminosity_25 = new_attr_integer("outdoorLuminosity", 25);
	attr_v2 age_min18 = new_attr_integer_range("age", 18, 120);
	attr_v2 minute_20_25 = new_attr_integer_range("minute", 20, 25);
	attr_v2 luminosity_max33 = new_attr_integer_range("outdoorLuminosity", 0, 33);
	attr_v2 reputation_min4 = new_attr_real_range("reputation", 4, 5);
	attr_v2 type_security = new_attr_string("type", "securityAppliance");
	attr_v2 type_lighting = new_attr_string("type", "lightingAppliance");
	attr_v2 household_role_child = new_attr_string("role", "child");
	attr_v2 household_id_home1 = new_attr_string("id", "home-1");
	attr_v2 type_camera = new_attr_string("type", "securityCamera");
	attr_v2 location_outdoor = new_attr_string("location", "outdoor");

	at_list = new_attr_list(1);
	at_list[0] = &household_id_home1;
	attr_v2 household_with_id = new_attr_dictionary("household", at_list, 1);

	at_list = new_attr_list(2);
	at_list[0] = &household_id_home1;
	at_list[1] = &household_role_child;
	attr_v2 household_with_id_role = new_attr_dictionary("household", at_list, 2);

	// p1
	rule perm1;
	perm1.users = new_attr_list(1);
	perm1.users_len = 1;
	perm1.users[0] = &id_alice;

	perm1.objects = new_attr_list(1);
	perm1.objects_len = 1;
	perm1.objects[0] = &owner_alice;

	perm1.contexts_len = 0;

	perm1.operations = new_operations_list(4);
	perm1.operations_len = 4;
	perm1.operations[0] = "create";
	perm1.operations[1] = "read";
	perm1.operations[2] = "update";
	perm1.operations[3] = "delete";
	show_rule(perm1, "perm1\0");

	// p2
	rule perm2;
	perm2.users = new_attr_list(2);
	perm2.users_len = 2;
	perm2.users[0] = &age_min18;
	perm2.users[1] = &household_with_id;

	perm2.objects = new_attr_list(2);
	perm2.objects_len = 2;
	perm2.objects[0] = &type_security;
	perm2.objects[1] = &household_with_id;

	perm2.contexts_len = 0;

	perm2.operations = new_operations_list(2);
	perm2.operations_len = 2;
	perm2.operations[0] = "read";
	perm2.operations[1] = "update";
	show_rule(perm2, "perm2\0");

	// p3
	rule perm3;
	perm3.users = new_attr_list(1);
	perm3.users_len = 1;
	perm3.users[0] = &household_with_id_role;

	perm3.objects = new_attr_list(2);
	perm3.objects_len = 2;
	perm3.objects[0] = &type_lighting;
	perm3.objects[1] = &household_with_id;

	perm3.contexts = new_attr_list(1);
	perm3.contexts_len = 1;
	perm3.contexts[0] = &luminosity_max33;

	perm3.operations = new_operations_list(2);
	perm3.operations_len = 2;
	perm3.operations[0] = "read";
	perm3.operations[1] = "update";
	show_rule(perm3, "perm3\0");

	// p4
	rule perm4;
	perm4.users = new_attr_list(1);
	perm4.users_len = 1;
	perm4.users[0] = &id_camera1;

	perm4.objects = new_attr_list(1);
	perm4.objects_len = 1;
	perm4.objects[0] = &id_lamp1;

	perm4.contexts_len = 0;

	perm4.operations = new_operations_list(2);
	perm4.operations_len = 2;
	perm4.operations[0] = "read";
	perm4.operations[1] = "update";
	show_rule(perm4, "perm4\0");

	// p5
	rule perm5;
	perm5.users = new_attr_list(1);
	perm5.users_len = 1;
	perm5.users[0] = &reputation_min4;

	perm5.objects = new_attr_list(3);
	perm5.objects_len = 3;
	perm5.objects[0] = &type_camera;
	perm5.objects[1] = &household_with_id;
	perm5.objects[2] = &location_outdoor;

	perm5.contexts = new_attr_list(1);
	perm5.contexts_len = 1;
	perm5.contexts[0] = &luminosity_max33;

	perm5.operations = new_operations_list(1);
	perm5.operations_len = 1;
	perm5.operations[0] = "contract";
	show_rule(perm5, "perm5\0");

	// p6
	rule perm6;
	perm6.users = new_attr_list(1);
	perm6.users_len = 1;
	perm6.users[0] = &id_some_device_x;

	perm6.objects = new_attr_list(5);
	perm6.objects_len = 5;
	perm6.objects[0] = &year_2020;
	perm6.objects[1] = &month_6;
	perm6.objects[2] = &day_30;
	perm6.objects[3] = &hour_17;
	perm6.objects[4] = &minute_20_25;

	perm6.contexts = new_attr_list(1);
	perm6.contexts_len = 1;
	perm6.contexts[0] = &luminosity_max33;

	perm6.operations = new_operations_list(1);
	perm6.operations_len = 1;
	perm6.operations[0] = "contract";
	show_rule(perm6, "perm6\0");

	// list of perms
	rule *perms = (rule *) malloc(sizeof(rule) * 6);
	perms[0] = perm1;
	perms[1] = perm2;
	perms[2] = perm3;
	perms[3] = perm4;
	perms[4] = perm5;
	perms[5] = perm6;

	// a request
	rule req;
	req.users = new_attr_list(1);
	req.users_len = 1;
	req.users[0] = &household_with_id_role;

	req.objects = new_attr_list(2);
	req.objects_len = 2;
	req.objects[0] = &type_lighting;
	req.objects[1] = &household_with_id;

	req.contexts = new_attr_list(1);
	req.contexts_len = 1;
	req.contexts[0] = &luminosity_25;

	req.operations = new_operations_list(1);
	req.operations_len = 1;
	req.operations[0] = "read";
	show_rule(req, "request\0");

	if (authorize_permissions(req, perms, 6))
		printf("\nauthorized crafted request for policy #3\n");
}

void _test_v2()
{
	attr_v2 year = new_attr_integer("year", 2020);
	attr_v2 reputation = new_attr_real("reputation", 4.5);
	attr_v2 hour_range = new_attr_integer_range("hour", 8, 18);
	attr_v2 id_alice = new_attr_string("id", "alice");

	attr_v2 householdId = new_attr_string("id", "home-1");
	attr_v2 **at_list = new_attr_list(1);
	at_list[0] = &householdId;
	attr_v2 household = new_attr_dictionary("household", at_list, 1);

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
