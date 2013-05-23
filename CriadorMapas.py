#!/usr/bin/env python
# -*- coding: iso-8859-15 -*-

import sys

# Multiplicador para o tamanho geral das coisas
TAM = 1.0

MAPA = []
FACES = []
CHAOS = []
TETOS = []
PAREDES = []
VERTICES_VertLinha = {}
VERTICES_LinhaVert = {}
NORMAIS_NormLinha = {}
NORMAIS_LinhaNorm = {}
nColunas, nLinhas = 0, 0
TEXTURA_P = "parede.tga"
TEXTURA_C = "chao.tga"
TEXTURA_T = "teto.tga"
TETO = False

class vertice():
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z
        
    def __str__(self):
        return "v " + str(self.x) + " " + str(self.y) + " " + str(self.z) + "\n"
    
    def tupla(self):
        return ( self.x, self.y, self.z )

class face():
    def __init__(self, vert1, vert2, vert3, vert4, norm):
        self.vertices = [ vert1, vert2, vert3, vert4 ]
        self.normal = norm
        
    def __str__(self):
        return "f " + str(self.vertices[0]) + "/" + "1" + "/" + str(self.normal) \
              + " " + str(self.vertices[1]) + "/" + "2" + "/" + str(self.normal) \
              + " " + str(self.vertices[2]) + "/" + "3" + "/" + str(self.normal) \
              + " " + str(self.vertices[3]) + "/" + "4" + "/" + str(self.normal) \
              + "\n"

# le o arquivo com texto q virara objeto
def lerArquivo(nomeArquivo):
    global nColunas, nLinhas, TEXTURA_C, TEXTURA_P, TEXTURA_T, TETO
    
    print "\nLendo arquivo mapa < " + nomeArquivo + " >"
    
    try:
        arquivo = open(nomeArquivo, mode='r')
    except:
        print "Erro ao tentar abrir arquivo"
        exit()
    
    texto = arquivo.read()
    print "Mapa lido:"
    print texto
    print "Processando..."
    
    # Analiza o arquivo escolhe texturas
    lista = texto.splitlines()
    
    if lista[0].startswith("@"):
        TEXTURA_P = lista[0].replace("@ ","")
        lista.remove(lista[0])
        
        if lista[0].startswith("@"):
            TEXTURA_C = lista[0].replace("@ ","")
            lista.remove(lista[0])
            
            if lista[0].startswith("@"):
                TEXTURA_T = lista[0].replace("@ ","")
                lista.remove(lista[0])
                TETO = True
            else:
                TETO = False
                
            if lista[0].startswith("%"):
                lista.remove(lista[0])
            
        lista.remove(lista[0])
            
    print "-> Textura para Paredes: " + TEXTURA_P
    print "-> Textura para Chaos: " + TEXTURA_C
    print "-> Textura para Tetos: " + TEXTURA_T
    
    if lista[0].startswith("N"):
        lista.remove(lista[0])
        lista.remove(lista[0])
    
    texto = ""
    for s in lista:
        texto += s + "\n"
    
    # Acha o numero de colunas do arquivo lido
    c, nColunas = 0, -1
    while c != '\n':
        nColunas += 1
        c = texto[nColunas]
    print "-> Colunas: " + str(nColunas)
    
    # Acha o numero de linhas
    c, nLinhas = 0, 0
    while nLinhas*(nColunas+1) < len(texto):
        c = texto[nLinhas*(nColunas+1)]
        if c == '\n':
            break
        nLinhas += 1
    print "-> Linhas: " + str(nLinhas)
    
    # Aloca e coloca os valores na matriz
    for i in range(nColunas):
        MAPA.append([])
        for j in range(nLinhas):
            c = texto[j*(nColunas+1)+i]
            if c == '#':
                MAPA[i].append(1)
            else:
                MAPA[i].append(0)
                
    print "Matriz carregada"

# prepara os dicionarios de vertices
def gerarVertices():
    verts = []
    
    print "Gerando Vertices"
    
    # pega os vertices da lista de faces e os coloca na lista de vertices
    for f in FACES:
        for i in range(4):
            verts.append(f.vertices[i].tupla())
            
    # limpa a lista de vertices para q nao haja vertices repetidos
    verts.sort()
    for v in verts:
        for v2 in verts[verts.index(v)+1:]:
            if v == v2:
                verts.remove(v2)
    
    # passa os vertices da lista para o dicionario
    for v in verts:
        VERTICES_VertLinha[v] = len(VERTICES_VertLinha)+1
        VERTICES_LinhaVert[len(VERTICES_LinhaVert)+1] = v
        
# prepara os dicionarios de normais
def gerarNormais():
    norms = []
    
    print "Gerando Normais"
    
    for f in FACES:
        norms.append(f.normal.tupla())
            
    # limpa a lista de normais para q nao haja normais repetidas
    norms.sort()
    for n in norms:
        for n2 in norms[norms.index(n)+1:]:
            if n == n2:
                norms.remove(n2)
    
    # passa as normais da lista para o dicionario
    for n in norms:
        NORMAIS_NormLinha[n] = len(NORMAIS_NormLinha)+1
        NORMAIS_LinhaNorm[len(NORMAIS_LinhaNorm)+1] = n

# prepara a lista de faces
def gerarFaces():
    
    print "Gerando Faces"
    
    # troca os vertices das faces pelos valores de suas linhas
    for f in CHAOS:
        for i in range(4):
            f.vertices[i] = VERTICES_VertLinha.get(f.vertices[i].tupla())
        f.normal = NORMAIS_NormLinha.get(f.normal.tupla())
        
    for f in TETOS:
        for i in range(4):
            f.vertices[i] = VERTICES_VertLinha.get(f.vertices[i].tupla())
        f.normal = NORMAIS_NormLinha.get(f.normal.tupla())
        
    for f in PAREDES:
        for i in range(4):
            f.vertices[i] = VERTICES_VertLinha.get(f.vertices[i].tupla())
        f.normal = NORMAIS_NormLinha.get(f.normal.tupla())
        
# cria uma face chao
def novoChao(vert, tamanho):
    vert1 = vertice(vert.x, vert.y, vert.z)
    vert2 = vertice(vert.x+tamanho, vert.y, vert.z)
    vert3 = vertice(vert.x+tamanho, vert.y+tamanho, vert.z)
    vert4 = vertice(vert.x, vert.y+tamanho, vert.z)
    norm = vertice( 0.0, 0.0, 1.0 )
    FACES.append( face(vert1, vert2, vert3, vert4, norm) )
    CHAOS.append( face(vert1, vert2, vert3, vert4, norm) )
    
# cria uma face teto
def novoTeto(vert, tamanho):
    vert1 = vertice(vert.x, vert.y, vert.z)
    vert2 = vertice(vert.x+tamanho, vert.y, vert.z)
    vert3 = vertice(vert.x+tamanho, vert.y+tamanho, vert.z)
    vert4 = vertice(vert.x, vert.y+tamanho, vert.z)
    norm = vertice( 0.0, 0.0, -1.0 )
    FACES.append( face(vert4, vert3, vert2, vert1, norm) )
    TETOS.append( face(vert4, vert3, vert2, vert1, norm) )
    
# cria uma face parede
def novaParede(vert, tamanho, orientacao):
    
    if orientacao == 's':
        vert1 = vertice(vert.x        , vert.y, vert.z        )
        vert2 = vertice(vert.x+tamanho, vert.y, vert.z        )
        vert3 = vertice(vert.x+tamanho, vert.y, vert.z+tamanho)
        vert4 = vertice(vert.x        , vert.y, vert.z+tamanho)
        norm = vertice( 0.0, 1.0, 0.0 )
        
    elif orientacao == 'n':
        vert1 = vertice(vert.x+tamanho, vert.y, vert.z        )
        vert2 = vertice(vert.x        , vert.y, vert.z        )
        vert3 = vertice(vert.x        , vert.y, vert.z+tamanho)
        vert4 = vertice(vert.x+tamanho, vert.y, vert.z+tamanho)
        norm = vertice( 0.0, -1.0, 0.0 )
        
    elif orientacao == 'o':
        vert1 = vertice(vert.x, vert.y+tamanho, vert.z        )
        vert2 = vertice(vert.x, vert.y        , vert.z        )
        vert3 = vertice(vert.x, vert.y        , vert.z+tamanho)
        vert4 = vertice(vert.x, vert.y+tamanho, vert.z+tamanho)
        norm = vertice( -1.0, 0.0, 0.0 )
        
    elif orientacao == 'l':
        vert1 = vertice(vert.x, vert.y        , vert.z        )
        vert2 = vertice(vert.x, vert.y+tamanho, vert.z        )
        vert3 = vertice(vert.x, vert.y+tamanho, vert.z+tamanho)
        vert4 = vertice(vert.x, vert.y        , vert.z+tamanho)
        norm = vertice( 1.0, 0.0, 0.0 )
        
    FACES.append( face(vert1, vert2, vert3, vert4, norm) )
    PAREDES.append( face(vert1, vert2, vert3, vert4, norm) )

# monta a primeira lista de face a partir da matriz lida do arquivo
def montarObjeto():
    print "Montando objeto"
    
    ponto = vertice(0.0,0.0,0.0)
    
    print "-> Chao"
    for i in range(nColunas):
        for j in range(nLinhas):
            if MAPA[i][j] == 0:
                ponto.x = i*TAM
                ponto.y = j*TAM
                novoChao(ponto,TAM)
                
    if TETO:
        print "-> Teto"
        ponto.z = 1.0*TAM
        for i in range(nColunas):
            for j in range(nLinhas):
                if MAPA[i][j] == 0:
                    ponto.x = i*TAM
                    ponto.y = j*TAM
                    novoTeto(ponto,TAM)
        
    print "-> Paredes"        
    ponto.z = 0.0
    verticais = [];
    horizontais = [];

    for i in range(nColunas-1):
        verticais.append([])
        for j in range(nLinhas):
            verticais[i].append(0)
            
    for i in range(nColunas):
        horizontais.append([])
        for j in range(nLinhas-1):
            horizontais[i].append(0)
            

    for j in range(nLinhas):
        for i in range(nColunas):
        
            # Preenche matriz de paredes verticais
            if i < nColunas-1:
                if MAPA[i][j] == MAPA[i+1][j]:
                    verticais[i][j] = 0
                else:
                    verticais[i][j] = 1
                    
            # Preenche matriz de paredes horizontais
            if j < nLinhas-1:
                if MAPA[i][j] == MAPA[i][j+1]:
                    horizontais[i][j] = 0
                else:
                    horizontais[i][j] = 1
    
    # cria as paredes verticais                
    for j in range(nLinhas):
        for i in range(nColunas-1):
            if verticais[i][j] == 1:
                # ajusta para q o lado invisivel fique voltado para dentro
                ponto.x = (i+1)*TAM
                ponto.y = j*TAM
                if MAPA[i][j] == 0:
                    novaParede(ponto, TAM, 'o')
                else:
                    novaParede(ponto, TAM, 'l')
                    
    # cria as paredes horizontais              
    for j in range(nLinhas-1):
        for i in range(nColunas):
            if horizontais[i][j] == 1:
                # ajusta para q o lado invisivel fique voltado para dentro
                ponto.x = i*TAM
                ponto.y = (j+1)*TAM
                if MAPA[i][j] == 0:
                    novaParede(ponto, TAM, 's')
                else:
                    novaParede(ponto, TAM, 'n')
                    
    print "-> Bordas"
    
    for i in range(nLinhas):
        ponto.y = i*TAM
        
        # elimina bordas q nao seriam vistas
        if MAPA[0][i] != 1:
            ponto.x = 0
            novaParede(ponto, TAM, 'l')
            
        # elimina bordas q nao seriam vistas
        if MAPA[nColunas-1][i] != 1:    
            ponto.x = nColunas*TAM
            novaParede(ponto, TAM, 'o')
        
    for i in range(nColunas):
        ponto.x = i*TAM
        
        # elimina bordas q nao seriam vistas
        if MAPA[i][0] != 1:
            ponto.y = 0
            novaParede(ponto, TAM, 'n')
            
        # elimina bordas q nao seriam vistas
        if MAPA[i][nLinhas-1] != 1:
            ponto.y = nLinhas*TAM
            novaParede(ponto, TAM, 's')
        
    print "Objeto Montado"
        
# salva arquivos finais
def salvarArquivos(nomeArquivo,X):
    print "Salvando arquivos finais"
    arquivo = open(nomeArquivo + ".obj", mode='w')
    arquivo.write("# Arquivo gerado com o criador de Mapas do Pega\n" \
                  + "mtllib " + X + ".mtl" + "\n" )
    
    for v in VERTICES_LinhaVert:
        arquivo.write( "v " + str( VERTICES_LinhaVert.get(v)[0] ) \
                      + " " + str( VERTICES_LinhaVert.get(v)[1] ) \
                      + " " + str( VERTICES_LinhaVert.get(v)[2]) \
                      + "\n" )
    
    arquivo.write( "vt 0.0 0.0\n" \
                 + "vt 1.0 0.0\n" \
                 + "vt 1.0 1.0\n" \
                 + "vt 0.0 1.0\n" )
        
    for n in NORMAIS_LinhaNorm:
        arquivo.write( "vn " + str( NORMAIS_LinhaNorm.get(n)[0] ) \
                       + " " + str( NORMAIS_LinhaNorm.get(n)[1] ) \
                       + " " + str( NORMAIS_LinhaNorm.get(n)[2]) \
                       + "\n" )

    arquivo.write( "s off\n" )

    arquivo.write( "usemtl Material_Chao\n" )
    
    for f in CHAOS:
        arquivo.write( str(f) )
        
    arquivo.write( "usemtl Material_Teto\n" )
    
    for f in TETOS:
        arquivo.write( str(f) )
    
    arquivo.write( "usemtl Material_Parede\n" )
        
    for f in PAREDES:
        arquivo.write( str(f) )
        
    arquivoM = open(nomeArquivo + ".mtl", mode='w')
    arquivoM.write("# Arquivo MTL gerado com o criador de Mapas do Pega para " + nomeArquivo + ".obj\n" \
                 + "# Material Count: 3 ou 2\n" )
    arquivoM.write("\nnewmtl Material_Chao\n" \
                 + "Ns 0.1\n" \
                 + "Ka 0.900000 0.900000 0.900000\n" \
                 + "Kd 0.750000 0.750000 0.750000\n" \
                 + "Ks 0.500000 0.500000 0.500000\n" \
                 + "Ni 0.500000\n" \
                 + "d 1.000000\n" \
                 + "illum 2\n" \
                 + "map_Kd " + TEXTURA_C +"\n" )

    if TETO :
        arquivoM.write("\nnewmtl Material_Teto\n" \
                     + "Ns 0.1\n" \
                     + "Ka 0.900000 0.900000 0.900000\n" \
                     + "Kd 0.750000 0.750000 0.750000\n" \
                     + "Ks 0.500000 0.500000 0.500000\n" \
                     + "Ni 0.500000\n" \
                     + "d 1.000000\n" \
                     + "illum 2\n" \
                     + "map_Kd " + TEXTURA_T +"\n" )
    
    arquivoM.write("\nnewmtl Material_Parede\n" \
                 + "Ns 0.1\n" \
                 + "Ka 0.900000 0.900000 0.900000\n" \
                 + "Kd 0.750000 0.750000 0.750000\n" \
                 + "Ks 0.500000 0.500000 0.500000\n" \
                 + "Ni 0.500000\n" \
                 + "d 1.000000\n" \
                 + "illum 2\n" \
                 + "map_Kd " + TEXTURA_P +"\n" )
    
    print "Arquivos salvos:" 
    print "-> " + nomeArquivo + ".obj"
    print "-> " + nomeArquivo + ".mtl"
    print ""

def limparGlobais():
    global MAPA, FACES, CHAOS, TETOS, PAREDES, TEXTURA_P, TEXTURA_C, TEXTURA_T, nColunas, nLinhas
    
    MAPA = []
    FACES = []
    CHAOS = []
    TETOS = []
    PAREDES = []
    VERTICES_VertLinha.clear()
    VERTICES_LinhaVert.clear()
    NORMAIS_NormLinha.clear()
    NORMAIS_LinhaNorm.clear()
    nColunas, nLinhas = 0, 0
    TEXTURA_P = "parede.tga"
    TEXTURA_C = "chao.tga"
    TEXTURA_T = "teto.tga"

#--------------------------------------------------------------------------------#
#                        Programa                                                #
#--------------------------------------------------------------------------------#

if len(sys.argv) >= 2:
    arquivos_mapas = sys.argv[1]
else:
    arquivo = open("mapas/MAPAS", mode='r')
    arquivos_mapas = arquivo.read()

for a in arquivos_mapas.splitlines():
    limparGlobais() 
    lerArquivo("mapas/"+a)
    montarObjeto()
    gerarVertices()
    gerarNormais()
    gerarFaces()
    salvarArquivos("modelos/"+a,a)
