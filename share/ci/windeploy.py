import common as c
from config import *
import os
import sys

artifact_name = '{}-{}-{}.zip'.format(app_name, app_version, os_name)
if len(sys.argv) > 1 and sys.argv[1] == 'artifact_name':  # subcommand
    c.print(artifact_name)
    exit(0)
artifact_path = os.path.abspath(artifact_name)

c.print('>> Making win deploy')

pwd = os.getcwd()
os.chdir(build_dir)

install_dir = os.path.abspath(app_name)
c.recreate_dir(install_dir)

env_cmd = c.get_cpp_env_cmd(bitness=bitness, msvc_version=msvc_version)
os.environ["DESTDIR"] = install_dir
install_cmd = 'cmake --build . --target install'
deploy_cmd = '{}/bin/windeployqt.exe "{}"'.format(qt_dir, install_dir)

c.run('{} && {}'.format(env_cmd, install_cmd))
binary = 'tasklog.exe'
os.rename(install_dir + '/bin/' + binary, install_dir + '/' + binary)
os.rmdir('bin')
c.run('{} && {}'.format(env_cmd, deploy_cmd))

c.archive(c.get_folder_files(os.path.relpath(install_dir)), artifact_path)
