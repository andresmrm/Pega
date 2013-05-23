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

//-----------------------------------------------------------------------------
//
//	Os canais funcionam da seguinte forma:
//		Se um cliente manda algum pacote para o servidor pelo canal 1
//		o servidor irá poder abrir esse pacote no canal 1.
//		Se o servidor manda um pacote para o cliente 0 pelo canal 2
//		o cliente 0 poderá abrir esse pacote no canal 2.
//		É só mandar de um lado e receber do outro. Lembrando que você
//		precisa saber o que chegou do outro lado e interpretar corretamente
//		os dados. Por isso aconselho mandar um "tipo de dado" de um lado
//		e fazer um casting para esse tipo de dado do outro, como exemplificado
//		no "gota.cpp". E usar um canal para cada "tipo de dado" diferente.
//		O canal 0 é reservado por essa biblioteca para mandar os avisos
//		de novo cliente conectado ou que foi embora. Eles seguem o tipo de
//		dado "AGUA_muda_num_clientes" definido logo abaixo.
//		Logo se você quer ter 2 canais para seus dados, abra 3. E seus
//		canais serão 1 e 2.
//
//-----------------------------------------------------------------------------

#ifndef AGUA_H_
#define AGUA_H_

#define AGUA_PRIMEIRA 0
#define AGUA_ULTIMA 1
#define AGUA_ULTIMAR 2
#define AGUA_SEM_CHAT 0
#define AGUA_COM_CHAT 1


// TRAFEGAR PELA COLUNA 0
// Pacote colocado na coluna 0 da tabela de entrada que avisa sobre entrada ou saída de um jogador
typedef struct{
	short int evento; 	// -1 cliente saiu / 1 cliente entrou
	short int id;		// id do cliente
	char nome[21];	// nome do cliente
} AGUA_muda_num_clientes;

// TRAFEGAR NAS DEMAIS COLUNAS
// Este pacote contem as informacoes sobre o enviado e ele em si (*dado)
typedef struct{
	short int id;
	void* dado;
} AGUA_pacote;

extern int AGUA_Num_Clients;            // Num total de clientes conectados
extern int AGUA_Rede_Ativa;             // 0 - Desativa / 1 - Servidor / 2 - Cliente



/* Inicia uma thread de um servidor com os dados recebidos. Retorna 1 - sucesso / 0 - falha.
 *  porta: porta que ficará na escuta
 *  nome: nome desse servidor
 */
int AGUA_iniciar_servidor( char porta[5], char nome[20] );


/* Inicia uma thread de um cliente com os dados recebidos. Retorna 1 - sucesso / 0 - falha.
 *  endereco: endereco ou IP do servidor
 *  porta: porta que está na escuta do servidor
 *  nome: nome desse cliente
 */
int AGUA_iniciar_cliente( char endereco[20], char porta[5], char nome[20] );


/* Manda um pacote pela rede para alguém conectado. Retorna 0 em falha / num de bytes enviados em sucesso
 *  canal: coluna da tabela de entrada em que o dado irá chagar do outro lado da rede
 *  tamanho: tamanho em bytes do dado que será enviado
 *  dado: ponteiro para o começo do dado
 *  alvo: destino do pacote
 *      -2: todos os clientes ( só pode ser usado pelo server )
 *      -1: para o server
 *      >=0: para o cliente nesse número
 */
int AGUA_mandar_pacote( char canal, unsigned short int tamanho, void* dado, int alvo );


/* Abre um pacote da tabela de entrada. Retorna 1 - sucesso / 0 - falha.
 *  canal: coluna da tabela de entrada de onde se deseja pegar um pacote
 *  linha: de qual parte da coluna se quer o pacote
 *      AGUA_PRIMEIRA: pega o primeiro pacote da coluna
 *      AGUA_ULTIMA: pega o último pacote da coluna
 *      AGUA_ULTIMAR: pega o último pacote da coluna e deleta todos os outros dessa coluna
 */
int AGUA_abrir_pacote( int canal, int linha, void **objeto );


// Remove o último pacote aberto. Retorna 1 - sucesso / 0 - falha.
int AGUA_remover_pacote();


/* Inicia essa biblioteca de rede. O canal zero é usado pela biblioteca para avisar sobre entrada e saída de clientes
 *  num_canais: número de canais por onde os dados poderão trafegar
 *  chat: se AGUA_SEM_CHAT não inicializa o chat / se AGUA_COM_CHAT inicializa
 */
int AGUA_fluir( int num_canais, int chat );

/* Finaliza essa biblioteca de rede.
 *  tempo: tempo em segundos esperado antes de matar threads ( se tempo < 0, aguarda elas acabarem )
 */
int AGUA_secar( float tempo );


#endif /*AGUA_H_*/
