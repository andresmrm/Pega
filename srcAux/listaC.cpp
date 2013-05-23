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

#include "listaC.h"
#include "stdlib.h"
#include "stdio.h"
#include <iostream>

//-----------------------------------------------------------------------------
//LISTA
//-----------------------------------------------------------------------------

int inserir_coluna(tabela *tab, int numero)
{
	coluna *temp = NULL;
	coluna *temp2 = NULL;

	if( !(temp = (coluna*) malloc(sizeof( coluna ))) ){printf("\nErro na alocagem da coluna\n");return 0;}

	if( tab->inicio == NULL )
	{
		tab->inicio = temp;
	}
	else
	{
		temp2 = tab->inicio;
		while( temp2->proxima != NULL )
		{
			temp2 = temp2->proxima;
		}
		temp2->proxima = temp;
	}

	temp->proxima = NULL;
	temp->linhas = NULL;
	temp->numero = numero;
	tab->num_colunas = tab->num_colunas + 1;

	return 1;
}

int inserir_linha(tabela *tab, int numero, unsigned short int tama, void *objeto)
{
	coluna *col = NULL;
	linhaC *alocada = NULL;
	linhaC *ultima = NULL;

	//Procura coluna
	col = tab->inicio;

	while( col != NULL && col->numero != numero )
	{
		col = col->proxima;
	}
	if( col != NULL && col->numero == numero )
	{
		if( !(alocada = (linhaC*) malloc(sizeof( linhaC ))) ){printf("\nErro na alocagem da linha\n");return 0;}
	}
	else
	{
		printf("\nErro na busca da coluna\n");
		return( 0 );
	}
	//----

	//Insere linha
	if( col->linhas == NULL )
	{
		col->linhas = alocada;
		alocada->anterior = NULL;
	}
	else
	{
		ultima = col->linhas;
		while( ultima->proxima != NULL )
		{
			ultima = ultima->proxima;
		}
		ultima->proxima = alocada;
		alocada->anterior = ultima;
	}

	alocada->objeto = objeto;
	alocada->tamanho = tama;
	alocada->proxima = NULL;
	tab->num_linhas++;

	return 1;
}

int remover_linha(tabela *tab, int numero, linhaC *lin)
{
	coluna *col = NULL;

	if( lin->anterior != NULL )
	{
		lin->anterior->proxima = lin->proxima;
	}
	//Caso a linha seja a primeira da coluna
	//Procura coluna
	else
	{
		col = tab->inicio;

		while( col != NULL && col->numero != numero )
		{
			col = col->proxima;
		}

		if( col != NULL && col->numero == numero )
		{
			col->linhas = lin->proxima;
		}
		else
		{
			return 0;
		}
	}
	//Remove linha
	if( lin->proxima != NULL )
	{
		lin->proxima->anterior = lin->anterior;
	}

	free( lin->objeto ); // FAZENDO ASSIM ELE SABE O TAMANHO DO Q TEM Q LIBERAR?
	free( lin );
	tab->num_linhas--;

	return 1;
}

int remover_linha(tabela *tab, int numero, void* objeto)
{
	coluna *col = NULL;
	linhaC *lin = NULL;

    col = tab->inicio;

    while( col != NULL && col->numero != numero )
    {
        col = col->proxima;
    }

    if( col != NULL && col->numero == numero )
    {
        lin = col->linhas;
    }
    else
    {
        return 0;
    }

	// Procura a linha com o objeto desejado
	while( lin->objeto != objeto && lin != NULL )
	{
		lin = lin->proxima;
	}

	// Não encontrou a linha
	if( lin == NULL )
	{
		printf("\nErro ao tentar procurar por objeto na lista para remover\n");
		return 0;
	}

	// Remove da anterior caso ela não seja a primeira
	if( lin->anterior != NULL )
	{
		lin->anterior->proxima = lin->proxima;
	}
	// Caso a linha seja a primeira da coluna
	else
	{
		col->linhas = lin->proxima;
	}

	// Remove da seguinte caso exista
	if( lin->proxima != NULL )
	{
		lin->proxima->anterior = lin->anterior;
	}

//	free( lin->objeto ); // FAZENDO ASSIM ELE SABE O TAMANHO DO Q TEM Q LIBERAR?
	free( lin );
	tab->num_linhas--;

	return 1;
}

int iniciar_tabela(tabela *tab, int colunas)
{
	int i = 0;

	tab->inicio = NULL;
	tab->num_colunas = 0;
	tab->num_linhas = 0;

	for(i=0;i<colunas;i++)
	{
		inserir_coluna(tab, i);
	}

	return 1;
}

int deletar_tabela(tabela *tab)
{
    int i;

    for( i=tab->num_colunas-1; i >= 0; i-- )
    {
        deletar_coluna(tab, i);
    }

    return 1;
}

int deletar_coluna(tabela *tab, int numero)
{
	coluna *temp = NULL;
	coluna *tempAnterior = NULL;
	linhaC *atual = NULL;
	linhaC *temp2 = NULL;

	temp = tab->inicio;

	// ajusta a tabela quando a coluna a ser removida for a primeira
	if( temp != NULL && temp->numero == numero )
	{
		tab->inicio = temp->proxima;
	}

	while( temp != NULL && temp->numero != numero )
	{
		tempAnterior = temp;
		temp = temp->proxima;
	}

	if( temp != NULL && temp->numero == numero )
	{
		atual = temp->linhas;
		if( tempAnterior != NULL )
		{
			tempAnterior->proxima = temp->proxima;
		}
	}
	else
	{
		return 0;
	}

	if( temp->linhas != NULL )
	{
		do
		{
			//atual->objeto <----- E o objeto? Quem remove? Chamar delete? Delete dele remove linha tb... E nao saber qual eh o delete dele, pois ele eh void
			temp2 = atual;
			atual = atual->proxima;
			free(temp2);
		}
		while( atual != NULL );
	}

	free(temp);

	return 1;
}

// Retorna o tamanho do objeto, ou 0 caso coluna vazia
void* ultima_linha(tabela *tab, int numero, void **objeto) // <-------------- PODE SER ACELERADA COLOCANDO NA ESTRUTURA UM PONTEIRO PARA ULTIMA LINHA
{
	coluna *col = NULL;
	linhaC *ultima = NULL;

	// Procura coluna
	col = tab->inicio;

	while( col != NULL && col->numero != numero )
	{
		col = col->proxima;
	}
	if( col == NULL && col->numero != numero )
	{
		printf("\nErro na busca da coluna\n");
		return NULL;
	}
	if( col->linhas == NULL )
	{
		return NULL; // coluna vazia
	}
	else
	{
		ultima = col->linhas;
		while( ultima->proxima != NULL )
		{
			ultima = ultima->proxima;
		}
		*objeto = ultima->objeto;

		return ultima;
	}
}

// Retorna o tamanho do objeto, ou 0 caso coluna vazia (e remove a linha achada)
void* ultima_linhaR(tabela *tab, int numero, void **objeto)
{
	coluna *col = NULL;
	linhaC *ultima = NULL;

	// Procura coluna
	col = tab->inicio;

	while( col != NULL && col->numero != numero )
	{
		col = col->proxima;
	}
	if( col == NULL && col->numero != numero )
	{
		printf("\nErro na busca da coluna\n");
		return NULL;
	}
	if( col->linhas == NULL )
	{
		return NULL; // coluna vazia
	}
	else
	{
		ultima = col->linhas;
		while( ultima->proxima != NULL )
		{
			ultima = ultima->proxima;
			remover_linha(tab, numero, ultima->anterior);
		}
		*objeto = ultima->objeto;

		return ultima;
	}
}

// Retorna a linha do objeto, ou 0 caso coluna vazia
void* primeira_linha(tabela *tab, int numero, void **objeto) // <-------------- PODE SER ACELERADA COLOCANDO NA ESTRUTURA UM PONTEIRO PARA ULTIMA LINHA
{
	coluna *col = NULL;
	linhaC *primeira = NULL;

    *objeto = NULL;

	// Procura coluna
	col = tab->inicio;

	while( col != NULL && col->numero != numero )
	{
		col = col->proxima;
	}
	if( col == NULL && col->numero != numero )
	{
		printf("\nErro na busca da coluna\n");
		return NULL;
	}
	if( col->linhas == NULL )
	{
		return NULL; // coluna vazia
	}
	else
	{
		primeira = col->linhas;
		*objeto = primeira->objeto;

		return primeira;
	}
}

void* retornar_objeto(tabela *tab, int numero, void **atual)        //<----------- Em caso de baixo FPS jun
{
    // Caso recebeu atual == NULL, pegar a primeira da coluna
	if( *atual == NULL )
	{
	    coluna *col = tab->inicio;

	    while( col != NULL && col->numero != numero )
        {
            col = col->proxima;
        }
        if( col == NULL || col->numero != numero )
        {
            printf("\nErro na busca da coluna\n");
            return NULL;
        }
        else if( col->linhas == NULL )
        {
            return NULL; // coluna vazia
        }
        else
        {
            *atual = col->linhas;
        }
	}
	else
	{
		*atual = ((linhaC*)*atual)->proxima;
	}

	if( *atual != NULL )
	{
		return ((linhaC*)*atual)->objeto;
	}
	else
	{
		return NULL;
	}
}
