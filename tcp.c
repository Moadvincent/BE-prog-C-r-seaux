/* librairie standard ... */
#include <stdlib.h>
#include <string.h>
/* pour getopt */
#include <unistd.h>
/* déclaration des types de base */
#include <sys/types.h>
/* constantes relatives aux domaines, types et protocoles */
#include <sys/socket.h>
/* constantes et structures propres au domaine UNIX */
#include <sys/un.h>
/* constantes et structures propres au domaine INTERNET */
#include <netinet/in.h>
/* structures retournées par les fonctions de gestion de la base de
   données du réseau */
#include <netdb.h>
/* pour les entrées/sorties */
#include <stdio.h>
/* pour la gestion des erreurs */
#include <errno.h>

#include "udp.h"
#include "affichage.h"
#include "tcp.h"



void tcp_source(int num_port,int taille,int nb_mess,char*dest)

{
  printf("On rentre dans tcp_puit\n");
 //Creation du socket local
  int option=0;
  int sock2;
  struct sockaddr_in adr_distant;
  int lg_adr_local=sizeof(adr_distant);

  if ((sock2=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1)      
    {
      printf("échec de création du socket \n");
      exit(1);
    }

  memset((char*)&adr_distant,option,sizeof(adr_distant));

  //identification de l'adresse du socket destinataire
  struct hostent *hp;
  char * pmesg = malloc(taille*sizeof(char));
  memset((char *)& adr_distant,0,sizeof(adr_distant));
  adr_distant.sin_family=AF_INET;
  adr_distant.sin_port=num_port;
  if ((hp=gethostbyname(dest))==NULL)
    {
      printf("erreur gethostbyname \n");
      exit(1);
    }
  memcpy((char*)&(adr_distant.sin_addr.s_addr),hp->h_addr,hp->h_length);

  // Etablissement de la connexion
  int connection;
  if ((connection=connect(sock2,(struct sockaddr*)&(adr_distant),sizeof(adr_distant)))==-1)
	{ printf("Echéc connect\n");
	  exit(1);}

 // envoi du message + affichage

  int sent;
  char msg='a';
  printf("SOURCE : lg_mesg_emis = %d,port= %d,dest=%s \n",taille,num_port,dest);

  for (int i=1; i<=nb_mess;i++)
    {
      formater_message(pmesg,i);
      construire_message(pmesg,msg,taille);
      printf("SOURCE:envoi n°%d (%d) : [",i,taille);
      
      
      if ((sent=write(sock2,pmesg,taille))==-1)
	{ printf("Erreur write\n");
	  exit(1);}
      afficher_message(pmesg,sent);
      if (msg=='z') 
	{ msg='a';}
      else
	{msg++;}
       
    }
  printf("SOURCE : FIN \n");
// fermeture de la connexion


if(close(sock2)==-1)
  {printf("échec de destruction du socket\n");
    exit(1);}
}


  void tcp_puit(int num_port, int taille,int nb_message)
  {
printf("On rentre dans tcp_puit\n");
  //Creation du socket local
  int option=0;
  int sock1;
  struct sockaddr_in adr_local,adr_distant;
  int lg_adr_local=sizeof(adr_local);

    
 if ((sock1=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1)       
    {
      printf("échec de création du socket \n");
      exit(1);
    }

   memset((char*)&adr_local,option,lg_adr_local);

 //Adressage  du socket
  adr_local.sin_family=AF_INET;
  adr_local.sin_port=num_port;
  adr_local.sin_addr.s_addr=INADDR_ANY;
  //Association de l'adresse au socket

  if ((bind(sock1,(struct sockaddr*)&adr_local,lg_adr_local))==-1)
    {
      printf("echec du bind\n");
      exit(1) ;
    }


  // etablissement de la connexion
  if (listen(sock1,5)==-1){
    printf("Trop de connexions -> echec");
  };
  int sockp;
  int adr=sizeof(adr_distant);
  if ((sockp=accept(sock1,(struct sockaddr*)&(adr_distant),&adr))==-1)
    {
      printf("echec de la connexion\n");
      exit(1) ;
    }

  
  
 

  //dimensionnement de la file des demandes de connexion

  /* int ecouter;
    if(ecouter=listen(sock1,5)==-1)
      {printf("erreur listen\n");
      exit(1);}*/

  //Recuperation  des messages
   char * pmesg = malloc(taille*sizeof(char));
   int rec=1;
   int i=1;
   printf("PUITS : lg_mesg_lu= %d,port= %d,receptions = infini  \n",taille,num_port); 
   while (rec !=0)
     {
       rec=recv(sockp,pmesg,taille,option);
       
       if (rec ==-1)
         {perror("erreur recv\n");
  	 exit(1);}
       if (rec!=0)
         {
  	 printf("PUITS : Réception n°%d (%d): [",i,taille);
  	 afficher_message(pmesg,rec);
         }

       if (i==nb_message)
         {
  	 rec=0;
  	 printf(" on a atteint le nombre de message à recevoir \n");
         }
       i++;
     }
  


// fermeture de la connexion


if(close(sock1)==-1)
  {printf("échec de destruction du socket\n");
    exit(1);}
  }
