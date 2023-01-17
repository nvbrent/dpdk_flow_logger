project('dpdk_flow_logger', 'C',
	version: '0.0.1',
    default_options: ['buildtype=debug', 'werror=true', 'warning_level=2'],
)

add_project_arguments('-DALLOW_EXPERIMENTAL_API', language: 'c')

dependency_libdpdk = dependency('libdpdk', required : false)
if not dependency_libdpdk.found()
	error('Please set PKG_CONFIG_PATH to DPDK packages path, for further instructions please refer to the DOCA Reference Applications guide')
endif

subdir('lib')
subdir('test')
