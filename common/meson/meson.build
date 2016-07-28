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
cdata.set('GST_VERSION_MAJOR', gst_version_major)
cdata.set('GST_VERSION_MINOR', gst_version_minor)
cdata.set('GST_VERSION_MICRO', gst_version_micro)
cdata.set('GST_VERSION_NANO', gst_version_nano)
cdata.set('GST_API_VERSION', '"@0@"'.format(apiversion))
cdata.set('GST_DATADIR', '"@0@/@1@"'.format(prefix, get_option('datadir')))
cdata.set('LOCALEDIR', '"@0@/@1@"'.format(prefix, get_option('localedir')))
cdata.set('LIBDIR', '"@0@/@1@"'.format(prefix, get_option('libdir')))
cdata.set('GST_API_VERSION', '"' + apiversion + '"')
cdata.set('GETTEXT_PACKAGE', '"gstreamer' + apiversion + '"')
cdata.set('GST_LEVEL_DEFAULT', 'GST_LEVEL_NONE')
cdata.set('GST_LICENSE', '"LGPL"')
cdata.set('GST_PACKAGE_ORIGIN', '"c"')
cdata.set('GST_PACKAGE_NAME', '"GStreamer Transcoder source release"')
cdata.set('PACKAGE', '"gst-transcoder"')
cdata.set('PACKAGE_NAME', '"GStreamer transcoder"')
cdata.set('PACKAGE_STRING', '"GStreamer Transcoder @0@"'.format(gst_version))
cdata.set('PACKAGE_TARNAME', '"gstreamer-transcoder"')
cdata.set('PACKAGE_BUGREPORT', '"https://github.com/pitivi/gst-transcoder"')
cdata.set('PACKAGE_URL', 'https://github.com/pitivi/gst-transcoder""')
cdata.set('PACKAGE_VERSION', '"@0@"'.format(gst_version))
cdata.set('PLUGINDIR', '"@0@/lib/gstreamer-@1@"'.format(get_option('prefix'), apiversion))
cdata.set('VERSION', '"@0@"'.format(gst_version))

if cc.has_function('getrusage', prefix : '#include<sys/resource.h>')
  cdata.set('HAVE_GETRUSAGE', 1)
endif

configure_file(input : 'config.h.meson',
  output : 'config.h',
  configuration : cdata)
