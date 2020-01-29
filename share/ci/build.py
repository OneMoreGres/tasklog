import common as c
from config import *
import os
from multiprocessing import cpu_count

c.print('>> Building {} on {}'.format(app_name, os_name))

os.environ["CMAKE_PREFIX_PATH"] = qt_dir + '/lib/cmake'
c.recreate_dir(build_dir)

env_cmd = c.get_cpp_env_cmd(bitness=bitness, msvc_version=msvc_version)

os.chdir(build_dir)
os.environ["MAKEFLAGS"] = "-j{}".format(cpu_count())
os.environ['CL'] = '/MP'
arch = ''
if os_name.startswith('win'):
    arch = '-A ' + ('Win32' if os_name == 'win32' else 'x64')
c.run('{} && cmake {} "{}"'.format(env_cmd, arch, os.path.dirname(cmake_file)))
c.run('{} && cmake --build .'.format(env_cmd))
