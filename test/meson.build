test_logger_exe = executable('test_logger', 
    ['test_logger.c'],
    dependencies: [dependency_libdpdk],
    install: false)

test('test_logger', test_logger_exe, 
    args: ['-a0:00.0', '-c0x1'],
    env: ['LD_PRELOAD=' + log_lib.full_path()])
