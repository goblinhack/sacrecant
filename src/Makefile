include ../build/Makefile.template

#
# For incremental builds, this is a lot faster
#
#ifeq ($(origin USE_PRECOMPILED),undefined)
#COMPILER_FLAGS+=-O0
#endif

NAME=sacrecant
BUILD_DIR=../.o
SCRIPT_DIR=../build/

TARGET_GAME=../$(NAME)$(EXE)
SRC_DIR=.

#
# To force clean and avoid "up to date" warning.
#
.PHONY: all clean clobber

all: $(TARGET_GAME)

# function to make object names from source files
# (changes suffix to .o, encodes directory names so subdirectories in build dir aren't necessary)
obj_name = $(addsuffix .o,$(subst /,__,$(subst ../,,$(basename $1))))

# recursively search all src/ subdirectories for cpp source files
CPP_SOURCES := $(subst ./,,$(shell find $(SRC_DIR) -name \*.cpp -not -path "*3rdparty*"))
CPP_SOURCES_NO_TESTS := $(wildcard *.cpp)
CPP_3RD_PARTY_SOURCES := $(subst ./,,$(shell find $(SRC_DIR)/3rdparty -name \*.cpp))
ASM_SOURCES := $(wildcard *.S)
SOURCES := $(CPP_3RD_PARTY_SOURCES) $(CPP_SOURCES) $(ASM_SOURCES)

OBJECTS := $(call obj_name,$(SOURCES))
OBJ_FILES := $(addprefix $(BUILD_DIR)/,$(OBJECTS))

DEP_FILES := $(OBJ_FILES:.o=.d)
DEP_FLAGS = -MMD -MP

#
# -isystem ignores header files for clang tidy
#
CFLAGS := -I. -isystem $(SDL2_INC_PATH) $(COMPILER_FLAGS) $(COMPILER_WARNINGS) $(DEP_FLAGS)

$(BUILD_DIR): ; mkdir -p $(BUILD_DIR)
$(OBJ_FILES): | $(BUILD_DIR)

# custom compile command for each source file needed because object file
# base names don't match source files that were in src/ subdirectories

PRECOMPILED_H := precompiled.hpp
PRECOMPILED_H_OBJ := $(PRECOMPILED_H).gch

#
# Must use the same flags as normal .cpp files for precompiled
#
%.hpp.gch: $(PRECOMPILED_H)
		@$(CC) $(CFLAGS) -c -o $@ $<

#
# Precompiled headers are good for the initial build, but after that it
# is faster to use dependencies
#
ifeq ($(origin USE_PRECOMPILED),undefined)

$(foreach x,$(CPP_SOURCES),\
$(eval $(BUILD_DIR)/$(call obj_name,$x): $x ;\
  @$(CC) $(CFLAGS) -c -o $$@ $$<))

else

$(foreach x,$(CPP_SOURCES),\
$(eval $(BUILD_DIR)/$(call obj_name,$x): $x | $(PRECOMPILED_H_OBJ) ;\
  @$(CC) -include $(PRECOMPILED_H) $(CFLAGS) -c -o $$@ $$<))

endif

$(foreach x,$(CPP_3RD_PARTY_SOURCES),\
$(eval $(BUILD_DIR)/$(call obj_name,$x): $x ;\
  @$(CC) $(CFLAGS) -c -o $$@ $$<))

$(foreach x,$(ASM_SOURCES),\
$(eval $(BUILD_DIR)/$(call obj_name,$x): $x ;\
  @$(CC) $(CFLAGS) -c -o $$@ $$<))

# use file list instead of wildcard for link command so we don't accidentally
# link with old object files for removed source files
$(TARGET_GAME): $(OBJ_FILES)
	@(cd $(BUILD_DIR); $(CC) $(OBJECTS) $(LDLIBS) $(LDFLAGS) -o $(TARGET_GAME))

#####################################################
# to show options:
# clang-tidy checks=* -list-checks
#
#	NOTE: Use these defines to avoid clang changing an area of code
#			xxx // NOLINT
#			// NOLINTBEGIN
#			xxx
#			// NOLINTEND
#####################################################
tidy:
	$(foreach x,$(CPP_SOURCES_NO_TESTS),\
		echo "Running tidy on $x..." ; \
		clang-tidy \
			-fix \
			-fix-errors \
			-fix-notes \
			--quiet \
			--system-headers=0 \
			-header-filter="*.hpp" \
			-checks="-*,\
			bugprone-*,\
			clang-analyzer-*,\
			cppcoreguidelines-*,\
			google-*,\
			conc*,\
			cert*,\
			hicpp-*,\
			misc-*,\
			modernize-*,\
			performance-*,\
			readability-*" \
			-header-filter=".*" \
			$x -- $(CFLAGS); \
	)
	git checkout 3rdparty
	$(SCRIPT_DIR)/splat "if (g == nullptr) [[unlikely]] {" "if (g == nullptr) [[unlikely]] {" "*" do
	$(SCRIPT_DIR)/splat "if (v == nullptr) [[unlikely]] {" "if (v == nullptr) [[unlikely]] {" "*" do
	$(SCRIPT_DIR)/splat "if (l == nullptr) [[unlikely]] {" "if (l == nullptr) [[unlikely]] {" "*" do
	$(SCRIPT_DIR)/splat "if (ext == nullptr) [[unlikely]] {" "if (ext == nullptr) [[unlikely]] {" "*" do
	$(SCRIPT_DIR)/splat "if (player == nullptr) [[unlikely]] {" "if (player == nullptr) [[unlikely]] {" "*" do
	$(SCRIPT_DIR)/splat "if (tp == nullptr) [[unlikely]] {" "if (tp == nullptr) [[unlikely]] {" "*" do
	$(SCRIPT_DIR)/splat "if (0)" "if (compiler_unused)" "*cpp" do
	$(SCRIPT_DIR)/splat "1 \|\| compiler_unused" "compiler_unused" "*" do
	$(SCRIPT_DIR)/splat-del "include.*OpenGL" "*cpp" do
	$(SCRIPT_DIR)/splat "<_" "<" "*cpp" do
	$(SCRIPT_DIR)/splat "Gamep *\/\*g\*\/" "Gamep g" "*" do
	$(SCRIPT_DIR)/splat "Levelsp *\/\*v\*\/" "Levelsp v" "*" do
	$(SCRIPT_DIR)/splat "Levelp *\/\*l\*\/" "Levelp l" "*" do
	$(SCRIPT_DIR)/splat "Widp *\/\*w\*\/" "Widp w" "*" do
	$(SCRIPT_DIR)/splat "int *\/\*x\*\/" "int x" "*" do
	$(SCRIPT_DIR)/splat "int *\/\*y\*\/" "int y" "*" do
	$(SCRIPT_DIR)/splat "uint32_t *\/\*button\*\/" "uint32_t button" "*" do
	$(SCRIPT_DIR)/splat "level_is_\([a-z0-9_]*\)(g, v, l, p) != nullptr" "level_is_\1_bool(g, v, l, p)" "*" do
	$(SCRIPT_DIR)/splat "empty()" "empty()" "*" do
	@echo "Done"

.PHONY: format tidy

clean:
	rm -f $(TARGET_GAME) ../stdout.txt ../stderr.txt
	@# safer delete of build dir instead of just 'rm -rf $(BUILD_DIR)'
	rm -f $(BUILD_DIR)/*.[od]
	@([ -d "$(BUILD_DIR)" ] && rmdir $(BUILD_DIR) 2>/dev/null) || true

clobber: clean
	rm -f ramdisk_data.cpp $(wildcard ramdisk_data*.S) $(wildcard *.gch)

# delete corresponding object file if dependency file needs regeneration
$(BUILD_DIR)/%.d: ; @rm -f "$(@:.d=.o)"

# include all dependency files
-include $(DEP_FILES)
