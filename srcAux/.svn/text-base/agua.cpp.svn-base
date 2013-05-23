//-----------------------------------------------------------------------------
// Copyright 2008 Andrés M. R. Martano
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>
//-----------------------------------------------------------------------------
// This code was based on the SDLnet chat example

#include "agua.h"
#include "listaC.h"

#include "SDL/SDL_net.h"
#include "SDL/SDL_thread.h"
#ifndef WIN32
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#else
#include <windows.h>
#include <time.h>
#endif

#define MAXLEN (1*10*1024) // 10 KB - adequate for text



// Pacote genérico para envio ou recepção de qualquer dado
typedef struct{
	char tipo;
	unsigned short int tamanho;
	void* dado;
} pacote;

// Um cliente conectado
typedef struct {
	TCPsocket sock;
	char *nome;
	unsigned short int id;
} cliente;


int thread_chat(void *);
int thread_servidor( void *data );
int thread_cliente( void *data );
int find_client_name(char *nome);
int putMsg(int alvo, const char *buf);
char *getMsg(TCPsocket sock, char **buf);
char *mformat(const char *format,...);
void send_all(char *buf);
void remove_client(int i);
cliente* add_client(TCPsocket sock, char *nome);
SDLNet_SocketSet create_sockset();
pacote receber_rede( TCPsocket sock, char **buf, unsigned short int id );
int enviar_rede( TCPsocket sock, pacote pac );



void *AGUA_Linha = NULL;                // Aponta para o pacote aberto
int AGUA_Canal = -1;                    // Canal do pacote aberto
int AGUA_Chat = 0;                      // 0 - Chat não será inicializado / 1 - será inicializado
int AGUA_Terminado = 0;                 // Determina quando as threads acabaram
int AGUA_Num_Clients = 0;               // Num total de clientes conectados
int AGUA_Rede_Ativa = 0;                // 0 - Desativa / 1 - Servidor / 2 - Cliente
char AGUA_Porta[6] = "10000";           // Porta usada para se comunicar
char AGUA_Nome[21] = {"Player"};        // Nome do jogador local
char AGUA_Endereco[20] = {};            // IP ao qual o cliente vai conectar
tabela AGUA_Entrada_Rede;               // Armazena os pacotes que chegam pela rede
SDL_Thread *AGUA_ThreadServidor = NULL; // Aponta para a thread servidor
SDL_Thread *AGUA_ThreadCliente = NULL;  // Aponta para a thread cliente
SDL_Thread *AGUA_Thread_Chat = NULL;    // Aponta para a thread chat
cliente *AGUA_Clientes = NULL;          // Armazena os clientes conectados atualmente
TCPsocket AGUA_Sock_Servidor = NULL; 	// Quando servidor esse socket será por onde ele recebe novos clientes
                                        // Quando cliete esse socket será o servidor conectado

// Chama a thread servidor
int AGUA_iniciar_servidor( char porta[5], char nome[20] )
{
    if( porta != NULL )
        strcpy(AGUA_Porta,porta);

    if( nome != NULL )
        strcpy(AGUA_Nome,nome);

    AGUA_ThreadServidor = SDL_CreateThread( thread_servidor, NULL );

    if(AGUA_ThreadServidor)
        return 1;
    else
        return 0;
}

// Chama a thread cliente
int AGUA_iniciar_cliente( char endereco[20], char porta[5], char nome[20] )
{
    if( endereco != NULL )
        strcpy(AGUA_Endereco,endereco);
    else
        return 0;

    if( porta != NULL )
        strcpy(AGUA_Porta,porta);

    if( nome != NULL )
        strcpy(AGUA_Nome,nome);

    AGUA_ThreadCliente = SDL_CreateThread( thread_cliente, NULL );

    if(AGUA_ThreadCliente)
        return 1;
    else
        return 0;
}

#ifdef WIN32
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
char *strsep(char **stringp, const char *delim)
{
	char *p;

	if(!stringp)
		return(NULL);
	p=*stringp;
	while(**stringp && !strchr(delim,**stringp))
		(*stringp)++;
	if(**stringp)
	{
		**stringp='\0';
		(*stringp)++;
	}
	else
	{
		*stringp=NULL;
	}

	return(p);
}

#endif

// Recebe um pacote da rede, se for uma mensagem do chat, aponta *buf para ela
// Se for um pacote de dados o coloca na lista de Entrada
pacote receber_rede( TCPsocket sock, char **buf, unsigned short int id )
{
	pacote pac;
	unsigned int recebidos;
	static char *_buf;
	void *objeto;
	AGUA_pacote* Apac;

	//printf(" ( Recebendo pacote ) \n");

	recebidos = SDLNet_TCP_Recv(sock,&(pac.tipo),sizeof(pac.tipo));
	if( recebidos < sizeof(pac.tipo) )
	{
		if(SDLNet_GetError() && strlen(SDLNet_GetError())) // sometimes blank!
			printf("SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
		pac.tipo = -2;
		return pac;
	}
	//printf("R<<Tipo %d>>\n",pac.tipo);

	recebidos = SDLNet_TCP_Recv(sock,&(pac.tamanho),sizeof(pac.tamanho));
	if( recebidos < sizeof(pac.tamanho) )
	{
		if(SDLNet_GetError() && strlen(SDLNet_GetError())) // sometimes blank!
			printf("SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
		pac.tipo = -2;
		return pac;
	}

	// caso esteja recebendo uma mensagem do chat
	if( pac.tipo == -1 )
	{
		//printf(" ( Pacote Mensagem ) \n");

		// allow for a NULL buf, use a static internal one...
		if(!buf) buf=&_buf;

		// free the old buffer
		if(*buf) free(*buf);

		*buf=NULL;

		// allocate the buffer memory
		*buf=(char*)malloc(pac.tamanho);
		if(!(*buf))
		{
			pac.tipo = -1;
			return pac;
		}

		// get the string buffer over the socket
		recebidos = SDLNet_TCP_Recv(sock,*buf,pac.tamanho);
		if(recebidos < pac.tamanho )
		{
			if(SDLNet_GetError() && strlen(SDLNet_GetError())) // sometimes blank!
				printf("SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
				free(*buf);
				buf=NULL;
		}
	}
	else if( pac.tipo > 0 )
	{
		//printf(" ( Pacote dados ) \n");

		// allocate the buffer memory
		objeto = malloc(pac.tamanho);
		if(!(objeto))
		{
			pac.tipo = -2;
			return pac;
		}

		// get the string buffer over the socket
		recebidos = SDLNet_TCP_Recv(sock,objeto,pac.tamanho);
		if(recebidos < pac.tamanho )
		{
			if(SDLNet_GetError() && strlen(SDLNet_GetError())) // sometimes blank!
				printf("SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
				free(objeto);
		}

		Apac = (AGUA_pacote*)malloc(sizeof(AGUA_pacote));
		Apac->dado = objeto;
		Apac->id = id;

//		inserir_linha(&AGUA_Entrada_Rede, pac.tipo, pac.tamanho, objeto);
		inserir_linha(&AGUA_Entrada_Rede, pac.tipo, sizeof(Apac), Apac);
	}

	return pac;
}

// Envia algo pela rede
int enviar_rede( TCPsocket sock, pacote pac )
{
	unsigned int enviados = 0;

	// send the length of the string
	enviados = SDLNet_TCP_Send(sock,&(pac.tipo),sizeof(pac.tipo));
	if( enviados < sizeof(pac.tipo) ) {
		if(SDLNet_GetError() && strlen(SDLNet_GetError())) // sometimes blank!
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		return 0;
	}

	// send the length of the string
	enviados = SDLNet_TCP_Send(sock,&(pac.tamanho),sizeof(pac.tamanho));
	if( enviados < sizeof(pac.tamanho) ) {
		if(SDLNet_GetError() && strlen(SDLNet_GetError())) // sometimes blank!
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		return 0;
	}

	// send the buffer, with the NULL as well
	enviados = SDLNet_TCP_Send(sock,pac.dado,pac.tamanho);
	if( enviados < pac.tamanho ) {
		if(SDLNet_GetError() && strlen(SDLNet_GetError())) // sometimes blank!
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		return 0;
	}

	return enviados;
}

// Manda um pacote pela rede
int AGUA_mandar_pacote( char tipo, unsigned short int tama, void* dado, int alvo )
{
	int i;
	int enviados = 0;
	pacote pac;

	pac.tipo = tipo;
	pac.tamanho = tama;
	pac.dado = dado;

    // Caso vai para o servidor
	if( alvo == -1 )
	{
		enviados = enviar_rede( AGUA_Sock_Servidor, pac );
	}
	// Caso vai para um cliente
	else if( alvo >= 0 && alvo < AGUA_Num_Clients )
	{
		enviados = enviar_rede( AGUA_Clientes[alvo].sock, pac );
	}
	// Caso vai para todos os clientes
	else if( alvo == -2 ) // Só pode ser usado pelo server? Então limite isso aqui!!!!!!!
	{
		for( i=0; i<AGUA_Num_Clients; i++ )
			enviados = enviar_rede( AGUA_Clientes[i].sock, pac );
	}

    // Sa não foi enviado corretamente
	if( enviados != pac.tamanho )
		return 0;
	else
		return enviados;
}

// send a string buffer over a TCP socket with error checking
// returns 0 on any errors, length sent on success
int putMsg(TCPsocket sock, const char *buf)
{
	pacote pac;
	Uint32 len,result;

	if(!buf || !strlen(buf))
		return(1);

	// determine the length of the string
	len=strlen(buf)+1; // add one for the terminating NULL

	pac.tipo = -1;
	pac.tamanho = len;
	pac.dado = (void*)buf;

	result = enviar_rede( sock, pac );

	// return the length sent
	return(result);
}

// this is a simplistic printf-like function the format only takes a string of
// chars that represent datatypes.
// it mallocs space and prints into that buffer.
// it uses a static pointer so that the user doesn't have to free the buffer
char *mformat(const char *format,...)
{
	va_list ap;
	Uint32 len=0;
	static char *str=NULL;
	char *p, *s;
	char c;
	int d;
	unsigned int u;

	if(str)
	{
		free(str);
		str=NULL;
	}
	if(!format)
		return(NULL);
	va_start(ap,format);
	for(p=(char*)format; *p; p++)
	{
		switch(*p)
		{
			case 's': // string
				s=va_arg(ap, char*);
				str=(char*)realloc(str,((len+strlen(s)+4)/4)*4);
				sprintf(str+len,"%s",s);
				break;
			case 'c': // char
				c=(char)va_arg(ap, int);
				str=(char*)realloc(str,len+4);
				sprintf(str+len,"%c",c);
				break;
			case 'd': // int
				d=va_arg(ap, int);
				str=(char*)realloc(str,((len+64)/4)*4);
				sprintf(str+len,"%d",d);
				break;
			case 'u': // unsigned int
				u=va_arg(ap, unsigned int);
				str=(char*)realloc(str,((len+64)/4)*4);
				sprintf(str+len,"%u",u);
				break;
		}
		// set len to the new string length
		if(str)
			len=strlen(str);
		else
			len=0;
	}
	va_end(ap);

	return(str);
}

// terminate the nick at "bad" characters...
void fix_nick(char *s)
{
	unsigned int i;

	if((i=strspn(s,"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ|_=+.,:;/\\?!@#$%^&*()~`"))!=strlen(s))
		s[i]='\0';
}

// test for nice nome uniqueness among already connected users
int unique_nick(char *s)
{
	return(find_client_name(s)==-1);
}

// add a cliente into our array of AGUA_Clientes
cliente *add_client(TCPsocket sock, char *nome)
{
	static unsigned short int id = 1;

	AGUA_muda_num_clientes *novo_cliente;

	printf(" [ Adicionando %s ] \n",nome);

	fix_nick(nome);
	if(!strlen(nome))
	{
		putMsg(sock,"Invalid Nickname...bye bye!");
		printf(" [ %s - nome invalido ] \n",nome);
		SDLNet_TCP_Close(sock);
		return(NULL);
	}
	if(!unique_nick(nome))
	{
		putMsg(sock,"Duplicate Nickname...bye bye!");
		printf(" [ %s - nome duplicado ] \n",nome);
		SDLNet_TCP_Close(sock);
		return(NULL);
	}
	AGUA_Clientes=(cliente*)realloc(AGUA_Clientes, (AGUA_Num_Clients+1)*sizeof(cliente));
	AGUA_Clientes[AGUA_Num_Clients].nome=nome;
	AGUA_Clientes[AGUA_Num_Clients].sock=sock;
	AGUA_Clientes[AGUA_Num_Clients].id = id;

	// server side info
	printf("--> %s\n",nome);
	// inform all AGUA_Clientes, including the new one, of the joined user
	send_all(mformat("ss","--> ",nome));

	// coloca o aviso de novo jogador na lista de entrada
	novo_cliente = (AGUA_muda_num_clientes*)malloc(sizeof(AGUA_muda_num_clientes));
	if(!(novo_cliente))
	{
		printf("Erro nao conseguir alocar espaco para aviso de novo jogador");
	}
	novo_cliente->evento = 1;
	novo_cliente->id = id;
	strcpy(novo_cliente->nome,nome);
	AGUA_Num_Clients++;
	inserir_linha(&AGUA_Entrada_Rede, 0, sizeof(AGUA_muda_num_clientes), novo_cliente);

	return(&AGUA_Clientes[AGUA_Num_Clients-1]);
}

// find a cliente in our array of AGUA_Clientes by it's socket.
// the socket is always unique
int find_client(TCPsocket sock)
{
	int i;

	for(i=0;i<AGUA_Num_Clients;i++)
	{
		if(AGUA_Clientes[i].sock==sock)
		{
			return(i);
		}
	}

	return(-1);
}

// find a cliente in our array of AGUA_Clientes by it's socket.
// the nome is always unique
int find_client_name(char *nome) //     <--------- CORRIGIR
{
	int i;

	for(i=0;i<AGUA_Num_Clients;i++)
	{
		if(!strcasecmp(AGUA_Clientes[i].nome,nome))
		{
			return(i);
		}
	}

	return(-1);
}

// remove o cliente from our array of AGUA_Clientes
void remove_client(int i)
{
	AGUA_muda_num_clientes *novo_cliente;

	if(i<0 && i>=AGUA_Num_Clients)
	{
		return;
	}

	// prepara o aviso de saida de jogador
	novo_cliente = (AGUA_muda_num_clientes*)malloc(sizeof(AGUA_muda_num_clientes));
	if(!(novo_cliente))
	{
		printf("Erro nao conseguir alocar espaco para aviso de novo jogador");
	}
	novo_cliente->evento = -1;
	novo_cliente->id = AGUA_Clientes[i].id;
	strcpy(novo_cliente->nome,AGUA_Clientes[i].nome);

	// close the old socket, even if it's dead...
	SDLNet_TCP_Close(AGUA_Clientes[i].sock);

	if(AGUA_Num_Clients>i)
	{
		memmove(&AGUA_Clientes[i], &AGUA_Clientes[i+1], (AGUA_Num_Clients-i)*sizeof(cliente));
	}
	AGUA_Clientes=(cliente*)realloc(AGUA_Clientes, AGUA_Num_Clients*sizeof(cliente));
	// server side info
	printf("<-- %s\n",novo_cliente->nome);

	// envia o aviso de saida de jogador pela rede
	inserir_linha(&AGUA_Entrada_Rede, 0, sizeof(AGUA_muda_num_clientes), novo_cliente); //<--------- novo_cliente sera free pela lista? pq aqui nao esta sendo

	AGUA_Num_Clients--;
	// inform all AGUA_Clientes, excluding the old one, of the disconnected user
	send_all(mformat("ss","<-- ",novo_cliente->nome));
}

// create a socket set that has the server socket and all the cliente sockets
SDLNet_SocketSet create_sockset()
{
	static SDLNet_SocketSet set=NULL;
	int i;

	if(set)
	{
		SDLNet_FreeSocketSet(set);
	}
	set=SDLNet_AllocSocketSet(AGUA_Num_Clients+1);
	if(!set)
	{
		printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
		exit(1); //most of the time this is a major error, but do what you want.
	}
	SDLNet_TCP_AddSocket(set,AGUA_Sock_Servidor);
	for(i=0;i<AGUA_Num_Clients;i++)
	{
		SDLNet_TCP_AddSocket(set,AGUA_Clientes[i].sock);
	}

	return(set);
}

// send a buffer to all AGUA_Clientes
void send_all(char *buf)
{
	int cindex;

	if(!buf || !AGUA_Num_Clients)
		return;
	cindex=0;
	printf("%s\n",buf);
	while(cindex<AGUA_Num_Clients)
	{
		// putMsg is in tcputil.h, it sends a buffer over a socket
		// with error checking
		if(putMsg(AGUA_Clientes[cindex].sock,buf))
			cindex++;
		else
			remove_client(cindex);
	}
}

// Realiza o comando digitado
void do_command(const char *msg, cliente *cliente)
{
	char *command,*p;
	int len;
	int servidor = 0;

    // Caso o comando foi digitado pelo servidor
    if(cliente == NULL)servidor = 1;

	if(!msg || !strlen(msg))
		return;
	len=strlen(msg);
	p=(char*)msg;
	command=strsep(&p," \t");
	// /NICK : change the cliente nome
	if(!strcasecmp(command,"NICK"))
	{
		if(p && strlen(p))
		{
		    char *old_name;

            // Ajusta comando entre servidor e cliente
		    if(servidor)
                old_name=AGUA_Nome;
            else
                old_name=cliente->nome;

			fix_nick(p);
			if(!strlen(p))
			{
			    if(servidor)
                    printf("--- Invalid Nickname!");
			    else
                    putMsg(cliente->sock,"--- Invalid Nickname!");
				return;
			}
			if(!unique_nick(p))
			{
			    if(servidor)
                    printf("--- Duplicate Nickname!");
			    else
                    putMsg(cliente->sock,"--- Duplicate Nickname!");
				return;
			}

			if(!servidor)
                cliente->nome=strdup(p);

			send_all(mformat("ssss","--- ",old_name," --> ",p));

			if(servidor)
                strcpy(AGUA_Nome,p);

			if(!servidor)
                free(old_name);
		}
		else
		{
		    if(servidor)
                    printf("--- /NICK nickname");
            else
                putMsg(cliente->sock,"--- /NICK nickname");
		}
		return;
	}
	// MSG : cliente to cliente message
	if(!strcasecmp(command,"MSG"))                // CORRIGIR ESSE COMANDO MSG PARA SERVIDOR TB
	{
		char *nome;
		int to;

		if(p)
		{
			nome=strsep(&p," ");
			to=find_client_name(nome);
			if(to<0)
			{
				putMsg(cliente->sock, mformat("sss","--- /MSG nickname ",nome," not found!"));
				return;
			}
			else if(p && strlen(p))
			{
				putMsg(cliente->sock,mformat("ssss",">",AGUA_Clientes[to].nome,"< ",p));
				putMsg(AGUA_Clientes[to].sock,mformat("ssss",">",cliente->nome,"< ",p));
				return;
			}
		}
		putMsg(cliente->sock,"--- /MSG nickname message...");
		return;
	}
	// /ME : emote! to everyone
	if(!strcasecmp(command,"ME"))
	{
		if(p && strlen(p))
		{
		    if(servidor)
                send_all(mformat("sss",AGUA_Nome," ",p));
            else
                send_all(mformat("sss",cliente->nome," ",p));
		}
		else
		{
            if(servidor)
                printf("--- /ME message...");
            else
                putMsg(cliente->sock,"--- /ME message...");
		}
		return;
	}
	// /QUIT : quit the server with a message
	if(!strcasecmp(command,"QUIT"))
	{
		if(!p || strcasecmp(p,"-h"))
		{
			if(p)
			{
			    if(servidor)
                    send_all(mformat("sssss","--- ",AGUA_Nome,"(O SERVIDOR)"," quits : ",p));
                else
                    send_all(mformat("ssss","--- ",cliente->nome," quits : ",p));
			}
			else
			{
			    if(servidor)
                    send_all(mformat("ssss","--- ",AGUA_Nome,"(O SERVIDOR)"," quits"));
                else
                    send_all(mformat("sss","--- ",cliente->nome," quits"));
			}

			if(!servidor)                                   // CASO FOR O SERVIDOR, REMOVER DE VERDADE
                remove_client(find_client(cliente->sock));

            AGUA_Terminado = 10;
		}
		else
		{
		    if(servidor)
                printf("--- /QUIT [message...]\n");
            else
                putMsg(cliente->sock,"--- /QUIT [message...]");
		}
		return;
	}
	// /WHO : list the users online back to the cliente                         ARRUMAR ESSE COMANDO PARA SERVIDOR TB
	if(!strcasecmp(command,"WHO"))
	{
		int i;
		IPaddress *ipaddr;
		Uint32 ip;
		char *host;

		putMsg(cliente->sock,"--- Begin /WHO ");
		for(i=0;i<AGUA_Num_Clients;i++)
		{
			ipaddr=SDLNet_TCP_GetPeerAddress(AGUA_Clientes[i].sock);
			if(ipaddr)
			{
				ip=SDL_SwapBE32(ipaddr->host);
				char abcde[20];
				char *temp = (char*)SDLNet_ResolveIP(ipaddr);
				if(temp!=NULL)
                    strcpy(abcde,temp);
				else
                    strcpy(abcde,"N/A");
				host=abcde;
				putMsg(cliente->sock,mformat("sssssdsdsdsdsd","--- ",AGUA_Clientes[i].nome,
						" ",host?host:"",
						"[",ip>>24,".", (ip>>16)&0xff,".", (ip>>8)&0xff,".", ip&0xff,
						"] port ",(Uint32)ipaddr->port));
			}
		}
		char abcd[15]="--- End /WHO";
		putMsg(cliente->sock,abcd);
		return;
	}
	// /HELP : tell the cliente all the supported commands
	if(!strcasecmp(command,"HELP"))
	{
	    if(servidor)
	    {
	        printf("--- Begin /HELP\n");
            printf("--- /HELP : this text\n");
            printf("--- /ME message... : emote!\n");
            printf("--- /MSG nickname message... : personal messaging\n");
            printf("--- /NICK nickname : change nickaname\n");
            printf("--- /QUIT [message...] : disconnect this cliente\n");
            printf("--- /WHO : list who is logged on\n");
            printf("--- End /HELP\n");
	    }
	    else
	    {
            putMsg(cliente->sock,"--- Begin /HELP");
            putMsg(cliente->sock,"--- /HELP : this text");
            putMsg(cliente->sock,"--- /ME message... : emote!");
            putMsg(cliente->sock,"--- /MSG nickname message... : personal messaging");
            putMsg(cliente->sock,"--- /NICK nickname : change nickaname");
            putMsg(cliente->sock,"--- /QUIT [message...] : disconnect this cliente");
            putMsg(cliente->sock,"--- /WHO : list who is logged on");
            putMsg(cliente->sock,"--- End /HELP");
	    }
		return;
	}

	// invalid command...respond appropriately
	if(servidor)
        printf("%s",mformat("sss","--- What does the '",command,"' command do?"));
	else
        putMsg(cliente->sock,mformat("sss","--- What does the '",command,"' command do?"));
}

// Roda o servidor
int thread_servidor( void *data )
{
	int r = 0;
	IPaddress ip;
	TCPsocket sock = NULL;
	SDLNet_SocketSet set;
	char *message=NULL;
	char *host;
	Uint32 ipaddr;
	Uint16 port;

//	strcpy(Nome_Rede,AGUA_Nome);

	// get the port from the commandline
	port=(Uint16)strtol(AGUA_Porta,NULL,0);

	// Resolve the argument into an IPaddress type
	if(SDLNet_ResolveHost(&ip,NULL,port)==-1)
	{
		printf("SDLNet_ResolveHost: %s\n",SDLNet_GetError());
		SDLNet_Quit();
		SDL_Quit();
		exit(3);
	}

	// perform a byte endianess correction for the next printf
	ipaddr=SDL_SwapBE32(ip.host);

	// output the IP address nicely
	printf(" ( IP Address : %d.%d.%d.%d ) \n",
			ipaddr>>24,
			(ipaddr>>16)&0xff,
			(ipaddr>>8)&0xff,
			ipaddr&0xff);

	// resolve the hostname for the IPaddress
	host=(char*)SDLNet_ResolveIP(&ip);

	// print out the hostname we got
	if(host)
		printf(" ( Hostname   : %s ) \n",host);
	else
		printf(" ( Hostname   : N/A ) \n");

	// output the port number
	printf(" ( Port       : %d ) \n",port);

	// open the server socket
	AGUA_Sock_Servidor=SDLNet_TCP_Open(&ip);
	if(!AGUA_Sock_Servidor)
	{
		printf("SDLNet_TCP_Open: %s\n",SDLNet_GetError());
		SDLNet_Quit();
		SDL_Quit();
		exit(4);
	}

	// Inicia o servidor no chat
	if(AGUA_Chat == AGUA_COM_CHAT)
	{
        AGUA_Thread_Chat = SDL_CreateThread(thread_chat,sock);
        if(!AGUA_Thread_Chat)
        {
            printf("SDL_CreateThread: %s\n",SDL_GetError());
            return 0;
        }
	}

	AGUA_Rede_Ativa = 1;

	printf(" ( Iniciando loop servidor ) \n");

	while(!AGUA_Terminado)
	{
		int numready,i;
		set=create_sockset();
		numready=SDLNet_CheckSockets(set, (Uint32)-1);
		if(numready==-1)
		{
			printf("SDLNet_CheckSockets: %s\n",SDLNet_GetError());
			AGUA_Terminado = 1;
		}
		if(!numready)
			continue;

		// Novo cliente tentando conectar
		if(SDLNet_SocketReady(AGUA_Sock_Servidor))
		{
			printf(" [ Atividade no socket server ] \n");
			numready--;
			//printf("Connection...\n");
			sock=SDLNet_TCP_Accept(AGUA_Sock_Servidor);
			if(sock)
			{
				printf(" [ Novo socket aberto ] \n");

				char *nome=NULL;

				//printf("Accepted...\n");
				r = (receber_rede(sock, &nome,0).tipo);
				if( r == -1 )
				{
					printf(" [ %s logando... ] \n",nome);
					cliente *cliente;
					cliente=add_client(sock,nome);
					if(cliente)
						do_command("WHO",cliente);
				}
				else if( r < -1 )
				{
					SDLNet_TCP_Close(sock);
					printf(" [ Novo socket fechado ] \n");
				}
			}
		}
		for(i=0; numready && i<AGUA_Num_Clients; i++)
		{
			if(SDLNet_SocketReady(AGUA_Clientes[i].sock))
			{
				r = receber_rede(AGUA_Clientes[i].sock, &message, AGUA_Clientes[i].id).tipo;
				// Se é um pacote de dados receber_rede já o colocou na lista de Entrada
				// Se é uma mensagem
				if( r == -1 )
				{
					char *str;

					numready--;
					// interpret commands
					if(message[0]=='/' && strlen(message)>1)
					{
						do_command(message+1,&AGUA_Clientes[i]);
					}
					else // it's a regular message
					{
						// forward message to ALL AGUA_Clientes...
						str=mformat("ssss","<",AGUA_Clientes[i].nome,"> ",message);
						if(str)
							send_all(str);
					}
					free(message);
					message=NULL;
				}
				// Se é um erro
				else if( r < -1 )
				{
					remove_client(i);
					printf(" [ Cliente %d disse algo estranho e foi quikado ] ",i);
				}
			}
		}
	}

    if(AGUA_Chat == AGUA_COM_CHAT)
        SDL_KillThread(AGUA_Thread_Chat);

	printf(" [ Loop servidor encerrado ] ");

	return 1;
}

// Roda o cliente
int thread_cliente( void *data )
{
	int r = 0;
	IPaddress ip;
	//TCPsocket sock;
	Uint16 port;
	SDLNet_SocketSet set;
	int numready;
	char *str=NULL;
	//char *nome = nome;
//	strcpy(Nome_Rede,AGUA_Nome);

	// get the port from the commandline
	port=(Uint16) strtol(AGUA_Porta,NULL,0);

	// Resolve the argument into an IPaddress type
	printf(" ( Conectando a %s : %d ) \n",AGUA_Endereco,port);
	if(SDLNet_ResolveHost(&ip,AGUA_Endereco,port)==-1)
	{
		printf("SDLNet_ResolveHost: %s\n",SDLNet_GetError());
		SDLNet_Quit();
		SDL_Quit();
		exit(5);
	}
	else
	{
		printf(" ( IP server encontrado ) \n");
	}

	// open the server socket
	AGUA_Sock_Servidor = SDLNet_TCP_Open(&ip);
	if(!AGUA_Sock_Servidor)
	{
		printf("SDLNet_TCP_Open: %s\n",SDLNet_GetError());
		SDLNet_Quit();
		SDL_Quit();
		exit(6);
	}
	else
	{
		printf(" ( Socket para servidor aberto ) \n");
	}

	// login with a nome
	if(!putMsg(AGUA_Sock_Servidor,AGUA_Nome))
	{
		SDLNet_TCP_Close(AGUA_Sock_Servidor);
		return 0;
	}
	else
	{
		printf(" ( Logado como %s ) \n",AGUA_Nome);
	}

    if(AGUA_Chat == AGUA_COM_CHAT)
    {
        AGUA_Thread_Chat = SDL_CreateThread(thread_chat,AGUA_Sock_Servidor);
        if(!AGUA_Thread_Chat)
        {
            printf("SDL_CreateThread: %s\n",SDL_GetError());
            return 0;
        }
    }

	set=SDLNet_AllocSocketSet(1);
	if(!AGUA_Terminado && !set)
	{
		printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
		AGUA_Terminado=2;
	}

	if(!AGUA_Terminado && SDLNet_TCP_AddSocket(set,AGUA_Sock_Servidor)==-1)
	{
		printf("SDLNet_TCP_AddSocket: %s\n",SDLNet_GetError());
		AGUA_Terminado=3;
	}

	// Sou um cliente
	AGUA_Rede_Ativa = 2;

	while(!AGUA_Terminado)
	{
		numready=SDLNet_CheckSockets(set, (Uint32)-1);
		if(numready==-1)
		{
			printf("SDLNet_CheckSockets: %s\n",SDLNet_GetError());
			AGUA_Terminado=4;
			break;
		}

		// check to see if the server sent us data
		if(numready && SDLNet_SocketReady(AGUA_Sock_Servidor))
		{
			r = receber_rede(AGUA_Sock_Servidor,&str,0).tipo;
			// Se é um pacote de dados receber_rede já o colocou na lista de Entrada
			// Se é uma mensagem
			if( r == -1 )
			{
				// post it to the screen
				printf("%s\n",str);
			}
			// Se é um erro
			else if( r < -1 )
			{
				char *errstr=SDLNet_GetError();
				printf("Recebido: %s\n",strlen(errstr)?errstr:"Server disconnected");
				AGUA_Terminado=5;
				break;
			}
		}
	}
	if(!AGUA_Terminado)
	{
		AGUA_Terminado=1;
	}

    if(AGUA_Chat == AGUA_COM_CHAT)
        SDL_KillThread(AGUA_Thread_Chat);
	//SDL_WaitThread(Chat_thread,NULL);

	// this shows our unique reason where the threads ended
	printf(" ( cliente encerrado (codigo %d) ) \n",AGUA_Terminado );

	return 1;
}

// Thread usada para o chat tanto do servidor como do cliente
int thread_chat(void *data)
{
	TCPsocket sock=(TCPsocket)data;
#ifndef _MSC_VER
	fd_set fdset;
	int result;
#endif
	char message[MAXLEN];

	while( !AGUA_Rede_Ativa && !AGUA_Terminado )
	{
		SDL_Delay(1);
	}

	// this is the local input loop
	while(!AGUA_Terminado)
	{
#ifndef _MSC_VER
		// set up the file descriptor set
		FD_ZERO(&fdset);
		FD_SET(fileno(stdin),&fdset);

		// wait for keyboard input on stdin
		result=select(fileno(stdin)+1, &fdset, NULL, NULL, NULL);
		if(result==-1)
		{
			perror("select");
			AGUA_Terminado=6;
			break;
		}

		// is there input?
		if(result && FD_ISSET(fileno(stdin),&fdset))
		{
#endif
			// get the string from stdin
			if(!fgets(message,MAXLEN,stdin))
			{
				AGUA_Terminado=7;
				break;
			}

			// strip the whitespace from the end of the line
			while(strlen(message) && strchr("\n\r\t ",message[strlen(message)-1]))
				message[strlen(message)-1]='\0';

			// if there was a message after stripping the end,
			// send it to the server
			if(strlen(message))
			{
				//printf("Sending: %s\n",message);
				if( AGUA_Rede_Ativa == 2 )
				{
					putMsg(sock,message);
				}
				else if( AGUA_Rede_Ativa == 1 )
				{
					char *str;

					// interpret commands
					if(message[0]=='/' && strlen(message)>1)
					{
						do_command(message+1,NULL);
					}
					else // it's a regular message
					{
						// forward message to ALL AGUA_Clientes...
						str=mformat("ssss","<",AGUA_Nome,"> ",message);
						if(str)
						{
							send_all(str);
						}
					}
				}
			}
#ifndef _MSC_VER
		}
#endif
	}
	if( !AGUA_Terminado ) AGUA_Terminado=1;

	return 1;
}

// Abre um pacote da tabela de entrada
int AGUA_abrir_pacote( int canal, int linha, void **objeto )
{
    int aberto = 0;

    // Caso deseja retornar apenas o primeiro pacote da coluna
    if( linha == AGUA_PRIMEIRA )
    {
        AGUA_Linha = primeira_linha(&AGUA_Entrada_Rede, canal, &(*objeto));
    }
    // Caso deseja retornar o último pacote da coluna e remover os outros
    else if( linha == AGUA_ULTIMAR )
    {
        AGUA_Linha = ultima_linhaR(&AGUA_Entrada_Rede, canal, &(*objeto));
    }
    // Caso deseja apenas retornar o último pacote da coluna
    else if( linha == AGUA_ULTIMA )
    {
        AGUA_Linha = ultima_linha(&AGUA_Entrada_Rede, canal, &(*objeto));
    }

    if(AGUA_Linha)
    {
        AGUA_Canal = canal;
        aberto = 1;
    }

    return aberto;
}

// Remove o último pacote aberto
int AGUA_remover_pacote()
{
    if(!AGUA_Linha)
    {
        printf("Nenhum pacote aberto\n");
        return 0;
    }
    remover_linha(&AGUA_Entrada_Rede, AGUA_Canal, (linhaC*)AGUA_Linha);
    AGUA_Linha = NULL;
    AGUA_Canal = -1;

    return 1;
}

// Carrega a biblioteca
int AGUA_fluir( int num_canais, int chat )
{
	if(SDLNet_Init()==-1)
	{
		printf("SDLNet_Init: %s\n",SDLNet_GetError());
		SDLNet_Quit();
		exit(3);
	}
	iniciar_tabela(&AGUA_Entrada_Rede, num_canais );

	AGUA_Chat = chat;

	return 1;
}

// Fecha a biblioteca
int AGUA_secar( float tempo )
{
    if( tempo >= 0 )
    {
        // Finaliza as thread após um certo tempo
        SDL_Delay(tempo*1000);
        if( AGUA_ThreadServidor )
            SDL_KillThread( AGUA_ThreadServidor );
        if( AGUA_ThreadCliente )
            SDL_KillThread( AGUA_ThreadCliente );
    }
    else
    {
        // Aguarda as threads terminarem
        if( AGUA_ThreadServidor ) SDL_WaitThread(AGUA_ThreadServidor,NULL);
        if( AGUA_ThreadCliente ) SDL_WaitThread(AGUA_ThreadCliente,NULL);
    }

	deletar_tabela( &AGUA_Entrada_Rede );

	if(AGUA_Sock_Servidor)
        SDLNet_TCP_Close(AGUA_Sock_Servidor);

	SDLNet_Quit();

	return 1;
}
