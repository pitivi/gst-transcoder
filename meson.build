project('gst-transcoder', 'c',
        version : '1.17.0.1',
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

gst_req = '>= @0@.@1@.0'.format(gst_version_major, gst_version_minor)

gobject_dep = dependency('gobject-2.0')
glib_dep = dependency('glib-2.0')
gst_dep = dependency('gstreamer-1.0', version : gst_req,
  fallback : ['gstreamer', 'gst_dep'])
gst_pbutils_dep = dependency('gstreamer-pbutils-1.0', version : gst_req,
    fallback : ['gst-plugins-base', 'pbutils_dep'])

# The GstTranscoder library
install_headers('gst-libs/gst/transcoding/transcoder/gsttranscoder.h',
                subdir : 'gstreamer-' + apiversion + '/gst/transcoder')

gst_transcoder = shared_library('gsttranscoder-' + apiversion,
  'gst-libs/gst/transcoding/transcoder/gsttranscoder.c',
  install: true,
  dependencies: [glib_dep, gobject_dep, gst_dep, gst_pbutils_dep],
  c_args: ['-Wno-pedantic'],
  soversion : '0')

incl = include_directories('gst-libs')
gst_transcoder_dep = declare_dependency(link_with: gst_transcoder,
  dependencies : [gst_dep, gst_pbutils_dep],
  include_directories : incl)

gir = find_program('g-ir-scanner', required : false)
gnome = import ('gnome')
build_gir = gir.found() and not meson.is_cross_build() and not get_option('disable_introspection')
if build_gir
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
endif

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

gst_transcoder_plugin = shared_library('gsttranscode',
  'gst/transcode/gsttranscodebin.c',
  'gst/transcode/gst-cpu-throttling-clock.c',
  'gst/transcode/gsturitranscodebin.c',
  install : true,
  dependencies : [glib_dep, gobject_dep, gst_dep, gst_pbutils_dep],
  c_args : gst_c_args,
  install_dir : '@0@/gstreamer-1.0'.format(get_option('libdir')),
)

executable('gst-transcoder-' + apiversion,
  'tools/gst-transcoder.c', 'tools/utils.c',
  install : true,
  dependencies : [glib_dep, gobject_dep, gst_dep, gst_pbutils_dep],
  link_with: [gst_transcoder]
)

python3 = find_program('python3')
run_command(python3, '-c', 'import shutil; shutil.copy("hooks/pre-commit.hook", ".git/hooks/pre-commit")')

encoding_targetsdir = join_paths(get_option('datadir'),
    'gstreamer-' + apiversion, 'encoding-profiles')

encoding_targets = [
  ['file-extension', ['data/targets/file-extension/ogv.gep',
                      'data/targets/file-extension/oga.gep',
                      'data/targets/file-extension/mkv.gep',
                      'data/targets/file-extension/mp3.gep',
                      'data/targets/file-extension/webm.gep',
                      'data/targets/file-extension/flv.gep',
                      'data/targets/file-extension/mp4.gep',
                      'data/targets/file-extension/avi.gep',],
  ],
  ['online-services', ['data/targets/online-service/youtube.gep',]],
  ['device', ['data/targets/device/dvd.gep',]],
]

foreach path_targets : encoding_targets
  dir = join_paths(encoding_targetsdir, path_targets.get(0))
  etargets = path_targets.get(1)
  install_data(sources: etargets, install_dir: dir)
endforeach

if build_machine.system() == 'windows'
  message('Disabling doc while building on Windows')
elif get_option('disable_doc')
  message('doc is disabled via options')
else
  if find_program('gtkdoc-scan', required : false).found()
    subdir('docs/libs')
  else
    message('Not building documentation as gtk-doc was not found')
  endif
endif
