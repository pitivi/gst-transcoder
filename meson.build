project('gst-renderer', 'c', meson_version : '>= 0.28.0')

subdir('common/meson')

VERSION='1.7.0.1'
GST_DEP_VERSION = '>= 1.6'

gobject_dep = dependency('gobject-2.0')
glib_dep = dependency('glib-2.0')
gst_dep = dependency('gstreamer-' + apiversion, version: GST_DEP_VERSION)
gst_pbutils_dep = dependency('gstreamer-pbutils-' + apiversion, version: GST_DEP_VERSION)

# The GstTranscoder library
install_headers('gst-libs/gst/transcoding/transcoder/gsttranscoder.h',
                subdir : 'gstreamer-' + apiversion + '/gst/transcoder')
gst_transcoder = shared_library('gsttranscoder',
  'gst-libs/gst/transcoding/transcoder/gsttranscoder.c',
  install: true,
  dependencies: [glib_dep, gobject_dep, gst_dep, gst_pbutils_dep],
  c_args: ['-Wno-pedantic'])

gnome = import ('gnome')
girtargets = gnome.generate_gir(gst_transcoder,
  sources : ['gst-libs/gst/transcoding/transcoder/gsttranscoder.h',
             'gst-libs/gst/transcoding/transcoder/gsttranscoder.c'],
  nsversion : apiversion,
  namespace : 'GstTranscoder',
  identifier_prefix : 'Gst',
  symbol_prefix : 'gst_',
  includes : ['GObject-2.0',
              'Gst-' + apiversion,
              'GstPbutils-' + apiversion],
  dependencies: [gst_dep],
  install : true,
  extra_args : ['--add-init-section=gst_init(NULL,NULL);']
)

pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries : gst_transcoder,
      subdirs : ['gstreamer-' + apiversion],
      version : VERSION,
      name : 'libgsttranscoder',
      filebase :'gst-transcoder-' + apiversion,
      description : 'A high level API to do media transcoding with GStreamer.')

gst_c_args = ['-DHAVE_CONFIG_H',
  '-D_GNU_SOURCE',
  '-DGST_EXPORTS',
  '-DGST_DISABLE_DEPRECATED',
]

configure_file(input : 'common/meson/config.h.meson',
  output : 'config.h',
  configuration : cdata)

gst_transcoder_plugin = shared_library('gsttranscoderplugin',
  'gst/transcode/gsttranscodebin.c',
  'gst/transcode/gst-cpu-throttling-clock.c',
  'gst/transcode/gsturitranscodebin.c',
  install : true,
  dependencies : [glib_dep, gobject_dep, gst_dep, gst_pbutils_dep],
  c_args : gst_c_args,
  install_dir : '@0@/gstreamer-1.0'.format(get_option('libdir')),
)

executable('gst-transcoder-' + apiversion,
  'tools/gst-transcoder.c',
  install : true,
  dependencies : [glib_dep, gobject_dep, gst_dep, gst_pbutils_dep],
  link_with: [gst_transcoder]
)
