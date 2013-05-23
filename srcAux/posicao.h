#ifndef POSICAO_H_
#define POSICAO_H_

//Uma posição qualquer
typedef struct posicao2{
		float x, y, z;
		float xr, zr;

		void mudar(float x2, float y2, float z2)
		{
			x = x2;
			y = y2;
			z = z2;
		}
		void adicionar(float x2, float y2, float z2)
		{
			x += x2;
			y += y2;
			z += z2;
		}
		void igualar(struct posicao2 pos2)
		{
			x = pos2.x;
			y = pos2.y;
			z = pos2.z;
			xr = pos2.xr;
			zr = pos2.zr;
		}
		void mudar_r( float xr2, float zr2 )
		{
			xr = xr2;
			zr = zr2;
		}
		void rodar( float x2, float z2 )
		{
			this->xr += x2;
			this->zr += z2;
			if( xr >= 360 ) xr -= 360;
			if( xr <= -360) xr += 360;
			if( zr >= 360 ) zr -= 360;
			if( zr <= -360) zr += 360;
		}
}posicao;

#endif /*POSICAO_H_*/
