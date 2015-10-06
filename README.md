# arewethereyet
Wait until a log contains a string, the log writer was killed, or a timeout was reached.

Usage: 
```
arewethereyet [PID] [timeout] [logfile] [searchstring]
```

Example:
```
$ sleep 100 &
[1] 6903
$ (sleep 10; echo "We are done here" >> test.log) &
[2] 6904
$ touch test.log
$ arewethereyet 6903 5.0 test.log "done here"
searching for "done here"
Found string in "We are done here"
```
Timeout is in seconds

Exit codes:
 * String found: 0
 * PID was killed: 1
 * Timeout: 2
 * Any error: -1
