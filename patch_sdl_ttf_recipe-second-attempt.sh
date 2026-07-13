#!/bin/sh
set -e

RECIPE=$(find "$HOME/.conan2/p" -path "*/e/conanfile.py" \
    -exec grep -l 'self.cpp_info.libs = \[f"SDL3_ttf{lib_suffix}"\]' {} \; \
    | head -n 1)

if [ -z "$RECIPE" ]; then
    echo "Could not find SDL3_ttf conanfile.py"
    exit 1
fi

echo "Patching:"
echo "$RECIPE"

sed -i \
's/if self.settings.os == "Windows" and not self.options.shared:/if self.settings.os == "Windows" and self.settings.compiler == "msvc" and not self.options.shared:/' \
"$RECIPE"

echo "Result:"
grep -A5 -B2 "lib_suffix" "$RECIPE"
