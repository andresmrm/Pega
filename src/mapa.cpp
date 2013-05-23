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

// 20/07/08 - 19:35 - O programador ficou bobo de ver as bombinhas sendo projetadas pela primeira vez e acertando a parede
// 26/07/08 - 16:43 - O programador ficou muito emocionado ao ver que quando o servidor movia seu personagem, um personagem também se movia na visao de um cliente
// 29/01/09 - 22:40 - O programador ficou realmente emocionado ao ver a interface nova no jogo. Toda azul, preta e cinza, semi-transparente.


#include "pega.h"

#define PROXIMO_MAPA 3

objeto3d *PANORAMA;
char PANO[50];

float abs2( float n )
{
	if(n>0)
		return n;
	else
		return -n;
}

// Converte um valor inteiro em uma string com o tempo correspondente, baseado no FPS do jogo
string converter_tempo( int t )
{
	string tempo;
	char temp[100];

	t = t / FPS;
	int min = t / 60;
	int seg = t % 60;

	if( min > 0 )
	{
		sprintf(temp,"%d",min);
		tempo.append(temp);
		tempo.append("min ");
	}

	sprintf(temp,"%d",seg);
	tempo.append(temp);
	tempo.append("s");

	return tempo;
}

// Centraliza uma string em uma posicao da tela
float centralizar_texto( int comp, float posicao, int fonte )
{
	return posicao - (comp/2.0)*FONTES[fonte].tamaX;
}

// Retorna o num de um jogador, a partir de seu endereco de memoria
short int retornarNumVetorJogadores( jogador* j)
{
	int i = 0;

	while( i < MAX_NUM_JOGADORES )
	{
		if( j == Jogadores[i] )
			return i;

		i++;
	}

	return -1;
}

// Envia o nome desse jogador para o servidor, assim ele pode atualiza-lo
int enviar_novo_nome_para_servidor()
{
	novo_nome *nn = NULL;

	//Prepara e manda o pacote com nome
	if( !( nn = (novo_nome*)malloc( sizeof(novo_nome) ) ) ) {
		printf("Erro na alocagem de espaco em enviar novo nome\n");
		return 0;
	}
	strcpy(nn->nome, Jogadores[Meu_Num]->retornar_nome().c_str());

	if( !( AGUA_mandar_pacote( 2, sizeof(novo_nome), nn, -1 ) ) ) {
		printf("Erro ao tentar mandar pacote de novo_nome\n");
		free(nn);
		return 0;
	}
	free(nn);

	return 1;
}

// Uma interface grafica para receber texto
int receber_texto( string* saida, unsigned short int tamanho )
{
	static int i = -1;
	static char nome[100];
	SDL_Event evento;
	char letra = ' ';

	SDL_EnableUNICODE( SDL_ENABLE );

	// Caso essa funcao esta sendo chamada para receber uma string
	if( i < 0 )
	{
		strcpy(nome,saida->c_str());
		i = 0;
	}

	SDL_PollEvent( &evento );

	if( evento.type == SDL_KEYDOWN )
	{
		if( i < tamanho )
		{
			if( evento.key.keysym.unicode == (Uint16)' ' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( ( evento.key.keysym.unicode >= (Uint16)'0' ) && ( evento.key.keysym.unicode <= (Uint16)'9' ) )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( ( evento.key.keysym.unicode >= (Uint16)'A' ) && ( evento.key.keysym.unicode <= (Uint16)'Z' ) )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( ( evento.key.keysym.unicode >= (Uint16)'a' ) && ( evento.key.keysym.unicode <= (Uint16)'z' ) )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)'.' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)'<' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)'>' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)'@' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)'?' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)'!' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)'+' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)'-' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)'_' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)'&' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)'(' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)')' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)'"' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)'=' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)':' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)';' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)'*' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)'%' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)'#' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)'$' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( evento.key.keysym.unicode == (Uint16)',' )
			{
				letra = (char)evento.key.keysym.unicode;
				nome[i] = letra;
				i++;
			}
			else if( ( evento.key.keysym.sym == SDLK_DELETE ) )
			{
				letra = ' ';
				nome[i] = letra;
			}
			else if( ( evento.key.keysym.sym == SDLK_RIGHT ) )
			{
				i++;
			}
		}

		if( ( evento.key.keysym.sym == SDLK_BACKSPACE ) && ( i > 0 ) )
		{
			i--;
			letra = ' ';
			nome[i] = letra;
		}
		else if( ( evento.key.keysym.sym == SDLK_LEFT ) && ( i > 0 ) )
		{
			i--;
		}
	}

	saida->assign(nome);

	// tira os espacos do final da string
	saida->assign( saida->substr( 0, saida->find_last_not_of(' ',saida->length())+1 ) );

	 if( evento.key.keysym.sym == SDLK_RETURN )
	{
		 for(int j=0; j<100; j++)
			 nome[j]='\0';
		 i = -1;
	}
	else if( evento.key.keysym.sym == SDLK_ESCAPE )
	{
		for(int j=0; j<100; j++)
			nome[j]='\0';
		i = -2;
	}

	SDL_EnableUNICODE( SDL_DISABLE );

	return i;
}

// Le o mapa de um arquivo
int ler_mapa(string nomeArquivo)
{
	int x = 0;
	int y = 0;
	char c = 0;
	string temp = "mapas/";
	temp.append(nomeArquivo);
	ifstream arquivo(temp.c_str());
	posicao pos;
	bool neblina = false;

	cout << "\nLendo arquivo com mapa:\n";

	if( arquivo.is_open() )
	{
		while( !arquivo.eof() )
		{
			arquivo.get(c);

			// desconsidera a parte que trata das texturas
			if( c == '@' )
			{
				while( c != '\n' )
					arquivo.get(c);

				arquivo.get(c);

				if( c == '@' )
				{
					while( c != '\n' )
						arquivo.get(c);

					arquivo.get(c);

					if( c == '@' )
					{
						while( c != '\n' )
							arquivo.get(c);

						arquivo.get(c);
					}

					if( c == '%' )
					{
						arquivo.get(c);
						int j = 0;
						while( c != '\n' )
						{
							arquivo.get(c);
							PANO[j] = c;
							j++;
						}
						PANO[j] = '\0';

						cout << "Panorama: " << PANO << "\n";

						string temp2 = "cp texturas/";
						temp2 += PANO;
						temp2.resize(temp2.length()-1);
						temp2 += " texturas/pano.tga";
						system(temp2.c_str());
						FOGO_carregar_objeto3D("pano.obj",1000,oPANORAMA);

						arquivo.get(c);
					}
				}

				arquivo.get(c);
			}

			if( c == 'N')
			{
				neblina = true;
				arquivo.get(c);
				arquivo.get(c);
			}
			else if( c == '\n' && !arquivo.eof() )
			{
				TAM_MAPA_X= x;
				x = 0;
				y++;
				cout << c;
			}
			else if( c == '.')
			{
				MAPA[x][y] = 0;
				x++;
				cout << c;
			}
			else if( c == '#')
			{
				MAPA[x][y] = 1;
				x++;
				cout << c;
			}
			else if( c == 'B' )
			{
				pos.mudar_r(90,0);
				pos.mudar(TAM_MAPA*x+TAM_MAPA/2, TAM_MAPA*y+TAM_MAPA/2, 2.5);
				new cerveja(pos,1,0);
				MAPA[x][y] = 0;
				x++;
				cout << c;
			}
			else if( c == 'F' )
			{
				pos.mudar_r(90,0);
				pos.mudar(TAM_MAPA*x+TAM_MAPA/2, TAM_MAPA*y+TAM_MAPA/2, 4);
				new cafe(pos,1,0);
				MAPA[x][y] = 0;
				x++;
				cout << c;
			}
			else if( c == 'L' )
			{
				pos.mudar_r(45,0);
				pos.mudar(TAM_MAPA*x+TAM_MAPA/2, TAM_MAPA*y+TAM_MAPA/2, 2.5);
				new livro(pos,1,0);
				MAPA[x][y] = 0;
				x++;
				cout << c;
			}
			else if( c == 'C' )
			{
				pos.mudar_r(90,0);
				pos.mudar(TAM_MAPA*x+TAM_MAPA/2, TAM_MAPA*y+TAM_MAPA/2, 5);
				new comp(pos,1,0);
				MAPA[x][y] = 0;
				x++;
				cout << c;
			}
			else if( c == 'V' )
			{
				pos.mudar_r(90,0);
				pos.mudar(TAM_MAPA*x+TAM_MAPA/2, TAM_MAPA*y+TAM_MAPA/2, 8);
				new virus(pos,1,0);
				MAPA[x][y] = 0;
				x++;
				cout << c;
			}
			else if( c == 'J' )
			{
				nascedouros[NUM_NASCEDOUROS].pos.mudar(TAM_MAPA*x+TAM_MAPA/2, TAM_MAPA*y+TAM_MAPA/2, 5);
				nascedouros[NUM_NASCEDOUROS].pos.mudar_r(-90,0);
				nascedouros[NUM_NASCEDOUROS].uso = 0;
				NUM_NASCEDOUROS++;
				MAPA[x][y] = 0;
				x++;
				cout << c;
			}
		}
		arquivo.close();

		if( neblina )
			FOGO_ativar_neblina();
		else
			FOGO_desativar_neblina();

		TAM_MAPA_Y = y;
	}
	else
	{
		cout << "Erro ao tentar abrir arquivo do mapa!\n";
		return 1;
	}

	if( TAM_MAPA_X <= 0 || TAM_MAPA_Y <= 0 )
	{
		cout << "Erro na leitura do mapa! (Talvez haja uma linha sobrando no final)\n";
		return 2;
	}

	cout << "Arquivo lido, mapa carregado\n\n";

	return 0;
}

// Gera uma posição em um lugar válido do mapa e a retorna
posicao gerar_posicao()
{
	int x = 0;
	int y = 0;
	int zr = 0;
	posicao pos;

	zr = rand()%360;
	pos.mudar_r(-90,zr);

	if( TAM_MAPA_X > 0 && TAM_MAPA_Y )
	{
		do
		{
			x = rand()%TAM_MAPA_X;
			y = rand()%TAM_MAPA_Y;
		}
		while( MAPA[x][y] == 1 );

		pos.mudar(TAM_MAPA*x+5,TAM_MAPA*y+5,5);
	}
	else
		pos.mudar(0,0,0);

	return pos;
}

// Percorre todos os elementos da lista de agiveis e os faz agir
void agir_lista(int n)
{
	void *lin = NULL;
	void *objeto, *objeto2;

	// Se há elementos na lista
    if( agiveis.num_linhas )
    {
    	objeto = retornar_objeto( &agiveis, n, &lin );

        do
        {
            objeto2 = retornar_objeto( &agiveis, n, &lin );
            if(objeto != NULL)
            {
                ((agivel*)objeto)->agir();
            }
            objeto = objeto2;
        }
        while( lin != NULL );

        if(objeto != NULL)
		{
			((agivel*)objeto)->agir();
		}
    }
}


int acertar( posicao posB, char dono, float tama )
{
	int i, acertou = -1;
	posicao posC, posE, posD, posE2, posD2;
	float dist;

	for( i=0; i<MAX_NUM_JOGADORES; i++ )
	{
		if( i != dono )
		{
			posC.igualar(Jogadores[i]->pos);
			posE.mudar( posC.x - Jogadores[i]->tamanho/2.5*cos(0.01745*posC.zr), posC.y - Jogadores[i]->tamanho/2.5*sin(0.01745*posC.zr),posC.z );
			posD.mudar( posC.x + Jogadores[i]->tamanho/2.5*cos(0.01745*posC.zr), posC.y + Jogadores[i]->tamanho/2.5*sin(0.01745*posC.zr),posC.z );
            posE2.mudar( posC.x - Jogadores[i]->tamanho/4.5*cos(0.01745*posC.zr), posC.y - Jogadores[i]->tamanho/4.5*sin(0.01745*posC.zr),posC.z );
			posD2.mudar( posC.x + Jogadores[i]->tamanho/4.5*cos(0.01745*posC.zr), posC.y + Jogadores[i]->tamanho/4.5*sin(0.01745*posC.zr),posC.z );


//            dist = mod(posE.x - posB.x) + mod(posD.x - posB.x) + mod(posE.y - posB.y) + mod(posD.y - posB.y);
//
//            if( dist < 13 )
//            {
//                Jogadores[i]->acertado();
//                acertou = 1;
//            }

            dist = tama;//cos(posB.zr-posC.zr);

			if( abs2(posC.x-posB.x)<dist && abs2(posC.y-posB.y)<dist )
			{
				acertou = i;
			}
			else if( abs2(posE.x-posB.x)<dist && abs2(posE.y-posB.y)<dist )
			{
				acertou = i;
			}
			else if( abs2(posD.x-posB.x)<dist && abs2(posD.y-posB.y)<dist )
			{
				acertou = i;
			}
			else if( abs2(posE2.x-posB.x)<dist && abs2(posE2.y-posB.y)<dist )
			{
				acertou = i;
			}
			else if( abs2(posD2.x-posB.x)<dist && abs2(posD2.y-posB.y)<dist )
			{
				acertou = i;
			}
		}

	}

	return acertou;
}

// Recebe a posicao atual do obj, a posicao para a qual ele deseja ir, a dist do centro até as bordas e retorna a posicao válida mais proxima da desejada
posicao fisica( posicao pos1, posicao pos2, float tama, int iteracoes )
{
	posicao posC, posE, posD;
//	posicao posEV, posDV;
//	int corr = 0;
	float difx = pos2.x - pos1.x;
	float dify = pos2.y - pos1.y;

	// Calcula a pos Central, Esquerda e Direita do objeto
    posC.igualar(pos2);
    for( int i=1; i <= iteracoes; i++ )
    {
		posE.mudar( posC.x - (float(i)/float(iteracoes)) *tama*cos(0.01745*posC.zr), posC.y - (float(i)/float(iteracoes)) *tama*sin(0.01745*posC.zr),posC.z );
		posD.mudar( posC.x + (float(i)/float(iteracoes)) *tama*cos(0.01745*posC.zr), posC.y + (float(i)/float(iteracoes)) *tama*sin(0.01745*posC.zr),posC.z );
//		posEV.igualar(posE);
//		posDV.igualar(posD);

		//printf("POSC (%.0f, %.0f, %.0f)(%.0f)\n",posC.x,posC.y,posC.z,posC.zr);
//		printf("POSE (%.0f, %.0f, %.0f)\n",posE.x,posE.y,posE.z);
//		printf("POSD (%.0f, %.0f, %.0f)\n",posD.x,posD.y,posD.z);

		// Verifica o X das posE e posD se não passou pelas bordas do mapa
		if( posE.x - DISTANCIA_FISICA < 0 && posE.x < posD.x ) posC.x = DISTANCIA_FISICA + (posC.x - posE.x);
		else if( posE.x + DISTANCIA_FISICA >= TAM_MAPA_X*TAM_MAPA*TAMANHO && posE.x > posD.x ) posC.x = TAM_MAPA_X*TAM_MAPA*TAMANHO - DISTANCIA_FISICA + (posC.x - posE.x);
		else if( posD.x - DISTANCIA_FISICA < 0) posC.x = DISTANCIA_FISICA + (posC.x - posD.x);
		else if( posD.x + DISTANCIA_FISICA >= TAM_MAPA_X*TAM_MAPA*TAMANHO) posC.x = TAM_MAPA_X*TAM_MAPA*TAMANHO - DISTANCIA_FISICA + (posC.x - posD.x);
		// Verifica o Y das posE e posD se não passou pelas bordas do mapa
		if( posE.y - DISTANCIA_FISICA < 0 && posE.y < posD.y ) posC.y = DISTANCIA_FISICA + (posC.y - posE.y);
		else if( posE.y + DISTANCIA_FISICA >= TAM_MAPA_Y*TAM_MAPA*TAMANHO && posE.y > posD.y ) posC.y = TAM_MAPA_Y*TAM_MAPA*TAMANHO - DISTANCIA_FISICA + (posC.y - posE.y);
		else if( posD.y - DISTANCIA_FISICA < 0 ) posC.y = DISTANCIA_FISICA + (posC.y - posD.y);
		else if( posD.y + DISTANCIA_FISICA >= TAM_MAPA_Y*TAM_MAPA*TAMANHO ) posC.y = TAM_MAPA_Y*TAM_MAPA*TAMANHO - DISTANCIA_FISICA + (posC.y - posD.y);


		if( MAPA[(int)((posE.x)/TAM_MAPA)][(int)((posE.y)/TAM_MAPA)] == 1 )
		{
			if( MAPA[(int)((posE.x)/TAM_MAPA)][(int)((posE.y-dify)/TAM_MAPA)] == 1 )
			{
				if( MAPA[(int)((posE.x-difx)/TAM_MAPA)][(int)((posE.y)/TAM_MAPA)] == 1 )
				{
					posC.igualar(pos1);
				}
				else
				{
					posC.x = pos1.x;
				}
			}
			else
			{
				posC.y = pos1.y;
			}
		}

		if( MAPA[(int)((posD.x)/TAM_MAPA)][(int)((posD.y)/TAM_MAPA)] == 1 )
		{
			if( MAPA[(int)((posD.x)/TAM_MAPA)][(int)((posD.y-dify)/TAM_MAPA)] == 1 )
			{
				if( MAPA[(int)((posD.x-difx)/TAM_MAPA)][(int)((posD.y)/TAM_MAPA)] == 1 )
				{
					posC.igualar(pos1);
				}
				else
				{
					posC.x = pos1.x;
				}
			}
			else
			{
				posC.y = pos1.y;
			}
		}
	}

	return posC;
}

posicao fisicaR( posicao pos1, posicao pos2, float tama )
{
    posicao posC, posE, posD;
    posC.igualar(pos2);
    posE.mudar( posC.x - tama*cos(0.01745*posC.zr), posC.y - tama*sin(0.01745*posC.zr),posC.z );
    posD.mudar( posC.x + tama*cos(0.01745*posC.zr), posC.y + tama*sin(0.01745*posC.zr),posC.z );

    //printf("POSC (%.0f, %.0f, %.0f)(%.0f)\n",posC.x,posC.y,posC.z,posC.zr);
    //printf("POSE (%.0f, %.0f, %.0f)\n",posE.x,posE.y,posE.z);
    //printf("POSD (%.0f, %.0f, %.0f)\n",posD.x,posD.y,posD.z);

    if( posE.x - DISTANCIA_FISICA < 0 && posE.x < posD.x ) posC.x = DISTANCIA_FISICA + (posC.x - posE.x);
    else if( posE.x + DISTANCIA_FISICA >= TAM_MAPA_X*TAM_MAPA*TAMANHO && posE.x > posD.x ) posC.x = TAM_MAPA_X*TAM_MAPA*TAMANHO - DISTANCIA_FISICA + (posC.x - posE.x);
    else if( posD.x - DISTANCIA_FISICA < 0) posC.x = DISTANCIA_FISICA + (posC.x - posD.x);
    else if( posD.x + DISTANCIA_FISICA >= TAM_MAPA_X*TAM_MAPA*TAMANHO) posC.x = TAM_MAPA_X*TAM_MAPA*TAMANHO - DISTANCIA_FISICA + (posC.x - posD.x);

    if( posE.y - DISTANCIA_FISICA < 0 && posE.y < posD.y ) posC.y = DISTANCIA_FISICA + (posC.y - posE.y);
    else if( posE.y + DISTANCIA_FISICA >= TAM_MAPA_Y*TAM_MAPA*TAMANHO && posE.y > posD.y ) posC.y = TAM_MAPA_Y*TAM_MAPA*TAMANHO - DISTANCIA_FISICA + (posC.y - posE.y);
    else if( posD.y - DISTANCIA_FISICA < 0 ) posC.y = DISTANCIA_FISICA + (posC.y - posD.y);
    else if( posD.y + DISTANCIA_FISICA >= TAM_MAPA_Y*TAM_MAPA*TAMANHO ) posC.y = TAM_MAPA_Y*TAM_MAPA*TAMANHO - DISTANCIA_FISICA + (posC.y - posD.y);

	if( MAPA[(int)((posE.x)/TAM_MAPA)][(int)((posE.y)/TAM_MAPA)] == 1 )
	{
		posC.igualar(pos1);
	}
	else if( MAPA[(int)((posD.x)/TAM_MAPA)][(int)((posD.y)/TAM_MAPA)] == 1 )
	{
		posC.igualar(pos1);
	}
	else if( MAPA[(int)((posC.x)/TAM_MAPA)][(int)((posC.y)/TAM_MAPA)] == 1 )
	{
		posC.igualar(pos1);
	}

	return posC;
}

void interface( casca *painel )
{
	string temp3;
    char temp[50];
//    char temp2[10];
//    float x = -0.2;

    if( ! Jogadores[Meu_Num]->bebado )
    {
		if(Jogadores[Meu_Num]->renascer)
		{
			FOGO_desenhar_2D(15,0,0,100,100);
			FOGO_desenhar_2D(13,0,0,100,100);
			temp3 = "Atingido. Renascer em:";
			FOGO_escrever(centralizar_texto(temp3.size(),2,0),-68,temp3.c_str(),0);
			temp3 = converter_tempo(Jogadores[Meu_Num]->renascer).c_str();
			FOGO_escrever(centralizar_texto(temp3.size(),2,1),-90,temp3.c_str(),1);
		}
		else if( Jogadores[Meu_Num]->cafeinado )
		{
			FOGO_desenhar_2D(13,0,0,100,100);
			temp3 = "Estado cafeinado por:";
			FOGO_escrever(centralizar_texto(temp3.size(),2,0),-68,temp3.c_str(),0);
			temp3 = converter_tempo(Jogadores[Meu_Num]->cafeinado).c_str();
			FOGO_escrever(centralizar_texto(temp3.size(),2,1),-90,temp3.c_str(),1);
		}
		else
		{
			FOGO_desenhar_2D(10,0,0,100,100);
		}

		temp3 = "Seus Pontos:";
		FOGO_escrever(centralizar_texto(temp3.size(),-81,0),-70,temp3.c_str(),0);
		sprintf(temp,"%d",Jogadores[Meu_Num]->pontos);
		temp3 = temp;
		FOGO_escrever(centralizar_texto(temp3.size(),-81,1),-90,temp3.c_str(),1);

		temp3 = "Pontos no Mapa:";
		FOGO_escrever(centralizar_texto(temp3.size(),82,0),-70,temp3.c_str(),0);
		sprintf(temp,"%d",NUM_PONTOS_RESTANTES);
		temp3 = temp;
		FOGO_escrever(centralizar_texto(temp3.size(),82,1),-90,temp3.c_str(),1);

		temp3 = "Tempo Restante:";
		FOGO_escrever(centralizar_texto(temp3.size(),-80,0),92,temp3.c_str(),0);
		temp3 = converter_tempo(TEMPO_RESTANTE).c_str();
		FOGO_escrever(centralizar_texto(temp3.size(),-80,2),82,temp3.c_str(),2);

		temp3 = "Nomes";
		FOGO_escrever(centralizar_texto(temp3.size(),60,0),92,temp3.c_str(),0);
		temp3 = "Pontos";
		FOGO_escrever(centralizar_texto(temp3.size(),90,0),92,temp3.c_str(),0);
		for(int i=0; i<MAX_NUM_JOGADORES; i++)
		{
			if( Jogadores[i]->id >= 0 )
			{
				temp3 = Jogadores[i]->retornar_nome();
				FOGO_escrever(centralizar_texto(temp3.size(),60,0),85-i*(FONTES[0].tamaY+2),temp3.c_str(),0);
				sprintf(temp,"%d",Jogadores[i]->pontos);
				temp3 = temp;
				FOGO_escrever(centralizar_texto(temp3.size(),90,0),85-i*(FONTES[0].tamaY+2),temp3.c_str(),0);
			}
		}

		// Minimapa
		float posX = 78;
		float posY = 39;
		float tamX = 20.35;
		float tamY = 27.25;
		float escalaX = tamX/TAM_MAPA_X;
		float escalaY = tamY/TAM_MAPA_Y;

		for(unsigned int k=0; k<TAM_MAPA_X; k++)
		{
			for(unsigned int j=0; j<TAM_MAPA_Y; j++)
			{
				if(MAPA[k][j])
					FOGO_desenhar_2D(17,posX+k*escalaX,posY+j*escalaY,escalaX,escalaY);
				else
					FOGO_desenhar_2D(16,posX+k*escalaX,posY+j*escalaY,escalaX,escalaY);
			}
		}
		if(!Jogadores[Meu_Num]->renascer)
			FOGO_desenhar_2D(18,posX+(Jogadores[Meu_Num]->pos.x-10)/TAM_MAPA*escalaX,posY+(Jogadores[Meu_Num]->pos.y-10)/TAM_MAPA*escalaY,escalaX,escalaY);
//		FOGO_desenhar_2D(17,posX,posY,tamX,tamY);
    }
    else
    {
    	FOGO_desenhar_2D(13,0,0,100,100);
		temp3 = "Estado embriagado por:";
		FOGO_escrever(centralizar_texto(temp3.size(),2,0),-68,temp3.c_str(),0);
		temp3 = converter_tempo(Jogadores[Meu_Num]->bebado).c_str();
		FOGO_escrever(centralizar_texto(temp3.size(),2,1),-90,temp3.c_str(),1);
    }
}

void interface_receber_texto( string nome, int cursor )
{
	float tam_char = FONTES[2].tamaX;
	string temp;

	temp = "Digite seu nome:";
	FOGO_escrever(centralizar_texto(temp.size(),0,2),65,temp.c_str(),2);
	FOGO_escrever(-(nome.size()*(tam_char/2)),-8,nome.c_str(),2);
	FOGO_escrever(cursor*tam_char-nome.size()*(tam_char/2),-8,"_",2);
	FOGO_escrever(cursor*tam_char-nome.size()*(tam_char/2),FONTES[2].tamaY-8,"_",2);
}

void carregar_mapa(string nomeArquivo)
{
	posicao pos;

	nomeArquivo.append(".obj");

//	FOGO_carregar_objeto3D("ceu.obj",TAM_MAPA*10,oCEU);
	FOGO_carregar_objeto3D(nomeArquivo.c_str(),TAM_MAPA,oMAPA);

	// Mapa
	pos.mudar(0,0,0);
	pos.mudar_r(0,0);
	new mapa(pos,0);
	// Céu
//	pos.mudar_r(180,0);
//	pos.mudar(-100,-100,100);
//	new casca( pos, 20000, 8, 0 );

	pos.mudar(0,0,0);
	pos.mudar_r(90,0);
	PANORAMA = new objeto3d( pos, 0 );
	PANORAMA->objeto = oPANORAMA;
}

int receber_comandos()
{
	Uint8 *keystates;
	SDL_Event evento;
	int var = EM_LOOP;

	while( SDL_PollEvent( &evento ) )
	{
		if( evento.type == SDL_MOUSEMOTION/* && evento.motion.type == SDL_MOUSEMOTION */)
		{

//				Jogadores[Meu_Num]->rodar(log(evento.motion.yrel),log(evento.motion.xrel));

//			if(evento.motion.yrel > 0)
//			{
//				Jogadores[Meu_Num]->rodar(-1-log(evento.motion.yrel),0);
////					cout << -log(evento.motion.yrel) << "< Y\n";
//			}
//			else if(evento.motion.yrel < 0)
//			{
//				Jogadores[Meu_Num]->rodar(1+log(-evento.motion.yrel),0);
////					cout << log(-evento.motion.yrel) << "< Y*\n";
//			}
			if(evento.motion.xrel > 0)
			{
				Jogadores[Meu_Num]->rodar(0,-1-log(evento.motion.xrel));
//					cout << -log(evento.motion.xrel) << "< X\n";
			}
			else if(evento.motion.xrel < 0)
			{
				Jogadores[Meu_Num]->rodar(0,1+log(-evento.motion.xrel));
//					cout << log(-evento.motion.xrel) << "< X\n";
			}
			SDL_WarpMouse(TELA_COMPRIMENTO/2, TELA_ALTURA/2);
		}
		else if( evento.type == SDL_QUIT )
		{
			var = SAIR;
		}
		else if( evento.type == SDL_KEYDOWN )
		{
			if( evento.key.keysym.sym == SDLK_ESCAPE )
			{
				var = SAIR;
			}
			else if( evento.key.keysym.sym == SDLK_F2 )
			{
				if( PapelRede == SERVIDOR )
				{
					var = PROXIMO_MAPA;
				}
			}
			else if( evento.key.keysym.sym == SDLK_F3 )
			{
				var = TROCAR_NOME;
			}
			else if( evento.key.keysym.sym == SDLK_F11 )
			{
				FOGO_alternar_tela_cheia();
			}
			else if( evento.key.keysym.sym == SDLK_z ){Jogadores[Meu_Num]->rodar(0,180);}
			else if( evento.key.keysym.sym == SDLK_q ){Jogadores[Meu_Num]->rodar(0,90);}
			else if( evento.key.keysym.sym == SDLK_e ){Jogadores[Meu_Num]->rodar(0,-90);}
		}

		//if( evento.type == SDL_MOUSEBUTTONDOWN && evento.button.button == 1 )
		if( (SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(1)) != 0 )
		{
			Jogadores[Meu_Num]->lancar();
		}
	}

	keystates = SDL_GetKeyState( NULL );

	if( keystates[ SDLK_c ] )
	{
		if( keystates[ SDLK_UP ] ){FOGO_CAMERA.mover(0,1,0);}
		if( keystates[ SDLK_DOWN ] ){FOGO_CAMERA.mover(0,-1,0);}
		if( keystates[ SDLK_LEFT ] ){FOGO_CAMERA.mover(-1,0,0);}
		if( keystates[ SDLK_RIGHT ] ){FOGO_CAMERA.mover(1,0,0);}
		if( keystates[ SDLK_PAGEUP ] ){FOGO_CAMERA.mover(0,0,1);}
		if( keystates[ SDLK_PAGEDOWN ] ){FOGO_CAMERA.mover(0,0,-1);}

		if( keystates[ SDLK_a ] ){FOGO_CAMERA.rodar(0,-1);}
		if( keystates[ SDLK_d ] ){FOGO_CAMERA.rodar(0,1);}
		if( keystates[ SDLK_w ] ){FOGO_CAMERA.rodar(1,0);}
		if( keystates[ SDLK_s ] ){FOGO_CAMERA.rodar(-1,0);}

		if( keystates[ SDLK_a ] ){Jogadores[Meu_Num]->rodar(0,1);}
		if( keystates[ SDLK_d ] ){Jogadores[Meu_Num]->rodar(0,-1);}
		if( keystates[ SDLK_w ] ){Jogadores[Meu_Num]->rodar(1,0);}
		if( keystates[ SDLK_s ] ){Jogadores[Meu_Num]->rodar(-1,0);}

		if( keystates[ SDLK_f ] ){FOGO_CAMERA.pos2.rodar(0,1);}
		if( keystates[ SDLK_h ] ){FOGO_CAMERA.pos2.rodar(0,-1);}
		if( keystates[ SDLK_t ] ){FOGO_CAMERA.pos2.rodar(1,0);}
		if( keystates[ SDLK_g ] ){FOGO_CAMERA.pos2.rodar(-1,0);}
		if( keystates[ SDLK_r ] )
		{
			FOGO_CAMERA.imprimir();
			int camX = FOGO_CAMERA.pos.x/20;
			int camY = FOGO_CAMERA.pos.y/20;
			int valorMapa = MAPA[camX][camY];
			cout << "POS0 : " << camX << " " << camY << "\n";
			cout << "MAPA : " << valorMapa<< "\n";
		}

	}
	else if( Meu_Num >= 0 )
	{
		if( keystates[ SDLK_UP ] || keystates[ SDLK_w ] ){Jogadores[Meu_Num]->mover(0,1,0);}
		if( keystates[ SDLK_DOWN ] || keystates[ SDLK_s ] ){Jogadores[Meu_Num]->mover(0,-1,0);}
		if( keystates[ SDLK_LEFT ] || keystates[ SDLK_a ] ){Jogadores[Meu_Num]->mover(-1,0,0);}
		if( keystates[ SDLK_RIGHT ] || keystates[ SDLK_d ] ){Jogadores[Meu_Num]->mover(1,0,0);}
		if( keystates[ SDLK_PAGEUP ] ){Jogadores[Meu_Num]->mover(0,0,1);}
		if( keystates[ SDLK_PAGEDOWN ] ){Jogadores[Meu_Num]->mover(0,0,-1);}

//		if( keystates[ SDLK_q ] )
//		{
//			FONTES[1].tamaX += 0.01;
//			cout << FONTES[1].tamaX << "\n";
//		}
//
//		if( keystates[ SDLK_e ] )
//		{
//			FONTES[1].tamaX -= 0.01;
//			cout << FONTES[1].tamaX << "\n";
//		}


		if( keystates[ SDLK_SPACE ] ){Jogadores[Meu_Num]->lancar();}

		if( !keystates[ SDLK_x ] ){
			FOGO_CAMERA.pos.igualar(Jogadores[Meu_Num]->pos);
			FOGO_CAMERA.pos.zr = -Jogadores[Meu_Num]->pos.zr;
			FOGO_CAMERA.pos.rodar(90,0);
		}

//			FOGO_CAMERA.pos2.mudar_r(0,Jogadores[Meu_Num]->pos.zr);
//			FOGO_CAMERA.pos2.mudar_r(0,0);

//			FOGO_CAMERA.pos2.mudar(20,10,0);
	}

	return var;
}

int rodar_mapa(string arquivo_mapa)
{
	int i;
	int var = EM_LOOP;
	int index = 0;
	int Freq_Rede = 0;
	posicao pos;
	bool lendo_texto = false;
	string nome;
	casca *painel = NULL;

	cout << "Iniciando mapa: < " << arquivo_mapa << " >\n";

	AR_tocar_musica(1);

	srand(time(0));
	NUM_PONTOS_RESTANTES = 0;
	NUM_NASCEDOUROS = 0;

	if( !( iniciar_tabela(&agiveis,2) ) ) return 0;

	if( ler_mapa(arquivo_mapa) ) return 0;
	carregar_mapa(arquivo_mapa);

	//Inicia a lista de jogadores e coloca seus nomes
	pos.mudar(-1,-1,-100);
	pos.mudar_r(-90,0);
	Jogadores[0]->id = 0;
	for( i=0; i < MAX_NUM_JOGADORES; i++ )
	{
		Jogadores[i]->cafeinado = 0;
		Jogadores[i]->bebado = 0;
		Jogadores[i]->tempo_de_tiro = 0;
		if( Jogadores[i]->id >= 0 )
		{
			if( NUM_NASCEDOUROS )
			{
				Jogadores[i]->pos.igualar(nascedouros[rand()%(NUM_NASCEDOUROS)].pos);
			}
			else
			{
				Jogadores[i]->pos.igualar(gerar_posicao());
			}
		}
		else
		{
			cout << "Jogador inativo: " << i << "\n";
			Jogadores[i]->pos.igualar(CEMITERIO);
		}
	}
	if( PapelRede == SERVIDOR )
	{

//		Jogadores[0]->alterar_nome(NOME);
		Jogadores[0]->marcado = 1;
	}

//	painel = new casca(pos,0.225,10,2);

	//-----------------------------------------------

	cout << "JOGUE!"<< "\n";

	AR_tocar_som(4,Meu_Num);

	var = EM_LOOP;
	while( var == EM_LOOP )
	{
		// Le os comandos do usuario e se eh para sair, seta "var"
		if( lendo_texto )
		{
			index = receber_texto(&nome,20);
			// caso o usuario deu ENTER
			if( index == -1 )
			{
				Jogadores[Meu_Num]->alterar_nome(nome);
				strcpy(NOME,Jogadores[Meu_Num]->retornar_nome().c_str());
				cout << "Vc trocou de nome para: " << nome << "\n";

				// Comunica ao servidor a mudanca de nome
				if( PapelRede == CLIENTE )
					enviar_novo_nome_para_servidor();

				lendo_texto = false;
			}
			// caso o usuario deu ESC
			else if( index == -2 )
			{
				lendo_texto = false;
				nome = Jogadores[Meu_Num]->retornar_nome();
			}
		}
		else
		{
			var = receber_comandos();
			if( var == TROCAR_NOME )
			{
				var = EM_LOOP;
				lendo_texto = true;
				nome = Jogadores[Meu_Num]->retornar_nome();
			}
		}

		for(i=0;i<MAX_NUM_JOGADORES;i++)
		{
			if(Jogadores[i]->bebado) Jogadores[i]->bebado--;
			if(Jogadores[i]->cafeinado) Jogadores[i]->cafeinado--;
			if(Jogadores[i]->tempo_de_tiro) Jogadores[i]->tempo_de_tiro--;

			if( PapelRede == SERVIDOR )
				Jogadores[i]->renascedor();

			if(!TIPOJOGO)
				if(Jogadores[i]->marcado) Jogadores[i]->vida--;
		}

		agir_lista(0);
		agir_lista(0);
		agir_lista(0);
		agir_lista(0);
		agir_lista(0);
		agir_lista(1);

		PANORAMA->pos.x = Jogadores[Meu_Num]->pos.x;
		PANORAMA->pos.y = Jogadores[Meu_Num]->pos.y;

		FOGO_desenhar_listas_inicio();
		FOGO_desenhar_lista(0);
		FOGO_desenhar_lista(1);
		if(lendo_texto)
			interface_receber_texto( nome, index );
		else
			interface(painel);
		FOGO_desenhar_listas_fim();

//		if ( Freq_Rede == 1 )
			Freq_Rede = 0;
//		else
//			Freq_Rede++;

//		cout << Jogadores[0]->pos.x << " " << Jogadores[1]->pos.x << " " << Jogadores[2]->pos.x << " " << Jogadores[3]->pos.x << " - A\n";

		// Caso seja o servidor
		if(PapelRede == SERVIDOR && Freq_Rede == 0)
		{
			lidar_entradas_servidor();
			gerar_saidas_servidor();
		}
		// Caso seja um cliente
		else if(PapelRede == CLIENTE && Freq_Rede == 0)
		{
			lidar_entradas_cliente();
			if( Meu_Num > 0 ) // Esse if evita que o cliente tente mandar sua pos para o server antes de ter recebido dele seu num
				gerar_saidas_cliente();
		}

//		cout << Jogadores[0]->pos.x << " " << Jogadores[1]->pos.x << " " << Jogadores[2]->pos.x << " " << Jogadores[3]->pos.x << " - B\n";

		// Quando todos os pontos forem coletados ou o servidor mudar de mapa
		if( PapelRede == SERVIDOR )
		{
			if( NUM_PONTOS_RESTANTES == 0 ) var = PROXIMO_MAPA;
		}
		else
		{
			if( NOME_MAPA != arquivo_mapa ) var = PROXIMO_MAPA;
		}

		TEMPO_RESTANTE--;
		if( TEMPO_RESTANTE == 0 ) var = CONTINUAR;
	}

	// altera var para q o jogo continue em loop rodando o proximo mapa
	if( var == 3 )
		var = EM_LOOP;

	deletar_tabela(&agiveis);
	// Esvazia a lista q tem os objetos e o mapa, mas nao os jogadores
	FOGO_reiniciar_uma_lista_render(0);

	cout << "O mapa acabou\n";

	if( PapelRede == CLIENTE )
	{
//		Meu_Num = -1;
		NOME_MAPA = "";
		NUM_PONTOS_RESTANTES = 0;
	}

	return var;
}
