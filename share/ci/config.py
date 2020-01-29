from os import getenv, path

app_name = 'TaskLog'
app_version = 'test'

qt_version = '5.14.0'
qt_modules = ['qtbase', 'qttools', 'icu', 'qttranslations', 'qtx11extras']
qt_dir = path.abspath('qt')

build_dir = path.abspath('build')
cmake_file = path.abspath(path.dirname(__file__) + '/../../CMakeLists.txt')
ts_files_dir = path.abspath(path.dirname(__file__) + '/../../translations')

os_name = getenv('OS', 'linux')
app_version += {'linux': '', 'macos': '-experimental',
                'win32': '-experimental', 'win64': '-experimental'}[os_name]
bitness = '32' if os_name == 'win32' else '64'
msvc_version = getenv('MSVC_VERSION', '2017/Community')

with open(cmake_file, 'rt') as f:
    for line in f:
        if not line.startswith('set (VERSION "'):
            continue
        app_version = line[line.index('"') + 1:line.rindex('"')]
        break
