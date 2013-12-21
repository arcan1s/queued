queued
======

Information
-----------
Daemon for starting jobs to queue of calculations. To start deamon just run `systemctl start queued` as root.

help message
------------
    queued [ -jd | --jobdir /var/lib/queued/job ] [ --priority 0 ] [ -q | --queuefile /var/lib/queued/queue ] [ -u | --user root ] [ -wd | --workdir /var/lib/queued/work ] [-v | --version ] [ -h | --help ]

    Parametrs:
      -jd  --jobdir    PATH     - path to job directory. Default is '/var/lib/queued/job'
           --priority  NUM      - defalut priority. Default is '0'
      -q   --queuefile PATH     - path to queue file. Default is '/var/lib/queued/queue'
      -u   --user      USERNAME - start jobs by user. Default is 'root'
      -wd  --workdir   PATH     - path to work directory. Default is '/var/lib/queued/work'
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


Instruction
===========

Dependencies
------------
* bash (sed, awk, etc)
* systemd

Installation
------------
* download sources
* copy source to `/`:

        sudo cp ../queued/* /
