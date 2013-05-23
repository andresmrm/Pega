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

#ifndef LISTAC_H_
#define LISTAC_H_

//-----------------------------------------------------------------------------
//ESTRUTURA DA LISTA
//-----------------------------------------------------------------------------
typedef struct linhaC2{
	struct linhaC2 *proxima;
	struct linhaC2 *anterior;
	unsigned short int tamanho;
	void *objeto;
}linhaC;

typedef struct coluna2{
	int numero;
	struct coluna2 *proxima;
	linhaC* linhas;
}coluna;

typedef struct{
	int num_colunas;
	int num_linhas;
	coluna *inicio;
}tabela;

int iniciar_tabela(tabela *tab, int colunas);
int deletar_tabela(tabela *tab);
int inserir_coluna(tabela *tab, int numero);
int deletar_coluna(tabela *tab, int numero);
int remover_linha(tabela *tab, int numero, linhaC *lin);
int remover_linha(tabela *tab, int numero, void* objeto);
int inserir_linha(tabela *tab, int numero, unsigned short int tama, void *objeto);
void* ultima_linha(tabela *tab, int numero, void **objeto);
void* ultima_linhaR(tabela *tab, int numero, void **objeto); // Retorna a última linha removendo as outras no processo
void* primeira_linha(tabela *tab, int numero, void **objeto);
void* retornar_objeto(tabela *tab, int numero, void **atual);

#endif /*LISTAC_H_*/
