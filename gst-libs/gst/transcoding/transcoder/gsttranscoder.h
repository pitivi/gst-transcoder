#ifndef _gCODER
#define _gCODER

#include <gst/gst.h>
#include <gst/pbutils/pbutils.h>

G_BEGIN_DECLS

typedef struct _GstTranscoderSignalDispatcher GstTranscoderSignalDispatcher;
typedef struct _GstTranscoderSignalDispatcherInterface GstTranscoderSignalDispatcherInterface;

/*********** Error definitions ************/
#define      GST_TRANSCODER_ERROR                         (gst_transcoder_error_quark ())
#define      GST_TYPE_TRANSCODER_ERROR                    (gst_transcoder_error_get_type ())

/**
 * GstTranscoderError:
 * @GST_TRANSCODER_ERROR_FAILED: generic error.
 */
typedef enum {
  GST_TRANSCODER_ERROR_FAILED = 0
} GstTranscoderError;

GType gst_transcoder_error_get_type (void);
const gchar * gst_transcoder_error_get_name (GstTranscoderError error);

/*********** GstTranscoder definition  ************/
#define GST_TYPE_TRANSCODER (gst_transcoder_get_type ())
#define GST_TRANSCODER(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_TRANSCODER, GstTranscoder))
#define GST_TRANSCODER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_TRANSCODER, GstTranscoderClass))
#define GST_IS_TRANSCODER(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_TRANSCODER))
#define GST_IS_TRANSCODER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_TRANSCODER))
#define GST_TRANSCODER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_TRANSCODER, GstTranscoderClass))

typedef struct _GstTranscoder  GstTranscoder;
typedef struct _GstTranscoderClass  GstTranscoderClass;
typedef struct _GstTranscoderPrivate GstTranscoderPrivate;

GType           gst_transcoder_get_type                   (void);

GstTranscoder * gst_transcoder_new                        (const gchar * source_uri,
                                                           const gchar * dest_uri,
                                                           const gchar * encoding_target);

GstTranscoder * gst_transcoder_new_full                   (const gchar * source_uri,
                                                           const gchar * dest_uri,
                                                           GstEncodingProfile *profile,
                                                           GstTranscoderSignalDispatcher *signal_dispatcher);

gboolean gst_transcoder_run                               (GstTranscoder *self,
                                                           GError ** error);

void gst_transcoder_set_cpu_usage                         (GstTranscoder *self,
                                                           gint cpu_usage);

void gst_transcoder_run_async                             (GstTranscoder *self);

void gst_transcoder_set_position_update_interval          (GstTranscoder *self,
                                                           guint interval);

gchar * gst_transcoder_get_source_uri                     (GstTranscoder * self);

gchar * gst_transcoder_get_dest_uri                       (GstTranscoder * self);

guint gst_transcoder_get_position_update_interval         (GstTranscoder *self);

GstClockTime gst_transcoder_get_position                  (GstTranscoder * self);

GstClockTime gst_transcoder_get_duration                  (GstTranscoder * self);

GstElement * gst_transcoder_get_pipeline                  (GstTranscoder * self);


/****************** Signal dispatcher *******************************/

#define GST_TYPE_TRANSCODER_SIGNAL_DISPATCHER                (gst_transcoder_signal_dispatcher_get_type ())
#define GST_TRANSCODER_SIGNAL_DISPATCHER(obj)                (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_TRANSCODER_SIGNAL_DISPATCHER, GstTranscoderSignalDispatcher))
#define GST_IS_TRANSCODER_SIGNAL_DISPATCHER(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_TRANSCODER_SIGNAL_DISPATCHER))
#define GST_TRANSCODER_SIGNAL_DISPATCHER_GET_INTERFACE(inst) (G_TYPE_INSTANCE_GET_INTERFACE ((inst), GST_TYPE_TRANSCODER_SIGNAL_DISPATCHER, GstTranscoderSignalDispatcherInterface))

struct _GstTranscoderSignalDispatcherInterface {
  GTypeInterface parent_iface;

  void (*dispatch) (GstTranscoderSignalDispatcher * self,
                    GstTranscoder * transcoder,
                    void (*emitter) (gpointer data),
                    gpointer data,
                    GDestroyNotify destroy);
};

typedef struct _GstTranscoderGMainContextSignalDispatcher      GstTranscoderGMainContextSignalDispatcher;
typedef struct _GstTranscoderGMainContextSignalDispatcherClass GstTranscoderGMainContextSignalDispatcherClass;

GType        gst_transcoder_signal_dispatcher_get_type    (void);

#define GST_TYPE_TRANSCODER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER             (gst_transcoder_g_main_context_signal_dispatcher_get_type ())
#define GST_IS_TRANSCODER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_TRANSCODER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER))
#define GST_IS_TRANSCODER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_TRANSCODER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER))
#define GST_TRANSCODER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GST_TYPE_TRANSCODER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER, GstTranscoderGMainContextSignalDispatcherClass))
#define GST_TRANSCODER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_TRANSCODER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER, GstTranscoderGMainContextSignalDispatcher))
#define GST_TRANSCODER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_TRANSCODER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER, GstTranscoderGMainContextSignalDispatcherClass))
#define GST_TRANSCODER_G_MAIN_CONTEXT_SIGNAL_DISPATCHER_CAST(obj)        ((GstTranscoderGMainContextSignalDispatcher*)(obj))

GType gst_transcoder_g_main_context_signal_dispatcher_get_type (void);

GstTranscoderSignalDispatcher * gst_transcoder_g_main_context_signal_dispatcher_new (GMainContext * application_context);

G_END_DECLS

#endif

