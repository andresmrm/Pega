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
 * entidades.cpp
 *
 *  Created on: 30/12/2008
 *      Author: andres
 */


#include "pega.h"


//--------------------------------------------------------------------
// Agivel
//--------------------------------------------------------------------
agivel::agivel( posicao pos2, int coluna_agivel, int coluna ) : objeto3d( pos2, coluna )
{
	this->tamanho = 1;
	this->coluna_agivel = coluna_agivel;
	inserir_linha(&agiveis, coluna_agivel, sizeof(this), this);
}
agivel::~agivel()
{
	remover_linha(&agiveis, coluna_agivel, this);
}

//--------------------------------------------------------------------
// Bomba
//--------------------------------------------------------------------
bomba::bomba( posicao pos2, short dono2, int coluna_agivel, int coluna ) : agivel( pos2, coluna_agivel, coluna )
{
	this->dono = dono2;
	this->vel_mov = 1;
	this->objeto = oBOMBA;

	AR_tocar_som_3D(0,dono,pos,Jogadores[Meu_Num]->pos);
}

void bomba::agir()
{
	mover(0,2,0);
}

void bomba::mover( float x, float y, float z )
{
	int acertado = -1;
    posicao pos2;

    pos2.igualar(pos);
    pos2.adicionar( (x*vel_mov*cos(0.01745*-pos.zr)+y*vel_mov*sin(0.01745*-pos.zr)), (y*vel_mov*cos(0.01745*-pos.zr)-x*vel_mov*sin(0.01745*-pos.zr)), z*vel_mov );
    pos.igualar( fisica( pos, pos2, tamanho/2, 1 ) );

    // Ve se acertou algum jogador
    acertado = acertar(pos,dono,tamanho);
    if( acertado != -1 )
    {
    	Jogadores[acertado]->acertado();
        AR_tocar_som_3D(1,acertado,Jogadores[acertado]->pos,Jogadores[Meu_Num]->pos);
        Jogadores[(int)dono]->acertou();
    }

    // Caso bata em algo, desaparece
    if( pos.x != pos2.x || pos.y != pos2.y ) delete this;
}

//--------------------------------------------------------------------
// Virus
//--------------------------------------------------------------------
virus::virus( posicao pos2, int coluna_agivel, int coluna ) : agivel( pos2, coluna_agivel, coluna )
{
	this->vel_mov = 0.5;
	this->objeto = oVIRUS;
	this->tamanho = 23;
	this->faltaRodar = 0.0;
	this->marcaPasso = (rand()%5 + 2);

//	AR_tocar_som(0,dono,pos,Jogadores[Meu_Num]->pos);
}

void virus::agir()
{
	int dirX, dirY, matX, matY;
	int angulo;
	int acertado = -1;

	dirX = round(sin(0.01745*-pos.zr));
    dirY = round(cos(0.01745*-pos.zr));
    matX = (((int)(this->pos.x+(10.0*dirX)))/20); //projecao meio quadrado a frente
    matY = (((int)(this->pos.y+(10.0*dirY)))/20);

	/*Se deve rodar, que rode! Para o lado certo...*/
	if(faltaRodar < 0.0){
		pos.rodar(0, -5.0);
		faltaRodar += 5.0;
		if(faltaRodar == 0.0){
			marcaPasso = rand()%5 + 2;
//			printf("Parei de girar! mat = %d, %d; dir = %d %d\n", matX, matY, dirX, dirY);
//			printf("pos = %.3f, %.3f; pos+ = %.3f, %.3f\n", pos.x, pos.y, (pos.x+(10*dirX)), (pos.y+(10*dirY)));
		}
	}
	else if(faltaRodar > 0.0){
		pos.rodar(0, 5.0);
		faltaRodar -= 5.0;
		if(faltaRodar == 0.0){
			marcaPasso = rand()%5 + 2;
//			printf("Parei de girar! mat = %d, %d; dir = %d %d\n", matX, matY, dirX, dirY);
//			printf("pos = %.3f, %.3f; pos+ = %.3f, %.3f\n", pos.x, pos.y, (pos.x+(10*dirX)), (pos.y+(10*dirY)));
		}
	}

	//se a projecao for pra uma parede ou sair do mapa ou acabarem os passos
	else if(((matX) < 0) || ((matX)+1 > (signed int)TAM_MAPA_X) ||
			((matY) < 0) || ((matY)+1 > (signed int)TAM_MAPA_Y) ||
			(pos.y+(10.0*dirY) < 0.0) || (pos.x+(10.0*dirX) < 0.0) ||
			(MAPA[matX][matY] == 1) || (marcaPasso < 1)){

		//isso fara ele girar mais vezes 90 graus do que 180 graus.
		angulo = (rand()%20);
		if(angulo < 3) angulo = -2;
		else if(angulo < 10) angulo = -1;
		else if(angulo < 17) angulo = 1;
		else angulo = 2;

		faltaRodar = angulo * 90.0;
		//e ele deve comecar a rodar no proximo frame

//		printf("Girando!, mat = %d, %d; dir = %d, %d;\tmarcaPasso = %d\n\t\tMAPA[matX][matY] = %d\n",
//		matX, matY, dirX, dirY, marcaPasso, MAPA[matX][matY]);
	}
	else{

		/* Taaaaaaaalveeeeeeeeeez ficaria mais facil colocar a funcao mover
		 * aqui, como parte desta funcao. Nao sei...
		 * */

		mover(0,1,0);
	}

	    // Ve se acertou algum jogador

    acertado = acertar(pos,-1,tamanho/2);
    if( acertado != -1 )
    {
    	Jogadores[acertado]->acertado();
        AR_tocar_som_3D(1,acertado,Jogadores[acertado]->pos,Jogadores[Meu_Num]->pos);
//        Jogadores[(int)dono]->acertou();
    }
}

void virus::mover( float x, float y, float z )
{
	posicao temp;
	int a, b, c, d;
	int dirX, dirY;
	dirX = round(sin(0.01745*-pos.zr));
    dirY = round(cos(0.01745*-pos.zr));
//	printf("Andou!! =P \n");

	temp.igualar( pos );
    pos.adicionar( (x*vel_mov*cos(0.01745*-pos.zr)+y*vel_mov*sin(0.01745*-pos.zr)), (y*vel_mov*cos(0.01745*-pos.zr)-x*vel_mov*sin(0.01745*-pos.zr)), z*vel_mov );

    /*Se a projecao dele passar de quadradinho, entao ele esta bem no meio de um.
     * Aqui eh uma regiao critica: marcaPasso down!, e, se for girar, tem que girar
     * nessa posicao.
     * Era so isso!! aaaaaaah!!!!!!! 2 dias pra entender isso! agh!!
     * */
    a = (int)((pos.x + 10.0*dirX)/ 20.0);
    b = (int)((temp.x + 10.0*dirX) / 20.0);
    c = (int)((pos.y + 10.0*dirY) / 20.0);
    d = (int)((temp.y + 10.0*dirY) / 20.0);

    //Efeito de rolagem dos virus! =D Rolling Virus! \o/
    pos.rodar(-4.0, 0); //-4 graus parece um valor otimo!

    if( ( a != b ) || (c !=  d) ){
    	marcaPasso--;
//    	printf("marcaPasso down!! Now %d\n", marcaPasso);
		if(marcaPasso == 0){ //se acabaram os passos, deve permanecer onde estava. "teleporta 1 frame pra tras"
			pos.x = temp.x;
			pos.y = temp.y;
		}
	}

	/*Tirei os codigos nao usados que estavam comentados aqui. Hehe!!*/
}

//--------------------------------------------------------------------
// Jogador
//--------------------------------------------------------------------

jogador::jogador( posicao pos2, float tama, int dese, int coluna ) : plano( pos2, tama, dese, coluna )
{
	vel_mov = 2;
	vel_rot = 1;
	vida = 100;
	tiros_dados = 0;
	renascer = 0;
	pontos = 0;
	marcado = 0;
	bebado = 0;
	cafeinado = 0;
	acertos = 0;
	tempo_de_tiro = 0;
	id = -1;
	nome = "";
}

void igualarJJ( jogador *jj, pseudoJogador *j )
{
	jj->acertos = j->acertos;
	jj->bebado = j->bebado;
	jj->desenho = j->desenho;
	jj->id = j->id;
	jj->marcado = j->marcado;
	jj->alterar_nome(j->nome);
	jj->pontos = j->pontos;
	jj->pos.igualar(j->pos);
	jj->renascer = j->renascer;
	jj->tamanho = j->tamanho;
	jj->tiros_dados = j->tiros_dados;
	jj->vel_mov = j->vel_mov;
	jj->vel_rot = j->vel_rot;
	jj->vida = j->vida;
	jj->cafeinado = j->cafeinado;
//	jj->tempo_de_tiro = j->tempo_de_tiro;
}

void pseudoJogador::igualar( jogador *j )
{
	this->acertos = j->acertos;
	this->bebado = j->bebado;
	this->desenho = j->desenho;
	this->id = j->id;
	this->marcado = j->marcado;
	strcpy(this->nome,(j->retornar_nome()).c_str());
	this->pontos = j->pontos;
	this->pos.igualar(j->pos);
	this->renascer = j->renascer;
	this->tamanho = j->tamanho;
	this->tiros_dados = j->tiros_dados;
	this->vel_mov = j->vel_mov;
	this->vel_rot = j->vel_rot;
	this->vida = j->vida;
	this->cafeinado = j->cafeinado;
//	this->tempo_de_tiro = j->tempo_de_tiro;
}

void jogador::acertado()
{
	if(TIPOJOGO)
	{
		cafeinado = 0;
		bebado = 0;
		tempo_de_tiro = 0;
		renascer = TEMPO_REVIVER;
		pos.igualar(CEMITERIO);
	}
	else
	{
		marcado = 1;
	}
}

void jogador::acertou()
{
	if( PapelRede == SERVIDOR )
	{
		marcado = 0;
		pontos++;
		acertos++;
	}
}

void jogador::rodar( float x, float z )
{
	if( !renascer )
	{
		posicao pos2;
		pos2.igualar(pos);
		pos2.rodar( x*vel_rot, z*vel_rot );
		pos.igualar( fisicaR( pos, pos2, tamanho/2 ) );
	}
}

void jogador::mover( float x, float y, float z )
{
	if( !renascer )
	{
		if(this->bebado)
		{
			x = -x;
			y = -y;
			z = -z;
		}

		if(this->cafeinado)
			vel_mov *= 2;

		posicao pos2;
		pos2.igualar(pos);
		pos2.adicionar( (x*vel_mov*cos(0.01745*-pos.zr)+y*vel_mov*sin(0.01745*-pos.zr)), (y*vel_mov*cos(0.01745*-pos.zr)-x*vel_mov*sin(0.01745*-pos.zr)), z*vel_mov );
		pos.igualar( fisica( pos, pos2, tamanho/2, 10 ) );

		if(this->cafeinado)
			vel_mov /= 2;
	}
}

void jogador::alterar_nome( string novo_nome )
{
	this->nome.assign( novo_nome );
}

void jogador::apagar_nome()
{
	nome[0] = '\0';
}

string jogador::retornar_nome()
{
	return this->nome;
}

int jogador::lancar()
{
	// Atira so se nao estiver morto e se o contador de tempo de tiro ja voltou para o zero
	if( !this->renascer && !tempo_de_tiro )
	{
		tempo_de_tiro = 15;
		// Caso servidor
		if( PapelRede == SERVIDOR )
		{
			tiros_dados++;

			new bomba(pos,Meu_Num);

			if( Num_Jogadores > 1 )
			{
				nova_bomba *nova = NULL;

				//Prepara e manda o pacote com as pos da bomba
				if( !( nova = (nova_bomba*)malloc( sizeof(nova_bomba) ) ) ) {
					printf("Erro na alocagem de espaco em lancar nova_bomba\n");
					return 0;
				}
				nova->dono = Meu_Num;
				//nova->pos = pos;
				if( !( AGUA_mandar_pacote( 3, sizeof(nova_bomba), nova, -2 ) ) ) {
					printf("Erro ao tentar mandar pacote de nova_bomba\n");
					free(nova);
					return 0;
				}
				free(nova);
			}
		}
		// Caso cliente
		else if( PapelRede == CLIENTE )
		{
			nova_bomba *nova = NULL;

			//Prepara e manda o pacote com as pos da bomba
			if( !( nova = (nova_bomba*)malloc( sizeof(nova_bomba) ) ) ) {
				printf("Erro na alocagem de espaco em lancar nova_bomba\n");
				return 0;
			}
			nova->dono = Meu_Num;
			//nova->pos = pos;
			if( !( AGUA_mandar_pacote( 3, sizeof(nova_bomba), nova, -1 ) ) ) {
				printf("Erro ao tentar mandar pacote de nova_bomba\n");
				free(nova);
				return 0;
			}
			free(nova);
		}
	}

	return 1;
}

void jogador::renascedor()
{
	if( renascer == 1 )
	{
		renascer--;
		if( NUM_NASCEDOUROS )
		{
			pos.igualar(nascedouros[rand()%(NUM_NASCEDOUROS)].pos);
		}
		else
		{
			pos.igualar(gerar_posicao());
		}
	}
	else if( renascer > 1 )
	{
		renascer--;
		pos.igualar(CEMITERIO);
	}
}

//--------------------------------------------------------------------
// Livro
//--------------------------------------------------------------------
livro::livro( posicao pos2, int coluna_agivel, int coluna ) : agivel( pos2, coluna_agivel, coluna )
{
	this->pontos = 1;
	this->objeto = oLIVRO;
	this->tamanho = 5;
	NUM_PONTOS_RESTANTES += this->pontos;
}

void livro::agir()
{
	int acertado;

	pos.rodar(0,-2);

	acertado = acertar(pos,-1,tamanho);
	if( acertado != -1 )
	{
//		AR_tocar_som(2);
		AR_tocar_som_3D(2,acertado,pos,Jogadores[Meu_Num]->pos);
		Jogadores[acertado]->pontos += this->pontos;
		NUM_PONTOS_RESTANTES -= this->pontos;
		delete this;
	}
}

//--------------------------------------------------------------------
// Computador
//--------------------------------------------------------------------
comp::comp( posicao pos2, int coluna_agivel, int coluna ) : agivel( pos2, coluna_agivel, coluna )
{
	this->pontos = 5;
	this->objeto = oCOMP;
	this->tamanho = 5;
	NUM_PONTOS_RESTANTES += this->pontos;
}

void comp::agir()
{
	int acertado;

	pos.rodar(0,-2);

	acertado = acertar(pos,-1,tamanho);
	if( acertado != -1 )
	{
//		AR_tocar_som(2);
		AR_tocar_som_3D(2,acertado,pos,Jogadores[Meu_Num]->pos);
		Jogadores[acertado]->pontos += this->pontos;
		NUM_PONTOS_RESTANTES -= this->pontos;
		delete this;
	}
}

//--------------------------------------------------------------------
// Cerveja
//--------------------------------------------------------------------
void cerveja::agir()
{
	int acertado;

	pos.rodar(0,-2);

	acertado = acertar(pos,-1,tamanho);
	if( acertado != -1 )
	{
//		AR_tocar_som(3);
		AR_tocar_som_3D(3,acertado,pos,Jogadores[Meu_Num]->pos);
		Jogadores[acertado]->bebado += 200;
		delete this;
	}
}

cerveja::cerveja( posicao pos2, int coluna_agivel, int coluna ) : agivel( pos2, coluna_agivel, coluna )
{
	this->objeto = oCERVEJA;
	this->tamanho = 5;
}

//--------------------------------------------------------------------
// Cafe
//--------------------------------------------------------------------
void cafe::agir()
{
	int acertado;

	pos.rodar(0,-2);

	acertado = acertar(pos,-1,tamanho);
	if( acertado != -1 )
	{
//		AR_tocar_som(3);
		AR_tocar_som_3D(3,acertado,pos,Jogadores[Meu_Num]->pos);
		Jogadores[acertado]->cafeinado += 200;
		delete this;
	}
}

cafe::cafe( posicao pos2, int coluna_agivel, int coluna ) : agivel( pos2, coluna_agivel, coluna )
{
	this->objeto = oCAFE;
	this->tamanho = 5;
}

//--------------------------------------------------------------------
// Mapa
//--------------------------------------------------------------------
mapa::mapa( posicao pos2, int coluna ) : objeto3d( pos2, coluna )
{
	this->objeto = oMAPA;
}

//--------------------------------------------------------------------
// Ceu
//--------------------------------------------------------------------
ceu::ceu( posicao pos2, int coluna ) : objeto3d( pos2, coluna )
{
	this->objeto = oCEU;
}
