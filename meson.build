project('jo', 'c',
        version: '1.6',
        license: 'GPL-2.0-or-later',
        meson_version: '>=0.57.0',
        default_options: ['warning_level=3', 'optimization=2'])


PACKAGE_VERSION = meson.project_version()

cc = meson.get_compiler('c')

headers = [
    'stddef.h',
    'stdint.h',
    'stdlib.h',
    'string.h',
    'unistd.h',
    'stdbool.h'
]

functions = [
    'strchr',
    'strrchr',
    'strlcpy',
    'strlcat',
    'snprintf',
    'pledge',
    'err',
    'errx'
]

foreach h: headers
cc.has_header(h, required: true)
endforeach
foreach f: functions
add_project_arguments(
    '-DHAVE_@0@='.format(f.to_upper()) +
    cc.has_function(f).to_int().to_string(),
    language: 'c')
endforeach

add_project_arguments('-DPACKAGE_VERSION="@0@"'.format(PACKAGE_VERSION),
                      language: 'c')


pandoc = find_program('pandoc', required: false)
if not pandoc.found()
warning('pandoc not found, man pages rebuild will not be possible')
jo1 = 'jo.1'
else
pandoc_commands = [pandoc, '-s', '-w', 'man', '-f', 'markdown', '-o']
jo1 = custom_target('jo.1',
                    output: 'jo.1',
                    input: 'jo.pandoc',
                    build_always_stale: true,
                    command: [pandoc_commands, '@OUTPUT@', '@INPUT@']).full_path()
run_command(pandoc_commands,
            join_paths(meson.current_build_dir(), 'jo.1'),
            join_paths(meson.current_source_dir(), 'jo.pandoc'))
custom_target('jo.md',
              output: 'jo.md',
              input: 'jo.pandoc',
              build_always_stale: true,
              command: [pandoc, '-s', '-w', 'gfm', '-f', 'markdown', '-o', '@OUTPUT@', '@INPUT@'])
endif

install_man(jo1)

bashcomp = dependency('bash-completion', required: false)
if bashcomp.found()
bashcompdir = bashcomp.get_variable(pkgconfig: 'completionsdir')
else
bashcompdir = join_paths(get_option('sysconfdir'), 'bash_completion.d')
endif

install_data('jo.bash', install_dir: bashcompdir)

m_dep = cc.find_library('m', required : false)

executable('jo',
           'jo.c',
           'base64.c',
           'base64.h',
           'json.c',
           dependencies: m_dep,
           install: true)

summary({'Prefix': get_option('prefix'),
         'C compiler': cc.get_id(),
         'Pandoc': pandoc,
         'Bash completion': join_paths(bashcompdir, 'jo.bash'),
         })
