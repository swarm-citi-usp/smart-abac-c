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

	// p2, authorizes req_e
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

	// p3, authorizes req
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
		printf("\nauthorized request for policy #3\n");

	// creating a graph

	node n_child = new_graph_node("child");
	node n_father = new_graph_node("father");
	node n_mother = new_graph_node("mother");
	node n_adultFamilyMember = new_graph_node("adultFamilyMember");
	node n_family_member = new_graph_node("familyMember");
	node n_person = new_graph_node("person");
	create_directed_edge(&n_child, &n_family_member);
	create_directed_edge(&n_father, &n_adultFamilyMember);
	create_directed_edge(&n_mother, &n_adultFamilyMember);
	create_directed_edge(&n_adultFamilyMember, &n_family_member);
	create_directed_edge(&n_family_member, &n_person);

	node n_securityCamera = new_graph_node("securityCamera");
	node n_intrusionAlarm = new_graph_node("intrusionAlarm");
	node n_securityAppliance = new_graph_node("securityAppliance");
	create_directed_edge(&n_securityCamera, &n_securityAppliance);
	create_directed_edge(&n_intrusionAlarm, &n_securityAppliance);

	graph g = new_graph(6+3);
	g.list[0] = &n_child;
	g.list[1] = &n_father;
	g.list[2] = &n_mother;
	g.list[3] = &n_adultFamilyMember;
	g.list[4] = &n_family_member;
	g.list[5] = &n_person;
	g.list[6] = &n_securityCamera;
	g.list[7] = &n_intrusionAlarm;
	g.list[8] = &n_securityAppliance;

	// a request to expand
	rule req_e;
	req_e.users = new_attr_list(2);
	req_e.users_len = 2;
	attr_v2 age_25 = new_attr_integer("age", 25);
	req_e.users[0] = &age_25;
	req_e.users[1] = &household_with_id;

	req_e.objects = new_attr_list(2);
	req_e.objects_len = 2;
	req_e.objects[0] = &type_camera;
	req_e.objects[1] = &household_with_id;

	req_e.contexts_len = 0;

	req_e.operations = new_operations_list(1);
	req_e.operations_len = 1;
	req_e.operations[0] = "read";
	show_rule(req_e, "request that will be expanded\0");

	if (!authorize_permissions(req_e, perms, 6))
		printf("\ndenied non-expanded request for policy #2\n");
	if (authorize_permissions_expand(req_e, perms, 6, g))
		printf("\nauthorized expanded request for policy #2\n\n");

	// many policies
	int n_perms = 3000, median;
	median = (int) (n_perms / 2);
	rule *many_perms = (rule *) malloc(sizeof(rule) * n_perms);
	for (int i = 0; i < n_perms; ++i)
		many_perms[i] = perm5;
	many_perms[median] = perm2;

	// benchmark

	int runs = 3000;
#ifdef MBED_MAJOR_VERSION
	Timer t;
	t.start();
	for (int i = 0; i < runs; i++)
		authorize_permissions_expand(req_e, perms, 6, g);
    t.stop();
    pc.printf("The time taken to authorize 1 request against 6 policies, %d times, was %f ms\n", runs, t.read() * 1000);

	t.start();
	authorize_permissions_expand(req_e, many_perms, n_perms, g);
    t.stop();
    pc.printf("The time taken to authorize 1 request against %d policies was %f ms\n", runs, t.read() * 1000);
#elif defined(ESP32)
    unsigned long startTime, endTime;
    startTime = millis();
	for (int i = 0; i < runs; i++)
		authorize_permissions_expand(req_e, perms, 6, g);
    endTime = millis();
    Serial.print("The time taken to authorize 1 request against 6 policies, ");
    Serial.print(runs);
    Serial.print(" times, was ");
    Serial.print(endTime - startTime);
    Serial.println(" ms");

    startTime = millis();
	authorize_permissions_expand(req_e, many_perms, n_perms, g);
    endTime = millis();
    Serial.print("The time taken to authorize 1 request against ");
    Serial.print(n_perms);
    Serial.print(" policies was ");
    Serial.print(endTime - startTime);
    Serial.println(" ms");
#elif defined(__unix__)
	#include <time.h>
	clock_t t;
	t = clock();
	double elapsed;
	for (int i = 0; i < runs; i++)
		authorize_permissions_expand(req_e, perms, 6, g);
	t = clock() - t;
	elapsed = ((double) t) / CLOCKS_PER_SEC;
	printf("The time taken to authorize 1 request against 6 policies, %d times, was %f ms\n", runs, elapsed * 1000);

	t = clock();
	authorize_permissions_expand(req_e, many_perms, n_perms, g);
	t = clock() - t;
	elapsed = ((double) t) / CLOCKS_PER_SEC;
	printf("The time taken to authorize 1 request against %d policies was %f ms\n", n_perms, elapsed * 1000);
#endif

	free(many_perms);
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
		printf("fail authorize_permissions 1\n");
	req.operations[0] = "wrong";
	if (authorize_permissions(req, perms, 1))
		printf("fail authorize_permissions 1 wrong\n");

	// manually expand attribute
	attr_v2 k_v = new_attr_string("k", "v");
	show_attr_v2(k_v);
	k_v.inner_list_len = 2;
	k_v.data_type = abac_string_list;
	k_v.string_list = (char **) malloc(sizeof(char *) * 2);
	k_v.string_list[0] = "v0";
	k_v.string_list[1] = "v1";
	show_attr_v2(k_v);

	// creating a graph

	node n_child = new_graph_node("child");
	node n_father = new_graph_node("father");
	node n_mother = new_graph_node("mother");
	node n_adultFamilyMember = new_graph_node("adultFamilyMember");
	node n_family_member = new_graph_node("familyMember");
	node n_person = new_graph_node("person");

	create_directed_edge(&n_child, &n_family_member);
	create_directed_edge(&n_father, &n_adultFamilyMember);
	create_directed_edge(&n_mother, &n_adultFamilyMember);
	create_directed_edge(&n_adultFamilyMember, &n_family_member);
	create_directed_edge(&n_family_member, &n_person);

	graph g = new_graph(6);
	g.list[0] = &n_child;
	g.list[1] = &n_father;
	g.list[2] = &n_mother;
	g.list[3] = &n_adultFamilyMember;
	g.list[4] = &n_family_member;
	g.list[5] = &n_person;
	printf("%s == %s\n", g.list[4]->value, n_family_member.value);

	size_t v_len;
	node *visited = find_ancestors_dfs(g, n_father, &v_len);
	if (v_len != 4)
		printf("fail find_ancestors_dfs\n");

	attr_v2 role_familyMember = new_attr_string("role", "familyMember");
	attr_v2 role_mother = new_attr_string("role", "mother");
	req.operations[0] = "read";
	req.users[0] = &role_mother;
	perm.users[0] = &role_familyMember;
	show_rule(perms[0], "perm updated");
	show_rule(req, "req for expansion");

	expand_attrs(&req, g);
	show_rule(req, "req after expansion");

	if (!authorize_permissions_expand(req, perms, 1, g))
		printf("fail authorize_permissions_expand 1\n");

	req.users[0] = &id_alice;
	if (authorize_permissions_expand(req, perms, 1, g))
		printf("fail authorize_permissions_expand 2\n");
}
