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

#include <Python.h>
#include "pega.h"
#include <stdlib.h>


void carregar_objetos()
{
	FOGO_carregar_objeto3D("bomba.obj",1,oBOMBA);
	FOGO_carregar_objeto3D("garrafa.obj",1,oCERVEJA);
	FOGO_carregar_objeto3D("livro.obj",1,oLIVRO);
	FOGO_carregar_objeto3D("comp.obj",1.5,oCOMP);
	FOGO_carregar_objeto3D("virus.obj",3,oVIRUS);
	FOGO_carregar_objeto3D("cafe.obj",2,oCAFE);
}

void carregar_texturas()
{
	FOGO_carregar_textura("texturas/drag.tga",1,0);

	if(0){
//		FOGO_carregar_textura("texturas/parede.tga",1,1);
//		FOGO_carregar_textura("texturas/chao.tga",1,2);
		FOGO_carregar_textura("texturas/jogador1.tga",0,4);
		FOGO_carregar_textura("texturas/jogador2.tga",0,5);
		FOGO_carregar_textura("texturas/jogador3.tga",0,6);
		FOGO_carregar_textura("texturas/jogador4.tga",0,7);
	} else {
//		FOGO_carregar_textura("texturas/parede4.tga",1,1);
//		FOGO_carregar_textura("texturas/chao4.tga",1,2);
		FOGO_carregar_textura("texturas/jogador12.tga",0,4);
		FOGO_carregar_textura("texturas/jogador22.tga",0,5);
		FOGO_carregar_textura("texturas/jogador32.tga",0,6);
		FOGO_carregar_textura("texturas/jogador42.tga",0,7);
	}

	FOGO_carregar_textura("texturas/fundoRanks.tga",0,3);
	FOGO_carregar_textura("texturas/ceu2.tga",1,8);
	FOGO_carregar_textura("texturas/painelRanks.tga",0,9);
	FOGO_carregar_textura("texturas/painel.tga",0,10);
	FOGO_carregar_textura("texturas/ranks.tga",0,11);
	FOGO_carregar_textura("texturas/painelPre.tga",0,12);
	FOGO_carregar_textura("texturas/especial.tga",0,13);
	FOGO_carregar_textura("texturas/fundo.tga",0,14);
	FOGO_carregar_textura("texturas/fundoCeu.tga",0,15);
	FOGO_carregar_textura("texturas/miniVazio.tga",0,16);
	FOGO_carregar_textura("texturas/miniCheio.tga",0,17);
	FOGO_carregar_textura("texturas/miniVc.tga",0,18);
	FOGO_carregar_textura("texturas/painelDemo.tga",0,19);
}

void carregar_fontes()
{
	float t = 7.8*float(TELA_COMPRIMENTO)/float(800);

	FONTES[0].endereco = "fontes/CRYSRG__.TTF";
//	FONTES[0].endereco = "fontes/DejaVuSansMono.ttf";
	FONTES[0].tamanho = 15*float(TELA_COMPRIMENTO)/float(800);
	FONTES[0].tamaX = FONTES[0].tamanho/t;
	FONTES[0].tamaY = FONTES[0].tamanho*2/t;

	FONTES[1].endereco = "fontes/CRYSRG__.TTF";
	FONTES[1].tamanho = 60*float(TELA_COMPRIMENTO)/float(800);
	FONTES[1].tamaX = FONTES[1].tamanho/t;
	FONTES[1].tamaY = FONTES[1].tamanho*2/t;

	FONTES[2].endereco = "fontes/CRYSRG__.TTF";
	FONTES[2].tamanho = 30*float(TELA_COMPRIMENTO)/float(800);
	FONTES[2].tamaX = FONTES[2].tamanho/t;
	FONTES[2].tamaY = FONTES[2].tamanho*2/t;

//	FOGO_carregar_fonte("fontes/FixedMiriamTransparent.ttf",30,1);
	FOGO_carregar_fonte(FONTES[0].endereco.c_str(),FONTES[0].tamanho,0);
	FOGO_carregar_fonte(FONTES[1].endereco.c_str(),FONTES[1].tamanho,1);
	FOGO_carregar_fonte(FONTES[2].endereco.c_str(),FONTES[2].tamanho,2);
//	FOGO_carregar_fonte("fontes/FixedMiriamTransparent.ttf",10,2);
}

int ler_lista_mapas(const char* nomeArquivo, string mapas[])
{
	char temp[100];
	int num = 0;
	ifstream arquivo(nomeArquivo);

	cout << "\nLendo arquivo com lista de mapas\n";

	if( arquivo.is_open() )
	{
		while( !arquivo.eof() )
		{
			arquivo.getline(temp,100);
			if( temp[0] != '\n' && temp[0] != '\0' )
			{
				mapas[num].append(temp);
				num++;
			}
		}
	}
	else
	{
		cout << "Erro ao tentar abrir arquivo de lista de mapas\n";
	}

	arquivo.close();

	return num;
}

string rankear()
{
	char temp[200];
	string listaJogadores;
	string chamada;
	string retorno;

	// Pega a hora atual do sistema
	time_t rawtime;
	struct tm * timeinfo;
	string hora;
	time( &rawtime );
	timeinfo = localtime( &rawtime );
	hora = asctime(timeinfo);
	hora.erase(hora.size()-1);

	for(int i=0; i < MAX_NUM_JOGADORES; i++)
	{
		if( Jogadores[i]->id >= 0 )
		{
			listaJogadores.append( hora );
			listaJogadores.append( "\t" );
			listaJogadores.append( Jogadores[i]->retornar_nome() );
			listaJogadores.append( "\t" );
			sprintf(temp,"%d",Jogadores[i]->pontos);
			listaJogadores.append( temp );
			listaJogadores.append( " " );
			sprintf(temp,"%d",Jogadores[i]->tiros_dados);
			listaJogadores.append( temp );
			listaJogadores.append( " " );
			sprintf(temp,"%d",Jogadores[i]->acertos);
			listaJogadores.append( temp );
			listaJogadores.append( "\n" );
		}
	}

//	cout << listaJogadores;

	PyObject * module;
	PyObject * function;
	PyObject * result;

	Py_Initialize();

	PySys_SetPath("rank");

	module = PyImport_ImportModule("rankeador");
	if(module)
	{
		if (PyObject_HasAttrString(module, "rankear"))
		{
			function = PyObject_GetAttrString(module, "rankear");

			if (PyCallable_Check(function))
			{
				result = PyObject_CallFunction(function, "s", listaJogadores.c_str());

				retorno.assign( PyString_AsString(PyObject_Str(result)) );
			}
			else
			{
				printf("Error -- attribute is not a function/method\n");
			}
		}
		else
		{
			printf("Error -- Module does not contain \"rankear\" function.\n");
		}
	}
	else
	{
		cout << "Modulo rankeador nao encontrado!";
	}

//	PyRun_SimpleString("print 'Rankeado!'");

	Py_Finalize();

	return retorno;
}

int mostrar_ranks( string ranks )
{
	int i;
	int temp;
	char temp3[100];
	string temp2;
	string maisParts[10][3];
	string rankPart;
	string maisJogs[10][3];
	string ranksJogs[MAX_NUM_JOGADORES];

	SDL_Event evento;
	int var = EM_LOOP;

	AR_tocar_musica(2);

	// Recolhe da string as 10 partidas com mais pontos
	for(i=0; i<10; i++)
	{
		temp = ranks.find_first_of("\n");
		temp2 = ranks.substr(0,temp);
		ranks.erase(0,temp+1);

		// separa em cada campo de dados
		temp = temp2.find_first_of('\t');
		maisParts[i][0] = temp2.substr(0,temp);
		temp2.erase(0,temp+1);
		temp = temp2.find_first_of('\t');
		maisParts[i][1] = temp2.substr(0,temp);
		temp2.erase(0,temp+1);
		temp = temp2.find_first_of(' ');
		maisParts[i][2] = temp2.substr(0,temp);
	}

	// Recolhe da string o rank dessa partida
	temp = ranks.find_first_of("\n");
	rankPart.assign( ranks.substr(0,temp) );
	ranks.erase(0,temp+1);

	// Recolhe da string os 10 jogadores com mais pontos
	for(i=0; i<10; i++)
	{
		temp = ranks.find_first_of("\n");
		temp2 = ranks.substr(0,temp);
		ranks.erase(0,temp+1);

		// separa em cada campo de dados
		temp = temp2.find_first_of('\t');
		maisJogs[i][0] = temp2.substr(0,temp);
		temp2.erase(0,temp+1);
		temp = temp2.find_first_of('\t');
		maisJogs[i][1] = temp2.substr(0,temp);
		temp2.erase(0,temp+1);
		temp = temp2.find_first_of(' ');
		maisJogs[i][2] = temp2.substr(0,temp);
	}

	// Recolhe da string o rank dos jogadores dessa partida
	i = 0;
	while( ranks.length() > 0 )
	{
		temp = ranks.find_first_of("\n");
		ranksJogs[i].assign( ranks.substr(0,temp) );
		ranks.erase(0,temp+1);
		i++;
	}

	FOGO_desenhar_listas_inicio();
	FOGO_desenhar_2D(3,0,0,100,100);
	FOGO_desenhar_2D(9,0,0,100,100);
	temp2 = "JOGADORES COM MAIS PONTOS:";
	FOGO_escrever(centralizar_texto(temp2.size(),0,2),85,temp2,2);

	temp2 = "Rank";
	FOGO_escrever(centralizar_texto(temp2.size(),-65,0),75,temp2.c_str(),0);
	temp2 = "Data";
	FOGO_escrever(centralizar_texto(temp2.size(),-30,0),75,temp2.c_str(),0);
	temp2 = "Nome";
	FOGO_escrever(centralizar_texto(temp2.size(),30,0),75,temp2.c_str(),0);
	temp2 = "Pontos";
	FOGO_escrever(centralizar_texto(temp2.size(),65,0),75,temp2.c_str(),0);

	for(i=0; i<10; i++)
	{
		sprintf(temp3,"%d",i+1);
		temp2 = temp3;
		FOGO_escrever(centralizar_texto(temp2.size(),-65,0),65-i*FONTES[2].tamaY,temp2.c_str(),0);
		FOGO_escrever(centralizar_texto(maisJogs[i][0].size(),-30,0),65-i*FONTES[2].tamaY,maisJogs[i][0].c_str(),0);
		FOGO_escrever(centralizar_texto(maisJogs[i][1].size(),30,0),65-i*FONTES[2].tamaY,maisJogs[i][1].c_str(),0);
		FOGO_escrever(centralizar_texto(maisJogs[i][2].size(),65,0),65-i*FONTES[2].tamaY,maisJogs[i][2].c_str(),0);
	}
	temp2 = "RANK DOS JOGADORES ATUAIS:";
	FOGO_escrever(centralizar_texto(temp2.size(),0,2),-35,temp2,2);

	temp2 = "Nome";
	FOGO_escrever(centralizar_texto(temp2.size(),-50,0),-45,temp2.c_str(),0);
	temp2 = "Pontos";
	FOGO_escrever(centralizar_texto(temp2.size(),0,0),-45,temp2.c_str(),0);
	temp2 = "Rank";
	FOGO_escrever(centralizar_texto(temp2.size(),50,0),-45,temp2.c_str(),0);

	for(i=0; i<MAX_NUM_JOGADORES; i++)
	{
		if( ranksJogs[i].length() > 0 )
		{
			temp2 = Jogadores[i]->retornar_nome();
			FOGO_escrever(centralizar_texto(temp2.size(),-50,0),-55-i*FONTES[2].tamaY,temp2.c_str(),0);
			sprintf(temp3,"%d",Jogadores[i]->pontos);
			temp2 = temp3;
			FOGO_escrever(centralizar_texto(temp2.size(),0,0),-55-i*FONTES[2].tamaY,temp2.c_str(),0);
			FOGO_escrever(centralizar_texto(ranksJogs[i].size(),50,0),-55-i*FONTES[2].tamaY,ranksJogs[i].c_str(),0);
		}
	}
	FOGO_desenhar_listas_fim();

	while( var == EM_LOOP )
	{
		while( SDL_PollEvent( &evento ) )
		{
			if( evento.type == SDL_KEYDOWN )
			{
				if( evento.type == SDL_QUIT )
				{
					var = SAIR;
				}
				else if( evento.key.keysym.sym == SDLK_ESCAPE )
				{
					var = SAIR;
				}
				else if( evento.key.keysym.sym == SDLK_RETURN )
				{
					var = CONTINUAR;
				}
			}
		}
		SDL_Delay(100);
	}

	if( var != SAIR )
	{
		FOGO_desenhar_listas_inicio();
		FOGO_desenhar_2D(3,0,0,100,100);
		FOGO_desenhar_2D(9,0,0,100,100);
		temp2 = "PARTIDAS COM MAIS PONTOS:";
		FOGO_escrever(centralizar_texto(temp2.size(),0,2),85,temp2,2);

		temp2 = "Rank";
		FOGO_escrever(centralizar_texto(temp2.size(),-65,0),75,temp2.c_str(),0);
		temp2 = "Data";
		FOGO_escrever(centralizar_texto(temp2.size(),-35,0),75,temp2.c_str(),0);
		temp2 = "Nomes";
		FOGO_escrever(centralizar_texto(temp2.size(),25,0),75,temp2.c_str(),0);
		temp2 = "Pontos";
		FOGO_escrever(centralizar_texto(temp2.size(),65,0),75,temp2.c_str(),0);

		for(i=0; i<10; i++)
		{
			sprintf(temp3,"%d",i+1);
			temp2 = temp3;
			FOGO_escrever(centralizar_texto(temp2.size(),-65,0),65-i*FONTES[2].tamaY,temp2.c_str(),0);
			FOGO_escrever(centralizar_texto(maisParts[i][0].size(),-35,0),65-i*FONTES[2].tamaY,maisParts[i][0].c_str(),0);
			FOGO_escrever(centralizar_texto(maisParts[i][1].size(),25,0),65-i*FONTES[2].tamaY,maisParts[i][1].c_str(),0);
			FOGO_escrever(centralizar_texto(maisParts[i][2].size(),65,0),65-i*FONTES[2].tamaY,maisParts[i][2].c_str(),0);
		}
		temp2 = "RANK DESSA PARTIDA:";
		FOGO_escrever(centralizar_texto(temp2.size(),0,2),-40,temp2,2);
		FOGO_escrever(centralizar_texto(rankPart.size(),0,2),-60,rankPart.c_str(),2);
		FOGO_desenhar_listas_fim();

		var = EM_LOOP;
		while( var == EM_LOOP )
		{
			while( SDL_PollEvent( &evento ) )
			{
				if( evento.type == SDL_KEYDOWN )
				{
					if( evento.type == SDL_QUIT )
					{
						var = SAIR;
					}
					else if( evento.key.keysym.sym == SDLK_ESCAPE )
					{
						var = SAIR;
					}
					else if( evento.key.keysym.sym == SDLK_RETURN )
					{
						var = CONTINUAR;
					}
				}
			}
			SDL_Delay(100);
		}
	}

	return var;
}

int jogo( string mapas[], unsigned short int num_mapas )
{
	int var = EM_LOOP;
	int mapa_atual = rand()%num_mapas-1;
	string ranks;

	TEMPO_RESTANTE = TEMPO_RESTANTE_INICIAL;

	// Controla a ordem dos mapas quando eh servidor
	if( PapelRede == SERVIDOR )
	{
		do
		{
			//mapa_atual++;
			//if( mapa_atual == num_mapas )
			//{
			//	mapa_atual = 0;
			//}
			mapa_atual = rand()%num_mapas;
			NOME_MAPA = mapas[mapa_atual];

			var = rodar_mapa(mapas[mapa_atual]);
		}
		while( var == EM_LOOP );
	}
	// Fica esperando do servidor qual sera o mapa a se jogar
	else
	{
		do
		{
			cout << "Aguardando resposta do servidor\n";
			while( Meu_Num == -1 || NOME_MAPA == "" )
			{
				SDL_Delay(1000);
				lidar_entradas_cliente();
			}
			cout << "Iniciando jogo\n";

			var = rodar_mapa(NOME_MAPA);
		}
		while( var == EM_LOOP );
	}

	cout << "O jogo acabou!\n\n";

	AR_tocar_som(5,Meu_Num);

	cout << "Salvando pontos\n";

	if( PapelRede == SERVIDOR )
	{
		ranks = rankear();
		AGUA_mandar_pacote( 4, (1+ranks.length())*sizeof(char), (void*)ranks.c_str(), -2 );
	}
	else
	{
		while( RANKS == "" )
		{
			SDL_Delay(1000);
			lidar_entradas_cliente();
		}
		ranks.assign(RANKS);
		RANKS = "";
	}

	if( var != SAIR )
	{
		var = mostrar_ranks(ranks);
	}

	posicao pos;
	pos.igualar(CEMITERIO);
	for(int i=0; i < MAX_NUM_JOGADORES; i++)
	{
//		Jogadores[i]->resetar();
		jogador *temp = Jogadores[i];
		Jogadores[i] = new jogador(pos,10,i+4,1);
		Jogadores[i]->id = temp->id;
//		Jogadores[i]->alterar_nome(temp->retornar_nome());

		delete temp;
	}

	return var;
}

int demonstracao()
{
	int i;

	string creditos[100], manual[50];
	SDL_Event evento;
	int var = EM_LOOP;
	string andres = "Andres Mantecon Ribeiro Martano";
	string prandi = "Rapael Prandi";
	string murillo = "Murillo Redher Batista";
	string lin = "Matheus Lin Truglio Alvarenga";

	AR_tocar_musica(0);

	i = 0;
	creditos[i] = "FoG"; i++;
	creditos[i] = ""; i++;
	creditos[i] = "Programacao Principal"; i++;
	creditos[i] = andres; i++;
	creditos[i] = ""; i++;
	creditos[i] = "Inteligencia Artificial"; i++;
	creditos[i] = lin; i++;
	creditos[i] = ""; i++;
	creditos[i] = "Modelagem 3D"; i++;
	creditos[i] = andres; i++;
	creditos[i] = ""; i++;
	creditos[i] = "Montagem de Mapas"; i++;
	creditos[i] = andres; i++;
	creditos[i] = murillo; i++;
	creditos[i] = prandi; i++;
	creditos[i] = ""; i++;
	creditos[i] = "Escolha de Texturas"; i++;
	creditos[i] = andres; i++;
	creditos[i] = murillo; i++;
	creditos[i] = prandi; i++;
	creditos[i] = ""; i++;
	creditos[i] = "Interface"; i++;
	creditos[i] = andres; i++;
	creditos[i] = ""; i++;
	creditos[i] = "Betatest"; i++;
	creditos[i] = andres; i++;
	creditos[i] = prandi; i++;
	creditos[i] = ""; i++;
	creditos[i] = ""; i++;
	creditos[i] = "Terceiros"; i++;
	creditos[i] = ""; i++;
	creditos[i] = "Loader OBJ"; i++;
	creditos[i] = "Karl Berg"; i++;
	creditos[i] = ""; i++;
	creditos[i] = "Crystal Font"; i++;
	creditos[i] = "Jerry Fitzpatrick"; i++;
	creditos[i] = ""; i++;
	creditos[i] = "SDL"; i++;
	creditos[i] = "OpenGL"; i++;

	i = 0;
	manual[i] = "Como jogar:"; i++;
	manual[i] = "Navegue pelo labirinto coletando os livros e"; i++;
	manual[i] = "computadores para ganhar pontos. Quem fizer"; i++;
	manual[i] = "mais pontos, ate o fim do tempo, ganha."; i++;
	manual[i] = "Vc eh um personagem em duas dimensoes, entao"; i++;
	manual[i] = "as vezes vc pode driblar as coisas virando"; i++;
	manual[i] = "de lado."; i++;
	manual[i] = ""; i++;
	manual[i] = "Itens:"; i++;
	manual[i] = "Livro: 1 ponto"; i++;
	manual[i] = "Computador: 5 pontos"; i++;
	manual[i] = "Cafe: dobra a velocidade"; i++;
	manual[i] = "Cerveja: deixa 'zonzo'"; i++;
	manual[i] = "Virus: imortal, fuja dele"; i++;
	manual[i] = ""; i++;
	manual[i] = "Comandos:"; i++;
	manual[i] = "Mouse e direcionais ou ASWD"; i++;
	manual[i] = "Botao do mouse atira"; i++;
	manual[i] = ""; i++;

	int y = 40;
	int contador = 0;
	string temp;
	cout << "Tela de apresentacao\n";
	while( var == EM_LOOP )
	{
		FOGO_desenhar_listas_inicio();
		FOGO_desenhar_2D(14,0,0,100,100);
		FOGO_desenhar_2D(19,0,0,100,100);

		temp = "PEGA";
		FOGO_escrever( centralizar_texto(temp.size(),0,1) ,75,temp.c_str(),1);

		//Creditos
		for(i=0; i<100; i++)
			if( y-i*FONTES[0].tamaY*1.5 < 50 && y-i*FONTES[0].tamaY*1.5 > -90 )
				FOGO_escrever( centralizar_texto(creditos[i].size(),50,0), y-i*FONTES[0].tamaY*1.5, creditos[i].c_str(), 0 );
		contador++;
		if(contador == 5 ){y++;contador=0;}
		if( y>250 )y = -90;

		temp = "Tecle F2 para entrar";
		FOGO_escrever( centralizar_texto(temp.size(),-50,2),40,temp.c_str(),2);

		// Manual
		for(i=0; i<50; i++)
			FOGO_escrever( centralizar_texto(manual[i].size(),-50,0), 30-i*FONTES[0].tamaY*1.5, manual[i].c_str(), 0 );

		temp = "Tecle ESC para sair";
		FOGO_escrever( centralizar_texto(temp.size(),-50,2),-85,temp.c_str(),2);

		FOGO_desenhar_listas_fim();

		while( SDL_PollEvent( &evento ) )
		{
			if( evento.type == SDL_QUIT )
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
					var = CONTINUAR;
				}
			}
		}
		SDL_Delay(100);
	}

	return var;
}

void verificar_esperas(bool esperando[])
{
	void *objeto;
	espera_ponto *ep;

	while( AGUA_abrir_pacote( 5, AGUA_PRIMEIRA, &objeto ) )
	{
		ep = (espera_ponto*)(((AGUA_pacote*)objeto)->dado);
//		jTemp = traduzir_id_para_jogador( ((AGUA_pacote*)objeto)->id );
//        cout << ((AGUA_pacote*)objeto)->id << " -> " << jTemp->id << "\n";
		esperando[((AGUA_pacote*)objeto)->id] = ep->pronto;
		AGUA_remover_pacote();
	}
}

int mandar_sinal_pronto()
{
	espera_ponto ep;
	ep.pronto = true;
	int alvo;

	// Regula para mandar ao servidor ou a todos os clientes
	if( PapelRede == SERVIDOR )
		alvo = -2;
	else
		alvo = -1;

	cout << "Enviando\n";

	//Prepara e manda pacote com sinal de pronto
	if( !( AGUA_mandar_pacote( 5, sizeof(espera_ponto), &ep, alvo ) ) ) {
		printf("Erro ao tentar mandar pacote em mandar_sinal_pronto\n");
		return 0;
	}

	cout << "Enviado\n";

	return 1;
}

int preJogo()
{
	SDL_Event evento;
	int var = EM_LOOP;
	int index = 0;
	string nome, temp;
	char temp2[50];
	bool lendo_texto = true;
	bool esperando[MAX_NUM_JOGADORES];
	int num_jogs = 2;

//	AR_tocar_musica(0);

	// Prepara o vetor q diz quem ja esta pronto para jogar
	for( int i=0; i < MAX_NUM_JOGADORES; i++ )
	{
		esperando[i] = false;
	}

	// Ajusta o id do Servidor
	if( Meu_Num == 0 )
	{
		Jogadores[Meu_Num]->id = 0;
	}

	// Aguarda o Num vindo do servidor
	while( Meu_Num == -1 )
	{
		cout << "Aguardando um num do servidor\n";
		lidar_entradas_cliente();
		SDL_Delay(1000);
	}

	nome = "";
	cout << "Tela de espera para jogo\n";
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
//			var = receber_comandos();
			if( var == TROCAR_NOME )
			{
				var = EM_LOOP;
				lendo_texto = true;
				nome = Jogadores[Meu_Num]->retornar_nome();
			}
		}

		FOGO_desenhar_listas_inicio();
		FOGO_desenhar_2D(14,0,0,100,100);
		FOGO_desenhar_2D(12,0,0,100,100);

		if(lendo_texto)
			interface_receber_texto( nome, index );
		else
		{
			temp = "Seu nome:";
			FOGO_escrever( centralizar_texto(temp.size(),0,2), 10, temp.c_str(), 2 );
			temp = Jogadores[Meu_Num]->retornar_nome();
			FOGO_escrever( centralizar_texto(temp.size(),0,1), -12, temp.c_str(), 1 );
			temp = "Tecle F3 para trocar de nome. Vc pode trocar de nome durante o jogo tb.";
			FOGO_escrever( centralizar_texto(temp.size(),0,0) ,-30, temp.c_str(),0);

			if( !esperando[Meu_Num] )
			{
				temp = "(Tecle F2 para continuar)";
				FOGO_escrever( centralizar_texto(temp.size(),0,2), 80, temp.c_str(), 2 );
			}
			else
			{
				temp = "(Aguardando outros jogadores)";
				FOGO_escrever( centralizar_texto(temp.size(),0,2), 80, temp.c_str(), 2 );
			}

			temp = "Numero de jogadores: ";
			sprintf(temp2,"%d",num_jogs);
			temp += temp2;
			FOGO_escrever( centralizar_texto(temp.size(),0,2), 65, temp.c_str(), 2 );
			temp = "(Tecle '1' ou '2' para mudar)";
			FOGO_escrever( centralizar_texto(temp.size(),0,0), 55, temp.c_str(), 0 );

			temp = "ESC para sair";
			FOGO_escrever( centralizar_texto(temp.size(),0,0), 45, temp.c_str(), 0 );
		}

		FOGO_desenhar_listas_fim();

		// Servidor
		if( PapelRede == SERVIDOR )
		{
			lidar_entradas_servidor();
			gerar_saidas_servidor();
		}

		if( !lendo_texto )
		{
			while( SDL_PollEvent( &evento ) )
			{
				if( evento.type == SDL_QUIT )
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
						if( num_jogs == 1 )
							var = CONTINUAR;
						else
						{
							esperando[Meu_Num] = true;
							if( PapelRede == CLIENTE )
								mandar_sinal_pronto();
						}
					}
					else if( evento.key.keysym.sym == SDLK_F3 )
					{
						lendo_texto = true;
					}
					else if( evento.key.keysym.sym == SDLK_1 )
					{
						num_jogs = 1;
						if( esperando[Meu_Num] )
							var = CONTINUAR;
					}
					else if( evento.key.keysym.sym == SDLK_2 )
					{
						num_jogs = 2;
					}
				}
			}

			if( num_jogs > 1 )
			{
				verificar_esperas(esperando);

				// Se cliente
				if( PapelRede == CLIENTE )
				{
					if( esperando[0] == true )
					{
						var = CONTINUAR;
					}
				}
				// Se servidor
				else if( PapelRede == SERVIDOR )
				{
					bool todos_prontos = true;
					int prontos = 0;
					for(int i=0; i < MAX_NUM_JOGADORES; i++)
					{
						if( Jogadores[i]->id >= 0 && esperando[i] == false )
						{
							todos_prontos = false;
						}
						else if( esperando[i] == true )
						{
							prontos++;
						}
					}
					if( todos_prontos && prontos >= 1 )
					{
						mandar_sinal_pronto();
						var = CONTINUAR;
					}
				}
			}
		}
	}

	return var;
}

int principal()
{
	int var = EM_LOOP;
	string mapas[100];
	unsigned short int num_mapas = 1;

	srand(time(NULL));

	FOGO_acender(3);
	FOGO_resolucao(TELA_COMPRIMENTO,TELA_ALTURA);
	FOGO_alterar_fps(FPS);
	FOGO_alterar_nome_janela("< PEGA >");
	FOGO_alterar_mini_nome_janela("pega");
	FOGO_esconder_mouse();
//	FOGO_alternar_neblina();
//	FOGO_alternar_tela_cheia();

//	AR_desativar();

	CEMITERIO.mudar(10000,10000,-1000);
	CEMITERIO.mudar_r(0,0);

	posicao pos;
	pos.igualar(CEMITERIO);
	for(int i=0; i < MAX_NUM_JOGADORES; i++)
	{
		Jogadores[i] = new jogador(pos,10,i+4,1);
	}

	carregar_objetos();
	carregar_texturas();
	carregar_fontes();

	if( PapelRede == SERVIDOR )
	{
		num_mapas = ler_lista_mapas("mapas/MAPAS", mapas);
		if( num_mapas <= 0 )
		{
			cout << "Sem mapas para jogar...\n";
			return 0;
		}
	}

	while( var == EM_LOOP )
	{
		var = demonstracao();

		if( var != SAIR )
			var = preJogo();

		if( var != SAIR )
			var = jogo( mapas, num_mapas );

		if( var != SAIR )
			var = EM_LOOP;
	}

	FOGO_apagar();

	return 0;
}
