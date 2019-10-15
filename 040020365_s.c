/*************************************
 * Bilgisayar Haberleþmesi Ödev-1    *
 *								     *
 * Öðrenci : MEHMET CAMBAZ           *
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
 * Amaç    : istemciler arasýnda veri iletimini saðlar
 * Kullaným: 040020365_s <uygulamano1> <uygulamano2>
 * örneðin   040020365_s 25000 25001
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


	/* 1. baðlantýyý bekle */
	conn = await_contact((appnum) atoi(argv[1]));
	if (conn < 0)
		exit(1);	//baðlantý baþarýsýzsa çýk

	(void) printf("birinci istemci ile baglanti saglandi. %d \n", conn);	

	/* 2. baðlantýyý bekle */
	conn2 = await_contact((appnum) atoi(argv[2]));
	if(conn2 < 0)
		exit(1);	//baðlantý baþarýsýzsa çýk
	
	(void) printf("ikinci istemci ile de baglanti saglandi. %d \n\n", conn2);

	/*baðlantý saðlandý, iletim iþlemine baþla*/
	while(1) {

		kacinci=1;
		if(ilet(conn, conn2, kacinci))
			break;		//iletim baþarýsýz olursa ilet fonksiyonu 1 döndürecek ve while'dan çýkarak program sonlanacak

		kacinci=2;
		if(ilet(conn2, conn, kacinci))
			break;		//iletim baþarýsýz olursa ilet fonksiyonu 1 döndürecek ve while'dan çýkarak program sonlanacak
	}

	/* while dan çýktýðýna göre ilet fonksiyonu 1 döndürdü, yani istemcilerden biri çýktý veya bir sorun oluþtu*/
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
		(void) send(conn2, buff, len, 0);	/*gönder*/
		(void) write(STDOUT_FILENO, buff, len);		//gelmiþ ve iletilmiþ yazýlarý ekrana çýkart
		(void) fflush(stdout);
		len=0;
		return 0;	//iletim iþlemi baþarýlýysa 0 döndür
	} 
	else
		return 1;	//iletim iþlemi baþarýsýzsa 1 döndür

}
