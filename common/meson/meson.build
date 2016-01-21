gst_version = '@0@.@1@.@2@'.format(gst_major, gst_minor, gst_micro)
apiversion = '1.0'
soversion = 0

prefix = get_option('prefix')

cc = meson.get_compiler('c')
cdata = configuration_data()
cdata.set('GST_VERSION_MAJOR', gst_major)
cdata.set('GST_VERSION_MINOR', gst_minor)
cdata.set('GST_VERSION_MICRO', gst_micro)
cdata.set('GST_VERSION_NANO', gst_nano)
cdata.set('GST_API_VERSION', '"@0@"'.format(apiversion))
cdata.set('HAVE_CPU_X86_64', true)
cdata.set('HOST_CPU', '"@0@"'.format(host_machine.cpu()))
cdata.set('TARGET_CPU', '"@0@"'.format(target_machine.cpu()))
cdata.set('GST_DATADIR', '"@0@/@1@"'.format(prefix, get_option('datadir')))
cdata.set('LOCALEDIR', '"@0@/@1@"'.format(prefix, get_option('localedir')))
cdata.set('LIBDIR', '"@0@/@1@"'.format(prefix, get_option('libdir')))
cdata.set('GST_API_VERSION', '"1.0"')
cdata.set('GETTEXT_PACKAGE', '"gstreamer-1.0"')
cdata.set('GST_LEVEL_DEFAULT', 'GST_LEVEL_NONE')
cdata.set('GST_LICENSE', '"LGPL"')
cdata.set('GST_PACKAGE_ORIGIN', '"Unknown package origin"')
cdata.set('GST_PACKAGE_NAME', '"GStreamer source release"')
cdata.set('PACKAGE', '"gstreamer"')
cdata.set('PACKAGE_NAME', '"GStreamer"')
cdata.set('PACKAGE_STRING', '"GStreamer @0@"'.format(gst_version))
cdata.set('PACKAGE_TARNAME', '"gstreamer"')
cdata.set('PACKAGE_BUGREPORT', '"someone@somewhere"')
cdata.set('PACKAGE_URL', '""')
cdata.set('PACKAGE_VERSION', '"@0@"'.format(gst_version))
cdata.set('PLUGINDIR', '"@0@/lib/gstreamer-1.0"'.format(get_option('prefix')))
cdata.set('VERSION', '"@0@"'.format(gst_version))
cdata.set('MEMORY_ALIGNMENT_MALLOC', 1)
cdata.set('GST_PLUGIN_SCANNER_INSTALLED', '"TMP_FIXME"')

check_headers = [['dlfcn.h','HAVE_DLFCN_H'],
  ['inttypes.h', 'HAVE_INTTYPES_H'],
  ['memory.h', 'HAVE_MEMORY_H'],
  ['poll.h', 'HAVE_POLL_H'],
  ['stdint.h', 'HAVE_STDINT_H'],
  ['stdio_ext.h', 'HAVE_STDIO_EXT_H'],
  ['strings.h', 'HAVE_STRINGS_H'],
  ['string.h', 'HAVE_STRING_H'],
  ['sys/param.h', 'HAVE_SYS_PARAM_H'],
  ['sys/poll.h', 'HAVE_SYS_POLL_H'],
  ['sys/prctl.h', 'HAVE_SYS_PRCTL_H'],
  ['sys/socket.h', 'HAVE_SYS_SOCKET_H'],
  ['sys/stat.h', 'HAVE_SYS_STAT_H'],
  ['sys/times.h', 'HAVE_SYS_TIMES_H'],
  ['sys/types.h', 'HAVE_SYS_TYPES_H'],
  ['sys/utsname.h', 'HAVE_SYS_UTSNAME_H'],
  ['sys/wait.h', 'HAVE_SYS_WAIT_H'],
  ['ucontext.h', 'HAVE_UCONTEXT_H'],
  ['unistd.h', 'HAVE_UNISTD_H'],
  ['valgrind/valgrind.h', 'HAVE_VALGRIND_VALGRIND_H'],
  ['winsock2.h', 'HAVE_WINSOCK2_H'],
]

foreach h : check_headers
  if cc.has_header(h.get(0))
    cdata.set(h.get(1), 1)
  endif
endforeach

cdata.set('SIZEOF_CHAR', cc.sizeof('char'))
cdata.set('SIZEOF_INT', cc.sizeof('int'))
cdata.set('SIZEOF_LONG', cc.sizeof('long'))
cdata.set('SIZEOF_SHORT', cc.sizeof('short'))
cdata.set('SIZEOF_VOIDP', cc.sizeof('void*'))

gmtoff_src = '''#include <time.h>
int main(void) {
  struct tm t;
  t.tm_gmtoff = 0;
  exit(0);
}
'''

if cc.compiles(gmtoff_src)
  cdata.set('HAVE_TM_GMTOFF', 1)
endif

if cc.has_function('gmtime_r', prefix : '#include<time.h>')
  cdata.set('HAVE_GMTIME_R', 1)
endif
if cc.has_function('localtime_r', prefix : '#include<time.h>')
  cdata.set('HAVE_LOCALTIME_R', 1)
endif
if cc.has_function('sigaction', prefix : '#include<signal.h>')
  cdata.set('HAVE_SIGACTION', 1)
endif
if cc.has_function('fseeko', prefix : '#include<stdio.h>')
  cdata.set('HAVE_FSEEKO', 1)
endif
if cc.has_function('ftello', prefix : '#include<stdio.h>')
  cdata.set('HAVE_FTELLO', 1)
endif
if cc.has_function('fsetpos', prefix : '#include<stdio.h>')
  cdata.set('HAVE_FSETPOS', 1)
endif
if cc.has_function('fgetpos', prefix : '#include<stdio.h>')
  cdata.set('HAVE_FGETPOS', 1)
endif
if cc.has_function('poll', prefix : '#include<poll.h>')
  cdata.set('HAVE_POLL', 1)
endif
if cc.has_function('pselect', prefix : '#include<sys/select.h>')
  cdata.set('HAVE_PSELECT', 1)
endif
cdata.set('HAVE_MMAP', 1)

if cc.has_function('posix_memalign', prefix : '#include<stdlib.h>')
  cdata.set('HAVE_POSIX_MEMALIGN', 1)
endif
if cc.has_function('getpagesize', prefix : '#include<unistd.h>')
  cdata.set('HAVE_GETPAGESIZE', 1)
endif

if cc.has_type('ptrdiff_t')
  cdata.set('HAVE_PTRDIFF_T')
endif

if cc.has_function('getrusage', prefix : '#include<sys/resource.h>')
  cdata.set('HAVE_GETRUSAGE', 1)
endif

configure_file(input : 'config.h.meson',
  output : 'config.h',
  configuration : cdata)
