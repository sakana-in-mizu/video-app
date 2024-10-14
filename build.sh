#! /bin/bash


if [ $# -ne 2 ]; then
    echo "Usage: $0 \${generator} \${config}"
    exit 0
fi

python_path=$(which python)
generator=$1
config=$2

if [ -z $python_path ]; then
    echo "Python not found!"
    exit 1
fi

definitions="-DPython_EXECUTABLE=$python_path"
if [ $generator = "Ninja" ]; then
    definitions="$definitions -DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
fi

cmake $definitions  -DCMAKE_BUILD_TYPE=$config -B build -G "$generator"
if [ $generator = "Ninja" ]; then
    mkdir -p .vscode
    cp build/compile_commands.json .vscode
fi

cmake --build build --config $config