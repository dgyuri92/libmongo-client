#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "bson.h"
#include "test.h"
#include "test-generator.h"

static gboolean
test_bson_reader_flat (void)
{
  bson *b;
  bson_cursor *c;

  gint32 i;
  gboolean bool;
  const gchar *s;
  gint64 l;
  gdouble d;

  b = test_bson_generate_flat ();

  TEST (bson_reader_find_string);
  g_assert ((c = bson_find (b, "hello")));
  g_assert_cmpint (bson_cursor_type (c), ==, BSON_TYPE_STRING);
  g_assert (bson_cursor_get_string (c, &s));
  g_assert_cmpstr (s, ==, "world");
  g_free (c);
  PASS ();

  TEST (bson_reder_find_boolean_true);
  g_assert ((c = bson_find (b, "TRUE")));
  g_assert_cmpint (bson_cursor_type (c), ==, BSON_TYPE_BOOLEAN);
  g_assert (bson_cursor_get_boolean (c, &bool));
  g_assert (bool == TRUE);
  g_free (c);
  PASS ();

  TEST (bson_reader_find_null);
  g_assert ((c = bson_find (b, "null")));
  g_assert_cmpint (bson_cursor_type (c), ==, BSON_TYPE_NULL);
  g_free (c);
  PASS ();

  TEST (bson_reader_find_int32);
  g_assert ((c = bson_find (b, "int32")));
  g_assert_cmpint (bson_cursor_type (c), ==, BSON_TYPE_INT32);
  g_assert (bson_cursor_get_int32 (c, &i));
  g_assert_cmpint (i, ==, 1984);
  g_free (c);
  PASS ();

  TEST (bson_reader_find_boolean_false);
  g_assert ((c = bson_find (b, "FALSE")));
  g_assert_cmpint (bson_cursor_type (c), ==, BSON_TYPE_BOOLEAN);
  g_assert (bson_cursor_get_boolean (c, &bool));
  g_assert (bool == FALSE);
  g_free (c);
  PASS ();

  TEST (bson_reader_find_string_2);
  g_assert ((c = bson_find (b, "goodbye")));
  g_assert_cmpint (bson_cursor_type (c), ==, BSON_TYPE_STRING);
  g_assert (bson_cursor_get_string (c, &s));
  g_assert_cmpstr (s, ==, "cruel world");
  g_free (c);
  PASS ();

  TEST (bson_reader_find_date);
  g_assert ((c = bson_find (b, "date")));
  g_assert_cmpint (bson_cursor_type (c), ==, BSON_TYPE_UTC_DATETIME);
  g_assert (bson_cursor_get_utc_datetime (c, &l));
  g_assert_cmpint (l, ==, 1294860709000);
  g_free (c);
  PASS ();

  TEST (bson_reader_find_double);
  g_assert ((c = bson_find (b, "double")));
  g_assert_cmpint (bson_cursor_type (c), ==, BSON_TYPE_DOUBLE);
  g_assert (bson_cursor_get_double (c, &d));
  g_assert_cmpfloat (d, ==, 3.14);
  g_free (c);
  PASS ();

  TEST (bson_reader_find_int64);
  g_assert ((c = bson_find (b, "int64")));
  g_assert_cmpint (bson_cursor_type (c), ==, BSON_TYPE_INT64);
  g_assert (bson_cursor_get_int64 (c, &l));
  g_assert_cmpint (l, ==, 9876543210);
  g_free (c);
  PASS ();

  bson_free (b);

  return TRUE;
}

static gboolean
test_bson_reader_nested (void)
{
  bson *b, *t;
  bson_cursor *c;

  b = test_bson_generate_nested ();

  TEST (bson_reader_complex_find_user);
  g_assert ((c = bson_find (b, "user")));
  g_assert_cmpint (bson_cursor_type (c), ==, BSON_TYPE_DOCUMENT);
  g_assert (bson_cursor_get_document (c, &t));
  PASS ();

  {
    const char *s;
    gint32 i;

    bson_cursor *c2;

    TEST (bson_reader_complex_find_user.name);
    g_assert ((c2 = bson_find (t, "name")));
    g_assert_cmpint (bson_cursor_type (c2), ==, BSON_TYPE_STRING);
    g_assert (bson_cursor_get_string (c2, &s));
    g_assert_cmpstr (s, ==, "V.A. Lucky");
    g_free (c2);
    PASS ();

    TEST (bson_reader_complex_find_user.id);
    g_assert ((c2 = bson_find (t, "id")));
    g_assert_cmpint (bson_cursor_type (c2), ==, BSON_TYPE_INT32);
    g_assert (bson_cursor_get_int32 (c2, &i));
    g_assert_cmpint (i, ==, 12345);
    g_free (c2);
    PASS ();
  }

  g_free (c);
  bson_free (t);

  TEST (bson_reader_complex_find_posts);
  g_assert ((c = bson_find (b, "posts")));
  g_assert_cmpint (bson_cursor_type (c), ==, BSON_TYPE_ARRAY);
  g_assert (bson_cursor_get_array (c, &t));
  PASS ();

  {
    const char *s;
    bson_cursor *c2;
    bson *d;

    TEST (bson_reader_complex_find_posts.1);
    g_assert ((c2 = bson_find (t, "1")));
    g_assert_cmpint (bson_cursor_type (c2), ==, BSON_TYPE_DOCUMENT);
    g_assert (bson_cursor_get_document (c2, &d));
    PASS ();

    {
      bson_cursor *c3;

      TEST (bson_reader_complex_find_posts.1.comments);
      g_assert ((c3 = bson_find (d, "comments")) == NULL);
      PASS ();
    }

    g_free (c2);
    bson_free (d);

    TEST (bson_reader_complex_find_posts.0);
    g_assert ((c2 = bson_find (t, "0")));
    g_assert_cmpint (bson_cursor_type (c2), ==, BSON_TYPE_DOCUMENT);
    g_assert (bson_cursor_get_document (c2, &d));
    PASS ();

    {
      bson_cursor *c3;
      bson *a;

      TEST (bson_reader_complex_find_posts.0.comments);
      g_assert ((c3 = bson_find (d, "comments")));
      g_assert_cmpint (bson_cursor_type (c), ==, BSON_TYPE_ARRAY);
      g_assert (bson_cursor_get_array (c3, &a));
      PASS ();

      {
	bson_cursor *c4;
	const gchar *s;

	TEST (bson_reader_complex_find_posts.0.comments.2);
	g_assert ((c4 = bson_find (a, "2")));
	g_assert_cmpint (bson_cursor_type (c4), ==, BSON_TYPE_STRING);
	g_assert (bson_cursor_get_string (c4, &s));
	g_assert_cmpstr (s, ==, "last!");
	g_free (c4);
	PASS ();
      }

      g_free (c3);
      bson_free (a);
    }

    g_free (c2);
    bson_free (d);
  }

  g_free (c);
  bson_free (b);

  return TRUE;
}

int
main (void)
{
  g_assert (test_bson_reader_flat ());
  g_assert (test_bson_reader_nested ());
}