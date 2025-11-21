cd external/skia
rm -rf out/Static  # clean build
python3 git-sync-deps
bin/gn gen out/Static --args='is_official_build=true is_component_build=false skia_use_harfbuzz=false'
ninja -C out/Static
