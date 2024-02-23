#!/usr/bin/env bash
#
# This script checks whether there are any unstaged changes. If clean,
# return zero, otherwise warn the user that there are unstaged changes
# and prompt, returning nonzero in the default case

set -e

# `git status --porcelain` formats things for scripting
# | M changed file, unstaged
# |M  changed file, staged (git add has run)
# |MM changed file, some staged and some unstaged changes (git add then changes)
# |?? untracked file
if [ ! $TOCK_NO_CHECK_UNSTAGED ]; then
  if git status --porcelain | grep '^.M.*\.[ch].*' -q; then
    echo "$(tput bold)Warning: This operation may overwrite files in place.$(tput sgr0)"
    echo "While this is probably what you want, it's often useful to"
    echo "stage all of your changes (git add ...) before running,"
    echo "just so you can double-check everything."
    echo ""
    echo "$(tput bold)git status:$(tput sgr0)"
    git status
    echo ""
    read -p "Continue with unstaged changes? [y/N] " response
    if [[ ! ( "$(echo "$response" | tr :upper: :lower:)" == "y" ) ]]; then
      exit 1
    fi
  fi
fi

