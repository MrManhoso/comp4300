#/bin/bash
# Assert in root dir

curr_dir="${PWD}" 
bin_dir="bin/"
build_dir="build/"
appname="comp4300"

build() {
  echo "Building..."
  cmake --build "${build_dir}"
  res=$?
  echo "Build DONE! Exit code: ${res}"
  return ${res}
}

copy_files() {
  echo "Copying files to ${bin_dir}"
  cp "${build_dir}src/${appname}" "${bin_dir}"
  cp -r assets/ "${bin_dir}"
  cp -r config/ "${bin_dir}"
  echo "Copying DONE!"
}

run() {
  cd "${bin_dir}"
  ./${appname}
  cd "${curr_dir}"
}

# run only:
if [ "${1}" == "ro" ]; then
  run
# build and run
else
  if ! build; then
    echo "Build failed, exiting!"
    exit 1
  fi
  copy_files
  if [ "${1}" == "r" ]; then
    run
  fi
fi