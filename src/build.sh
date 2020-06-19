SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $SCRIPT_DIR
clang++ -g main.cpp `llvm-config --cxxflags --ldflags --libs core BitWriter --system-libs` -lpthread -o main