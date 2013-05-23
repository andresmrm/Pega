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

#ifndef PEGA_H_
#define PEGA_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "SDL/SDL.h"
#include "../srcAux/listaC.h"
#include "../srcAux/fogo.h"
#include "../srcAux/ar.h"
#include "../srcAux/agua.h"

#define TIPOJOGO 1      // 0 - Pega-pega / 1 - FPS

#define oMAPA 0
#define oCEU 1
#define oLIVRO 2
#define oCERVEJA 3
#define oBOMBA 4
#define oCOMP 5
#define oVIRUS 6
#define oCAFE 7
#define oPANORAMA 8

#define TELA_COMPRIMENTO 1024
#define TELA_ALTURA 768
#define FPS 30

#define DISTANCIA_FISICA 1

#define MAX_NUM_JOGADORES 2
#define MAX_TAM_MAPA_X 100				// Tamanho X maximo para os mapas
#define MAX_TAM_MAPA_Y 100				// Tamanho Y maximo para os mapas
#define TAM_MAPA 20						// Um multiplicador para o tamanho do mapa em jogo

#define TEMPO_REVIVER 100

#define SAIR -1
#define EM_LOOP 0
#define CONTINUAR 1
#define TROCAR_NOME 2

#define SERVIDOR 1
#define CLIENTE 2

posicao gerar_posicao();
int principal();

// Função do servidor usada para mandar suas saídas pela rede
int gerar_saidas_servidor();
// Função que manda as saídas do cliente pela rede
int gerar_saidas_cliente();
// Função do servidor que lida com os pacotes recebidos
void lidar_entradas_servidor();
// Função do cliente que lida com os pacotes recebidos
void lidar_entradas_cliente();

// Função que inicia o servidor
int thread_servidor( void *data );
// Função que inicia o cliente
int thread_cliente( void *data );

int acertar(posicao,char,float);
posicao fisica(posicao,posicao,float, int);
posicao fisicaR(posicao,posicao,float);

int rodar_mapa(string arquivo_mapa);
int receber_texto( string* saida, unsigned short int tamanho );
void interface_receber_texto( string nome, int cursor );
int enviar_novo_nome_para_servidor();


//--------------------------------------------------------------------
// CLASSES
//--------------------------------------------------------------------

class agivel : public objeto3d
{
	private:
		unsigned short int coluna_agivel;

	public:
		agivel( posicao pos2, int coluna_agivel, int coluna );
		~agivel();
		virtual void agir() = 0;
};

class bomba : public agivel
{
	private:
		short dono;
		float vel_mov;

	public:
		bomba( posicao pos2, short dono2, int coluna_agivel=0, int coluna=0 );
		void mover( float x, float y, float z );
		void agir();
};

class virus : public agivel
{
	float vel_mov;
	float faltaRodar;
	int marcaPasso;

	public:
		virus( posicao pos2, int coluna_agivel, int coluna );
		void mover( float x, float y, float z );
		void agir();
};

class jogador : public plano
{
	private:
		string nome;

	public:
		float vel_mov;
		float vel_rot;

		unsigned short int tiros_dados;
		unsigned short int renascer;
		unsigned short int pontos;
		unsigned short int bebado;
		unsigned short int cafeinado;
		unsigned short int acertos;
		unsigned short int tempo_de_tiro;

		int id;

		char marcado;
		int vida;

		jogador( posicao pos2, float tama, int dese, int coluna );
		void rodar( float x, float z );
		void mover( float x, float y, float z );
		void acertado();
		void acertou();
		void alterar_nome( string novo_nome );
		void apagar_nome();
		void renascedor();
		int lancar();
		string retornar_nome();
};

class pseudoJogador
{
public:
	float vel_mov;
	float vel_rot;
	char nome[21];

	unsigned short int tiros_dados;
	unsigned short int renascer;
	unsigned short int pontos;
	unsigned short int bebado;
	unsigned short int cafeinado;
	unsigned short int acertos;
	unsigned short int tempo_de_tiro;

	int tamanho;
	posicao pos;
	int desenho;

	int id;

	char marcado;
	int vida;

	void igualar( jogador *j );
};

class cerveja : public agivel
{
	public:
		cerveja( posicao pos2, int coluna_agivel, int coluna );
		void agir();
};

class cafe : public agivel
{
	public:
		cafe( posicao pos2, int coluna_agivel, int coluna );
		void agir();
};

class livro : public agivel
{
	int pontos;

	public:
		livro( posicao pos2, int coluna_agivel, int coluna );
		void agir();
};

class comp : public agivel
{
	int pontos;

	public:
		comp( posicao pos2, int coluna_agivel, int coluna );
		void agir();
};

class mapa : public objeto3d
{
	public:
		mapa( posicao pos2, int coluna );
};

class ceu : public objeto3d
{
	public:
		ceu( posicao pos2, int coluna );
};


//--------------------------------------------------------------------
// ESTRUTURAS
//--------------------------------------------------------------------

typedef struct{
	posicao pos;
	unsigned short int uso;
} nascedouro;

typedef struct{
	string endereco;
	float tamaX;
	float tamaY;
	float tamanho;
} fonte;



// TRAFEGAR PELA COLUNA 1
// servidor -> cliente
// Seram mandados os pseudo-jogadores

// cliente -> servidor
typedef struct{
	posicao pos;
} pos_jogador; // Pacote de pos de um cliente


// TRAFEGAR PELA COLUNA 2
// servidor -> cliente
typedef struct{
	char arquivo[41];
	unsigned int tempo_restante;
	unsigned int pontos_restantes;
} dados_mapa; // Pacote com os dados do mapa

// cliente -> servidor
typedef struct{
	char nome[21];
} novo_nome; // Pacote com um novo nome q o cliente acabou de trocar, mandado para o servidor


// TRAFEGAR PELA COLUNA 3
// servidor <-> cliente
typedef struct{
	char dono;
} nova_bomba; // Pacote com uma nova bomba


// TRAFEGAR PELA COLUNA 4
// servidor -> cliente
// Sera mandado uma string com os ranks

// TRAFEGAR PELA COLUNA 5
// servidor ->cliente
typedef struct{
	bool pronto;
} espera_ponto; // Pacote usado para dizer quando um jogador esta pronto para comecar o jogo

//--------------------------------------------------------------------
// GLOBAIS
//--------------------------------------------------------------------

extern tabela agiveis;
extern int PapelRede;
extern char NOME[20];
extern short int Meu_Num;
extern char Num_Jogadores;
extern unsigned int TAM_MAPA_X;
extern unsigned int TAM_MAPA_Y;
extern char MAPA[MAX_TAM_MAPA_X][MAX_TAM_MAPA_Y];
extern unsigned short int NUM_PONTOS_RESTANTES;
extern unsigned int TEMPO_RESTANTE;
extern unsigned int TEMPO_RESTANTE_INICIAL;
extern char Porta[6];
extern char Ip[20];
extern string NOME_MAPA;
extern string RANKS;
extern jogador *Jogadores[MAX_NUM_JOGADORES];
extern nascedouro nascedouros[50];
extern unsigned short int NUM_NASCEDOUROS;
extern fonte FONTES[];
extern bool SOM_ATIVO;
extern posicao CEMITERIO;

void igualarJJ( jogador *jj, pseudoJogador *j );
short int retornarNumVetorJogadores(jogador* j);
float centralizar_texto( int comp, float posicao, int fonte );

#endif /*PEGA_H_*/
