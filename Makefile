CC 	=  gcc 
DEFS	= -DYY_NO_UNPUT  -DDEBUG
#
# We assume ucd-snmp is installed in /opt/ucd-snmp-4.2. If not, replace with 
# actual package location
# -lsnmp is ucd-snmp 
# -lcrypto is OpenSSL. 
# 
# CFLAGS	= -g -Wall -I/usr/local/include
CFLAGS	= -g -Wall -I/opt/ucd-snmp-4.2/include
LIBS  	= -L/opt/ucd-snmp-4.2/lib -lsnmp -lcrypto -lnsl -lfl -lm

docsis: Makefile docsis_decode.o docsis_yy.o docsis_main.o md5.o hmac_md5.o docsis_snmp.o ethermac.h ethermac.o version.h docsis_encode.o 
	$(CC) $(CFLAGS) $(DEFS) -g -o docsis docsis_main.o docsis_encode.o docsis_decode.o docsis_yy.o md5.o hmac_md5.o docsis_snmp.o ethermac.o $(LIBS)
docsis_yy.o: docsis_yy.c
	$(CC) $(CFLAGS) $(DEFS) -c -o docsis_yy.o docsis_yy.c
docsis_yy.c: docsis_lex.l docsis_yy.tab.c docsis_symtable.h
	flex -odocsis_yy.c docsis_lex.l
docsis_yy.tab.c: docsis_yy.y docsis.h docsis_common.h docsis_encode.h
	bison -t -v docsis_yy.y
md5.o: md5.c md5.h
	$(CC) $(CFLAGS) $(DEFS) -c -o md5.o md5.c
hmac_md5.o: md5.h hmac_md5.c
	$(CC) $(CFLAGS) $(DEFS) -c -o hmac_md5.o hmac_md5.c
docsis_snmp.o: docsis.h docsis_snmp.c
	$(CC) $(CFLAGS) $(DEFS) -c -o docsis_snmp.o docsis_snmp.c
docsis_decode.o: docsis_common.h docsis_decode.h docsis.h docsis_decode.c
	$(CC) $(CFLAGS) $(DEFS) -c -o docsis_decode.o docsis_decode.c
docsis_encode.o: docsis_common.h docsis_encode.h docsis_encode.c
	$(CC) $(CFLAGS) $(DEFS) -c -o docsis_encode.o docsis_encode.c
docsis_main.o: docsis_common.h docsis_encode.h docsis_decode.h docsis.h docsis_symtable.h docsis_globals.h ethermac.h version.h  docsis_main.c
	$(CC) $(CFLAGS) $(DEFS) -c -o docsis_main.o docsis_main.c
ethermac.o: ethermac.h ethermac.c
	$(CC) $(CFLAGS) $(DEFS) -c -o ethermac.o ethermac.c
clean: 
	rm -f docsis_yy.output docsis_yy.tab.c docsis_yy.c docsis *.o  core
