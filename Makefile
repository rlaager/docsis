CC 	=  gcc 
DEFS	= -DYY_NO_UNPUT  -DDEBUG
#
# We assume ucd-snmp is installed in /usr/local. If not, replace with 
# actual package location
# 
CFLAGS	= -g -Wall -I/usr/local/include
LIBS  	= -L/usr/local/lib -lsnmp -lcrypto -lfl -lm
CFLAGS	= -g -Wall -I/opt/ucd-snmp-4.2/include
LIBS  	= -L/opt/ucd-snmp-4.2/lib -lsnmp -lcrypto -lfl -lm


docsis: Makefile docsis_main.c docsis_decode.o docsis.h docsis_globals.h docsis_yy.o docsis_main.o docsis_lex.l md5.o hmac_md5.o docsis_snmp.o ethermac.h ethermac.o version.h
	$(CC) $(CFLAGS) $(DEFS) -g -o docsis docsis_main.c docsis_decode.o docsis_yy.o md5.o hmac_md5.o docsis_snmp.o ethermac.o $(LIBS)
docsis_yy.o: docsis_yy.c
	$(CC) $(CFLAGS) $(DEFS) -c -o docsis_yy.o docsis_yy.c
docsis_yy.c: docsis_lex.l docsis_yy.tab.c docsis_symtable.h
	flex -odocsis_yy.c docsis_lex.l
docsis_yy.tab.c: docsis_yy.y docsis.h
	bison -t -v docsis_yy.y
md5.o: md5.c md5.h
	$(CC) $(CFLAGS) $(DEFS) -c -o md5.o md5.c
hmac_md5.o: md5.h hmac_md5.c
	$(CC) $(CFLAGS) $(DEFS) -c -o hmac_md5.o hmac_md5.c
docsis_snmp.o: docsis.h docsis_snmp.c
	$(CC) $(CFLAGS) $(DEFS) -c -o docsis_snmp.o docsis_snmp.c
docsis_decode.o: docsis.h docsis_decode.c
	$(CC) $(CFLAGS) $(DEFS) -c -o docsis_decode.o docsis_decode.c
ethermac.o: ethermac.h ethermac.c
	$(CC) $(CFLAGS) $(DEFS) -c -o ethermac.o ethermac.c
clean: 
	rm -f docsis_yy.output docsis_yy.tab.c docsis_yy.c docsis *.o 
