# pitrex-baremetal
![PiRrex](http://vide.malban.de/wp-content/uploads/2020/01/pitrex-1-1024x843.png)

Read more about PiTrex in general at: [PiTrex Wiki](http://www.ombertech.com/cnk/pitrex/wiki/index.php).

Read more about PiTrex Baremetal at: [Malbans PiTrex baremetal pages](http://vide.malban.de/pitrex) (and the pages accessable by the menu).

To develop baremetal PiTrex you can download a prepared virtual machine for Virtual box (Ubuntu 16.04). Don't forget to update the sources with the ones found here (on github):  [PiTrex baremetal development virtual machine](http://vide.malban.de/pitrex/pitrex-baremetal-quick-start-unfinished).


# Structure of this repository

## Baremetal "library"
All stuff under the directory "pitrex" is the actual "baremetal boot source" to build baremetal programs. For a short description of the "bootup" look at: [PiTrex – Baremetal knowledge](http://vide.malban.de/pitrex/pitrex-baremetal-knowledge).

## Baremetal "vectrexInterface"
All stuff under the directory "pitrex/vectrex" is the actual "interface" to the vectrex. For a short description at: [PiTrex – Baremetal knowledge](http://vide.malban.de/pitrex/pitrex-baremetal-vectrexinterface-c).

All other "single" directories within the repository represent single "programs" runnable under the baremetal environment. Within each directory is a single Makefiles.baremetal to create baremetal images.
