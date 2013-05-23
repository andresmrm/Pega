/*-----------------------------------------------------------------------------
 * Copyright 2008 Andrés M. R. Martano
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *---------------------------------------------------------------------------*/

/*
    Essa é uma biblioteca de áudio usando SDL_mixer.
    Consulte o exemplo "vento.cpp" para mais detalhes sobre como usá-la.
    Os sons devem ser em WAV e as músicas podem ser WAV ou OGG.
    O WAV deve ser 8bit mono 22050Hz
 */

#ifndef AR_H_
#define AR_H_

#include "posicao.h"

/*
 * Inicia essa biblioteca de audio (Serão criados 10 espaços para músicas e 10 para sons)
 */
int AR_soprar();


/*
 * Carrega uma música e a coloca na posição "pos" do vetor de músicas
 */
int AR_carregar_musica( const char nome[20], int pos );


/*
 * Carrega um som e o coloca na posição "pos" do vetor de sons
 */
int AR_carregar_som( const char nome[20], int pos );


/*
 * Começa a tocar a música da posição "pos" do vetor de músicas
 */
int AR_tocar_musica( int pos );


/*
 * Toca o som da posição "pos" do vetor de sons pelo canal passado
 */
int AR_tocar_som( int pos, int canal );


/*
 * Toca o som da posição "pos" do vetor de sons
 * Regula o volume do som de acordo com a distancia entre emissor e receptor
 */
int AR_tocar_som_3D( int pos, int canal, posicao emissor, posicao receptor);

/*
 * Desliga essa biblioteca e libera as músicas e sons carregados
 */
void AR_emudecer();


// Retorna verdadeiro se a música está tocando
int AR_musica_tocando();

// Retorna verdadeiro se a música está pausada
int AR_musica_pausada();

// Continua uma musica pausada
void AR_continuar_musica();

// Pausa uma música
void AR_pausar_musica();

// Para uma música
void AR_parar_musica();

// Desativam/Ativam a saida do som
void AR_desativar();
void AR_ativar();

#endif /*AR_H_*/

