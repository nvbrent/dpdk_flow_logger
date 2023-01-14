project('dpdk_flow_logger', 'C',
	version: '0.0.1',
    default_options: ['warning_level=2']
)

add_project_arguments('-DALLOW_EXPERIMENTAL_API', language: 'c')

dependency_libdpdk = dependency('libdpdk', required : false)
if not dependency_libdpdk.found()
	error('Please set PKG_CONFIG_PATH to DPDK packages path, for further instructions please refer to the DOCA Reference Applications guide')
endif
app_dependencies = [dependency_libdpdk]
app_inc_dirs = include_directories()
app_srcs = [
	meson.project_name() + '.c',
]

log_lib = library(meson.project_name(), app_srcs,
	dependencies : app_dependencies,
	include_directories : app_inc_dirs,
	install: false)

subdir('test')
