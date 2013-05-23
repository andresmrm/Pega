//----------------------------------------------------------------------
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
//----------------------------------------------------------------------

#include "fogo.h"
#include "SDL/SDL.h"
#include "listaC.h"
#include "ModelType.h"
//#include <ftgl/ftgl.h>
//#include <FTGL/FTGLPixmapFont.h>
#include <FTGL/ftgl.h>

SDL_Surface *FOGO_TELA = NULL;
int FOGO_COMPRIMENTO_TELA = 800.0;
int FOGO_ALTURA_TELA = 600.0;
int FOGO_CORES_TELA = 32;
int FOGO_TELA_CHEIA = 0;
int FOGO_FLAGS_VIDEO = 0;
int FOGO_FRAMES_SEGUNDO = 30;
int FOGO_NEBLINA = 0;
string FOGO_NOME_JANELA = "Janela desnomeada!";
string FOGO_MINI_NOME_JANELA = "sem nome...";
bool FOGO_MOSTRAR_MOUSE = true;

#define N_TEXTURAS 20
#define N_OBJETOS3D 20
#define N_FONTES 10

//Câmera
camera FOGO_CAMERA;

//Lista onde serão colocadas as coisas a serem desenhadas
tabela FOGO_COISAS;

// Luz ambiente
GLfloat LightAmbient[]  = { 0.2f, 0.2f, 0.2f, 0.2f };
// Luz difusa
GLfloat LightDiffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
// Posição da luz
GLfloat LightPosition[] = { 1.0f, 0.0f, 0.0f, 0.0f };

GLfloat LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

typedef struct				// Estrutura para trabalhar com imagem TGA
{
	GLubyte	*imageData;		// Image Data (Up To 32 Bits)
	GLuint	bpp;			// Image Color Depth In Bits Per Pixel.
	GLuint	width;			// Image Width
	GLuint	height;			// Image Height
	GLuint	texID;			// Texture ID Used To Select A Texture
} TextureImageTGA;			// Structure Name


// Vetor para as texturas carregadas
TextureImageTGA FOGO_TEXTURAS[N_TEXTURAS];

// Vetor para os objetos 3D carregados
ModelType FOGO_OBJETOS3D[N_OBJETOS3D];

// Vetor para as fontes carregadas
FTGLPixmapFont *FOGO_FONTES[N_FONTES];

// Cor padrao para o fundo
GLfloat FOGO_COR_FUNDO[4] = {0.3f, 0.50f, 1.0f, 1.0f};

// Cor da neblina
GLfloat FOGO_COR_NEBLINA[4]= {0.8f, 0.8f, 0.85f, 1.0f};
//                    {0.65f, 0.65f, 0.65f, 1.0f}; cinza

posicao FOGO_relativo; // usado para transladar uma lista inteira

void FOGO_alterar_nome_janela(string nome)
{
	FOGO_NOME_JANELA = nome;
	SDL_WM_SetCaption(FOGO_NOME_JANELA.c_str(),
		FOGO_MINI_NOME_JANELA.c_str());
}

void FOGO_alterar_mini_nome_janela(string nome)
{
	FOGO_MINI_NOME_JANELA = nome;
	SDL_WM_SetCaption(FOGO_NOME_JANELA.c_str(),
		FOGO_MINI_NOME_JANELA.c_str());
}

void ajustar_neblina()
{
	if( FOGO_NEBLINA )
	{
		glClearColor(FOGO_COR_NEBLINA[0],FOGO_COR_NEBLINA[1],
		FOGO_COR_NEBLINA[2],FOGO_COR_NEBLINA[3]);
		//Tipos de neblina: GL_EXP, GL_EXP2, GL_LINEAR
		glFogi(GL_FOG_MODE, GL_EXP);		        // Modo
		glFogfv(GL_FOG_COLOR, FOGO_COR_NEBLINA);	// Cor
		glFogf(GL_FOG_DENSITY, 0.020f/TAMANHO);		// Densidade
		glHint(GL_FOG_HINT, GL_NICEST);		    	// Hint
		glFogf(GL_FOG_START, 5.0f);			    	// Começo do Fim
		glFogf(GL_FOG_END, 200.0f);			    	// Fim
		glEnable(GL_FOG);
	}
	else
	{
		glClearColor( FOGO_COR_FUNDO[0], FOGO_COR_FUNDO[1],
			FOGO_COR_FUNDO[2], FOGO_COR_FUNDO[3] );
		glDisable(GL_FOG);
	}
}

void FOGO_alternar_neblina()
{
    FOGO_NEBLINA = !FOGO_NEBLINA;
    ajustar_neblina();
}

void FOGO_ativar_neblina()
{
    FOGO_NEBLINA = true;
    ajustar_neblina();
}

void FOGO_desativar_neblina()
{
    FOGO_NEBLINA = false;
    ajustar_neblina();
}

void FOGO_esconder_mouse()
{
	SDL_ShowCursor(SDL_DISABLE);
	FOGO_MOSTRAR_MOUSE = false;
}

void FOGO_alternar_tela_cheia()
{
    FOGO_TELA_CHEIA = !FOGO_TELA_CHEIA;
    SDL_WM_ToggleFullScreen( FOGO_TELA );

    if( !FOGO_MOSTRAR_MOUSE )
    {
    	SDL_ShowCursor(SDL_ENABLE);
    	SDL_ShowCursor(SDL_DISABLE);
    }
}

void FOGO_alterar_cor_neblina(float r, float g, float b)
{
	FOGO_COR_NEBLINA[0] = r;
	FOGO_COR_NEBLINA[1] = g;
	FOGO_COR_NEBLINA[2] = b;
	ajustar_neblina();
}

void FOGO_alterar_cor_fundo(float r, float g, float b)
{
	FOGO_COR_FUNDO[0] = r;
	FOGO_COR_FUNDO[1] = g;
	FOGO_COR_FUNDO[2] = b;
	ajustar_neblina();
}

void FOGO_alterar_fps(int fps)
{
    FOGO_FRAMES_SEGUNDO = fps;
}

/*
 * Esse "LoadTGA" não é meu... achar copyright!
 */
int LoadTGA(TextureImageTGA *texture, char *filename, int mip_map)				// Loads A TGA File Into Memory
{
//    mip_map = 1;

	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};		// Uncompressed TGA Header
	GLubyte		TGAcompare[12];									// Used To Compare TGA Header
	GLubyte		header[6];										// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;									// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;										// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;											// Temporary Variable
	GLuint		type=GL_RGBA;									// Set The Default GL Mode To RBGA (32 BPP)

	FILE *file = fopen(filename, "rb");							// Open The TGA File

	if(	file==NULL ||											// Does File Even Exist?
		fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
		memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0				||	// Does The Header Match What We Want?
		fread(header,1,sizeof(header),file)!=sizeof(header))				// If So Read Next 6 Header Bytes
		{
			if (file != NULL) fclose(file);									// If Anything Failed, Close The File

			return 0;													// Return False
		}

		texture->width  = header[1] * 256 + header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
		texture->height = header[3] * 256 + header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)

		if( texture->width <= 0 || texture->height <= 0 || (header[4]!=24 && header[4]!=32) )
		{
			fclose(file);												// If Anything Failed, Close The File
			return 0;													// Return False
		}

		texture->bpp = header[4];										// Grab The TGA's Bits Per Pixel (24 or 32)
		bytesPerPixel = texture->bpp/8;									// Divide By 8 To Get The Bytes Per Pixel
		imageSize = texture->width*texture->height*bytesPerPixel;		// Calculate The Memory Required For The TGA Data

		texture->imageData = (GLubyte *)malloc(imageSize);				// Reserve Memory To Hold The TGA Data

		if( texture->imageData==NULL || fread(texture->imageData, 1, imageSize, file)!=imageSize) // Does The Image Size Match The Memory Reserved?
		{
			if(texture->imageData!=NULL)								// Was Image Data Loaded
			{
				free(texture->imageData);								// If So, Release The Image Data
				texture->imageData = NULL;
			}

			fclose(file);												// Close The File
			return 0;													// Return False
		}

		for(GLuint i=0; i<(unsigned int)imageSize; i+=bytesPerPixel)	// Loop Through The Image Data
		{																// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
			temp=texture->imageData[i];									// Temporarily Store The Value At Image Data 'i'
			texture->imageData[i] = texture->imageData[i + 2];			// Set The 1st Byte To The Value Of The 3rd Byte
			texture->imageData[i + 2] = temp;							// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
		}

		fclose (file);													// Close The File

		// Build A Texture From The Data
		glGenTextures(1, &texture[0].texID);							// Generate OpenGL texture IDs

		glBindTexture(GL_TEXTURE_2D, texture[0].texID);					// Bind Our Texture

		//Tipos: GL_NEAREST, GL_LINEAR, GL_LINEAR_MIPMAP_NEAREST
		if(mip_map)glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		else glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		if(mip_map)glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        else glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (texture[0].bpp==24)											// Was The TGA 24 Bits
		{
			type=GL_RGB;												// If So Set The 'type' To GL_RGB
		}

		if(mip_map)gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture[0].width, texture[0].height, type, GL_UNSIGNED_BYTE, texture[0].imageData); // CONSTROE BITMAPS
        else glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);


		return 1;														// Texture Building Went Ok, Return True
}

void redimensionar_janela( int largura, int altura )
{
	/* largura/altura  */
	GLfloat razao;

	/* Evita divisão por zero */
	if ( altura == 0 )
	altura = 1;

	razao = ( GLfloat )largura / ( GLfloat )altura;

	/* Ajusta viewport. */
	glViewport( 0, 0, ( GLint )largura, ( GLint )altura );

	/* Ajusta matriz de projeção */
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );

	/* Ajusta perpectiva */
	gluPerspective( 45.0f, razao, 0.1f, 100000.0f );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	/* Ajusta para tela cheia */
	if( FOGO_TELA_CHEIA )
			SDL_WM_ToggleFullScreen( FOGO_TELA );
}

void FOGO_resolucao(int comprimento, int altura) {
    FOGO_COMPRIMENTO_TELA = comprimento;
    FOGO_ALTURA_TELA = altura;
    FOGO_TELA = SDL_SetVideoMode( FOGO_COMPRIMENTO_TELA,
		FOGO_ALTURA_TELA, FOGO_CORES_TELA, FOGO_FLAGS_VIDEO );
    redimensionar_janela( FOGO_COMPRIMENTO_TELA, FOGO_ALTURA_TELA );
}

void desenhar_quadrado( int textura, float tama )
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, LightSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, LightDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LightAmbient);
	glBindTexture( GL_TEXTURE_2D, FOGO_TEXTURAS[textura].texID );
	glBegin( GL_QUADS );
	glNormal3f( 0.0f, 0.0f, 1.0f );
	//XX
	//OX
	glTexCoord2f( 1.0f, 0.0f );
	glVertex3f( -0.5f*tama*TAMANHO, -0.5f*tama*TAMANHO, 0.0f );
	//XX
	//XO
	glTexCoord2f( 0.0f, 0.0f );
	glVertex3f( 0.5f*tama*TAMANHO, -0.5f*tama*TAMANHO, 0.0f );
	//XO
	//XX
	glTexCoord2f( 0.0f, 1.0f );
	glVertex3f( 0.5f*tama*TAMANHO, 0.5f*tama*TAMANHO, 0.0f );
	//OX
	//XX
	glTexCoord2f( 1.0f, 1.0f );
	glVertex3f( -0.5f*tama*TAMANHO, 0.5f*tama*TAMANHO, 0.0f );
	glEnd( );
}

/* Desenha uma imagem 2D */
void FOGO_desenhar_2D( int textura, float x, float y, float comprimento, float altura )
{
	float tama = FOGO_COMPRIMENTO_TELA/100.0/1.208;
	x -= 50;
	y -= 50;
	
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,0.0f);

	glOrtho( -FOGO_ALTURA_TELA/tama, FOGO_ALTURA_TELA/tama, -FOGO_COMPRIMENTO_TELA/tama, FOGO_COMPRIMENTO_TELA/tama, 0, 1);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glBindTexture( GL_TEXTURE_2D, FOGO_TEXTURAS[textura].texID );
	glBegin( GL_QUADS );
	//OX
	//XX
	glTexCoord2f( 0.0f, 1.0f ); glVertex2f( x, y );
	//XO
	//XX
	glTexCoord2f( 1.0f, 1.0f ); glVertex2f( x+comprimento, y );
	//XX
	//XO
	glTexCoord2f( 1.0f, 0.0f ); glVertex2f( x+comprimento, y+altura );
	//XX
	//OX
	glTexCoord2f( 0.0f, 0.0f ); glVertex2f( x, y+altura );
	glEnd( );
	
	glEnable(GL_LIGHTING);
	glEnable( GL_DEPTH_TEST );
}

/* Escreve texto */
void FOGO_escrever( float x, float y, string texto, int num, float r, float g, float b )
{
	/* Ajusta cor do texto se necessário */
    if( r != 1.0 || g != 1.0 || b != 1.0 )glColor3f( r, g, b);

	glDisable(GL_LIGHTING);
    glLoadIdentity();
    glTranslatef(0,0,-0.1);
    glRasterPos2f(x*(0.0622933/float(FOGO_COMPRIMENTO_TELA)+0.0004718), y*0.000414);

    if( FOGO_FONTES[num] != NULL)
    	(*FOGO_FONTES[num]).Render(texto.c_str());

    if( r != 1.0 || g != 1.0 || b != 1.0 )glColor3f( 1.0f, 1.0f, 1.0f);
    
    glEnable(GL_LIGHTING);
}

void controlar_fps()
{
	static Uint32 tempo = 0;

	if( FOGO_FRAMES_SEGUNDO > 0 )
	{
		tempo = SDL_GetTicks() - tempo;

		if( (int)(tempo) < 1000 / FOGO_FRAMES_SEGUNDO )
		{
			SDL_Delay( ( 1000 / FOGO_FRAMES_SEGUNDO ) - tempo );
		}

		tempo = SDL_GetTicks();
	}
}

// Carrega uma textura e coloca no vetor de texturas
int FOGO_carregar_textura( const char *nome, int mip_map, int pos )
{
	if ( !LoadTGA( &FOGO_TEXTURAS[pos], (char*)nome, mip_map ) )
        return 0;
    else
        return 1;
}

// Carrega uma fonte e coloca no vetor de fontes
int FOGO_carregar_fonte( const char *nome, int tamanho, int pos )
{
    FOGO_FONTES[pos] = new FTGLPixmapFont(nome);

    if( !FOGO_FONTES[pos]->Error() )
    {
    	cout << "Fonte carregada: " << nome << "\n";
        (*FOGO_FONTES[pos]).FaceSize(tamanho);
        return 1;
    }
    else
    {
    	FOGO_FONTES[pos] = NULL;
    	cout << "Erro ao tentar abrir fonte: " << nome << "\n";
        return 0;
    }
}

// Carrega um objeto 3D e coloca no vetor de objetos 3D
int FOGO_carregar_objeto3D( const char *nome, float escala, int pos )
{
    return FOGO_OBJETOS3D[pos].LoadObj((char*)nome, escala);
}

int iniciar_GL()
{
	/* Texture Mapping */
	glEnable( GL_TEXTURE_2D );
	/* Smooth Shading */
	glShadeModel( GL_SMOOTH );
	/* Depth Buffer */
	glClearDepth( 1.0f );

    glEnable(GL_NORMALIZE);
    glMatrixMode(GL_PROJECTION);

	/* Depth Test */
	glEnable( GL_DEPTH_TEST );
	/* Tipo de Depth Test */
	glDepthFunc( GL_LEQUAL );
	/* Tipo de Perspectiva */
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	/* Alpha Blending */
 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 	glEnable(GL_BLEND);

	/* Culling */
 	glEnable(GL_CULL_FACE);
 	glCullFace(GL_BACK);

    ajustar_neblina();
    
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
    glEnable(GL_LIGHT1);
    

    useLights = true;
    useSmooth = true;

	return 0;
}

int iniciar_SDL_fogo()
{
	const SDL_VideoInfo *videoInfo;

	/* inicializa a SDL */
	if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 )
	{
		fprintf( stderr, "Erro ao inicializar SDL: %s\n",SDL_GetError( ) );
		return 0;
	}

	/* obtém informações sobre o vídeo */
	videoInfo = SDL_GetVideoInfo( );

	if ( !videoInfo )
	{
		fprintf( stderr, "Erro ao identificar video: %s\n", SDL_GetError( ) );
		return 0;
	}

	FOGO_FLAGS_VIDEO  = SDL_OPENGL;          /* OpenGL em SDL        */
	FOGO_FLAGS_VIDEO |= SDL_GL_DOUBLEBUFFER; /* Double Buffering     */
	FOGO_FLAGS_VIDEO |= SDL_HWPALETTE;       /* Palheta em Hardware  */
	FOGO_FLAGS_VIDEO |= SDL_RESIZABLE;       /* Redimensionar Janela */

	/* Verifica se pode colocar superficies em hardware */
	if ( videoInfo->hw_available )
		FOGO_FLAGS_VIDEO |= SDL_HWSURFACE;
	else
		FOGO_FLAGS_VIDEO |= SDL_SWSURFACE;

	/* Verifica se pode fazer blit por hardware */
	if ( videoInfo->blit_hw ) FOGO_FLAGS_VIDEO |= SDL_HWACCEL;

	/* OpenGL double buffering */
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	/* Cria janela */
	FOGO_TELA = SDL_SetVideoMode( FOGO_COMPRIMENTO_TELA, FOGO_ALTURA_TELA, FOGO_CORES_TELA, FOGO_FLAGS_VIDEO );
	if ( !FOGO_TELA )
	{
		fprintf( stderr, "Erro ao inicializar janela: %s\n", SDL_GetError( ) );
		return 0;
	}

	/* Inicia OpenGL */
	iniciar_GL();

	redimensionar_janela( FOGO_COMPRIMENTO_TELA, FOGO_ALTURA_TELA );

	return 1;
}
//-----------------------------------------------------------------------------

//Limpa o buffer e prepara para começar a desenhar listas
void FOGO_desenhar_listas_inicio()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	FOGO_relativo.mudar(0,0,0);

	glLoadIdentity();

	FOGO_CAMERA.posicionar2();
	FOGO_CAMERA.posicionar();
	
	GLfloat LightPosition[] = { 1.0, 1.0, 0.1f, 0.0f };
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	
}
//-----------------------------------------------------------------------------

//Desenha uma lista permitindo deslocamento
int FOGO_desenhar_lista( int coluna, float x, float y, float z )
{
	void *lin = NULL;
	void *objeto;

	// Permite um deslocamento inicial da lista
	glTranslatef( x * TAMANHO, y * TAMANHO, z * TAMANHO );

	// Passa por cada elemento da lista desenhando-o
	do
	{
		objeto = retornar_objeto( &FOGO_COISAS, coluna, &lin );
		if(objeto != NULL)
		{
			((coisa*)objeto)->desenhar();
		}
	}
	while( lin != NULL );

	// Desfaz o deslocamento inicial
	glTranslatef( -x * TAMANHO, -y * TAMANHO, -z * TAMANHO );

	return 1;
}
//-----------------------------------------------------------------------------

//Desenha uma lista
int FOGO_desenhar_lista( int coluna )
{
	void *lin = NULL;
	void *objeto;

	// Passa por cada elemento da lista desenhando-o
	do
	{
		objeto = retornar_objeto( &FOGO_COISAS, coluna, &lin );
		if(objeto != NULL)
		{
			((coisa*)objeto)->desenhar();
		}
	}
	while( lin != NULL );

	return 1;
}
//-----------------------------------------------------------------------------

//Troca o buffer e controla FPS
int FOGO_desenhar_listas_fim()
{
	
	controlar_fps();

    SDL_GL_SwapBuffers( );

    return 1;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//Objetos a serem colocados na lista de desenho
//-----------------------------------------------------------------------------
//Coisa - objeto base
coisa::coisa( int coluna )
{
	inserir_linha(&FOGO_COISAS, coluna, sizeof(this), this);
	this->coluna = coluna;
}
coisa::~coisa()
{
	remover_linha(&FOGO_COISAS, coluna, this);
}

//Um plano 2D
casca::casca( posicao pos2, float tama, int dese, int coluna ) : coisa(coluna)
{
	this->pos.igualar(pos2);
	this->tamanho = tama;
	this->desenho = dese;
}

casca::casca( posicao pos2, int coluna ) : coisa(coluna)
{
	this->pos.igualar(pos2);
	this->tamanho = 1;
	this->desenho = 0;
}

void casca::desenhar(void)
{
	glPushMatrix();
	glTranslatef( ( pos.x - FOGO_relativo.x ) * TAMANHO,
		( pos.y - FOGO_relativo.y ) * TAMANHO,
		( pos.z - FOGO_relativo.z ) * TAMANHO );
	glRotatef(pos.zr,0.0,0.0,1.0);
	glRotatef(pos.xr,1.0,0.0,0.0);
	desenhar_quadrado( desenho, tamanho );
	glPopMatrix();
}

//Um plano 2D
plano::plano( posicao pos2, float tama, int dese, int coluna ) : coisa(coluna)
{
	this->pos.igualar(pos2);
	this->tamanho = tama;
	this->desenho = dese;
}

plano::plano( posicao pos2, int coluna ) : coisa(coluna)
{
	this->pos.igualar(pos2);
	this->tamanho = 1;
	this->desenho = 0;
}

void plano::desenhar(void)
{
	glPushMatrix();
	glTranslatef( ( pos.x - FOGO_relativo.x ) * TAMANHO,
		( pos.y - FOGO_relativo.y ) * TAMANHO,
		( pos.z - FOGO_relativo.z ) * TAMANHO );
	glRotatef(pos.zr,0.0,0.0,1.0);
	glRotatef(pos.xr,1.0,0.0,0.0);
	desenhar_quadrado( desenho, tamanho );
	glRotatef(180, 0.0, 1.0, 0.0);
	desenhar_quadrado( desenho, tamanho );
	glRotatef(-180, 0.0, 1.0, 0.0);
	glPopMatrix();
}

//Um objeto3d
objeto3d::objeto3d( posicao pos2, int obj, int coluna ) : coisa(coluna)
{
	this->pos.igualar(pos2);
	this->objeto = obj;
}

objeto3d::objeto3d( posicao pos2, int coluna ) : coisa(coluna)
{
	this->pos.igualar(pos2);
	this->objeto = 0;
}

void objeto3d::desenhar(void)
{
	glPushMatrix();
	glTranslatef( ( pos.x - FOGO_relativo.x ) * TAMANHO,
		( pos.y - FOGO_relativo.y ) * TAMANHO,
		( pos.z - FOGO_relativo.z ) * TAMANHO );
	glRotatef(pos.zr,0.0,0.0,1.0);
	glRotatef(pos.xr,1.0,0.0,0.0);
	FOGO_OBJETOS3D[this->objeto].Draw();
	glPopMatrix();
}

//-----------------------------------------------------------------------------

// Esvazia uma lista de coisas
int FOGO_reiniciar_uma_lista_render( int coluna )
{
	if( !( deletar_coluna( &FOGO_COISAS, coluna ) ) )return 0;
	if( !( inserir_coluna( &FOGO_COISAS, coluna ) ) )return 0;

	return 1;
}

// Esvazia todas as listas de coisas
int FOGO_reiniciar_lista_render( int colunas )
{
	deletar_tabela(&FOGO_COISAS);
	if( !( iniciar_tabela( &FOGO_COISAS, colunas ) ) )return 0;

	return 1;
}

//Carrega a biblioteca
int FOGO_acender( int colunas )
{
	if( !( iniciar_tabela( &FOGO_COISAS, colunas ) ) )return 0;
	if( !( iniciar_SDL_fogo() ) )return 0;
	FOGO_CAMERA.iniciar();

	return 1;
}

//Descarrega a biblioteca
int FOGO_apagar()
{
	SDL_QuitSubSystem( SDL_INIT_VIDEO | SDL_INIT_TIMER );
	deletar_tabela(&FOGO_COISAS);

	return 1;
}
