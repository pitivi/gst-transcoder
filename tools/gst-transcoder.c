/* GStreamer
 *
 * Copyright (C) 2015 Thibault Saunier <tsaunier@gnome.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <string.h>

#include "utils.h"
#include "../gst-libs/gst/transcoding/transcoder/gsttranscoder.h"

static const gchar *HELP_SUMMARY =
    "gst-transcoder-1.0 transcodes a stream defined by its first <input-uri>\n"
    "argument to the place defined by its second <output-uri> argument\n"
    "into the format described in its third <encoding-format> argument,\n"
    "or using the given <output-uri> file extension.\n"
    "\n"
    "The <encoding-format> argument:\n"
    "===============================\n"
    "\n"
    "If the encoding format is not defined, it will be guessed with\n"
    "the given <output-uri> file extension."
    "\n"
    "<encoding-format> describe the media format into which the\n"
    "input stream is going to be transcoded. We have two different\n"
    "ways of describing the format:\n"
    "\n"
    "GstEncodingProfile serialization format\n"
    "---------------------------------------\n"
    "\n"
    "GStreamer encoding profiles can be descibed with a quite extensive\n"
    "syntax which is descibed in the GstEncodingProfile documentation.\n"
    "\n"
    "The simple case looks like:\n"
    "\n"
    "    muxer_source_caps:videoencoder_source_caps:audioencoder_source_caps\n"
    "\n"
    "Name and category of serialized GstEncodingTarget\n"
    "-------------------------------------------------\n"
    "\n"
    "Encoding targets describe well known formats which\n"
    "those are provided in '.gep' files. You can list\n"
    "available ones using the `--list` argument.\n";

static void
position_updated_cb (GstTranscoder * transcoder, GstClockTime pos)
{
  GstClockTime dur = -1;
  gchar status[64] = { 0, };

  g_object_get (transcoder, "duration", &dur, NULL);

  memset (status, ' ', sizeof (status) - 1);

  if (pos != -1 && dur > 0 && dur != -1) {
    gchar dstr[32], pstr[32];

    /* FIXME: pretty print in nicer format */
    g_snprintf (pstr, 32, "%" GST_TIME_FORMAT, GST_TIME_ARGS (pos));
    pstr[9] = '\0';
    g_snprintf (dstr, 32, "%" GST_TIME_FORMAT, GST_TIME_ARGS (dur));
    dstr[9] = '\0';
    g_print ("%s / %s %s\r", pstr, dstr, status);
  }
}

static GList *
get_profiles_of_type (GstEncodingProfile * profile, GType profile_type)
{
  GList *tmp, *profiles = NULL;

  if (GST_IS_ENCODING_CONTAINER_PROFILE (profile)) {
    for (tmp = (GList *)
        gst_encoding_container_profile_get_profiles
        (GST_ENCODING_CONTAINER_PROFILE (profile)); tmp; tmp = tmp->next) {
      if (G_OBJECT_TYPE (tmp->data) == profile_type)
        profiles = g_list_prepend (profiles, tmp->data);
    }
  } else if (GST_IS_ENCODING_VIDEO_PROFILE (profile)) {
    profiles = g_list_prepend (profiles, profile);
  }

  return profiles;
}

static gboolean
set_video_size (GstEncodingProfile * profile, const gchar * value)
{
  GList *video_profiles, *tmp;
  gchar *p, *tmpstr, **vsize;
  gint width, height;

  if (!value)
    return TRUE;

  p = tmpstr = g_strdup (value);

  for (; *p; ++p)
    *p = g_ascii_tolower (*p);

  vsize = g_strsplit (tmpstr, "x", -1);
  g_free (tmpstr);

  if (!vsize[1] || vsize[2]) {
    g_strfreev (vsize);
    error ("Video size should be in the form: WxH, got %s", value);

    return FALSE;
  }

  width = g_ascii_strtoull (vsize[0], NULL, 0);
  height = g_ascii_strtoull (vsize[1], NULL, 10);
  g_strfreev (vsize);

  video_profiles = get_profiles_of_type (profile,
      GST_TYPE_ENCODING_VIDEO_PROFILE);
  for (tmp = video_profiles; tmp; tmp = tmp->next) {
    GstCaps *rest = gst_encoding_profile_get_restriction (tmp->data);

    if (!rest)
      rest = gst_caps_new_empty_simple ("video/x-raw");
    else
      rest = gst_caps_copy (rest);

    gst_caps_set_simple (rest, "width", G_TYPE_INT, width,
        "height", G_TYPE_INT, height, NULL);

    gst_encoding_profile_set_restriction (tmp->data, rest);
  }

  return TRUE;
}

static gboolean
set_audio_rate (GstEncodingProfile * profile, gint rate)
{
  GList *audio_profiles, *tmp;

  if (rate < 0)
    return TRUE;

  audio_profiles =
      get_profiles_of_type (profile, GST_TYPE_ENCODING_AUDIO_PROFILE);
  for (tmp = audio_profiles; tmp; tmp = tmp->next) {
    GstCaps *rest = gst_encoding_profile_get_restriction (tmp->data);

    if (!rest)
      rest = gst_caps_new_empty_simple ("audio/x-raw");
    else
      rest = gst_caps_copy (rest);

    gst_caps_set_simple (rest, "rate", G_TYPE_INT, rate, NULL);
    gst_encoding_profile_set_restriction (tmp->data, rest);
  }

  return TRUE;
}

static void
list_encoding_targets (void)
{
  GList *tmp, *targets = gst_encoding_list_all_targets (NULL);

  for (tmp = targets; tmp; tmp = tmp->next) {
    GstEncodingTarget *target = tmp->data;
    GList *usable_profiles = get_usable_profiles (target);

    if (usable_profiles) {
      GList *tmpprof;

      g_print ("\n%s (%s): %s\n * Profiles:\n",
          gst_encoding_target_get_name (target),
          gst_encoding_target_get_category (target),
          gst_encoding_target_get_description (target));

      for (tmpprof = usable_profiles; tmpprof; tmpprof = tmpprof->next)
        g_print ("     - %s: %s",
            gst_encoding_profile_get_name (tmpprof->data),
            gst_encoding_profile_get_description (tmpprof->data));

      g_print ("\n");
      g_list_free (usable_profiles);
    }
  }

  g_list_free_full (targets, (GDestroyNotify) g_object_unref);
}

static void
_warning_cb (GstTranscoder * self, GError * error, GstStructure * details)
{
  gboolean cant_encode;
  GstCaps *caps;

  if (details && gst_structure_get (details, "can-t-encode-stream",
          G_TYPE_BOOLEAN, &cant_encode, "stream-caps", GST_TYPE_CAPS,
          &caps, NULL)) {
    gchar *codec_desc = gst_pb_utils_get_codec_description (caps);

    warn ("WARNING: Input stream encoded with %s can't be encoded", codec_desc);
    gst_caps_unref (caps);
    g_free (codec_desc);

    return;
  }
  warn ("Got warning: %s", error->message);
}

int
main (int argc, char *argv[])
{
  gint res = 0;
  GError *err = NULL;
  gint cpu_usage = 100, rate = -1;
  gboolean list;
  GstEncodingProfile *profile;
  GstTranscoder *transcoder;
  gchar *src_uri, *dest_uri, *encoding_format = NULL, *size = NULL;
  GOptionContext *ctx;

  GOptionEntry options[] = {
    {"cpu-usage", 'c', 0, G_OPTION_ARG_INT, &cpu_usage,
        "The CPU usage to target in the transcoding process", NULL},
    {"list-targets", 'l', G_OPTION_ARG_NONE, 0, &list,
        "List all encoding targets", NULL},
    {"size", 's', 0, G_OPTION_ARG_STRING, &size,
        "set frame size (WxH or abbreviation)", NULL},
    {"audio-rate", 'r', 0, G_OPTION_ARG_INT, &rate,
        "set audio sampling rate (in Hz)", NULL},
    {"video-encoder", 'v', 0, G_OPTION_ARG_STRING, &size,
        "The video encoder to use.", NULL},
    {NULL}
  };

  g_set_prgname ("gst-transcoder");

  ctx = g_option_context_new ("<source uri> <destination uri> "
      "[<encoding target name[/<encoding profile name>]]");
  g_option_context_set_summary (ctx, HELP_SUMMARY);

  g_option_context_add_main_entries (ctx, options, NULL);
  g_option_context_add_group (ctx, gst_init_get_option_group ());

  if (!g_option_context_parse (ctx, &argc, &argv, &err)) {
    g_print ("Error initializing: %s\n", GST_STR_NULL (err->message));
    g_clear_error (&err);
    g_option_context_free (ctx);
    return 1;
  }
  gst_pb_utils_init ();

  if (list) {
    list_encoding_targets ();
    return 0;
  }

  if (argc < 3 || argc > 4) {
    g_print ("%s", g_option_context_get_help (ctx, TRUE, NULL));
    g_option_context_free (ctx);

    return -1;
  }
  g_option_context_free (ctx);

  src_uri = ensure_uri (argv[1]);
  dest_uri = ensure_uri (argv[2]);

  if (argc == 3) {
    encoding_format = get_file_extension (dest_uri);

    if (!encoding_format)
      goto no_extension;
  } else {
    encoding_format = argv[3];
  }

  profile = create_encoding_profile (encoding_format);
  if (!profile) {
    error ("Could not find any encoding format for %s\n", encoding_format);
    warn ("You can list available targets using %s --list", argv[0]);
    res = 1;
    goto done;
  }

  set_video_size (profile, size);
  set_audio_rate (profile, rate);

  transcoder = gst_transcoder_new_full (src_uri, dest_uri, profile, NULL);
  gst_transcoder_set_avoid_reencoding (transcoder, TRUE);

  gst_transcoder_set_cpu_usage (transcoder, cpu_usage);
  g_signal_connect (transcoder, "position-updated",
      G_CALLBACK (position_updated_cb), NULL);
  g_signal_connect (transcoder, "warning", G_CALLBACK (_warning_cb), NULL);

  g_assert (transcoder);

  ok ("Starting transcoding...");
  gst_transcoder_run (transcoder, &err);
  if (err)
    error ("\nFAILURE: %s", err->message);
  else
    ok ("\nDONE.");

done:
  g_free (dest_uri);
  g_free (src_uri);
  return res;

no_extension:
  error ("No <encoding-format> specified and no extension"
      " available in the output target: %s", dest_uri);
  res = 1;

  goto done;
}
