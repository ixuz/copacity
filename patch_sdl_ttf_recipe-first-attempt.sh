#!/bin/sh
set -e

RECIPE=$(find "$HOME/.conan2/p" -path "*/e/conanfile.py" \
    -exec grep -l 'self\.cpp_info\.libs = \[f"SDL3_ttf{lib_suffix}"\]' {} \; \
    | head -n 1)

if [ -z "$RECIPE" ]; then
    echo "Could not find SDL3_ttf recipe containing the expected cpp_info.libs line."
    exit 1
fi

echo "Patching:"
echo "$RECIPE"

sed -i 's/self\.cpp_info\.libs = \[f"SDL3_ttf{lib_suffix}"\]/self.cpp_info.libs = ["SDL3_ttf"]/' "$RECIPE"

echo
echo "Result:"
grep -n "cpp_info.libs" "$RECIPE"

echo
echo "Now rebuild the package:"
echo "  conan remove 'sdl_ttf/3.2.2:*' -c"
echo "  conan install . -of build/windows --build missing -pr:h=conan/profiles/windows"
