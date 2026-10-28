include build/Makefile.template

pre:
	@build/check_files.sh
	@build/ramdisk.py

TOP=$(shell pwd)
DIRS=src
FORMAT_DIRS=$(DIRS) \
		src/things \
		src/things/beams \
		src/things/boosts \
		src/things/boots \
		src/things/boss \
		src/things/buffs \
		src/things/debuffs \
		src/things/dungeon \
		src/things/effect \
		src/things/environ \
		src/things/food \
		src/things/internal \
		src/things/internal/cursor \
		src/things/internal/levels \
		src/things/internal/things \
		src/things/minion \
		src/things/mob \
		src/things/monst \
		src/things/player \
		src/things/potions \
		src/things/projectiles \
		src/things/rings \
		src/things/sacrifices \
		src/things/spells \
		src/things/staff \
		src/things/traps \
		src/things/treasure \
		src/things/wand \
		src/tests

all: pre
	@$(foreach var,$(DIRS), $(MAKE) -C $(var) $(CORES) $@; )

format:
	(cd $(TOP)/src; $(TOP)/build/check_tps.sh)
	(cd $(TOP)/src; $(TOP)/build/check_sound.sh)
	$(foreach var,$(FORMAT_DIRS), (cd $(var); cp $(TOP)/.clang-format .) ;)
	$(foreach var,$(FORMAT_DIRS), (cd $(var); sh $(TOP)/build/shell_source.sh do) ;)
	$(foreach var,$(FORMAT_DIRS), (cd $(var); sh $(TOP)/build/tidy_source.sh do) ;)
	$(foreach var,$(FORMAT_DIRS), (cd $(var); sh $(TOP)/build/format.sh do) ;)
	-codespell src
	-cspell lint . 2>/dev/null | grep -v 3rd | grep "fix:" | sed 's/^.* - //g' | sort | uniq

tidy:
	$(foreach var,$(DIRS), $(MAKE) -C $(var) $(CORES) $@; )

clean_files:
	@rm -rf data/sounds/
	@rm -rf data/gfx/
	@rm -rf appdata/
	@rm -f build.log
	@rm -f $(TARGET)
	@rm -f $(TARGET).html
	@rm -f $(TARGET).js
	@rm -f $(TARGET).wasm
	@rm -rf $(TARGET).dsym
	@rm -f $(TARGET).exe

clean: clean_files
	$(foreach var,$(DIRS), $(MAKE) -C $(var) $@; )

clobber: clean_files
	$(foreach var,$(DIRS), $(MAKE) -C $(var) $@; )

.PHONY: all clean_files clean clobber format pre

.DEFAULT_GOAL := all
