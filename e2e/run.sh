#!/usr/bin/env bash
# Index sample.cc with a scip-clang binary and print the access roles.
# Usage: e2e/run.sh [path/to/scip-clang]   (defaults to the bazel build)
set -euo pipefail
here="$(cd "$(dirname "$0")" && pwd)"
bin="${1:-$here/../bazel-bin/indexer/scip-clang}"
out="$here/out"
mkdir -p "$out"
cat > "$out/compile_commands.json" <<JSON
[{"directory": "$here", "file": "$here/sample.cc",
  "arguments": ["clang++", "-std=c++17", "-c", "$here/sample.cc", "-o", "/dev/null"]}]
JSON
(cd "$here" && "$bin" --compdb-path "$out/compile_commands.json" --index-output-path "$out/index.scip")
python3 -I "$here/dump_roles.py" "$out/index.scip" sample.cc
