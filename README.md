# scansio-ftp-parse.c
Extracts IP addresses from a scansio FTP results file. This file usually has around 22 million ip addresses, in json format.

https://scans.io/series/21-ftp-banner-full_ipv4

This program can parse 22 million lines in 5 seconds. 

# Example

    root@lel:/mnt/tmpfs# ./scansio_ftp_parse fa9pzamo9ravx7mp-21-ftp-banner-full_ipv4-20160201T022940-zgrab-results.json
    writing: 20160201T022940_1.txt
    [...]
    writing: 20160201T022940_228.txt
    
    real	0m5.413s
    user	0m0.000s
    sys	0m5.312s
    root@lel:/mnt/tmpfs#

