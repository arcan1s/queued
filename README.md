queued
======

Information
-----------
Daemon for starting jobs to queue of calculations. To start deamon just run `systemctl start queued` as root.

License
-------
GPLv3

help message
------------
    queued [ -c /etc/queued.conf ] [ -v | --version ] [ -h | --help ]

    Parametrs:
      -c               PATH     - path to configuration file. Default is `/etc/queued.conf`
      -v   --version            - show version and exit
      -h   --help               - show this help and exit

Configuration
-------------
All settings are stored in `/etc/queued.conf`. After edit them you must restart daemon (`systemctl restart queued`).

How-to run jobs
---------------
* create shell script with the command (it have a name `script.sh` for example)
* create priority file (`script.sh.pr`) with the job priority if it is needed
* create user file (`script.sh.user`) with the job username if it is needed
* copy files to `$WORKDIR`

See also
--------
    man 1 queued
    man 5 queued.conf

Instruction
===========

Dependencies
------------
* bash (sed, awk, etc)
* systemd

Installation
------------
* download sources
* run `install.sh` from source directory:

        cd queued
        sudo ./install.sh "/path/to/root"
