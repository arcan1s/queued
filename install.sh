#!/bin/bash

func_error() {
  echo "Error installing"
  exit 1
}

[ -z "$1" ] && DESTDIR="/" || DESTDIR="$1"
[ -d "$DESTDIR" ] || mkdir -p "$DESTDIR" || func_error

cd "$(dirname "${BASH_SOURCE[0]}")"
# daemon and configuration
install -D -m755 "usr/bin/queued" "$DESTDIR/usr/bin/queued" || func_error
install -D -m644 "etc/queued.conf" "$DESTDIR/etc/queued.conf" || func_error
# service
install -D -m644 "usr/lib/systemd/system/queued.service" "$DESTDIR/usr/lib/systemd/system/queued.service"  || func_error
# man pages
install -D -m644 "usr/share/man/man1/queued.1" "$DESTDIR/usr/share/man/man1/queued.1" || func_error
install -D -m644 "usr/share/man/man5/queued.conf.5" "$DESTDIR/usr/share/man/man5/queued.conf.5" || func_error

exit 0
