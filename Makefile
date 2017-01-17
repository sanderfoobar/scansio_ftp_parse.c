build: scansio_ftp_parse.c
	gcc scansio_ftp_parse.c -O3 -lm -mtune=native -march=native -o scansio_ftp_parse

debug: scansio_ftp_parse.c
	gcc scansio_ftp_parse.c -O3 -lm -mtune=native -march=native -g -o scansio_ftp_parse
