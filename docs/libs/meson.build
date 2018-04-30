types = configure_file(input : 'gst-transcoder.types',
  output : 'gst-transcoder.types',
  configuration : configuration_data())

doc_deps_names = ['glib-2.0',
                  'gstreamer-@0@'.format(apiversion),
                  'gstreamer-plugins-base-@0@'.format(apiversion)]

doc_deps = []
foreach doc_dep : doc_deps_names
    runcmd = run_command('pkg-config', '--variable=prefix', doc_dep)
    if runcmd.returncode() == 0
        tmp = '--extra-dir=' + runcmd.stdout().strip() + '/share/gtk-doc/html/'
        tmp.strip()
        doc_deps = doc_deps + [tmp]
    endif
endforeach

docconf = configuration_data()
docconf.set('GST_API_VERSION', apiversion)
docconf.set('PACKAGE_VERSION', gst_version)
docconf.set('PLUGINDIR', '@0@/lib/gstreamer-1.0'.format(get_option('prefix')))

version_entities = configure_file(input : 'version.entities.in',
  output : 'version.entities',
  configuration : docconf)


gnome.gtkdoc('gst-transcoder',
  main_sgml : 'gst-transcoder-docs.sgml',
  src_dir : '@0@/../../gst-libs/gst/transcoding/transcoder'.format(meson.current_source_dir()),
  scan_args : ['--deprecated-guards=GST_DISABLE_DEPRECATED',
            '--ignore-decorators=GST_EXPORT',
            ],
  scanobjs_args : ['--type-init-func="gst_init(NULL,NULL)"'],
  gobject_typesfile : types,
  dependencies : [gst_transcoder_dep, gst_dep],
  fixxref_args: doc_deps + ['--html-dir=' + get_option('prefix') + '/share/gtk-doc/html/'],
  content_files : ['gst-transcoder-sections.txt'],
  install : true,
  install_dir : 'gstreamer-transcoder',
)
