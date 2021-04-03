#include "iso8601.h"
#include <mpd/compiler.h>

#include <check.h>

#include <stdlib.h>
#include <time.h>

START_TEST(test_iso8601)
{
	char buffer[64];
	time_t t, now;
	bool success;

	now = time(NULL);
	success = iso8601_datetime_format(buffer, sizeof(buffer), now);
	ck_assert(success);

	t = iso8601_datetime_parse(buffer);
	ck_assert_int_eq(t, now);
}
END_TEST

static Suite *
create_suite(void)
{
	Suite *s = suite_create("iso8601");
	TCase *tc_core = tcase_create("Core");
	tcase_add_test(tc_core, test_iso8601);
	suite_add_tcase(s, tc_core);
	return s;
}

int
main(void)
{
	Suite *s = create_suite();
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	int number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
