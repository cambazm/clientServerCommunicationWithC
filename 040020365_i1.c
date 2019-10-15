/*************************************
 * Bilgisayar Haberleþmesi Ödev-1    *
 *								     *
 * Öðrenci : MEHMET CAMBAZ           *
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
 * Amaç    : sunucuya baðlanacak 1.istemci
 * Kullaným: 040020365_i1 <bilgisayaradý> <uygulamano>
 * örneðin   040020365_i1 127.0.0.1 25000
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

	/* bilgisayar adýný sayýsal deðere çevir */
	comp = cname_to_comp(argv[1]);
	if (comp == -1)
		exit(1);	//bilgisayar adý tanýnamadýysa çýk

	/* sunucuya baðlantý kur */
	conn = make_contact(comp, (appnum) atoi(argv[2]));
	if (conn < 0) 
		exit(1);	//baðlantý kurmada sorun varsa çýk

	(void) printf("Sunucu %s ile baglanti kuruldu. %d\n\n", argv[1], conn);
	(void) printf(INPUT_PROMPT);
	(void) fflush(stdout);

	
	while ( (len = readln(buff, BUFFSIZE)) > 0 ) {	/*komut satýrýndan yazýyý al*/

		buff[len - 1] = '\n';
		(void) send(conn, buff, len, 0);	/*sunucuya gönder*/


			if ((len2 = recvln(conn, buff2, BUFFSIZE)) < 1)		/*sunucudan gelecek yazýyý bekle*/
				break;

			(void) printf(RECEIVED_PROMPT);		//alýnan katar için baþlangýç kýsmý
			(void) fflush(stdout);
			(void) write(STDOUT_FILENO, buff2, len2);	//alýnan katarý ekrana bas					
			
			(void) printf(INPUT_PROMPT);		//yazýlan katar için baþlangýç kýsmý
			(void) fflush(stdout);		
	}

	/* kullanýcý ctrl+c yaparak çýkarsa veya sunucudan EOF gönderilirse program sonlanýr */

	(void) printf("\nBaglanti sonlandirildi.\n");
	(void) send_eof(conn);		//sonlanýrken EOF gönder ki bekleyen varsa sonlansýn

	return 0;
}

