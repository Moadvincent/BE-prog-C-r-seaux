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
/* Sous-programmes*/

#include "affichage.h"
#include "udp.h"
#include "tcp.h"
void main (int argc, char **argv)
{
	int c;
	extern char *optarg;
	extern int optind;
	int nb_message =-1; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
	int source = -1 ; /* 0=puits, 1=source */
	int udp= 0;          /*  indique le mode utilisé */
	int num_port = atoi(argv[argc-1]);
	//	num_port=htons(num_port);
	int lg_message = 30; /*taille par défaut d'un message*/
	int maxsize = 30;
	int messmax=100000-1;
	char * dest=argv[argc-2];
	while ((c = getopt(argc, argv, "pn:sul:")) != -1) {
		switch (c) {
		case 'p':
			if (source == 1) {
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1);
			}
			source = 0;
			break;

		case 's':
			if (source == 0) {
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1) ;
			}
			source = 1;
			break;

		case 'n':
			nb_message = atoi(optarg);
			if (nb_message > messmax)
			  {printf("nb de message max est de %d :\n",messmax);
			    exit(1);}
			break;



		case 'u':
		       udp=1;
		       break;

		case 'l':
		  lg_message = atoi(optarg);
		  if (lg_message > maxsize)
		    {printf("vous avez depasse la taille max permise de %d octets: envoi impossible\n",maxsize);
		      exit(1);}
		       break;



		default:
			printf("usage: cmd [-p|-s][-n ##]\n");
			break;
		}
	}

	if (source == -1) {
		printf("usage: cmd [-p|-s][-n ##]\n");
		exit(1) ;
	}

	if (source == 1)
		printf("on est dans le source\n");
	else
		printf("on est dans le puits\n");

	if (nb_message != -1) {
		if (source == 1)
			printf("nb de tampons à envoyer : %d\n", nb_message);
		else
			printf("nb de tampons à recevoir : %d\n", nb_message);
	} else {
		if (source == 1) {
			nb_message = 10 ;
			printf("nb de tampons à envoyer = 10 par défaut\n");
		} else
		printf("nb de tampons à envoyer = infini\n");

	}
  printf("Source = %d et udp=%d\n",source,udp);
	//Appel de la fonction à executer

	if (source == 1 & udp == 1)
	  {
	  udp_source(num_port,lg_message,nb_message,dest);
	  exit(1);
	  }
	else if(source == 1 & udp == 0)
	  {
	   tcp_source(num_port,lg_message,nb_message,dest);
	  exit(1);
	  }

	 else if(source == 0 & udp == 1)
	   {
       udp_puit(num_port,lg_message,nb_message);
     }
	 else {
	  tcp_puit(num_port,lg_message,nb_message);
	  exit(1);}

	   }
