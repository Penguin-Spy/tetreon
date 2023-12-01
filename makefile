# ----------------------------
# Makefile Options
# ----------------------------

NAME = TETREON
ICON = icon.png
DESCRIPTION = "Tetris clone with modern tetris rules"
COMPRESSED = YES
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
