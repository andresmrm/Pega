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

#include "SDL/SDL_mixer.h"
#include "ar.h"
#include <cmath>

// Vetor onde serão colocadas as músicas carregadas
#define N_MUSICAS 10
Mix_Music *AR_Musicas[N_MUSICAS];

// Vetor onde serão colocados os sons carregados
#define N_SONS 10
Mix_Chunk *AR_Sons[N_SONS];

// Regula se a biblioteca estara ativa ou desativa
bool AR_Ativa = false;

void AR_desativar()
{
	AR_Ativa = false;
}

void AR_ativar()
{
	AR_Ativa = true;
}

// Inicia essa biblioteca de audio
int AR_soprar()
{
    int i = 0;

    // Limpa os vetores
    for( i=0; i<N_MUSICAS; i++ )
    {
        AR_Musicas[i] = NULL;
    }

    for( i=0; i<N_SONS; i++ )
    {
        AR_Sons[i] = NULL;
    }

    //Inicia SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return 0;
    }

    AR_Ativa = true;

    return 1;
}

// Carrega uma música e a coloca na posição "pos" do vetor de músicas
int AR_carregar_musica( const char nome[20], int pos )
{
    // Carrega a música
    AR_Musicas[pos] = Mix_LoadMUS( (char*)nome );

    // Caso erro
    if( AR_Musicas[pos] == NULL )
    {
        printf("Erro ao carregar musica (%d) %s\n",pos,nome);
        return 0;
    }

    return 1;
}

// Carrega um som e o coloca na posição "pos" do vetor de sons
int AR_carregar_som( const char nome[20], int pos )
{
    // Carrega o som
    AR_Sons[pos] = Mix_LoadWAV( (char*)nome );

    // Caso erro
    if(AR_Sons[pos] == NULL)
    {
        printf("Erro ao carregar som (%d) %s\n",pos,nome);
        return 0;
    }

    return 1;
}

// Começa a tocar a música da posição "pos"
int AR_tocar_musica( int pos )
{
    if( AR_Ativa )
    {
    	if( AR_Musicas[pos] == NULL || Mix_PlayMusic( AR_Musicas[pos], -1 ) == -1 )
		{
			printf("Erro ao tentar tocar musica (%d)\n",pos);
			return 0;
		}
	}

    return 1;
}

// Retorna verdadeiro se a música está tocando
int AR_musica_tocando()
{
	return Mix_PlayingMusic();
}

// Retorna verdadeiro se a música está pausada
int AR_musica_pausada()
{
	return Mix_PausedMusic();
}

// Continua uma musica pausada
void AR_continuar_musica()
{
	Mix_ResumeMusic();
}

// Pausa uma música
void AR_pausar_musica()
{
	Mix_PauseMusic();
}

// Para uma música
void AR_parar_musica()
{
	Mix_HaltMusic();
}

// Toca o som da posição "pos" pelo canal passado
int AR_tocar_som( int pos, int canal )
{
    if( AR_Ativa )
    {
    	if( AR_Sons[pos] == NULL || Mix_PlayChannel( canal, AR_Sons[pos], 0 ) == -1 )
		{
			printf("Erro ao tentar tocar som (%d)\n",pos);
			return 0;
		}
    }

    return 1;
}

/*
 * Toca o som da posição "pos" do vetor de sons pelo canal passado
 * Regula o volume do som de acordo com a distancia entre emissor e receptor
 */
int AR_tocar_som_3D( int pos, int canal, posicao emissor, posicao receptor)
{
	if( canal > 7 || canal < 0 )
	{
		printf("Canal de som passado invalido!\n");
		return 0;
	}

	// calcula a distancia entre as duas posicoes
	float distancia = sqrt(pow(emissor.x-receptor.x,2) + pow(emissor.y-receptor.y,2) + pow(emissor.z-receptor.z,2) );

//	distancia = exp(distancia/123)*10-10;

	// aplica a atenuacao logaritmica do som
	distancia = log(distancia)*42;

	if( distancia > 255 )
		distancia = 255;

	if( distancia < 0 )
		distancia = 0;

	Mix_HaltChannel(canal);

	Mix_SetPosition(canal, 0, distancia);

	AR_tocar_som( pos, canal );

	return 1;
}

// Desliga essa biblioteca e libera as músicas e sons carregados
void AR_emudecer()
{
    int i = 0;

    // Para a musica
    Mix_HaltMusic();

    // Libera as musicas
    for( i=0; i<N_MUSICAS; i++ )
    {
        if( AR_Musicas[i] != NULL ) Mix_FreeMusic( AR_Musicas[i] );
    }

    // Libera os sons
    for( i=0; i<N_SONS; i++ )
    {
        if( AR_Sons[i] != NULL ) Mix_FreeChunk( AR_Sons[i] );
    }

    //Quit SDL_mixer
    Mix_CloseAudio();

    AR_Ativa = 0;
}
