#/bin/bash
# Assert in build/ dir

curr_dir="${PWD}"
bin_dir="../bin/"
appname="comp4300"

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
  cmake --build .
  cp "src/${appname}" "${bin_dir}"
  cp -r ../assets/ "${bin_dir}"
  cp -r ../config/ "${bin_dir}"

  if [ "${1}" == "r" ]; then
    run
  fi
fi