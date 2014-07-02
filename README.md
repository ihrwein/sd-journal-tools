# Tools for systemd's journal

 * jdump: dumps all available journal entries
 * jtail: prints the last 10 entries, then waits for new ones (like tail -f)

You can generate log messages with the logger command.

# Dependencies
Tested on RHEL 7.0:
 * gcc
 * systemd-devel
