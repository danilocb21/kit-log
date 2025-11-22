sources = files(
  'main.cpp',
  'data.cpp',
  'solution.cpp',
  'MLP.cpp',
  'GILS.cpp',
  'RNVD.cpp',
  'neighborhood.cpp',
  'construction.cpp',
  'perturbation.cpp',
  'subsequence.cpp',
)

incdir = include_directories('../include')

executable(
  meson.project_name(), 
  sources,
  include_directories: [incdir, incdir_deps],
  dependencies: [dependencies, external_dependencies],
  install: true
)

