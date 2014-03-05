/*
 * vinfo.c
 *
 * Vinfo is a convenience tool that displays
 * information about video files from the command line.
 *
 * Note that the external program `avprobe' must be installed
 * because vinfo does not use any API's to get video information.
 * Instead, `avprobe' is executed in a new process.
 *
 * Nathan Forbes
 */

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define V_DEFAULT_PROGRAM_NAME         "vinfo"
#define V_PROGRAM_VERSION              "0.0.1"
#define V_HOME_KEY                     "HOME"
#define V_PATH_KEY                     "PATH"
#define V_AVPROBE_EXEC                 "avprobe"
#define V_AVPROBE_OUTPUT_TEMP_FILENAME ".vinfo_avprobe_output"
#define V_DURATION_TAG                 "Duration: "
#define V_VIDEO_TAG                    "Video: "

#define V_DISPLAY_DATA_MAX 16
#define V_SIZE_BUFFER_MAX  16
#define V_VIDEO_FILES_MAX 128

#define V_DURATION   0
#define V_FPS        1
#define V_RESOLUTION 2

#define V_BC 'B'
#define V_KC 'K'
#define V_MC 'M'
#define V_GC 'G'

#define V_KV 1000LU
#define V_MV 1000000LU
#define V_GV 1000000000LU

#define VFALSE ((vboolean) 0)
#define VTRUE  ((vboolean) 1)

#define v_free(p) \
  do              \
  {               \
    if (!p)       \
      break;      \
    free (p);     \
    p = NULL;     \
  } while (0)

typedef unsigned char vboolean;

extern char **environ;

static const char *program_name;

static char *   avprobe_abspath          = NULL;
static char *   avprobe_temp_output_path = NULL;
static char *   info_buffer              = NULL;
static vboolean show_duration            = VFALSE;
static vboolean show_fps                 = VFALSE;
static vboolean show_resolution          = VFALSE;
static vboolean show_size                = VFALSE;
static char *   video_files              [V_VIDEO_FILES_MAX];

static struct
{
  char data[V_DISPLAY_DATA_MAX];
} display_data[3];

static const char *supported_video_extensions[] =
{
  ".3gpp", ".amv", ".asf",
  ".avi", ".divx", ".dpg",
  ".m4v", ".mkv", ".moov",
  ".mov", ".mp4", ".mpeg",
  ".mpg", ".swf", ".wmv",
  NULL
};

static vboolean
v_streq (const char *s1, const char *s2)
{
  size_t n;

  n = strlen (s1);
  if ((n == strlen (s2)) && (memcmp (s1, s2, n) == 0))
    return VTRUE;
  return VFALSE;
}

static void
v_error (const char *fmt, ...)
{
  va_list ap;

  fprintf (stderr, "%s: error: ", program_name);
  va_start (ap, fmt);
  vfprintf (stderr, fmt, ap);
  va_end (ap);
  fputc ('\n', stderr);
}

static void *
v_malloc (size_t n)
{
  void *p;

  p = malloc (n);
  if (!p)
  {
    v_error ("malloc failed (%s)", strerror (errno));
    exit (EXIT_FAILURE);
  }
  return p;
}

static char *
v_strdup (const char *s, ssize_t n)
{
  size_t nn;
  char *p;

  if (n <= 0)
    nn = strlen (s);
  else
    nn = n;

  p = (char *) v_malloc (nn + 1);
  strncpy (p, s, nn);
  p[nn] = '\0';
  return p;
}

static const char *
v_basename (const char *p)
{
  char *pp;

  if (p && *p)
  {
    pp = strrchr (p, '/');
    if (pp && *pp)
      return ++pp;
    return p;
  }
  return NULL;
}

static void
v_set_program_name (const char *p)
{
  const char *pp;

  pp = v_basename (p);
  if (pp && *pp)
    program_name = pp;
  else
    program_name = V_DEFAULT_PROGRAM_NAME;
}

static void
v_usage (vboolean error)
{
  fprintf ((!error) ? stdout : stderr,
           "Usage: %s [-dfrs] VIDEO_FILENAME...\n",
           program_name);
}

static void
v_help (void)
{
  v_usage (VFALSE);
  fputs ("Options:\n"
         "  -d, --duration    Display duration of VIDEO_FILENAME\n"
         "  -f, --fps         Display frames per second of VIDEO_FILENAME\n"
         "  -r, --resolution  Display resolution of VIDEO_FILENAME\n"
         "  -s, --size        Display size of VIDEO_FILENAME\n"
         "  -?, -h, --help    Display this message and exit\n"
         "  -v, --version     Display version information and exit\n"
         "Providing no options is the same as providing '-dfrs'.\n",
         stdout);
  exit (EXIT_SUCCESS);
}

static void
v_version (void)
{
  fputs (V_DEFAULT_PROGRAM_NAME " " V_PROGRAM_VERSION "\n"
         "Written by Nathan Forbes (2014)\n",
         stdout);
  exit (EXIT_SUCCESS);
}

static const char *
v_get_env_value (const char *key)
{
  size_t i;
  size_t n;
  const char *value;

  n = strlen (key);
  for (i = 0; environ[i]; ++i)
  {
    if ((memcmp (environ[i], key, n) == 0) && (environ[i][n] == '='))
    {
      value = environ[i] + (n + 1);
      return value;
    }
  }
  return NULL;
}

static void
v_set_avprobe_abspath (void)
{
  size_t i;
  size_t j;
  size_t n;
  size_t nexec;
  size_t pos;
  vboolean complete;
  const char *path;

  path = v_get_env_value (V_PATH_KEY);
  if (!path || !*path)
  {
    v_error ("environment variable '%s' not set", V_PATH_KEY);
    exit (EXIT_FAILURE);
  }

  n = 0;
  pos = 0;
  nexec = strlen (V_AVPROBE_EXEC);

  for (i = 0; path[i]; ++i)
  {
    if ((path[i] == ':') || !path[i + 1])
    {
      if (!pos)
        n = i - 1;
      else if ((i - pos) > n)
        n = (i - pos) - 1;
      pos = i - 1;
    }
  }

  n += nexec + 1;
  char tmpbuf[n];

  pos = 0;
  complete = VFALSE;

  for (i = 0; path[i]; ++i)
  {
    if (path[i] == ':')
      complete = VTRUE;
    else
      tmpbuf[pos++] = path[i];
    if (!path[i + 1] && !complete)
      complete = VTRUE;
    if (complete)
    {
      tmpbuf[pos++] = '/';
      for (j = 0; V_AVPROBE_EXEC[j]; ++j)
        tmpbuf[pos++] = V_AVPROBE_EXEC[j];
      tmpbuf[pos] = '\0';
      if (access (tmpbuf, X_OK) == 0)
      {
        avprobe_abspath = v_strdup (tmpbuf, pos);
        return;
      }
      pos = 0;
      complete = VFALSE;
    }
  }
}

static void
v_set_avprobe_temp_output_path (void)
{
  size_t ntemp;
  size_t nhome;
  const char *home;

  home = v_get_env_value (V_HOME_KEY);
  nhome = strlen (home);
  ntemp = strlen (V_AVPROBE_OUTPUT_TEMP_FILENAME);
  avprobe_temp_output_path = (char *) v_malloc (nhome + ntemp + 2);

  strncpy (avprobe_temp_output_path, home, nhome);
  avprobe_temp_output_path[nhome] = '/';
  strncpy (avprobe_temp_output_path + (nhome + 1),
           V_AVPROBE_OUTPUT_TEMP_FILENAME,
           ntemp);
  avprobe_temp_output_path[nhome + ntemp + 1] = '\0';
}

static void
v_execute_avprobe (const char *video_file)
{
  int status;
  int child_status;
  int fd;
  int s_fd;
  pid_t child;
  pid_t w;
  char *xargv[3];

  xargv[0] = avprobe_abspath;
  xargv[1] = v_strdup (video_file, -1);
  xargv[2] = NULL;

  child = fork ();
  switch (child)
  {
    case -1:
    {
      v_error ("fork call failed: %s", strerror (errno));
      break;
    }
    case 0:
    {
      child_status = EXIT_SUCCESS;
      fd = open (avprobe_temp_output_path, O_RDWR | O_CREAT | O_TRUNC, 0600);
      if (fd == -1)
      {
        v_error ("failed to open temp file `%s'", avprobe_temp_output_path);
        exit (EXIT_FAILURE);
      }
      s_fd = dup (fileno (stderr));
      if (dup2 (fd, fileno (stderr)) == -1)
      {
        v_error ("failed to redirect output to `%s'",
                  avprobe_temp_output_path);
        exit (EXIT_FAILURE);
      }
      if (execv (avprobe_abspath, xargv) == -1)
      {
        child_status = errno;
        v_error ("execv call failed: %s", strerror (child_status));
      }
      _exit (child_status);
      fflush (stderr);
      close (fd);
      dup2 (s_fd, fileno (stderr));
      close (s_fd);
      break;
    }
    default:
    {
      do
      {
        w = waitpid (child, &status, WUNTRACED | WCONTINUED);
        if (w == -1)
          v_error ("waitpid call failed: %s", strerror (errno));
      }
      while (!WIFEXITED (status) && !WIFSIGNALED (status));
      break;
    }
  }

  v_free (xargv[1]);
}

static void
v_null_video_files (void)
{
  size_t x;

  for (x = 0; x < V_VIDEO_FILES_MAX; ++x)
    video_files[x] = NULL;
}

static void
v_parse_args (char ***argv)
{
  size_t x;

  v_set_program_name (*(*argv)++);

  if (!**argv)
  {
    v_usage (VTRUE);
    exit (EXIT_FAILURE);
  }

  v_null_video_files ();

  do
  {
    if (!**argv)
      break;
    if (**(*argv) == '-')
    {
      if (*++*(*argv) == '-')
      {
        (void) *++*(*argv);
        if (v_streq (**argv, "help"))
          v_help ();
        else if (v_streq (**argv, "version"))
          v_version ();
        else if (v_streq (**argv, "duration"))
          show_duration = VTRUE;
        else if (v_streq (**argv, "fps"))
          show_fps = VTRUE;
        else if (v_streq (**argv, "resolution"))
          show_resolution = VTRUE;
        else if (v_streq (**argv, "size"))
          show_size = VTRUE;
        else
        {
          v_error ("unknown option `--%s'", **argv);
          exit (EXIT_FAILURE);
        }
      }
      else
      {
        while (***argv)
        {
          switch (***argv)
          {
            case '?':
            case 'h':
              v_help ();
            case 'v':
              v_version ();
            case 'd':
              show_duration = VTRUE;
              break;
            case 'f':
              show_fps = VTRUE;
              break;
            case 'r':
              show_resolution = VTRUE;
              break;
            case 's':
              show_size = VTRUE;
              break;
            default:
              v_error ("unknown option `-%c'", ***argv);
              exit (EXIT_FAILURE);
          }
          (void) *++*(*argv);
        }
      }
    }
    else
    {
      for (x = 0; video_files[x]; ++x)
        ;
      if (x >= V_VIDEO_FILES_MAX)
      {
        v_error ("too many file arguments");
        exit (EXIT_FAILURE);
      }
      video_files[x] = **argv;
    }
  }
  while (*(*argv)++);

  if (!video_files[0])
  {
    v_error ("no file arguments given");
    exit (EXIT_FAILURE);
  }

  if (!show_duration && !show_fps && !show_resolution && !show_size)
  {
    show_duration = VTRUE;
    show_fps = VTRUE;
    show_resolution = VTRUE;
    show_size = VTRUE;
  }
}

static vboolean
v_valid_video_file (const char *video_path)
{
  size_t i;
  vboolean ext_match;
  char *ext;
  struct stat st;

  memset (&st, 0, sizeof (struct stat));
  if (stat (video_path, &st) == 0)
  {
    if (S_ISREG (st.st_mode))
    {
      ext = strrchr (video_path, '.');
      if (ext && *ext)
      {
        ext_match = VFALSE;
        for (i = 0; supported_video_extensions[i]; ++i)
        {
          if (v_streq (ext, supported_video_extensions[i]))
          {
            ext_match = VTRUE;
            break;
          }
        }
        if (!ext_match)
          return VFALSE;
      }
      else
        return VFALSE;
      if (!st.st_size)
        return VFALSE;
    }
    else
      return VFALSE;
  }
  else
    return VFALSE;
  return VTRUE;
}

static void
v_format_size (char *buffer, const char *path)
{
  unsigned long bytes;
  struct stat st;

  memset (&st, 0, sizeof (struct stat));
  if ((stat (path, &st) == 0) && S_ISREG (st.st_mode))
  {
    bytes = (unsigned long) st.st_size;
    if ((bytes / V_GV) > 0)
      snprintf (buffer, V_SIZE_BUFFER_MAX, "%.1f%c",
                ((double) bytes / (double) V_GV), V_GC);
    else if ((bytes / V_MV) > 0)
      snprintf (buffer, V_SIZE_BUFFER_MAX, "%.1f%c",
                ((double) bytes / (double) V_MV), V_MC);
    else if ((bytes / V_KV) > 0)
      snprintf (buffer, V_SIZE_BUFFER_MAX, "%.1f%c",
                ((double) bytes / (double) V_KV), V_KC);
    else
      snprintf (buffer, V_SIZE_BUFFER_MAX, "%lu%c", bytes, V_BC);
  }
  else
    *buffer = '\0';
}

static void
v_fill_info_buffer (void)
{
  int fd;
  size_t n;
  ssize_t p;
  struct stat st;

  v_free (info_buffer);

  memset (&st, 0, sizeof (struct stat));
  if (stat (avprobe_temp_output_path, &st) == 0)
  {
    if (S_ISREG (st.st_mode))
      n = (size_t) st.st_size;
    else
    {
      v_error ("`%s' is not a regular file (this should never happen)",
                avprobe_temp_output_path);
      exit (EXIT_FAILURE);
    }
  }
  else
  {
    v_error ("`%s' does not exist", avprobe_temp_output_path);
    exit (EXIT_FAILURE);
  }

  fd = open (avprobe_temp_output_path, O_RDONLY);
  if (fd == -1)
  {
    v_error ("failed to open `%s'", avprobe_temp_output_path);
    exit (EXIT_FAILURE);
  }

  info_buffer = (char *) v_malloc (n + 1);
  p = read (fd, info_buffer, n);
  if (p < 0)
  {
    v_error ("failed to read data from `%s'", avprobe_temp_output_path);
    close (fd);
    exit (EXIT_FAILURE);
  }

  close (fd);
  info_buffer[p] = '\0';
}

/* takes a strict argument of a string containing
   only two characters that are guaranteed to be digits */
static unsigned int
v_str_to_uint (const char *s)
{
  unsigned long long n;
  unsigned long long ig;
  size_t i;

  n = 0LLU;
  if (s && *s)
  {
    ig = 0LLU;
    i = 0;
    while (s[i])
      ig = ig * 10LLU + s[i++] - '0';
    n = ig;
  }
  return (unsigned int) n;
}

static vboolean
v_is_digit (const char c)
{
  switch (c)
  {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return VTRUE;
    default:
      ;
  }
  return VFALSE;
}

static vboolean
v_check_orig_duration_format (char orig[V_DISPLAY_DATA_MAX])
{
  if (v_is_digit (orig[0]) && v_is_digit (orig[1]) && (orig[2] == ':') &&
      v_is_digit (orig[3]) && v_is_digit (orig[4]) && (orig[5] == ':') &&
      v_is_digit (orig[6]) && v_is_digit (orig[7]))
    return VTRUE;
  return VFALSE;
}

static void
v_convert_duration_format (char orig[V_DISPLAY_DATA_MAX])
{
  unsigned int hours;
  unsigned int minutes;
  unsigned int seconds;
  size_t n;
  char buf[3];

  if (!v_check_orig_duration_format (orig))
  {
    strncpy (display_data[V_DURATION].data, orig, strlen (orig));
    display_data[V_DURATION].data[strlen (orig)] = '\0';
    return;
  }

  buf[0] = orig[0];
  buf[1] = orig[1];
  buf[2] = '\0';
  hours = v_str_to_uint (buf);

  buf[0] = orig[3];
  buf[1] = orig[4];
  minutes = v_str_to_uint (buf);

  buf[0] = orig[6];
  buf[1] = orig[7];
  seconds = v_str_to_uint (buf);

  display_data[V_DURATION].data[0] = '\0';

  if (hours)
  {
    snprintf (display_data[V_DURATION].data,
              V_DISPLAY_DATA_MAX, "%uh", hours);
    if (!minutes && !seconds)
      return;
  }

  if (minutes)
  {
    n = strlen (display_data[V_DURATION].data);
    snprintf (display_data[V_DURATION].data + n,
              V_DISPLAY_DATA_MAX, "%um", minutes);
    if (!seconds)
      return;
  }

  if (seconds)
  {
    n = strlen (display_data[V_DURATION].data);
    snprintf (display_data[V_DURATION].data + n,
              V_DISPLAY_DATA_MAX, "%us", seconds);
  }
}

static void
v_parse_info_buffer (void)
{
  size_t i;
  size_t pos;
  char dur[V_DISPLAY_DATA_MAX];
  char *p;

  if (show_duration)
  {
    p = strstr (info_buffer, V_DURATION_TAG);
    if (p && *p)
    {
      p = p + strlen (V_DURATION_TAG);
      pos = 0;
      for (i = 0; p[i] != '.'; ++i)
        dur[pos++] = p[i];
      dur[pos] = '\0';
      v_convert_duration_format (dur);
    }
    else
      display_data[V_DURATION].data[0] = '\0';
  }
  else
    display_data[V_DURATION].data[0] = '\0';

  if (show_fps)
  {
    p = strstr (info_buffer, V_VIDEO_TAG);
    if (p && *p)
    {
      p = p + strlen (V_VIDEO_TAG);
      for (i = 0; p[i]; ++i)
      {
        if ((p[i] == ' ') &&
            (p[i + 1] == 'f') &&
            (p[i + 2] == 'p') &&
            (p[i + 3] == 's'))
        {
          for (i = i - 1; p[i]; --i)
            if (p[i] == ' ')
              break;
          pos = 0;
          for (i = i + 1; (p[i] != ' ') && (p[i] != ','); ++i)
            display_data[V_FPS].data[pos++] = p[i];
          display_data[V_FPS].data[pos] = '\0';
        }
      }
    }
    else
      display_data[V_FPS].data[0] = '\0';
  }
  else
    display_data[V_FPS].data[0] = '\0';

  if (show_resolution)
  {
    p = strstr (info_buffer, V_VIDEO_TAG);
    if (p && *p)
    {
      p = p + strlen (V_VIDEO_TAG);
      for (i = 0; p[i]; ++i)
      {
        if ((p[i] == 'x') && v_is_digit (p[i - 1]) && v_is_digit (p[i + 1]))
        {
          for (i = i - 1; p[i]; --i)
            if (p[i] == ' ')
              break;
          pos = 0;
          for (i = i + 1; (p[i] != ' ') && (p[i] != ','); ++i)
            display_data[V_RESOLUTION].data[pos++] = p[i];
          display_data[V_RESOLUTION].data[pos] = '\0';
          break;
        }
      }
    }
    else
      display_data[V_RESOLUTION].data[0] = '\0';
  }
  else
    display_data[V_RESOLUTION].data[0] = '\0';
}

static void
v_print_display_data (const char *video_filename, size_t lvn)
{
  size_t i;
  size_t n;
  const char *basename;

  basename = v_basename (video_filename);
  if (show_duration || show_fps || show_resolution || show_size)
  {
    fputs (basename, stdout);
    fputs (": ", stdout);
    n = strlen (basename);
    for (i = 0; i < (lvn - n); ++i)
      fputc (' ', stdout);
  }

  if (show_duration)
  {
    if (*display_data[V_DURATION].data)
      fputs (display_data[V_DURATION].data, stdout);
    else
      fputs ("(unknown)", stdout);
  }

  if (show_fps)
  {
    if (show_duration)
      fputs (" - ", stdout);
    if (*display_data[V_FPS].data)
    {
      fputs (display_data[V_FPS].data, stdout);
      fputs ("fps", stdout);
    }
    else
      fputs ("(unknown)", stdout);
  }

  if (show_resolution)
  {
    if (show_duration || show_fps)
      fputs (" - ", stdout);
    if (*display_data[V_RESOLUTION].data)
      fputs (display_data[V_RESOLUTION].data, stdout);
    else
      fputs ("(unknown)", stdout);
  }

  if (show_size)
  {
    if (show_duration || show_fps || show_resolution)
      fputs (" - ", stdout);
    char buf[V_SIZE_BUFFER_MAX];
    v_format_size (buf, video_filename);
    if (*buf)
      fputs (buf, stdout);
    else
      fputs ("(unknown)", stdout);
  }

  if (show_duration || show_fps || show_resolution || show_size)
    fputc ('\n', stdout);
}

static size_t
v_longest_video_name (void)
{
  size_t i;
  size_t n;
  size_t longest;

  longest = 0;
  for (i = 0; video_files[i]; ++i)
  {
    n = strlen (v_basename (video_files[i]));
    if (n > longest)
      longest = n;
  }
  return longest;
}

static void
v_atexit (void)
{
  v_free (avprobe_abspath);
  unlink (avprobe_temp_output_path);
  v_free (avprobe_temp_output_path);
  v_free (info_buffer);
}

int
main (int argc, char **argv)
{
  size_t x;
  size_t lvn;

  atexit (v_atexit);
  v_parse_args (&argv);

  v_set_avprobe_abspath ();
  if (!avprobe_abspath || !*avprobe_abspath)
  {
    v_error ("failed to find external program `%s'", V_AVPROBE_EXEC);
    v_error ("please ensure it is installed on the system");
    exit (EXIT_FAILURE);
  }

  v_set_avprobe_temp_output_path ();
  if (!avprobe_temp_output_path || !*avprobe_temp_output_path)
  {
    v_error ("failed to set temporary data file path");
    exit (EXIT_FAILURE);
  }

  lvn = v_longest_video_name ();
  for (x = 0; video_files[x]; ++x)
  {
    if (!v_valid_video_file (video_files[x]))
      continue;
    v_execute_avprobe (video_files[x]);
    v_fill_info_buffer ();
    v_parse_info_buffer ();
    v_print_display_data (video_files[x], lvn);
  }

  exit (EXIT_SUCCESS);
  return 0; /* compiler */
}

