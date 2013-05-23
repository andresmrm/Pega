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


#include "pega.h"

// Retorna o jogador com a ID procurada
jogador* traduzir_id_para_jogador(short int id)
{
	int i = 0;

	while( i < MAX_NUM_JOGADORES )
	{
		if( id == Jogadores[i]->id )
		{
//			cout << i << " I \n";
			return Jogadores[i];
		}

		i++;
	}

	return NULL;
}

// Função do servidor usada para mandar suas saídas pela rede
int gerar_saidas_servidor()
{
//	cout << "OUT - SERVIDOR\n";

	dados_mapa dm;
	pseudoJogador j[MAX_NUM_JOGADORES];

    if( Num_Jogadores > 1 )
    {
    	//Prepara e manda o pacote com os dados de cada jogador
		for( int i=0; i<MAX_NUM_JOGADORES; i++ )
		{
			j[i].igualar( Jogadores[i] );
		}

		if( !( AGUA_mandar_pacote( 1, MAX_NUM_JOGADORES * sizeof(pseudoJogador), &j, -2 ) ) )
		{
			printf("Erro ao tentar mandar pacote em lidar_saidas_servidor\n");
			return 0;
		}

		//Prepara e manda pacote com os dados do mapa
		strcpy(dm.arquivo, NOME_MAPA.c_str());
		dm.tempo_restante = TEMPO_RESTANTE;
		dm.pontos_restantes = NUM_PONTOS_RESTANTES;
		if( !( AGUA_mandar_pacote( 2, sizeof(dados_mapa), &dm, -2 ) ) )
		{
			printf("Erro ao tentar mandar pacote em lidar_saidas_servidor\n");
			return 0;
		}
    }

//    cout << "OUT - SERVIDOR2\n";

	return 1;
}

// Função do servidor que lida com os pacotes recebidos
void lidar_entradas_servidor()
{
//	cout << "IN - SERVIDOR\n";

	int i = 0;
	void *objeto = NULL;
	AGUA_muda_num_clientes *mnc;
	pos_jogador *posjog;
	novo_nome *nn;
	nova_bomba *novbom;
	jogador *jTemp;

	// Vê se mudou numero de jogadores
	if( AGUA_abrir_pacote( 0, AGUA_PRIMEIRA, &objeto ) )
	{
		mnc = (AGUA_muda_num_clientes*)objeto;
		// O pacote informa que há um novo jogador
		if( mnc->evento == 1 )
		{
			i = 0;
			// Procura para um espaco inativo no vetor de jogadores
			while( i < MAX_NUM_JOGADORES-1 && Jogadores[i]->id != -1 )
				i++;

			// Caso achou insere o novo jogador la
			if( Jogadores[i]->id == -1 )
			{
				Num_Jogadores++;
				Jogadores[i]->alterar_nome(mnc->nome);
				Jogadores[i]->pos.igualar( gerar_posicao() );
				Jogadores[i]->id = mnc->id;
			}
			else
			{
				cout << "Jogador tentou entrar, mas não coube!";
			}
		}
		// O pacote informa que saiu um jogador
		else if( mnc->evento == -1 )
		{
			i = 0;
			while( i < MAX_NUM_JOGADORES-1 && Jogadores[i]->id != mnc->id )
				i++;

			if( Jogadores[i]->id == mnc->id )
			{
				Num_Jogadores--;
				Jogadores[i]->apagar_nome();
				Jogadores[i]->pos.igualar(CEMITERIO);
			}
			else
			{
				cout << "Erro ao tentar remover jogador! Nao encontrado!";
			}
		}
		AGUA_remover_pacote();
	}

    // Vê novas pos de jogadores
    while( AGUA_abrir_pacote( 1, AGUA_PRIMEIRA, &objeto ) )
	{
        posjog = (pos_jogador*)(((AGUA_pacote*)objeto)->dado);
        jTemp = traduzir_id_para_jogador( ((AGUA_pacote*)objeto)->id );
//        cout << ((AGUA_pacote*)objeto)->id << " -> " << jTemp->id << "\n";
//        if( jTemp->renascer == 0 )
        	jTemp->pos.igualar(posjog->pos);
        AGUA_remover_pacote();
	}

    // Vê se algum jogador mudou de nome
	while( AGUA_abrir_pacote( 2, AGUA_PRIMEIRA, &objeto ) )
	{
		nn = (novo_nome*)(((AGUA_pacote*)objeto)->dado);
		jTemp = traduzir_id_para_jogador( ((AGUA_pacote*)objeto)->id );
		cout << "Jogador " << jTemp->retornar_nome() << " trocou de nome para " << nn->nome << "\n";
		jTemp->alterar_nome( nn->nome );
		AGUA_remover_pacote();
	}

    // Vê se há novas bombas
    while( AGUA_abrir_pacote( 3, AGUA_PRIMEIRA, &objeto ) )
	{
        novbom = (nova_bomba*)(((AGUA_pacote*)objeto)->dado);
        if( !( AGUA_mandar_pacote( 3, sizeof(nova_bomba), novbom, -2 ) ) ) {
            printf("Erro ao tentar mandar pacote de nova_bomba\n");
            AGUA_remover_pacote();
        }
        new bomba(Jogadores[(int)novbom->dono]->pos,novbom->dono);
        AGUA_remover_pacote();
	}

//    cout << "IN - SERVIDOR2\n";
}

// Função do cliente que lida com os pacotes recebidos
void lidar_entradas_cliente()
{
//	cout << "IN - CLIENTE\n";

	int i;
	void *objeto = NULL;
	pseudoJogador *j = NULL;
	dados_mapa *dm;
	nova_bomba *novbom;
	char *r;

	// Recebe todos os outros jogadores
	if( AGUA_abrir_pacote( 1, AGUA_ULTIMAR, &objeto ) )
	{
		posicao pos;
		pos.igualar(CEMITERIO);

		if( Meu_Num > 0 )
			pos.igualar(Jogadores[Meu_Num]->pos);

		j = (pseudoJogador*)(((AGUA_pacote*)objeto)->dado);
		for( i=0; i<MAX_NUM_JOGADORES; i++ )
		{
//            if( i != Meu_Num )
//            {
                igualarJJ(Jogadores[i],&(j[i]));
//            }

            // Pega o seu "Meu_Num" quando ele for -1 (comeco do jogo)
            if( Meu_Num == -1 && NOME == Jogadores[i]->retornar_nome() )
            {
            	Meu_Num = i;
            }
        }
        AGUA_remover_pacote();

		if( Meu_Num > 0 && pos.z >= 0 )
			Jogadores[Meu_Num]->pos.igualar(pos);
	}

	// Recebe dados do mapa
	if( AGUA_abrir_pacote( 2, AGUA_ULTIMAR, &objeto ) )
	{
		dm = (dados_mapa*)(((AGUA_pacote*)objeto)->dado);
		NOME_MAPA = dm->arquivo;
		TEMPO_RESTANTE = dm->tempo_restante;
		NUM_PONTOS_RESTANTES = dm->pontos_restantes;
		AGUA_remover_pacote();
	}

	// Novas bombas
    while( AGUA_abrir_pacote( 3, AGUA_PRIMEIRA, &objeto ) )
    {
        novbom = (nova_bomba*)(((AGUA_pacote*)objeto)->dado);
        new bomba(Jogadores[(int)novbom->dono]->pos,novbom->dono);
        AGUA_remover_pacote();
    }

    // Recebe os rankings
	while( AGUA_abrir_pacote( 4, AGUA_PRIMEIRA, &objeto ) )
	{
		r = (char*)(((AGUA_pacote*)objeto)->dado);
		RANKS.assign(r);
		AGUA_remover_pacote();
	}

//	cout << "IN - CLIENTE2\n";
}

// Função que manda as saídas do cliente
int gerar_saidas_cliente()
{
//	cout << "OUT - CLIENTE\n";

	pos_jogador posjog;

	//Prepara e manda o pacote com a pos do jogador local
	if( Meu_Num < 0 || Meu_Num >= MAX_NUM_JOGADORES )
	{
        printf("Esse cliente nao sabe um Meu_Num valido em lidar_saidas_cliente\n");
        return 0;
	}
	posjog.pos.igualar(Jogadores[Meu_Num]->pos);
	if( !( AGUA_mandar_pacote( 1, sizeof(pos_jogador), &posjog, -1 ) ) )
	{
        printf("Erro ao tentar mandar pacote em lidar_saidas_clientes\n");
	    return 0;
	}

//	cout << "OUT - CLIENTE2\n";

	return 1;
}

int main(int argc, char *argv[])
{
	AGUA_fluir( 10, AGUA_SEM_CHAT );

	AR_soprar();
	AR_carregar_musica("sons/fim_jogo.ogg",2);
	AR_carregar_musica("sons/floresta.ogg",1);
	AR_carregar_musica("sons/wc2-o.ogg",0);
	AR_carregar_som("sons/xiup.wav",0);
	AR_carregar_som("sons/puoft.wav",1);
	AR_carregar_som("sons/purom.wav",2);
	AR_carregar_som("sons/glum.wav",3);
	AR_carregar_som("sons/novo_mapa.wav",4);
	AR_carregar_som("sons/fim_jogo.wav",5);
//	AR_tocar_musica(0);

	printf(" ____________ \n");
	printf("|            |\n");
	printf("|    PEGA    |\n");
	printf("|____________|\n\n");


	// Caso argumento de terminal diz q eh servidor
	if( argc == 3 && !strcmp(argv[1],"1") )
	{
		cout << "[ Servidor ]\n";
		PapelRede = atoi(argv[1]);
		cout << "Nome: " << argv[2] << "\n";
		strcpy(NOME,argv[2]);
	}
	// Caso argumento de terminal diz q nao usa rede
	else if( argc == 2 && !strcmp(argv[1],"0") )
	{
		cout << "[ SEM REDE ]\n";
		PapelRede = atoi(argv[1]);
		strcpy(NOME,"SemRede");
	}
	// Caso argumento de terminal diz q eh cliente
	else if( argc == 4 && !strcmp(argv[1],"2") )
	{
		cout << "[ Cliente ]" << " - IP do servidor: " << argv[2] <<  "\n";
		PapelRede = atoi(argv[1]);
		strcpy(Ip,argv[2]);
		cout << "Nome: " << argv[3] << "\n";
		strcpy(NOME,argv[3]);
	}
	else
	{
		PapelRede = 0;

		cout << "Pode se chamar pelo terminal como:\n";
		cout << "Sem rede: 0 \n";
		cout << "Servidor: 1 <nome>\n";
		cout << "Cliente: 2 <nome> <ip servidor>\n\n";

		printf("Seu nome de jogador:\n");
		scanf("%s",((char*)&NOME));
		printf("Vc sera o Servidor (1) ou um Cliente (2) ?\n");
		scanf("%d",&PapelRede);
		if( PapelRede == CLIENTE )
		{
			printf("IP SERVIDOR: ");
			scanf("%s",(char*)&Ip);
			//strcpy(ip,"127.0.0.1");
		}
	}

	if( PapelRede == SERVIDOR )
	{
		printf(" ( Iniciando servidor... ) \n");
		AGUA_iniciar_servidor(Porta,NOME);
		Meu_Num = 0;
	}
	else if( PapelRede == CLIENTE )
	{
		printf(" ( Iniciando cliente... ) \n");
		AGUA_iniciar_cliente(Ip,Porta,NOME);
	}

	// Espera o servidor ou cliente iniciar
	while( !AGUA_Rede_Ativa && PapelRede != 0 )
	{
	    SDL_Delay(1000);
	}

	if( PapelRede == 0 ){ PapelRede = SERVIDOR; Meu_Num = 0; } // Para quando for host sem rede

	principal();

	AGUA_secar( 0 );
	AR_emudecer();

	cout << "FIM, THE END, Fin, GAME OVER, GG!\n";

	return 1;
}
