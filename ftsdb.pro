#

# This include is not strictly necessary but
# might be good way to catch errors early
include( $$(QMKBLD)/root.pri )


TEMPLATE = subdirs
SUBDIRS = src test

# Emacs tags support
tag.target = TAGS
tag.commands = ctags -R -e --extra=+q
tag.depends =

QMAKE_EXTRA_TARGETS += tag

