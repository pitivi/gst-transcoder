project('gst-renderer', 'c',
        version : '1.10.0',
        meson_version : '>= 0.36.0')

gst_version = meson.project_version()
version_arr = gst_version.split('.')
gst_version_major = version_arr[0]
gst_version_minor = version_arr[1]
gst_version_micro = version_arr[2]
if version_arr.length() == 4
  gst_version_nano = version_arr[3]
else
  gst_version_nano = 0
endif

apiversion = '1.0'
soversion = 0

prefix = get_option('prefix')

cc = meson.get_compiler('c')
cdata = configuration_data()
cdata.set('GST_LICENSE', '"LGPL"')
cdata.set('GST_PACKAGE_ORIGIN', '"c"')
cdata.set('GST_PACKAGE_NAME', '"GStreamer Transcoder source release"')
cdata.set('PACKAGE', '"gst-transcoder"')
cdata.set('VERSION', '"@0@"'.format(gst_version))

if cc.has_function('getrusage', prefix : '#include<sys/resource.h>')
  cdata.set('HAVE_GETRUSAGE', 1)
endif

configure_file(output : 'config.h', configuration : cdata)

GST_DEP_VERSION = '>= @0@'.format(meson.project_version())

gobject_dep = dependency('gobject-2.0')
glib_dep = dependency('glib-2.0')
gst_dep = dependency('gstreamer-' + apiversion, version: GST_DEP_VERSION)
gst_pbutils_dep = dependency('gstreamer-pbutils-' + apiversion, version: GST_DEP_VERSION)

# The GstTranscoder library
install_headers('gst-libs/gst/transcoding/transcoder/gsttranscoder.h',
                subdir : 'gstreamer-' + apiversion + '/gst/transcoder')

gst_transcoder = shared_library('gsttranscoder-' + apiversion,
  'gst-libs/gst/transcoding/transcoder/gsttranscoder.c',
  install: true,
  dependencies: [glib_dep, gobject_dep, gst_dep, gst_pbutils_dep],
  c_args: ['-Wno-pedantic'],
  soversion : '0')

gst_transcoder_dep = declare_dependency(link_with: gst_transcoder)

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
  extra_args : ['--add-init-section=extern gboolean gst_init(gint *argc, gchar **argv); gst_init(NULL,NULL);']
)

pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries : gst_transcoder,
      subdirs : ['gstreamer-' + apiversion],
      version : meson.project_version(),
      name : 'libgsttranscoder-' + apiversion,
      filebase :'gst-transcoder-' + apiversion,
      description : 'A high level API to do media transcoding with GStreamer.')

gst_c_args = ['-DHAVE_CONFIG_H',
  '-D_GNU_SOURCE',
  '-DGST_EXPORTS',
  '-DGST_DISABLE_DEPRECATED',
]

configure_file(output : 'config.h', configuration : cdata)

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
