#!/bin/sh
# Re-download every imported file at its pinned commit and compare (proves "unmodified").
cd "$(dirname "$0")"; rc=0
while read p f s; do
  base=$(awk -v p="$p" '$1==p {print $2}' SOURCES.lock)
  t=$(curl -sSfL "$base/$f" | sha256sum | cut -c1-64)
  [ "$t" = "$s" ] || { echo "DIFF $p/$f"; rc=1; }
done < SHA256SUMS
[ $rc = 0 ] && echo "all $(wc -l < SHA256SUMS) files match upstream"; exit $rc
