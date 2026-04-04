include ../build/Makefile.template

#
# For incremental builds, this is a lot faster
#
#ifeq ($(origin USE_PRECOMPILED),undefined)
#COMPILER_FLAGS+=-O0
#endif

NAME=sacrecant
BUILD_DIR=../.o

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
		$(CC) $(CFLAGS) -c -o $@ $<

#
# Precompiled headers are good for the initial build, but after that it
# is faster to use dependancies
#
ifeq ($(origin USE_PRECOMPILED),undefined)

$(foreach x,$(CPP_SOURCES),\
$(eval $(BUILD_DIR)/$(call obj_name,$x): $x ;\
  $(CC) $(CFLAGS) -c -o $$@ $$<))

else

$(foreach x,$(CPP_SOURCES),\
$(eval $(BUILD_DIR)/$(call obj_name,$x): $x | $(PRECOMPILED_H_OBJ) ;\
  $(CC) -include $(PRECOMPILED_H) $(CFLAGS) -c -o $$@ $$<))

endif

$(foreach x,$(CPP_3RD_PARTY_SOURCES),\
$(eval $(BUILD_DIR)/$(call obj_name,$x): $x ;\
  $(CC) $(CFLAGS) -c -o $$@ $$<))

$(foreach x,$(ASM_SOURCES),\
$(eval $(BUILD_DIR)/$(call obj_name,$x): $x ;\
  $(CC) $(CFLAGS) -c -o $$@ $$<))

# use file list instead of wildcard for link command so we don't accidentally
# link with old object files for removed source files
$(TARGET_GAME): $(OBJ_FILES)
	cd $(BUILD_DIR); $(CC) $(OBJECTS) $(LDLIBS) $(LDFLAGS) -o $(TARGET_GAME); $(DSYM)

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
			misc-confusable-identifiers,\
			misc-const-correctness,\
			misc-definitions-in-headers,\
			misc-misleading-bidirectional,\
			misc-misleading-identifier,\
			misc-misplaced-const,\
			misc-new-delete-overloads,\
			misc-non-copyable-objectsl,\
			misc-redundant-expression,\
			misc-static-assert,\
			misc-unconventional-assign-operator,\
			misc-unused-alias-decls,\
			misc-use-internal-linkage,\
			modernize-*,\
			performance-*,\
			readability-*" \
			-header-filter=".*" \
			$x -- $(CFLAGS); \
	)
	splat "if (ext == nullptr) [[unlikely]] {" "if (ext == nullptr) [[unlikely]] {" "*" do
	splat "if (player == nullptr) [[unlikely]] {" "if (player == nullptr) [[unlikely]] {" "*" do
	splat "if (tp == nullptr) [[unlikely]] {" "if (tp == nullptr) [[unlikely]] {" "*" do
	git checkout 3rdparty
	@echo "Done"

.PHONY: style check-style tidy

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
