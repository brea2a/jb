
AM_CFLAGS	= -Wall -std=c99 -O2

bin_PROGRAMS	= jo
jo_SOURCES	= jo.c json.c json.h
jo_EXTRA	= jo.pandoc
dist_man_MANS	= jo.1
jo_LDADD	= -lm

#jo.1: jo.pandoc
#	@test -n "$(PANDOC)" || \
#	  { echo 'pandoc' not found during configure.; exit 1; }
#	$(PANDOC) -s -w man jo.pandoc -o $@ $<

#jo.md: jo.pandoc
#	@test -n "$(PANDOC)" || \
#	  { echo 'pandoc' not found during configure.; exit 1; }
#	$(PANDOC) -s -w man jo.pandoc -o $@ $<

# docdir		= $(datadir)/doc/@PACKAGE@
# doc_DATA	= README.md 

TESTS		= tests.sh

EXTRA_DIST	= $(TESTS)
