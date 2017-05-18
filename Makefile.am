
AM_CFLAGS	= -Wall -O2

bin_PROGRAMS	= jo
jo_SOURCES	= jo.c json.c json.h base64.c base64.h
jo_EXTRA	= jo.pandoc
dist_man_MANS	= jo.1
jo_LDADD	= -lm

if USE_PANDOC
# Add targets to rebuild pages
jo.1: jo.pandoc
	@test -n "$(PANDOC)" || \
	  { echo 'pandoc' not found during configure.; exit 1; }
	$(PANDOC) -s -w man+simple_tables -o $@ $<

jo.md: jo.pandoc
	@test -n "$(PANDOC)" || \
	  { echo 'pandoc' not found during configure.; exit 1; }
	$(PANDOC) -s -w markdown+simple_tables -o $@ $<

endif

# docdir		= $(datadir)/doc/@PACKAGE@
# doc_DATA	= README.md 

# If on OS/X, fail if $COPYFILE_DISABLE is not in the environment
# so that tar doesn't bundle the AppleDouble attributes

dist-hook:
	if test $$(uname -s) = "Darwin" -a "x$$COPYFILE_DISABLE" = "x"; then echo "Set COPYFILE_DISABLE before making dist" >&2; exit 2; fi 

TEST_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) \
                  $(top_srcdir)/build-aux/tap-driver.sh

TESTS		= tests/jo.test

EXTRA_DIST	= $(jo_EXTRA) \
		  $(TESTS) \
		  tests/jo.01.sh tests/jo.01.exp \
		  tests/jo.02.sh tests/jo.02.exp \
		  tests/jo.03.sh tests/jo.03.exp \
		  tests/jo.04.sh tests/jo.04.exp \
		  tests/jo.05.sh tests/jo.05.exp \
		  tests/jo.06.sh tests/jo.06.exp \
		  tests/jo.07.sh.in \
		  tests/jo.08.sh tests/jo.08.exp \
		  tests/jo.09.sh tests/jo.09.exp \
		  tests/jo.10.sh tests/jo.10.exp \
		  tests/jo.11.sh tests/jo.11.exp \
		  tests/jo.12.sh tests/jo.12.exp \
		  tests/jo.13.sh tests/jo.13.exp tests/jo-logo.png \
		  tests/jo.14.sh tests/jo.14.exp 
