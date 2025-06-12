//Aiden D'Antuono

#include "SpriteSheet.h"
#include <stdio.h>

Sprite::Sprite()
{
	image=NULL;
}
Sprite::~Sprite()
{
	al_destroy_bitmap(image);
}
void Sprite::InitSprites(int width, int height)
{
	x = 80;
	y = -10;


	maxFrame = 11;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 6;
	frameWidth = 50;
	frameHeight = 64;
	animationColumns = 8;
	animationDirection = 1;

	image = al_load_bitmap("guy.bmp");
	al_convert_mask_to_alpha(image, al_map_rgb(255,0,255));
}

void Sprite::UpdateSprites(int width, int height, int dir)
{
	int oldx = x;
	int oldy = y;

	if(dir == 1){ //right key
		animationDirection = 1; 
		x+=2; 
		if (++frameCount > frameDelay)
		{
			frameCount=0;
			if (++curFrame > 7)
				curFrame=1;
		}
	} else if (dir == 0){ //left key
		animationDirection = 0; 
		x-=2; 
		if (++frameCount > frameDelay)
		{
			frameCount=0;
			if (++curFrame > 7)
				curFrame=1;
		}
	}
	else { //represent that they hit the space bar and that mean direction = 0
		animationDirection = dir;
		if (++frameCount > frameDelay) {
			frameCount = 0;
			curFrame = 0;
		}
	}

	

	//check for collided with foreground tiles
	if (animationDirection==0)
	{ 
		if (collided(x, y + frameHeight)) { //collision detection to the left
			x = oldx; 
			y= oldy;
		}

	}
	else if (animationDirection ==1)
	{ 
		if (collided(x + frameWidth, y + frameHeight)) { //collision detection to the right
			x = oldx;
			y= oldy;
		}
	}
	

	//Ceiling collision handling
	if (y < 0) {
		y = 0;
	}
}

bool Sprite::CollisionEndBlock()
{
	if (endValue(x + frameWidth/2, y + frameHeight + 5))
		return true;
	else
		return false;
}

void Sprite::DrawSprites(int xoffset, int yoffset)
{
	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = (curFrame / animationColumns) * frameHeight;

	if (animationDirection==1 || animationDirection == 2){
		al_draw_bitmap_region(image, fx, fy, frameWidth,frameHeight, x-xoffset, y-yoffset, 0);
	}else if (animationDirection == 0 ){
		al_draw_bitmap_region(image, fx, fy, frameWidth,frameHeight, x-xoffset, y-yoffset, ALLEGRO_FLIP_HORIZONTAL);
	}
}

int Sprite::jumping(int jump, const int JUMPIT)
{
	if (jump==JUMPIT) {
		if (!collided(x + frameWidth/2, y + frameHeight + 5))
			jump = 0; 
	}
	else
	{
		//Late Jump Frame
		if (jump <= -10) {
			curFrame = 10;
		}
		//Early Jump Frame
		else if (jump <= 20) {
			curFrame = 9;
		}
		//Start of Jump Frame
		else {
			curFrame = 8;
		}
		y -= jump/3; 
		jump--;
	}

	if (jump<0) 
	{ 
		if (collided(x + frameWidth/2,  y + frameHeight))
		{ 
			//Landing Jump Frame
			curFrame = 11;
			frameCount = 0;
			jump = JUMPIT; 
			while (collided(x + frameWidth/2,y + frameHeight))
			{
				y -= 3;
			}
		} 
	}
	return jump;
}