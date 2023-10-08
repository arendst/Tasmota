#! /bin/bash
set -euxo pipefail
editorconfig-checker
flake8
find . -type f -iregex ".*\.\(c\|h\|cpp\|ino\)" -exec clang-format-6.0 -i -style=file {} \; && git diff --exit-code
