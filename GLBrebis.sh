#!/bin/bash -e
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
pushd ${DIR}
groovy -cp "libs/*:src" src/com/glbrebis/GLBrebis $@
popd
