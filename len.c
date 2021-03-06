/*
 * len.c
 *
 * Physics related exercise for learning purposes.
 * Nathan Forbes
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_P_NAME "len"

const char *p_name;

typedef struct {
  enum {
    UK, IN, FT, MI,
    MM, CM, M, KM,
  } type;
  double val;
  const char *str;
} unit;

static void
set_unit_type (const char *s, unit *u)
{
  u->str = s;

  if (!strcmp (s, "in"))
    u->type = IN;
  else if (!strcmp (s, "ft"))
    u->type = FT;
  else if (!strcmp (s, "mi"))
    u->type = MI;
  else if (!strcmp (s, "mm"))
    u->type = MM;
  else if (!strcmp (s, "cm"))
    u->type = CM;
  else if (!strcmp (s, "m"))
    u->type = M;
  else if (!strcmp (s, "km"))
    u->type = KM;
  else
    u->type = UK;
}

static void
convert_in (double v, unit *u)
{
  double d;

  switch (u->type) {
  case IN:
    u->val = v;
    return;
  case FT:
    d = 12;
    break;
  case MI:
    d = 63360;
    break;
  case MM:
    d = 0.0393700787;
    break;
  case CM:
    d = 0.393700787;
    break;
  case M:
    d = 39.3700787;
    break;
  case KM:
    d = 39370.0787;
    break;
  default:
    return;
  }

  u->val = v / d;
}

static void
convert_ft (double v, unit *u)
{
  double d;

  switch (u->type) {
  case IN:
    u->val = v * 12;
    return;
  case FT:
    u->val = v;
    return;
  case MI:
    d = 5280;
    break;
  case MM:
    d = 0.0032808399;
    break;
  case CM:
    d = 0.032808399;
    break;
  case M:
    d = 3.2808399;
    break;
  case KM:
    d = 3280.8399;
    break;
  default:
    return;
  }

  u->val = v / d;
}

static void
convert_mi (double v, unit *u)
{
  switch (u->type) {
  case IN:
    u->val = v * 63360;
    break;
  case FT:
    u->val = v * 5280;
    break;
  case MI:
    u->val = v;
    break;
  case MM:
    u->val = v * 1609344;
    break;
  case CM:
    u->val = v * 160934.4;
    break;
  case M:
    u->val = v * 1609.344;
    break;
  case KM:
    u->val = v * 1.609344;
    break;
  }
}

static void
convert_mm (double v, unit *u)
{
  double d;

  switch (u->type) {
  case IN:
    d = 25.4;
    break;
  case FT:
    d = 304.8;
    break;
  case MI:
    d = 1609344;
    break;
  case MM:
    u->val = v;
    return;
  case CM:
    d = 10;
    break;
  case M:
    d = 1000;
    break;
  case KM:
    d = 1000000;
    break;
  }

  u->val = v / d;
}

static void
convert_cm (double v, unit *u)
{
  double d;

  switch (u->type) {
  case IN:
    d = 2.54;
    break;
  case FT:
    d = 30.48;
    break;
  case MI:
    d = 160934.4;
    break;
  case MM:
    d = 0.1;
    break;
  case CM:
    u->val = v;
    return;
  case M:
    d = 100;
    break;
  case KM:
    d = 100000;
    break;
  }

  u->val = v / d;
}

static void
convert_m (double v, unit *u)
{
  double d;

  switch (u->type) {
  case IN:
    d = 0.0254;
    break;
  case FT:
    d = 0.3048;
    break;
  case MI:
    d = 1609.344;
    break;
  case MM:
    d = 0.001;
    break;
  case CM:
    d = 0.01;
    break;
  case M:
    u->val = v;
    return;
  case KM:
    d = 1000;
    break;
  }

  u->val = v / d;
}

static void
convert_km (double v, unit *u)
{
  double d;

  switch (u->type) {
  case IN:
    d = 0.0000254;
    break;
  case FT:
    d = 0.0003048;
    break;
  case MI:
    d = 1.609344;
    break;
  case MM:
    d = 0.000001;
    break;
  case CM:
    d = 0.00001;
    break;
  case M:
    d = 0.001;
    break;
  case KM:
    u->val = v;
    return;
  }

  u->val = v / d;
}

static void
convert_unit (unit *i, unit *o)
{
  void (*c_fun) (double, unit *);

  switch (i->type) {
  case IN:
    c_fun = convert_in;
    break;
  case FT:
    c_fun = convert_ft;
    break;
  case MI:
    c_fun = convert_mi;
    break;
  case MM:
    c_fun = convert_mm;
    break;
  case CM:
    c_fun = convert_cm;
    break;
  case M:
    c_fun = convert_m;
    break;
  case KM:
    c_fun = convert_km;
    break;
  default:
    c_fun = NULL;
  }

  if (c_fun)
    (*c_fun) (i->val, o);
}

static void
set_p_name (char *g_name)
{
  if (!g_name || !*g_name) {
    p_name = DEFAULT_P_NAME;
    return;
  }

  char *n = strrchr (g_name, '/');

  if (!n) {
    p_name = g_name;
    return;
  }

  *++n;
  p_name = n;
}

int
main (int argc, char **argv)
{
  set_p_name (*argv);

  if (argc != 4) {
    if (argc > 1 && (!strcmp (argv[1], "-u") || !strcmp (argv[1], "--units")))
    {
      fprintf (stdout, "%s: valid units are:\n"
          "      in  --  inches\n"
          "      ft  --  feet\n"
          "      mi  --  miles\n"
          "      mm  --  millimeter\n"
          "      cm  --  centimeter\n"
          "      m   --  meter\n"
          "      km  --  kilometer\n", p_name);
      return EXIT_SUCCESS;
    }
    fprintf (stderr, "usage: %s QUANTITY IN-UNIT OUT-UNIT\n"
        "      Ex: %s 10 ft in\n"
        "      Use `-u' or `--units' to see available units\n",
        p_name, p_name);
    return EXIT_FAILURE;
  }

  unit i_unit;
  unit o_unit;

  memset (&i_unit, 0, sizeof (unit));
  memset (&o_unit, 0, sizeof (unit));
  i_unit.val = atof (argv[1]);
  set_unit_type (argv[2], &i_unit);
  set_unit_type (argv[3], &o_unit);

  if (i_unit.type == UK) {
    fprintf (stderr, "%s: error: not a valid unit of length: `%s'\n",
        p_name, i_unit.str);
    return EXIT_FAILURE;
  }

  if (o_unit.type == UK) {
    fprintf (stderr, "%s: error: not a valid unit of length: `%s'\n",
        p_name, o_unit.str);
    return EXIT_FAILURE;
  }

  convert_unit (&i_unit, &o_unit);
  fprintf (stdout, "%s: %g%s = %g%s\n",
      p_name, i_unit.val, i_unit.str,
      o_unit.val, o_unit.str);
  return EXIT_SUCCESS;
}

