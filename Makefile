palestra.exe: main.o area_clienti.o area_gestore.o cliente.o data.o hash.o lezione.o lista_lezioni.o utils.o prenotazione.o orario.o hash_prenotazioni.o report.o
	gcc main.o area_clienti.o area_gestore.o cliente.o data.o hash.o lezione.o lista_lezioni.o utils.o prenotazione.o orario.o hash_prenotazioni.o report.o -o palestra.exe

main.o: main.c area_clienti.h area_gestore.h lista_lezioni.h utils.h hash.h hash_prenotazioni.h report.h
	gcc -c main.c

orario.o: orario.c orario.h
	gcc -c orario.c

area_clienti.o: area_clienti.c cliente.h hash.h data.h lista_lezioni.h utils.h hash_prenotazioni.h
	gcc -c area_clienti.c

area_gestore.o: area_gestore.c area_gestore.h cliente.h lezione.h lista_lezioni.h hash.h utils.h orario.h data.h hash_prenotazioni.h report.h
	gcc -c area_gestore.c

cliente.o: cliente.c cliente.h data.h
	gcc -c cliente.c

data.o: data.c data.h
	gcc -c data.c

hash.o: hash.c hash.h cliente.h
	gcc -c hash.c

lezione.o: lezione.c lezione.h data.h orario.h
	gcc -c lezione.c

lista_lezioni.o: lista_lezioni.c lista_lezioni.h lezione.h
	gcc -c lista_lezioni.c

prenotazione.o: prenotazione.c prenotazione.h cliente.h lezione.h data.h utils.h
	gcc -c prenotazione.c

utils.o: utils.c utils.h data.h hash.h cliente.h lezione.h lista_lezioni.h orario.h hash_prenotazioni.h prenotazione.h
	gcc -c utils.c

hash_prenotazioni.o: hash_prenotazioni.c hash_prenotazioni.h prenotazione.h
	gcc -c hash_prenotazioni.c

report.o: report.c prenotazione.h lezione.h report.h data.h utils.h hash_prenotazioni.h lista_lezioni.h
	gcc -c report.c

clean:
	rm -f *.o palestra.exe
