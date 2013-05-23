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
/*
 * globais.cpp
 *
 *  Created on: 30/12/2008
 *      Author: andres
 */


#include "pega.h"


unsigned int TAM_MAPA_X = 0;						// Tamanho X do mapa atual
unsigned int TAM_MAPA_Y = 0;						// Tamanho Y do mapa atual

tabela agiveis;                         			// Lista onde ficam objetos que agem sozinhos, como as bombas
jogador *Jogadores[MAX_NUM_JOGADORES];				// Vetor de jogadores
short int Meu_Num = -1;                       		// Numero do jogador local no vetor de jogadores (servidor gerelmente é o 0)

char MAPA[MAX_TAM_MAPA_X][MAX_TAM_MAPA_Y];			// Matriz com mapa usado pela física
nascedouro nascedouros[50];							// Possiveis lugares onde um jogador pode nascer
unsigned short int NUM_NASCEDOUROS = 0;				// Num de nasc. usados

unsigned short int NUM_PONTOS_RESTANTES = 0;		// Numero de objetos q valem pontos e ainda estao no mapa
unsigned int TEMPO_RESTANTE = 0;					// Tempo restante para o jogo atual
unsigned int TEMPO_RESTANTE_INICIAL = 4000;			// Tempo restante inicial
string NOME_MAPA = "";								// Nome do mapa atual
string RANKS = "";									// Usada para o cliente receber os ranks do servidor e mostra-los

char Num_Jogadores = 1;             				// Numero total de jogadores no jogo
int PapelRede = 0;                  				// Define o jogador local como servidor (1) ou cliente (2)

char Porta[6] = "10000";            				// Porta usada para se comunicar
char NOME[20] = "Pokemon";                 			// Nome do jogador local
char Ip[20] = {};                   				// IP ao qual o cliente vai conectar

fonte FONTES[10];									// Vetor com os dados de cada fonte

bool SOM_ATIVO = true;								// Regula se havera som ou nao
posicao CEMITERIO;
