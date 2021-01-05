# pushd

pushd takes messages on stdin and pipes each line to your pushover
account.  It gives the possibility to allow a specific string to be
ignored (note: no regex).  Having it statically linked comes from
my previous attempt, where I had it be executed on for each message,
hoping that a static binary would still work even if the system goes
kaputt.  Using it basically as daemon with syslog is the better option
and reduces the need for a static link.  For now it'll be kept as is.
It depends on cURL for major laziness.

It can be used as part of _/etc/rc.local_ to show your server has booted
up:

```
echo Booting up... | /usr/local/bin/pushd 2>/dev/null 1>&2 &
```

You can also use it with syslog by changing _/etc/syslog.conf_ adding

```
kern.* |/usr/local/bin/pushd
```
