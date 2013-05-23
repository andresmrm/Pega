#!/usr/bin/env python
# -*- coding: iso-8859-15 -*-

'''
Texto passado para esse codigo deve ser do formato:

Jogador1
Jogador2
...


E sera retornado:

MelhorPartida1
...
MelhorPartida10
Rank da Partida Atual
MelhorJogador1
...
MelhorJogador10
Rank do Jogador1
Rank do Jogador2
...
'''
 

import sys

#globais
NOME_ARQUIVO_JOGADORES = "rank/jogadores"
NOME_ARQUIVO_PARTIDAS = "rank/partidas"

def lerArquivo( nomeArquivo ):
    try:
        arquivo = open(nomeArquivo, mode='r')
    except:
        print "Erro ao tentar abrir arquivo, ignorando"
        return []
        
    # le o arquivo, o separa em linhas e retorna isso
    return (arquivo.read()).splitlines()

def escreverArquivo( nomeArquivo, lista ):
    try:
        arquivo = open(nomeArquivo, mode='w')
    except:
        print "Erro ao tentar abrir arquivo"
        exit()
        
    for linha in lista:
        arquivo.write(linha + "\n");
        
def ordenar( listaRanks ):
    listaPontos = []
    
    for rank in listaRanks:
        pontos = int((((rank.partition('\t'))[2]).partition('\t')[2]).partition(' ')[0])
        listaPontos.append( (pontos,rank) )
    
    listaPontos.sort(None,None,True)
    
    listaOrdenada = []
    
    for rank in listaPontos:
        listaOrdenada.append(rank[1])
        
    return listaOrdenada

def calcularPartida( jogadores ):
    
    data = (jogadores[0].partition('\t'))[0]
    nome = ""
    pontos = 0
    tirosDados = 0
    acertos = 0
    
    for jogador in jogadores:
        temp = ((jogador.partition('\t'))[2]).partition('\t')
        nome += temp[0] + " "
        temp = temp[2].partition(' ')
        pontos += int(temp[0])
        temp = temp[2].partition(' ')
        tirosDados += int(temp[0])
        temp = temp[2].partition(' ')
        acertos += int(temp[0])
        
    return data + "\t" + nome + "\t" + str(pontos) + " " + str(tirosDados) + " " + str(acertos)
    
def melhoresPontuacoes( lista ):
    melhoresPontos = []
    for i in range(0,10):
        if len(lista) > i:
            melhoresPontos.append(lista[i])
        else:
            melhoresPontos.append('')
        
    return melhoresPontos

def acharRank( procurado, lista ):
    i = 0
    while lista[i] != procurado:
        i += 1
    
    return i+1

def rankear( jogadoresArg ):    
    # Insere os novos jogadores no arquivo de ranks, ordenando-o
    jogadores = jogadoresArg.splitlines()
    listaRanksJog = lerArquivo(NOME_ARQUIVO_JOGADORES)
    for jogador in jogadores:
        listaRanksJog.append( jogador )
    listaRanksJog = ordenar(listaRanksJog)
    escreverArquivo(NOME_ARQUIVO_JOGADORES, listaRanksJog)
    
    # Insere a nova partida no arquivo de ranks, ordenando-o
    partidaAtual = calcularPartida(jogadores)
    listaRanksPart = lerArquivo(NOME_ARQUIVO_PARTIDAS)
    listaRanksPart.append( partidaAtual )
    listaRanksPart = ordenar(listaRanksPart)
    escreverArquivo(NOME_ARQUIVO_PARTIDAS, listaRanksPart)
    
    # Acha os 10 jogadores e 10 partidas com mais pontos 
    melhoresPontosJogs = melhoresPontuacoes(listaRanksJog)
    melhoresPontosPart = melhoresPontuacoes(listaRanksPart)
    
    # Acha o rank dos jogadores e partidas atuais
    ranksJogsAtuais = []
    for jogador in jogadores:
        ranksJogsAtuais.append( acharRank(jogador, listaRanksJog) )
    rankPartAtual = acharRank( partidaAtual, listaRanksPart )
    
    # Prepara o retorno conforme especificacao no comeco desse arquivo
    resultado = ""
    for partidas in melhoresPontosPart:
        resultado += partidas + "\n"
    resultado += str(rankPartAtual) + "\n"
    for jogador in melhoresPontosJogs:
        resultado += jogador + "\n"
    for jogador in ranksJogsAtuais:
        resultado += str(jogador) + "\n"    
    
    return resultado


#if len(sys.argv) >= 2:
#    print sys.argv[1];
#    print "Resposta"
#    resultado = rankear(sys.argv)
#    exit(resultado);
#else:
#    print 'aahhh!';