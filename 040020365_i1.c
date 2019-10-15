/*************************************
 * Bilgisayar Haberle�mesi �dev-1    *
 *								     *
 * ��renci : MEHMET CAMBAZ           *
 * No      : 040020365               *
 * E-mail  : mehmet_cambaz@yahoo.com *
 *************************************/

/** istemci 1 **/

#include <stdlib.h>
#include <stdio.h>
#include "cnaiapi.h"

#define BUFFSIZE 256
#define INPUT_PROMPT	"1: Gir    -> "
#define RECEIVED_PROMPT	"1: Alinan -> "

int recvln(connection, char *, int);
int readln(char *, int);

/*-----------------------------------------------------------------------
 *
 * Program : istemci 1
 * Ama�    : sunucuya ba�lanacak 1.istemci
 * Kullan�m: 040020365_i1 <bilgisayarad�> <uygulamano>
 * �rne�in   040020365_i1 127.0.0.1 25000
 *-----------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
	computer comp;
	connection conn;
	char buff[BUFFSIZE]={0};
	char buff2[BUFFSIZE]={0};
	int	len=0,len2=0;

	if (argc != 3) {
		(void) fprintf(stderr, "Kullanim: %s <bilgisayaradi> <uygulamano>\n", argv[0]);
		exit(1);
	}

	/* bilgisayar ad�n� say�sal de�ere �evir */
	comp = cname_to_comp(argv[1]);
	if (comp == -1)
		exit(1);	//bilgisayar ad� tan�namad�ysa ��k

	/* sunucuya ba�lant� kur */
	conn = make_contact(comp, (appnum) atoi(argv[2]));
	if (conn < 0) 
		exit(1);	//ba�lant� kurmada sorun varsa ��k

	(void) printf("Sunucu %s ile baglanti kuruldu. %d\n\n", argv[1], conn);
	(void) printf(INPUT_PROMPT);
	(void) fflush(stdout);

	
	while ( (len = readln(buff, BUFFSIZE)) > 0 ) {	/*komut sat�r�ndan yaz�y� al*/

		buff[len - 1] = '\n';
		(void) send(conn, buff, len, 0);	/*sunucuya g�nder*/


			if ((len2 = recvln(conn, buff2, BUFFSIZE)) < 1)		/*sunucudan gelecek yaz�y� bekle*/
				break;

			(void) printf(RECEIVED_PROMPT);		//al�nan katar i�in ba�lang�� k�sm�
			(void) fflush(stdout);
			(void) write(STDOUT_FILENO, buff2, len2);	//al�nan katar� ekrana bas					
			
			(void) printf(INPUT_PROMPT);		//yaz�lan katar i�in ba�lang�� k�sm�
			(void) fflush(stdout);		
	}

	/* kullan�c� ctrl+c yaparak ��karsa veya sunucudan EOF g�nderilirse program sonlan�r */

	(void) printf("\nBaglanti sonlandirildi.\n");
	(void) send_eof(conn);		//sonlan�rken EOF g�nder ki bekleyen varsa sonlans�n

	return 0;
}

