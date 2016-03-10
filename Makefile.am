
#AM_CFLAGS	= -Wall -O2

bin_PROGRAMS	= jo
jo_SOURCES	= jo.c json.c json.h
jo_EXTRA	= jo.pandoc
dist_man_MANS	= jo.1
jo_LDADD	= -lm

if USE_PANDOC
# Add targets to rebuild pages
jo.1: jo.pandoc
	@test -n "$(PANDOC)" || \
	  { echo 'pandoc' not found during configure.; exit 1; }
	$(PANDOC) -s -w man -o $@ $<

jo.md: jo.pandoc
	@test -n "$(PANDOC)" || \
	  { echo 'pandoc' not found during configure.; exit 1; }
	$(PANDOC) -s -w markdown -o $@ $<

endif

# docdir		= $(datadir)/doc/@PACKAGE@
# doc_DATA	= README.md 

TEST_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) \
                  $(top_srcdir)/build-aux/tap-driver.sh

TESTS		= tests/jo.test

EXTRA_DIST	= $(jo_EXTRA) \
		  $(TESTS) \
		  tests/jo.01.in tests/jo.01.exp \
		  tests/jo.02.in tests/jo.02.exp \
		  tests/jo.03.in tests/jo.03.exp \
		  tests/jo.04.in tests/jo.04.exp \
		  tests/jo.05.in tests/jo.05.exp \
		  tests/jo.06.in tests/jo.06.exp
