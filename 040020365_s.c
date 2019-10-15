/*************************************
 * Bilgisayar Haberle�mesi �dev-1    *
 *								     *
 * ��renci : MEHMET CAMBAZ           *
 * No      : 040020365               *
 * E-mail  : mehmet_cambaz@yahoo.com *
 *************************************/

/** sunucu **/

#include <stdlib.h>
#include <stdio.h>
#include "cnaiapi.h"

#define BUFFSIZE 256

#define RECEIVED_PROMPT "'den alinan  -> "
#define RECEIVED_PROMPT2 "2'den alinan  -> "

int recvln(connection, char *, int);
int readln(char *, int);
int ilet(int, int, int);


/*-----------------------------------------------------------------------
 * Program : sunucu
 * Ama�    : istemciler aras�nda veri iletimini sa�lar
 * Kullan�m: 040020365_s <uygulamano1> <uygulamano2>
 * �rne�in   040020365_s 25000 25001
 *-----------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
	connection conn, conn2;
	int len=0, kacinci=0;
	char buff[BUFFSIZE]={0};

	if (argc != 3) {
		(void) fprintf(stderr, "Kullanim: %s <uygulamano1> <uygulamano2>\n", argv[0]);
		exit(1);
	}

	(void) printf("Sunucu baglanti bekliyor...\n\n");


	/* 1. ba�lant�y� bekle */
	conn = await_contact((appnum) atoi(argv[1]));
	if (conn < 0)
		exit(1);	//ba�lant� ba�ar�s�zsa ��k

	(void) printf("birinci istemci ile baglanti saglandi. %d \n", conn);	

	/* 2. ba�lant�y� bekle */
	conn2 = await_contact((appnum) atoi(argv[2]));
	if(conn2 < 0)
		exit(1);	//ba�lant� ba�ar�s�zsa ��k
	
	(void) printf("ikinci istemci ile de baglanti saglandi. %d \n\n", conn2);

	/*ba�lant� sa�land�, iletim i�lemine ba�la*/
	while(1) {

		kacinci=1;
		if(ilet(conn, conn2, kacinci))
			break;		//iletim ba�ar�s�z olursa ilet fonksiyonu 1 d�nd�recek ve while'dan ��karak program sonlanacak

		kacinci=2;
		if(ilet(conn2, conn, kacinci))
			break;		//iletim ba�ar�s�z olursa ilet fonksiyonu 1 d�nd�recek ve while'dan ��karak program sonlanacak
	}

	/* while dan ��kt���na g�re ilet fonksiyonu 1 d�nd�rd�, yani istemcilerden biri ��kt� veya bir sorun olu�tu*/
	(void) send_eof(conn);
	(void) printf("\nBaglanti 1 sonlandirildi.\n");
	(void) send_eof(conn2);
	(void) printf("\nBaglanti 2 sonlandirildi.\n\n");

	return 0;
}

int ilet(int conn, int conn2, int kacinci)
{
	int len=0;
	char buff[BUFFSIZE]={0};

	if (( len = recvln(conn, buff, BUFFSIZE) ) > 0) {
	
		/*geldi*/
		(void) printf("%d", kacinci);
		(void) printf(RECEIVED_PROMPT);
		(void) fflush(stdout);
		(void) send(conn2, buff, len, 0);	/*g�nder*/
		(void) write(STDOUT_FILENO, buff, len);		//gelmi� ve iletilmi� yaz�lar� ekrana ��kart
		(void) fflush(stdout);
		len=0;
		return 0;	//iletim i�lemi ba�ar�l�ysa 0 d�nd�r
	} 
	else
		return 1;	//iletim i�lemi ba�ar�s�zsa 1 d�nd�r

}
