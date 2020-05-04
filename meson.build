project('libasync', 'cpp')

cxxshim_coroutine_dep = subproject('cxxshim').get_variable('clang_coroutine_dep')
frigg_dep = subproject('frigg').get_variable('frigg_dep')
gtest_dep = dependency('gtest', main: true, required: false)

gtest_binary = executable('gtests',
	'tests/basic.cpp',
	'tests/queue.cpp',
	'tests/mutex.cpp',
	include_directories: include_directories('include/'),
	cpp_args: '-fcoroutines-ts',
	dependencies: [
		cxxshim_coroutine_dep,
		frigg_dep,
		gtest_dep
	],
	override_options: [
		'cpp_std=c++2a'
	])

test('gtest test', gtest_binary)