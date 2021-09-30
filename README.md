# rawcapture
---
## rawcapture tools for directly capturing network card data

```shell
Usage: rawcapture [interface] [size] [times]
    [interface]: eth0,rndis0,......
    [size]: capture and display buffer size
    [times]: capture times, if NULL will always running

    e.g.
        rawcapture eth0 32 3
```
---
```shell
HWaddr:12:34:56:78:9a:bc
start capture...
-1[Bad address]: 33 33 00 00 00 16 12 34 56 78 9a bc 86 dd 60 00 00 00 00 60 00 01 00 00 00 00 00 00 00 00 00 00
-1[Bad address]: ff ff ff ff ff ff 12 34 56 78 9a bc 08 00 45 10 01 3c 00 00 40 00 40 11 39 a2 00 00 00 00 ff ff
-1[Bad address]: 33 33 00 00 00 16 12 34 56 78 9a bc 86 dd 60 00 00 00 00 60 00 01 00 00 00 00 00 00 00 00 00 00
```
---
```c

data struct:
    [6 OCTETS]                 [6 OCTETS]             [2 OCTETS]        [46 TO 1500 OR 1504 OR 1982 OCTETS]
DESTINATION ADDRESS          SOURCE ADDRESS          LENGTH/TYPE          MAC CLIENT DATA(SEE 3.2.7)/PAD
```
