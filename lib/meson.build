app_dependencies = [dependency_libdpdk]
app_dependencies += dependency('json-c', required: true)
app_inc_dirs = include_directories('.')
app_srcs = [
	meson.project_name() + '.c',
    'wrapper_init.c',
	'flow_to_json.c',
]

log_lib = library(meson.project_name(), app_srcs,
	dependencies : app_dependencies,
	include_directories : app_inc_dirs,
	install: false)
