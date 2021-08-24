#!/bin/sh
#
# Given a path to your shell, runs the tests for each stage
# sequentially until one fails, and prints the detailed results for
# the failing set.

tput() { command tput "$@" 2>/dev/null; }
attr() { tput AF "$1" || tput setaf "$1"; }
red() { attr 1; }
green() { attr 2; }
yellow() { attr 3; }
reset() { tput me || tput sgr0; }
die() { echo "$@"; exit 1; }

# I'm legit guilty about the couple of disabled shellcheck warnings in
# this file.  They are far from the biggest portability issues in this
# code, though, sadly.
#
# shellcheck disable=SC2166
if [ $# -lt 1 -o $# -gt 3 ]; then
die "Usage: validate PATH_TO_SHELL [STAGE] [NUMBER]"
fi

sh_under_test=$(which "$1")
sh_under_test=$(cd -P -- "$(dirname -- "$sh_under_test")" && echo "$(pwd -P)/$(basename -- "$sh_under_test")")

if [ ! -x "$sh_under_test" ]; then
die "Can't execute $sh_under_test; not running any tests."
fi

[ -x "$(which expect)" ] || die '"expect" is missing'

byos=$(dirname -- "$0")

if [ ! -d "$byos/stage_1" ]; then
die "Something went wrong; stage_1 missing in $byos."
fi

# build helpers
for c in "$byos"/helpers/*.c; do
                          exe="$byos"/helpers/"$(basename "$c" .c)"
# shellcheck disable=SC2166,SC2039
if [ ! -x "$exe" -o "$c" -nt "$exe" ]; then "$c"; fi
done

log=$(mktemp -t byos-validate.XXXXXX)
trap 'rm -f $log' EXIT

do_stage() {
stage=$1
printf '%s%s:%s ' "$(yellow)" "$(basename "$stage")" "$(reset)"
for t in "$stage"/*.t; do
do_test "$stage" "$t"
done
echo
}

do_test() {
short=$(basename "$1")
test=$2
if "$byos"/helpers/timeout 15 \
"$byos"/helpers/harness.tcl "$test" "$sh_under_test" > "$log"; then
printf '%s%s%s ' "$(green)" "$(basename "$test")" "$(reset)"
else
echo "$(red)$(basename "$test")$(reset)"
echo
cat "$log"
echo
echo '---------------------------'
echo "You still need to do $short"
die 'Keep working!'
fi
}

if [ $# -eq 3 ]; then
exec "$byos"/helpers/harness.tcl "$byos/$2/$3"*.t "$sh_under_test"
elif [ $# -eq 2 ]; then
do_stage "$2"
exit
fi
for stage in "$byos"/stage_[0-9]; do
do_stage "$stage"
done
echo

echo "$(yellow)⸙ Congratulations! ⸙$(reset)"
echo "Your shell passes all the tests.  Why don't you publish it,"
echo "and let julian@cipht.net know how you liked this tutorial?"
