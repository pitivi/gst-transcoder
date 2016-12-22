#ifndef __GST_TRANSCODER_UTILS_H
#define __GST_TRANSCODER_UTILS_H

#include "utils.h"
#include <gst/gst.h>
#include <gst/pbutils/pbutils.h>

void print (GstDebugColorFlags c, gboolean err, gboolean nline, const gchar * format, va_list var_args);
void ok (const gchar * format, ...);
void warn (const gchar * format, ...);
void error (const gchar * format, ...);

gchar * ensure_uri (const gchar * location);
gchar * get_file_extension (gchar * uri);

GList * get_usable_profiles (GstEncodingTarget * target);
GstEncodingProfile * create_encoding_profile (const gchar * pname);

#endif /*__GST_TRANSCODER_UTILS_H*/
