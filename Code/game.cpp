/* ======================================================
   SFile: S
   SDate: S
   SRevision: S
   SCreator: Gergely Zara S
   SNotice: S
   ======================================================*/

#include "game.h"
#include "Header_Display.h"
#include "Game_Display.cpp"

global_variable bool32 GlobalInterfaceOn;
global_variable bool32 GlobalIsPlayerInitialized;
//ide jönnke a játék dolgai pl network?
/**********************************************************************************Sound*/
internal_function void
GameOutputSound(game_sound_output_buffer *SoundBuffer, int Hz)//structoknak a cuccaibna tárolni akarunk azért*
{
	/*
		Create a BUFFER here as well? Which you can change several times before 
		drawing into the actual buffer which gets displayed right away in the window
		1.background->buffer
		understand pitch better... and buffers...........
		2.player->buffer
	*/
	local_persist real32 tSine;
	int16 Volume = 3000;
	//Hz = 300;
	int WavePeriod = SoundBuffer->SamplesPerSecond / Hz;

	int16 *SampleOut = SoundBuffer->Samples;

	for (int SampleIndex = 0;
		SampleIndex < SoundBuffer->SampleCount;
		++SampleIndex)
	{
		real32 SineValue = sinf(tSine);//truncation, fixed point math....
		int16 SampleValue = (int16)(SineValue * Volume);

		*SampleOut++ = SampleValue;
		*SampleOut++ = SampleValue;

		tSine += 2.0f * Pi32 *1.0f/(real32)WavePeriod;

	}
}
/**********************************************************************************PAINT*/
//internal_function void
//RenderSky(game_offscreen_buffer *Buffer, int XOffset, int YOffset)
//{//bal->jobb, lent->fent
//	uint8 *Row = (uint8 *)Buffer->Memory;//256 os négyzet, row writtne into memory
//	for (int Y = 0; Y < Buffer->Height; ++Y)
//	{
//		uint8 *Pixel = (uint8 *)Row;//pixel writtne into row
//		for (int X = 0; X < Buffer->Width; ++X)
//		{//BB GG RR xx
//			//4byte/pixel
//			*Pixel = 0;//(uint8)(X*4 + XOffset);//the gradient is the 8 bits of y going up
//			Pixel++;
//
//			*Pixel = 0;//(uint8)(Y*4 + YOffset);
//			Pixel++;
//
//			*Pixel = 0;
//			Pixel++;
//
//			*Pixel = 255;
//			Pixel++;
//		}
//		Row += Buffer->Pitch;//pitch is what gives markings to 1d representation 1234PITCH567P...
//	}
//}
//internal_function void
//DrawRectangle(game_offscreen_buffer *Buffer, int LeftX, int UpY)
//{
//	//ifoutofbounds
//	if (
//		LeftX > Buffer->Width - 10 || UpY > Buffer->Height - 10
//		|| LeftX < 0 || UpY < 0
//		)
//	{
//		return;
//	}
//	uint32 Color = 0xFFFFFFFF;
//	//color Col = {};
//	//Color = Col.Color(255, 0, 0, 255);
//
//	uint8 *Column = (
//		(uint8 *)Buffer->Memory
//		+ LeftX * Buffer->BytesPerPixel
//		+ UpY * Buffer->Pitch);
//	for (int X = LeftX; X < LeftX + 10; ++X)
//	{	
//		uint32 *Pixel = (uint32 *)Column;
//		for (int Y = UpY; Y < UpY + 10; ++Y)
//		{			
//			*Pixel++ = Color;		
//		}
//		Column += Buffer->Pitch;
//	}
//}
//internal_function void
//DrawGameCursor(game_offscreen_buffer *Buffer, int LeftX, int UpY)
//{
//	//ifoutofbounds
//	if (
//		LeftX > Buffer->Width - 10 || UpY > Buffer->Height - 10
//		|| LeftX < 0 || UpY < 0
//		)
//	{
//		return;
//	}
//	uint32 Color = 0xFFFFFFFF;
//	//color Col = {};
//	//Color = Col.Color(255, 0, 0, 255);
//
//	uint8 *Column = (
//		(uint8 *)Buffer->Memory
//		+ LeftX * Buffer->BytesPerPixel
//		+ UpY * Buffer->Pitch);
//	for (int X = LeftX; X < LeftX + 10; ++X)
//	{
//		uint32 *Pixel = (uint32 *)Column;
//		for (int Y = UpY; Y < UpY + 10; ++Y)
//		{
//			*Pixel++ = Color;
//		}
//		Column += Buffer->Pitch;
//	}
//}
//inline uint32 Color(uint8 B, uint8 G, uint8 R, uint8 A)
//{
//	uint32 Result;
//	Result = (B << 24) | (G << 16) | (R << 8) | (A << 0);
//	return Result;
//}
internal_function void InitCube(map *Map, v3 Location, v3 Size)
{
	uint32 PolyCol = Color(0, 255, 192, 203);
	Map->MapPolygons[Map->MapPolyCount] = polygon
	{
		v3{ 0,0,200 } * Size + Location, 
		v3{ 100,0,200 } * Size + Location, 
		v3{ 100,-100,200 } * Size + Location,
		v3{ 0,-100,200 } * Size + Location
		,PolyCol
	};
	Map->MapPolyCount += 1;

	Map->MapPolygons[Map->MapPolyCount] = polygon
	{
		Map->MapPolygons[Map->MapPolyCount - 1].Points[1],
		v3{ 0,0,300 } * Size + Location,
		v3{ 100,0,300 } * Size + Location,
		v3{ 100,-100,300 } * Size + Location
		//v3{ 0,-100,300 } * Size + Location
		,PolyCol
	};
	Map->MapPolyCount += 1;

	Map->MapPolygons[Map->MapPolyCount] = polygon
	{
		v3{ 0,0,200 } * Size + Location,
		v3{ 100,0,200 } * Size + Location,
		v3{ 100,0,300 } * Size + Location,
		v3{ 0,0,300 } * Size + Location
		,PolyCol
	};
	Map->MapPolyCount += 1;

	Map->MapPolygons[Map->MapPolyCount] = polygon
	{
		v3{ 0,-100,200 } * Size + Location,
		v3{ 100,-100,200 } * Size + Location,
		v3{ 100,-100,300 } * Size + Location,
		v3{ 0,-100,300 } * Size + Location
		,PolyCol
	};
	Map->MapPolyCount += 1;

	Map->MapPolygons[Map->MapPolyCount] = polygon
	{
		v3{ 0,0,200 } * Size + Location,
		v3{ 0,0,300 } * Size + Location,
		v3{ 0,-100,300 } * Size + Location,
		v3{ 0,-100,200 } * Size + Location
		,PolyCol
	};
	Map->MapPolyCount += 1;

	Map->MapPolygons[Map->MapPolyCount] = polygon
	{
		v3{ 100,0,200 } * Size + Location,
		v3{ 100,0,300 } * Size + Location,
		v3{ 100,-100,300 } * Size + Location,
		v3{ 100,-100,200 } * Size + Location
		,PolyCol
	};
	Map->MapPolyCount += 1;
}
internal_function void
InitGameBoard(map *Map)
{
	for (int a = 0; a < 9; a++)
	{
		InitCube(Map, v3{ 0,0, (real64)a * 100 }, v3{ 1,-1,1 });
		InitCube(Map, v3{ 100,0, (real64)a * 100 }, v3{ 1,-1,1 });
		InitCube(Map, v3{ 200,0, (real64)a * 100 }, v3{ 1,-1,1 });
		InitCube(Map, v3{ 300,0, (real64)a * 100 }, v3{ 1,-1,1 });
		InitCube(Map, v3{ 400,0, (real64)a * 100 }, v3{ 1,-1,1 });
		InitCube(Map, v3{ 500,0, (real64)a * 100 }, v3{ 1,-1,1 });
		InitCube(Map, v3{ 600,0, (real64)a * 100 }, v3{ 1,-1,1 });
		InitCube(Map, v3{ 700,0, (real64)a * 100 }, v3{ 1,-1,1 });
		InitCube(Map, v3{ 800,0, (real64)a * 100 }, v3{ 1,-1,1 });

	}	
}
internal_function void
MapInit(map *Map)
{
	InitGameBoard(Map);


	InitCube(Map, v3{ 0,0,-1200 }, v3{1,-1,1});
	InitCube(Map, v3{ 0,0,-1000 }, v3{ 2,-2,2 });
	InitCube(Map, v3{ 0,0,-3000 }, v3{ 10,-10,10 });

	InitCube(Map, v3{ 3000,0,-1200 }, v3{ 1,-1,1 });
	InitCube(Map, v3{ 3000,0,-1000 }, v3{ 2,-2,2 });
	InitCube(Map, v3{ 3000,0,-3000 }, v3{ 10,-10,10 });

	InitCube(Map, v3{ -3000,0,-1200 }, v3{ 1,-1,1 });
	InitCube(Map, v3{ -3000,0,-1000 }, v3{ 2,-2,2 });
	InitCube(Map, v3{ -3000,0,-3000 }, v3{ 10,-10,10 });

	InitCube(Map, v3{ 0,3000,-1200 }, v3{ 1,-1,1 });
	InitCube(Map, v3{ 0,3000,-1000 }, v3{ 2,-2,2 });
	InitCube(Map, v3{ 0,3000,-3000 }, v3{ 10,-10,10 });

	//

	InitCube(Map, v3{ 0,0,2000 }, v3{ 1,-1,1 });
	InitCube(Map, v3{ 0,0,2200 }, v3{ 2,-2,2 });
	InitCube(Map, v3{ 0,0,200 }, v3{ 10,-10,10 });

	InitCube(Map, v3{ 3000,0,2000 }, v3{ 1,-1,1 });
	InitCube(Map, v3{ 3000,0,2200 }, v3{ 2,-2,2 });
	InitCube(Map, v3{ 3000,0,200 }, v3{ 10,-10,10 });

	InitCube(Map, v3{ -3000,0,2000 }, v3{ 1,-1,1 });
	InitCube(Map, v3{ -3000,0,2200 }, v3{ 2,-2,2 });
	InitCube(Map, v3{ -3000,0,200 }, v3{ 10,-10,10 });

	InitCube(Map, v3{ 0,3000,2000 }, v3{ 1,-1,1 });
	InitCube(Map, v3{ 0,3000,2200 }, v3{ 2,-2,2 });
	InitCube(Map, v3{ 0,3000,200 }, v3{ 10,-10,10 });

	//

	InitCube(Map, v3{ 0,0,-3200 }, v3{ 1,-1,1 });
	InitCube(Map, v3{ 0,0,-3000 }, v3{ 2,-2,2 });
	InitCube(Map, v3{ 0,0,-5000 }, v3{ 10,-10,10 });

	InitCube(Map, v3{ 3000,0,-3200 }, v3{ 1,-1,1 });
	InitCube(Map, v3{ 3000,0,-3000 }, v3{ 2,-2,2 });
	InitCube(Map, v3{ 3000,0,-5000 }, v3{ 10,-10,10 });

	InitCube(Map, v3{ -3000,0,-3200 }, v3{ 1,-1,1 });
	InitCube(Map, v3{ -3000,0,-3000 }, v3{ 2,-2,2 });
	InitCube(Map, v3{ -3000,0,-5000 }, v3{ 10,-10,10 });

	InitCube(Map, v3{ 0,3000,-3200 }, v3{ 1,-1,1 });
	InitCube(Map, v3{ 0,3000,-3000 }, v3{ 2,-2,2 });
	InitCube(Map, v3{ 0,3000,-5000 }, v3{ 10,-10,10 });

	//CUBE2
	
}
void CatModelInit(cat_model *CatModel, player_instance *Player)
{
	/***************************************************************************************************/
	CatModel->RightPelvis[0] = 
		((Player->parentedLocalRightPelvis - CatModel->RightPelvis[0]) + CatModel->RightPelvis[0]) 
		+ v3{1, 0, 0};
	CatModel->RightPelvis[1] =
		((Player->parentedLocalRightPelvis - CatModel->RightPelvis[1]) + CatModel->RightPelvis[1])
		+ v3{ 0.5f, 0, 0.5f };
	CatModel->RightPelvis[2] =
		((Player->parentedLocalRightPelvis - CatModel->RightPelvis[2]) + CatModel->RightPelvis[2])
		+ v3{ 0, 0, 1 };
	CatModel->RightPelvis[3] =
		((Player->parentedLocalRightPelvis - CatModel->RightPelvis[3]) + CatModel->RightPelvis[3])
		+ v3{ -0.5f, 0, 0.5f };
	CatModel->RightPelvis[4] =
		((Player->parentedLocalRightPelvis - CatModel->RightPelvis[4]) + CatModel->RightPelvis[4])
		+ v3{ -1.0f, 0, .0f };
	CatModel->RightPelvis[5] =
		((Player->parentedLocalRightPelvis - CatModel->RightPelvis[5]) + CatModel->RightPelvis[5])
		+ v3{ -0.5f, 0, -0.5f };
	CatModel->RightPelvis[6] =
		((Player->parentedLocalRightPelvis - CatModel->RightPelvis[5]) + CatModel->RightPelvis[5])
		+ v3{ 0, 0, -1 };
	CatModel->RightPelvis[7] =
		((Player->parentedLocalRightPelvis - CatModel->RightPelvis[5]) + CatModel->RightPelvis[5])
		+ v3{ 0.5f, 0, -0.5f };
	/****************************************************************************************************/
	CatModel->RightKnee[0] =
		((Player->parentedLocalRightKnee - CatModel->RightKnee[0]) + CatModel->RightKnee[0])
		+ v3{ 1, 0, 0 };
	CatModel->RightKnee[1] =
		((Player->parentedLocalRightKnee - CatModel->RightKnee[1]) + CatModel->RightKnee[1])
		+ v3{ 0.5f, 0, 0.5f };
	CatModel->RightKnee[2] =
		((Player->parentedLocalRightKnee - CatModel->RightKnee[2]) + CatModel->RightKnee[2])
		+ v3{ 0, 0, 1 };
	CatModel->RightKnee[3] =
		((Player->parentedLocalRightKnee - CatModel->RightKnee[3]) + CatModel->RightKnee[3])
		+ v3{ -0.5f, 0, 0.5f };
	CatModel->RightKnee[4] =
		((Player->parentedLocalRightKnee - CatModel->RightKnee[4]) + CatModel->RightKnee[4])
		+ v3{ -1.0f, 0, .0f };
	CatModel->RightKnee[5] =
		((Player->parentedLocalRightKnee - CatModel->RightKnee[5]) + CatModel->RightKnee[5])
		+ v3{ -0.5f, 0, -0.5f };
	CatModel->RightKnee[6] =
		((Player->parentedLocalRightKnee - CatModel->RightKnee[5]) + CatModel->RightKnee[5])
		+ v3{ 0, 0, -1 };
	CatModel->RightKnee[7] =
		((Player->parentedLocalRightKnee - CatModel->RightKnee[5]) + CatModel->RightKnee[5])
		+ v3{ 0.5f, 0, -0.5f };
	/*********************************************************************************************************/
	CatModel->RightFoot[0] =
		((Player->parentedLocalRightFoot - CatModel->RightFoot[0]) + CatModel->RightFoot[0])
		+ v3{ 1, 0, 0 };
	CatModel->RightFoot[1] =
		((Player->parentedLocalRightFoot - CatModel->RightFoot[1]) + CatModel->RightFoot[1])
		+ v3{ 0, 1, 0 };
	CatModel->RightFoot[2] =
		((Player->parentedLocalRightFoot - CatModel->RightFoot[2]) + CatModel->RightFoot[2])
		+ v3{ 0, 0, 1 };
	CatModel->RightFoot[3] =
		((Player->parentedLocalRightFoot - CatModel->RightFoot[3]) + CatModel->RightFoot[3])
		+ v3{ -1, 0, 0 };
	CatModel->RightFoot[4] =
		((Player->parentedLocalRightFoot - CatModel->RightFoot[4]) + CatModel->RightFoot[4])
		+ v3{ 0, -1, 0 };
	CatModel->RightFoot[5] =
		((Player->parentedLocalRightFoot - CatModel->RightFoot[5]) + CatModel->RightFoot[5])
		+ v3{ 0, 0, -1 };

}
//internal_function void
//DrawLetter(game_offscreen_buffer *Buffer, letter_color_array LetterArray, int LetterX, int LetterY)
//{//bbggrrxx	
//	real32 nrow = 0;
//	real32 ncol = 0;
//	int8 n = 0;
//	uint32 Color = LetterArray.LetterColorCode[0];
//
//	uint8 *Column = (
//		(uint8 *)Buffer->Memory
//		+ LetterX * Buffer->BytesPerPixel
//		+ LetterY * Buffer->Pitch);
//	for (int X = LetterX; X < LetterX + 4; X = X+1)//4/8 mukszik 8/4 nem mert a for belépései mások...
//	{
//		uint32 *Pixel = (uint32 *)Column;
//		for (int Y = LetterY; Y < LetterY + 8; ++Y)
//		{
//			n = (int8)nrow;
//			uint32 Color = LetterArray.LetterColorCode[n];
//			*Pixel++ = Color;
//			nrow += 0.5f;	//itt kell az X el operálni		
//		}
//		Column = Column + 2 * Buffer->Pitch;		
//	}	
//}
////Line drawing function pipeline.
///**************************************************************************Line drawer*/
//inline void PixelColor(game_offscreen_buffer *Buffer, int x, int y)
//{
//	uint8 *PixelToColor = (
//		(uint8 *)Buffer->Memory
//		+ x * Buffer->BytesPerPixel
//		+ y * Buffer->Pitch);
//	uint32 *Pixel = (uint32 *)PixelToColor;
//	uint32 Color = 0xFFFFFFFF;
//	*Pixel++ = Color;
//}
//void LineDrawerI(
//	game_offscreen_buffer *Buffer,
//	int x, int y,
//	real64 errorAB, real64 slopeAB,
//	real64 Bx
//)
//{//msdn color list https://msdn.microsoft.com/en-us/library/system.drawing.color(v=vs.110).aspx	
//	for (
//			x = x;
//			x < (int)Bx;
//			x = x + 1
//		)		
//	{	
//		//ezekre a helyekre be lehetne építeni a szomszédos feltételeket, 
//		//hogy ki tudjon lépni félúton, de lehet nem éri meg ezzle lassítan ia függvényt
//		if (x > 0 && x < 640 && y > 0 && y < 480)
//		{
//			PixelColor(Buffer, x, y);
//		}		
//		errorAB = errorAB + slopeAB;
//		if (errorAB > 0.5)//az egészet át kell írni?
//		{
//			y = y + 1;	//we have an Y wise upside-down buffer		
//			errorAB -= 1.0;
//		}		
//	}
//}
//void LineDrawerII(
//	game_offscreen_buffer *Buffer,
//	int y, int x,
//	real64 errorBA, real64 slopeBA,
//	real64 By
//)
//{
//	for (
//		y = y;
//		y < (int)By;
//		y = y + 1
//		)
//	{
//		if (x > 0 && x < 640 && y > 0 && y < 480)
//		{
//			PixelColor(Buffer, x, y);
//		}
//		errorBA = errorBA + slopeBA;
//		if (errorBA > 0.5)//az egészet át kell írni?
//		{
//			x = x + 1;	//we have an Y wise upside-down buffer		
//			errorBA -= 1.0;
//		}
//	}
//}
//void LineDrawerIII(
//	game_offscreen_buffer *Buffer,
//	int y, int x,
//	real64 errorBA, real64 slopeBA,
//	real64 By
//)
//{
//	for (
//		y = y;
//		y < (int)By;
//		y = y + 1
//		)
//	{
//		if ((-x) > 0 && (-x) < 640 && y > 0 && y < 480)
//		{
//			PixelColor(Buffer, (-x), y);
//		}
//		errorBA = errorBA + slopeBA;
//		if (errorBA > 0.5)//az egészet át kell írni?
//		{
//			x = x + 1;	//we have an Y wise upside-down buffer		
//			errorBA -= 1.0;
//		}
//	}
//}
//void LineDrawerIV(
//	game_offscreen_buffer *Buffer,
//	int x, int y,
//	real64 errorAB, real64 slopeAB,
//	real64 Bx
//)
//{
//	for (
//		x = x;
//		x < (int)Bx;
//		x = x + 1
//		)
//	{
//		if ((-x) > 0 && (-x) < 640 && y > 0 && y < 480)
//		{
//			PixelColor(Buffer, (-x), y);
//		}
//		errorAB = errorAB + slopeAB;
//		if (errorAB > 0.5)//az egészet át kell írni?
//		{
//			y = y + 1;	//we have an Y wise upside-down buffer		
//			errorAB -= 1.0;
//		}
//	}
//}
//void LineDrawerV(
//	game_offscreen_buffer *Buffer,
//	int x, int y,
//	real64 errorAB, real64 slopeAB,
//	real64 Bx
//)
//{
//	for (
//		x = x;
//		x < (int)Bx;
//		x = x + 1
//		)
//	{
//		if ((-x) > 0 && (-x) < 640 && (-y) > 0 && (-y) < 480)
//		{
//			PixelColor(Buffer, (-x), (-y));
//		}
//		errorAB = errorAB + slopeAB;
//		if (errorAB > 0.5)//az egészet át kell írni?
//		{
//			y = y + 1;	//we have an Y wise upside-down buffer		
//			errorAB -= 1.0;
//		}
//	}
//}
//void LineDrawerVI(
//	game_offscreen_buffer *Buffer,
//	int y, int x,
//	real64 errorBA, real64 slopeBA,
//	real64 By
//)
//{
//	for (
//		y = y;
//		y < (int)By;
//		y = y + 1
//		)
//	{
//		if ((-x) > 0 && (-x) < 640 && (-y) > 0 && (-y) < 480)
//		{
//			PixelColor(Buffer, (-x), (-y));
//		}
//		errorBA = errorBA + slopeBA;
//		if (errorBA > 0.5)//az egészet át kell írni?
//		{
//			x = x + 1;	//we have an Y wise upside-down buffer		
//			errorBA -= 1.0;
//		}
//	}
//}
//void LineDrawerVII(
//	game_offscreen_buffer *Buffer,
//	int y, int x,
//	real64 errorBA, real64 slopeBA,
//	real64 By
//)
//{
//	for (
//		y = y;
//		y < (int)By;
//		y = y + 1
//		)
//	{
//		if (x > 0 && x < 640 && (-y) > 0 && (-y) < 480)
//		{
//			PixelColor(Buffer, x, (-y));
//		}
//		errorBA = errorBA + slopeBA;
//		if (errorBA > 0.5)//az egészet át kell írni?
//		{
//			x = x + 1;	//we have an Y wise upside-down buffer		
//			errorBA -= 1.0;
//		}
//	}
//}
//void LineDrawerVIII(
//	game_offscreen_buffer *Buffer,
//	int x, int y,
//	real64 errorAB, real64 slopeAB,
//	real64 Bx
//)
//{//msdn color list https://msdn.microsoft.com/en-us/library/system.drawing.color(v=vs.110).aspx	
//	for (
//		x = x;
//		x < (int)Bx;
//		x = x + 1
//		)
//	{
//		if (x > 0 && x < 640 && (-y) > 0 && (-y) < 480)
//		{
//			PixelColor(Buffer, x, (-y));
//		}
//		errorAB = errorAB + slopeAB;
//		if (errorAB > 0.5)//az egészet át kell írni?
//		{
//			y = y + 1;	//we have an Y wise upside-down buffer		
//			errorAB -= 1.0;
//		}
//	}
//}
//internal_function void 
//PreparationForLineDrawer(
//	game_offscreen_buffer *Buffer, 
//	v3 pointA, v3 pointB, 
//	camera_buffer *CameraBuffer
//)
//{ 
//	v3 inA = pointA;
//	v3 inB = pointB;
//	/************Alignment of drawer with buffer*/
//	//itt x/y fel volt cserélve
//	inA.x = pointA.x;
//	inA.y = pointA.y;
//	inB.x = pointB.x;
//	inB.y = pointB.y;
//	/**************************************************************************PROJECTION*/		
//	inA.x -= (CameraBuffer->PlayerBuffer.x + 320);//fel voltak cserélv
//	inA.y -= (CameraBuffer->PlayerBuffer.y + 240);
//	inB.x -= (CameraBuffer->PlayerBuffer.x + 320);
//	inB.y -= (CameraBuffer->PlayerBuffer.y + 240);
//	inA.x /= (1.00f - inA.z*0.001f);//fel voltak cserélv
//	inA.y /= (1.00f - inA.z*0.001f);
//	inB.x /= (1.00f - inB.z*0.001f);
//	inB.y /= (1.00f - inB.z*0.001f);	
//	inA.x += 320;//fel voltak cserélv
//	inA.y += 240;
//	inB.x += 320;
//	inB.y += 240;
//	/***********************************************************************************/	
//	//inA.x /= (1.001f - inA.z*0.001f);//fel voltak cserélv
//	//inA.y /= (1.001f - inA.z*0.001f);
//	//inB.x /= (1.001f - inB.z*0.001f);
//	//inB.y /= (1.001f - inB.z*0.001f);
//	/*****************************/
//	real64 deltaY;
//	real64 deltaX;
//	deltaY = inB.y - inA.y;//y,y
//	deltaX = inB.x - inA.x;//x,x
//
//	int y = (int)inA.y;//y
//	int x = (int)inA.x;//x
//
//	int Bx = (int)inB.x;
//	int By = (int)inB.y;
//
//	real64 slopeAB = AbsVal(deltaY / deltaX);
//	real64 slopeBA = AbsVal(deltaX / deltaY);
//	real64 errorAB = slopeAB - 0.5;
//	real64 errorBA = slopeBA - 0.5;
//
//	//real64 distanceAB = mathFunctions.DistanceBetweenTwoPoints(inA, inB);
//
//	//Bresenham's line algorithm. 
//	if (
//		inA.x > 640 && inB.x > 640 ||
//		inA.x < 0 && inB.x < 0 ||
//		inA.y > 480 && inB.x > 480 ||
//		inA.y < 0 && inB.y < 0
//		)//ÍRD ÁT ne returnnel hanem foglald bele az alsó részt és true nak kelljen lennie
//	{
//		return;
//	}
//	if (deltaY >= 0 && deltaX >= deltaY && deltaX >= 0)//AZ ELLENTÉTES OLDALON KI LEHET VENNI AZ =-t H NE KEZELJÜK KÉTSZER A FELTÉTELT
//	{ //I
//		//LineDrawer(Buffer, x, y, errorAB, slopeAB, inB.x, x, Bx, By);
//		LineDrawerI(Buffer, x, y, errorAB, slopeAB, inB.x);
//	}
//	if (deltaY >= 0 && deltaX <= deltaY && deltaX >= 0)
//	{ //II
//		//LineDrawer(Buffer, y, x, errorBA, slopeBA, inB.y, -x, Bx, By);
//		LineDrawerII(Buffer, y, x, errorBA, slopeBA, inB.y);
//	}
//	if (deltaY >= 0 && AbsVal(deltaX) <= deltaY && deltaX <= 0)
//	{ //III
//		//LineDrawer(Buffer, y, -x, errorBA, slopeBA, inB.y, -x, -Bx, By);
//		LineDrawerIII(Buffer, y, -x, errorBA, slopeBA, inB.y);
//	}
//	if (deltaY >= 0 && AbsVal(deltaX) >= deltaY && deltaX <= 0)
//	{ //IV
//		//LineDrawer(Buffer, -x, y, errorAB, slopeAB, -inB.x, -x, -Bx, By);
//		LineDrawerIV(Buffer, -x, y, errorAB, slopeAB, -inB.x);
//	}
//	if (deltaY <= 0 && AbsVal(deltaX) >= AbsVal(deltaY) && deltaX <= 0)
//	{ //V
//		//LineDrawer(Buffer, -x, -y, errorAB, slopeAB, -inB.x, -x, -Bx, -By);
//		LineDrawerV(Buffer, -x, -y, errorAB, slopeAB, -inB.x);
//	}
//	if (deltaY <= 0 && AbsVal(deltaX) <= AbsVal(deltaY) && deltaX <= 0)
//	{ //VI
//		//LineDrawer(Buffer, -y, -x, errorBA, slopeBA, -inB.y, x, -Bx, -By);
//		LineDrawerVI(Buffer, -y, -x, errorBA, slopeBA, -inB.y);
//	}
//	if (deltaY <= 0 && deltaX <= AbsVal(deltaY) && deltaX >= 0)
//	{ //VII
//		//LineDrawer(Buffer, -y, x, errorBA, slopeBA, -inB.y, x, Bx, -By);
//		LineDrawerVII(Buffer, -y, x, errorBA, slopeBA, -inB.y);
//	}
//	if (deltaY <= 0 && deltaX >= AbsVal(deltaY) && deltaX >= 0)
//	{ //VIII
//		//LineDrawer(Buffer, x, -y, errorAB, slopeAB, inB.x, x, Bx, -By);
//		LineDrawerVIII(Buffer, x, -y, errorAB, slopeAB, inB.x);
//	}
//}
/*********************************************************************TEXT DRAW*/
internal_function void
UserInterface(game_offscreen_buffer *Buffer, 
	game_input *Input, game_controller_input *Controller,
	write_cursor *WriteCursor
)
{
	letter_codes LetterCode = {};
	letter_color_array Code = {};

	//WriteCursor->WriteCursorX = 40;
	//WriteCursor->WriteCursorY = 40;
	int VKCodeToProcess[256];
	int n;

	if (Controller->Right.EndedDown)
	{
		VKCodeToProcess[n] = 'D';
		WriteCursor->LetterBuffer[n] = 'D';
		n += 1;
		//kell egy ][ int buffer, amit aztán ki is rajzolok forral a végén is..
		//itt aztán mindig léptetjü ka write cursort, amit majd el kell raktározni valahol....
	}
	else if (Controller->T.EndedDown)
	{
		VKCodeToProcess[n] = 'T';
		WriteCursor->LetterBuffer[n] = 'T';
		n += 1;
	}
	else
	{
		VKCodeToProcess[n] = 'E';
	}
	//VKCodeToProcess = 'Q';
	for (n; n < 100; n++)
	{
		LetterCode.GetLetter(&VKCodeToProcess[n]);
		Code = LetterCode.GetLetter(&VKCodeToProcess[n]);

		DrawLetter(Buffer, Code, WriteCursor->WriteCursorX, WriteCursor->WriteCursorY);//!!!!!!!!
		DrawLetter(Buffer, Code, WriteCursor->WriteCursorX, WriteCursor->WriteCursorY + 1);
	}
}
/**********************************************************************************************/
void VectorCollectionForPixelTinting(//ez a karakter kirajzolása volt
	game_offscreen_buffer *Buffer, player_instance *Player,
	camera_buffer *CameraBuffer
	)
{	
	/*PreparationForLineDrawer(
		Buffer, Player->localCharacterOriginPoint, Player->parentedLocalLeftPelvis, CameraBuffer
	);
	PreparationForLineDrawer(
		Buffer, Player->parentedLocalLeftPelvis, Player->parentedLocalLeftKnee, CameraBuffer
	);
	PreparationForLineDrawer(
		Buffer, Player->parentedLocalLeftKnee, Player->parentedLocalLeftFoot, CameraBuffer
	);
	PreparationForLineDrawer(
		Buffer, Player->localCharacterOriginPoint, Player->parentedLocalRightPelvis, CameraBuffer
	);
	PreparationForLineDrawer(
		Buffer, Player->parentedLocalRightPelvis, Player->parentedLocalRightKnee, CameraBuffer
	);
	PreparationForLineDrawer(
		Buffer, Player->parentedLocalRightKnee, Player->parentedLocalRightFoot, CameraBuffer
	);
	PreparationForLineDrawer(
		Buffer, Player->localCharacterOriginPoint, Player->parentedLocalChest, CameraBuffer
	);
	PreparationForLineDrawer(
		Buffer, Player->parentedLocalChest, Player->parentedLocalHead, CameraBuffer
	);
	PreparationForLineDrawer(
		Buffer, Player->parentedLocalChest, Player->parentedLocalLeftShoulder, CameraBuffer
	);
	PreparationForLineDrawer(
		Buffer, Player->parentedLocalLeftShoulder, Player->parentedLocalLeftElbow, CameraBuffer
	);
	PreparationForLineDrawer(
		Buffer, Player->parentedLocalLeftElbow, Player->parentedLocalLeftHand, CameraBuffer
	);
	PreparationForLineDrawer(
		Buffer, Player->parentedLocalChest, Player->parentedLocalRightShoulder, CameraBuffer
	);
	PreparationForLineDrawer(
		Buffer, Player->parentedLocalRightShoulder, Player->parentedLocalRightElbow, CameraBuffer
	);
	PreparationForLineDrawer(
		Buffer, Player->parentedLocalRightElbow, Player->parentedLocalRightHand, CameraBuffer
	);*/
}
/**********************************************************************************************/
void LocalPlayerAssembleClients(game_offscreen_buffer *Buffer, client_parts *ClientParts)
{
	//here a list is made of all the player points that have to be drawn	
	/********************************************************************************************/
	/*v3 inA = origin;
	inA.x = inA.x * 100 + xScreenAdjust;
	inA.y = inA.y * 100 + yScreenAdjust;
	v3 forIn = v3{ inA.x, inA.y, 0 };
	/********************************************************************************************/	
	v3 localOrigin = { ClientParts->OriginX, ClientParts-> OriginY, ClientParts-> OriginZ, };
	v3 parentedLocalLPelvis = { ClientParts->LPelvisX, ClientParts-> LPelvisY, ClientParts-> LPelvisZ, };
	v3 parentedLocalLKnee = { ClientParts->LKneeX, ClientParts-> LKneeY, ClientParts-> LKneeZ, };
	v3 parentedLocalLFoot = { ClientParts->LFootX, ClientParts-> LFootY, ClientParts-> LFootZ, };
	v3 parentedLocalRPelvis = { ClientParts->RPelvisX, ClientParts-> RPelvisY, ClientParts-> RPelvisZ, };
	v3 parentedLocalRKnee = { ClientParts->RKneeX, ClientParts-> RKneeY, ClientParts-> RKneeZ, };
	v3 parentedLocalRFoot = { ClientParts->RFootX, ClientParts-> RFootY, ClientParts-> RFootZ, };
	v3 parentedLocalHead = { ClientParts->HeadX, ClientParts-> HeadY, ClientParts-> HeadZ, };
	v3 parentedLocalChest = { ClientParts->ChestX, ClientParts-> ChestY, ClientParts-> ChestZ, };
	v3 parentedLocalLShoulder = { ClientParts->LShoulderX, ClientParts-> LShoulderY, ClientParts-> LShoulderZ, };
	v3 parentedLocalLElbow = { ClientParts->LElbowX, ClientParts-> LElbowY, ClientParts-> LElbowZ, };
	v3 parentedLocalLHand = { ClientParts->LHandX, ClientParts-> LHandY, ClientParts-> LHandZ, };
	v3 parentedLocalRShoulder = { ClientParts->RShoulderX, ClientParts-> RShoulderY, ClientParts-> RShoulderZ, };
	v3 parentedLocalRElbow = { ClientParts->RElbowX, ClientParts-> RElbowY, ClientParts-> RElbowZ, };
	v3 parentedLocalRHand = { ClientParts->RHandX, ClientParts-> RHandY, ClientParts-> RHandZ, };
	//
	//HERE YOU DRAW NETTEST CLIENT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	DrawRectangle(Buffer, ClientParts->OriginX, ClientParts->OriginY);
	
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//DrawRectangle(Buffer, 150.0f, 150.0f);
	//PreparationForLineDrawer(Buffer, localOrigin, parentedLocalLPelvis);	
}
internal_function void 
LocalPlayerAssembleLocalPlayer(local_player_parts *LocalPlayerParts, player_instance *Player)
{//dissassemble
	LocalPlayerParts->OriginX = Player->localCharacterOriginPoint.x; 
	LocalPlayerParts->OriginY = Player->localCharacterOriginPoint.y; 
	LocalPlayerParts->OriginZ = Player->localCharacterOriginPoint.z;
	LocalPlayerParts->LPelvisX = Player->parentedLocalLeftPelvis.x; 
	LocalPlayerParts->LPelvisY = Player->parentedLocalLeftPelvis.y;
	LocalPlayerParts->LPelvisZ = Player->parentedLocalLeftPelvis.z;
	LocalPlayerParts->LKneeX = Player->parentedLocalLeftKnee.x;
	LocalPlayerParts->LKneeY = Player->parentedLocalLeftKnee.y;
	LocalPlayerParts->LKneeZ = Player->parentedLocalLeftKnee.z;
	LocalPlayerParts->LFootX = Player->parentedLocalLeftFoot.x;
	LocalPlayerParts->LFootY = Player->parentedLocalLeftFoot.y;
	LocalPlayerParts->LFootZ = Player->parentedLocalLeftFoot.z;
	LocalPlayerParts->RPelvisX = Player->parentedLocalRightPelvis.x;
	LocalPlayerParts->RPelvisY = Player->parentedLocalRightPelvis.y;
	LocalPlayerParts->RPelvisZ = Player->parentedLocalRightPelvis.z;
	LocalPlayerParts->RKneeX = Player->parentedLocalRightKnee.x;
	LocalPlayerParts->RKneeY = Player->parentedLocalRightKnee.y;
	LocalPlayerParts->RKneeZ = Player->parentedLocalRightKnee.z;
	LocalPlayerParts->RFootX = Player->parentedLocalRightFoot.x;
	LocalPlayerParts->RFootY = Player->parentedLocalRightFoot.y;
	LocalPlayerParts->RFootZ = Player->parentedLocalRightFoot.z;
	LocalPlayerParts->HeadX = Player->parentedLocalHead.x;
	LocalPlayerParts->HeadY = Player->parentedLocalHead.y;
	LocalPlayerParts->HeadZ = Player->parentedLocalHead.z;
	LocalPlayerParts->ChestX = Player->parentedLocalChest.x;
	LocalPlayerParts->ChestY = Player->parentedLocalChest.y;
	LocalPlayerParts->ChestZ = Player->parentedLocalChest.z;
	LocalPlayerParts->LShoulderX = Player->parentedLocalLeftShoulder.x;
	LocalPlayerParts->LShoulderY = Player->parentedLocalLeftShoulder.y;
	LocalPlayerParts->LShoulderZ = Player->parentedLocalLeftShoulder.z;
	LocalPlayerParts->LElbowX = Player->parentedLocalLeftElbow.x;
	LocalPlayerParts->LElbowY = Player->parentedLocalLeftElbow.y;
	LocalPlayerParts->LElbowZ = Player->parentedLocalLeftElbow.z;
	LocalPlayerParts->LHandX = Player->parentedLocalLeftHand.x;
	LocalPlayerParts->LHandY = Player->parentedLocalLeftHand.y;
	LocalPlayerParts->LHandZ = Player->parentedLocalLeftHand.z;
	LocalPlayerParts->RShoulderX = Player->parentedLocalRightShoulder.x;
	LocalPlayerParts->RShoulderY = Player->parentedLocalRightShoulder.y;
	LocalPlayerParts->RShoulderZ = Player->parentedLocalRightShoulder.z;
	LocalPlayerParts->RElbowX = Player->parentedLocalRightElbow.x;
	LocalPlayerParts->RElbowY = Player->parentedLocalRightElbow.y;
	LocalPlayerParts->RElbowZ = Player->parentedLocalRightElbow.z;
	LocalPlayerParts->RHandX = Player->parentedLocalRightHand.x;
	LocalPlayerParts->RHandY = Player->parentedLocalRightHand.y;
	LocalPlayerParts->RHandZ = Player->parentedLocalRightHand.z;
	//		
}
/**************************************************************************************GET NETWORK*/
void LocalCharacterInitialization(player_instance *Player, camera_buffer *CameraBuffer)
{//T-Pose
	CameraBuffer->PlayerBuffer = v3{0.0f, 200.0f, 0.0f};
	Player->playerBuffer = CameraBuffer->PlayerBuffer;
	Player->localCharacterOriginPoint = v3{ 0,0,0 };
	//Player->localCharacterOriginPoint = CameraBuffer->PlayerBuffer - Player->localCharacterOriginPoint; //v3{150,150,0};//000

	v3 rightPelvisOffset = { -5.0f, -10.0f, 0.0f };//Parent:Origin
	v3 rightKneeOffset = { -5.0f, -20.0f, 0 };//Parent:RightPelvis
	v3 rightFootOffset = { -5.0f, -30.0f, 0 };//Parent:RightKnee
	v3 leftPelvisOffset = { 5.0f, -10.0f, 0 };//!!!!!!!!!!!!!!!!parentingChain 5,-5
	v3 leftKneeOffset = { 5.0f, -20.0f, 0 };//!!!!
	v3 leftFootOffset = { 5.0f, -30.0f, 0 };
	/****************************************************************************************************/
	v3 headOffset = { 0.0f, 20.0f, 0 };
	v3 chestOffset = { 0.0f, 15.0f, 0 };
	v3 leftShoulderOffset = { 7.5f, 10.0f, 0.0f };
	v3 leftElbowOffset = { 10.0f, 0.0f, 0.0f };
	v3 leftHandOffset = { 10.0f, -10.0f, 0.0f };
	v3 rightShoulderOffset = { -7.5f, 10.0f, 0.0f };
	v3 rightElbowOffset = { -10.0f, 0.0f, 0.0f };
	v3 rightHandOffset = { -10.0f, -10.0f, 0.0f };
	/****************************************************************************************************/
	//Player->playerBuffer;
	//Player->playerCam = 
	Player->parentedLocalRightPelvis = Player->localCharacterOriginPoint - rightPelvisOffset;//!!!!!!!!!!
	Player->parentedLocalRightKnee = Player->localCharacterOriginPoint - rightKneeOffset;
	Player->parentedLocalRightFoot = Player->localCharacterOriginPoint - rightFootOffset;
	Player->parentedLocalLeftPelvis = Player->localCharacterOriginPoint - leftPelvisOffset;
	/*!!!*/Player->parentedLocalLeftKnee = Player->localCharacterOriginPoint - leftKneeOffset;
	Player->parentedLocalLeftFoot = Player->localCharacterOriginPoint - leftFootOffset;//redundants
																									/****************************************************************************************************/
	Player->parentedLocalHead = Player->localCharacterOriginPoint - headOffset;
	Player->parentedLocalChest = Player->localCharacterOriginPoint - chestOffset;
	Player->parentedLocalLeftShoulder = Player->localCharacterOriginPoint - leftShoulderOffset;
	Player->parentedLocalLeftElbow = Player->localCharacterOriginPoint - leftElbowOffset;
	Player->parentedLocalLeftHand = Player->localCharacterOriginPoint - leftHandOffset;
	Player->parentedLocalRightShoulder = Player->localCharacterOriginPoint - rightShoulderOffset;
	Player->parentedLocalRightElbow = Player->localCharacterOriginPoint - rightElbowOffset;
	Player->parentedLocalRightHand = Player->localCharacterOriginPoint - rightHandOffset;
	/********************************************************************Axes init*/
	//There is a mistake here, foot IS KNEE, AND HAND IS ELBOW... -> and up the chain
	Player->AxisOrigin = v3{ 0, 1, 0 };
	Player->AxisRPelvis = v3{ 0, 0, 1 };
	Player->AxisRKnee = v3{ 1, 0, 0 };
	Player->AxisRFoot = v3{ 1, 0, 0 };//!!!does it rotate at all
	Player->AxisLPelvis = v3{ 0, 0, 1 };
	Player->AxisLKnee = v3{ 1, 0, 0 };
	Player->AxisLFoot = v3{ 1, 0, 0 };//!!!
	Player->AxisHead = v3{ 0, 1, 0 };
	Player->AxisChest = v3{ 0, 1, 0 };
	Player->AxisRShoulder = v3{ 0, 0, 1 };
	Player->AxisRElbow = v3{ 1, 0, 0 };
	Player->AxisRHand = v3{ 1, 0, 0 };//
	Player->AxisLShoulder = v3{ 0, 0, 1 };
	Player->AxisLElbow = v3{ 1, 0, 0 };
	Player->AxisLHand = v3{ 1, 0, 0 };//	
	/******************************************************************************/
}
/************************************************************************PLAYER MOVEMENT HEADER*/
v3 childDiff(v3 parentedJointIn, v3 childJointIn)
{
	v3Ops V3Ops = {};
	v3 childDiff;
	childDiff = V3Ops.v3Sub(parentedJointIn, childJointIn);//nem tudjuk mindkettõt fuck
	return childDiff;
}
v3 QJointRot(v3 Joint, real32 Angle, v3 Axis)
{
	v3 Result;
	quaternion_operations QRot = {};
	Result = QRot.QuaternionPointRotation(Joint, Angle, Axis);
	return Result;
}
v3 childParenting(v3 childJointIn, v3 parentedJointIn, v3 childDiffIn)
{
	v3Ops V3Ops = {};
	childJointIn = V3Ops.v3Sub(parentedJointIn, childDiffIn);
	return (childJointIn);
}
void BallCollision(ball *Ball)
{
	v3 NormalVector;
	v3 IncomingVector = IncomingVector -2* dot(IncomingVector, NormalVector) * NormalVector;

}
void SquareCollider(map *Map, player_instance *Player)
{
	for (int n = 0; n < Map->MapPolyCount; n++)
	{
		//int G = -1;
		//G = G + (AbsVal(G));
		v3 LocalA = Map->MapPolygons[n].Points[0];
		v3 LocalB = Map->MapPolygons[n].Points[1];
		v3 LocalC = Map->MapPolygons[n].Points[2];
		v3 LocalD = Map->MapPolygons[n].Points[3];
		v3 LocalP = Player->parentedLocalRightFoot;

		LocalP.x = 
			(
				dot
				(
					(LocalP - LocalA), 
					normal(v3{ 0,0,0 }, (LocalB - LocalA))
				)
			);// +LocalA.x;
																					//
		v3 LocalP2 = Player->parentedLocalRightFoot;//!!!
		v3 BADiff = LocalB - LocalA;
		v3 BANorm = normal(v3{ 0,0,0 }, BADiff);
		v3 LocalBA = v3{
			-(BANorm.y),
			(BANorm.x),
			(BANorm.z)
		};
		//localA t is meg kell cserélni -xy
		LocalP2.y = (dot((LocalP2 - LocalA), LocalBA));// +LocalA.y;

		LocalB = LocalB - LocalA;
		if (
			//elõre kéne valami height/width
			LocalP.x >= 0 && LocalP.x <= LocalB.x
			&& LocalP2.y <= 10 && LocalP2.y >= 0//ez a 0 nem nulla????
			)
		{
			Player->originDirection =
				Player->originDirection - dot(Player->originDirection, LocalBA)*LocalBA;
			Player->isRightFootColliding = true;
			break;
			//return;
		}
		else
		{
			Player->isRightFootColliding = false;
		}
	}

	//leftfoot
	for (int n = 0; n < Map->MapPolyCount; n++)
	{
		//int G = -1;
		//G = G + (AbsVal(G));
		v3 LocalA = Map->MapPolygons[n].Points[0];
		v3 LocalB = Map->MapPolygons[n].Points[1];
		v3 LocalC = Map->MapPolygons[n].Points[2];
		v3 LocalD = Map->MapPolygons[n].Points[3];
		v3 LocalP = Player->parentedLocalLeftFoot;

		LocalP.x = (dot((LocalP - LocalA), normal(v3{ 0,0,0 }, (LocalB - LocalA))));// +LocalA.x;
																					//
		v3 LocalP2 = Player->parentedLocalLeftFoot;//!!!
		v3 BADiff = LocalB - LocalA;
		v3 BANorm = normal(v3{ 0,0,0 }, BADiff);
		v3 LocalBA = v3{
			-(BANorm.y),
			(BANorm.x),
			(BANorm.z)
		};
		//localA t is meg kell cserélni -xy
		LocalP2.y = (dot((LocalP2 - LocalA), LocalBA));// +LocalA.y;

		LocalB = LocalB - LocalA;
		if (
			//elõre kéne valami height/width
			LocalP.x >= 0 && LocalP.x <= LocalB.x
			&& LocalP2.y <= 10 && LocalP2.y >= 0//ez a 0 nem nulla????
			)
		{
			Player->originDirection =
				Player->originDirection - dot(Player->originDirection, LocalBA)*LocalBA;
			Player->isLeftFootColliding = true;
			break;
			return;
		}
		else
		{
			Player->isLeftFootColliding = false;
		}
	}
}
void Collisions(player_instance *Player, map *Map)
{
	v3Ops V3Ops = {};
	intersection_of_2lines LineIntersect = {};
	v3 coll1ToAdd = v3{ -300.0f, 30.00f, 0.0f };//coll 75
	v3 coll2ToAdd = v3{ +300.0f, 30.00f, 0.0f };
	v3 collPoint1RFoot = LineIntersect.IntersectionOfTwoLinesIn3d(coll1ToAdd, coll2ToAdd, Player->parentedLocalRightFoot);

	SquareCollider(Map, Player);	
}
v3 LocalOriginMovement(player_instance *Player, cat_model *CatModel, real32 Force)
{
	v3Ops V3Ops = {};

	//Player->originDirection = movDir;
	Player->originForce = Force;
	real32 x = Player->originDirection.x;
	real32 y = Player->originDirection.y;

	v3 ModelDiff[64];
	for (int n = 0; n < 8; n++)
	{
		ModelDiff[n] = childDiff(Player->localCharacterOriginPoint , CatModel->RightPelvis[n]);
	}
	for (int n = 8; n < 16; n++)
	{
		ModelDiff[n] = Player->localCharacterOriginPoint - CatModel->RightKnee[n-8];
	}
	for (int n = 16; n < 24; n++)
	{
		ModelDiff[n] = Player->localCharacterOriginPoint - CatModel->RightFoot[n-16];
	}

	v3 childDiffOut15 = childDiff(Player->localCharacterOriginPoint, Player->playerBuffer);

	v3 childDiffOut1 = childDiff(Player->localCharacterOriginPoint, Player->parentedLocalLeftPelvis);
	v3 childDiffOut2 = childDiff(Player->localCharacterOriginPoint, Player->parentedLocalRightPelvis);
	/*!!!*/v3 childDiffOut3 = childDiff(Player->parentedLocalLeftPelvis, Player->parentedLocalLeftKnee);
	v3 childDiffOut4 = childDiff(Player->parentedLocalRightPelvis, Player->parentedLocalRightKnee);
	v3 childDiffOut5 = childDiff(Player->parentedLocalLeftKnee, Player->parentedLocalLeftFoot);
	v3 childDiffOut6 = childDiff(Player->parentedLocalRightKnee, Player->parentedLocalRightFoot);
	/******************/
	v3 childDiffOut7 = childDiff(Player->localCharacterOriginPoint, Player->parentedLocalChest);
	v3 childDiffOut8 = childDiff(Player->parentedLocalChest, Player->parentedLocalHead);
	/******************/
	v3 childDiffOut9 = childDiff(Player->parentedLocalChest, Player->parentedLocalLeftShoulder);
	v3 childDiffOut10 = childDiff(Player->parentedLocalChest, Player->parentedLocalRightShoulder);
	v3 childDiffOut11 = childDiff(Player->parentedLocalLeftShoulder, Player->parentedLocalLeftElbow);
	v3 childDiffOut12 = childDiff(Player->parentedLocalRightShoulder, Player->parentedLocalRightElbow);
	v3 childDiffOut13 = childDiff(Player->parentedLocalLeftElbow, Player->parentedLocalLeftHand);
	v3 childDiffOut14 = childDiff(Player->parentedLocalRightElbow, Player->parentedLocalRightHand);
	/****************************************************************************************************/
	v3 movedOrigin = V3Ops.v3Add(Player->localCharacterOriginPoint, v3{ x, y, 0 });
	Player->localCharacterOriginPoint = V3Ops.v3Add(Player->localCharacterOriginPoint, v3{ x, y, 0 });
	//minden... localorigin - faszom
	//elég a pelvis? nem elég a pelvis?
	/****************************************************************************************************/
	for (int n = 0; n < 8; n++)
	{
		CatModel->RightPelvis[n] = childParenting(v3{ 0, 0, 0 }, Player->localCharacterOriginPoint, ModelDiff[n]);
	}
	for (int n = 8; n < 16; n++)
	{
		CatModel->RightKnee[n-8] = Player->localCharacterOriginPoint - ModelDiff[n];
	}
	for (int n = 16; n < 24; n++)
	{
		CatModel->RightFoot[n-16] = Player->localCharacterOriginPoint - ModelDiff[n];
	}

	Player->playerBuffer = childParenting(Player->playerBuffer, Player->localCharacterOriginPoint, childDiffOut15);

	Player->parentedLocalLeftPelvis = childParenting(Player->parentedLocalLeftPelvis, Player->localCharacterOriginPoint, childDiffOut1);
	Player->parentedLocalRightPelvis = childParenting(Player->parentedLocalRightPelvis, Player->localCharacterOriginPoint, childDiffOut2);
	/*!!!*/Player->parentedLocalLeftKnee = childParenting(Player->parentedLocalLeftKnee, Player->parentedLocalLeftPelvis, childDiffOut3);
	Player->parentedLocalRightKnee = childParenting(Player->parentedLocalRightKnee, Player->parentedLocalRightPelvis, childDiffOut4);
	Player->parentedLocalLeftFoot = childParenting(Player->parentedLocalLeftFoot, Player->parentedLocalLeftKnee, childDiffOut5);
	Player->parentedLocalRightFoot = childParenting(Player->parentedLocalRightFoot, Player->parentedLocalRightKnee, childDiffOut6);
	/******************/
	Player->parentedLocalChest = childParenting(Player->parentedLocalChest, Player->localCharacterOriginPoint, childDiffOut7);
	Player->parentedLocalHead = childParenting(Player->parentedLocalHead, Player->parentedLocalChest, childDiffOut8);
	/******************/
	Player->parentedLocalLeftShoulder = childParenting(Player->parentedLocalLeftShoulder, Player->parentedLocalChest, childDiffOut9);
	Player->parentedLocalRightShoulder = childParenting(Player->parentedLocalRightShoulder, Player->parentedLocalChest, childDiffOut10);
	Player->parentedLocalLeftElbow = childParenting(Player->parentedLocalLeftElbow, Player->parentedLocalLeftShoulder, childDiffOut11);
	Player->parentedLocalRightElbow = childParenting(Player->parentedLocalRightElbow, Player->parentedLocalRightShoulder, childDiffOut12);
	Player->parentedLocalLeftHand = childParenting(Player->parentedLocalLeftHand, Player->parentedLocalLeftElbow, childDiffOut13);
	Player->parentedLocalRightHand = childParenting(Player->parentedLocalRightHand, Player->parentedLocalRightElbow, childDiffOut14);
	/****************************************************************************************************/
	return movedOrigin;
}
/**********************************************************************************QUATERNION*/
void Physics(player_instance *Player)
{
	v3Ops V3Ops = {};
	//Every origin will have an amount of power reserve, which can be distributed into all directions
	//as a form of force. This will move the character, keep it from collapsing, etc.
	//Collision check is coming from the light script, in 3D the intersections of lines should be inspected, not the color of the pixel.
	real32 gravitationalForce = 10.0f;//global?
	real32 characterPower = 30.0f;//global?
								  /********************************************************************************Collision detection*/
								  //Collisions(Player);
								  /********************************************************************************************Gravity*/
	if (!Player->isRightFootColliding && !Player->isLeftFootColliding)
	{
		Player->originDirection = V3Ops.v3Add(Player->originDirection, v3{ 0, +0.02f, 0 });//applyGravity
		if (Player->originDirection.y > 0.6f)//maxFallingVelocity
		{
			Player->originDirection.y = 0.6f;
		}
		//a playert nem kell hurcolni mert úgyis pointerek majd egyi nstance ra mutatnak belül
	}
	else
	{
		//Player->originDirection.y = 0.0f;
		/********************************************************Friction*/
		if (Player->originDirection.x > 0.000001f || Player->originDirection.y > 0.000001f
			|| Player->originDirection.x < -0.000001f || Player->originDirection.y < -0.000001f
			&& Player->isAnyPartColliding
			)
		{//az ifet ki lehetne bõvíteni azzal h nyomom-e a gombot? nem valósághû
			Player->originDirection = v3
			{
				Player->originDirection.x * (0.95f /*- 0.9 * Player->nOfPartsColliding*/),
				Player->originDirection.y * (0.95f /*- 0.9 * Player->nOfPartsColliding*/),
				Player->originDirection.z * (0.95f /*- 0.9 * Player->nOfPartsColliding*/)
			};

			//LeftPelvisJoint(1f, localCharacterOriginPoint);
			//LeftKneeJoint(1f, parentedLocalLeftPelvis);
			//LeftFootJoint(1f, parentedLocalLeftKnee);
		}
		else
		{
			//Player->originDirection = v3{ 0, 0, 0 };
		}
	}
	//LocalOriginMovement(Player, 1);
}
void QParentedJointMovement(player_instance *Player, cat_model *CatModel)
{	
	/*************************************************************************************/
	v3Ops V3Ops = {};
	//LocalOriginMovement(Player, 1);	
	/********************************************************************************Origin*/
	//
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!ORIGIN
	//
	//Player->localCharacterOriginPoint = QJointRot(Player->localCharacterOriginPoint, Player->originRotAmount, Player->AxisOrigin);
	//Children////////////////////////////////////////////////////////////////////////////////////////////
	//
	//OriginRPelvis

	v3 RightPelvisChildren[8];

	Player->AxisRPelvis = QJointRot(Player->AxisRPelvis, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalRightPelvis = V3Ops.v3Sub(Player->parentedLocalRightPelvis, Player->localCharacterOriginPoint);
	Player->parentedLocalRightPelvis = QJointRot(Player->parentedLocalRightPelvis, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalRightPelvis = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalRightPelvis);
	//OriginRKnee
	Player->AxisRKnee = QJointRot(Player->AxisRKnee, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalRightKnee = V3Ops.v3Sub(Player->parentedLocalRightKnee, Player->localCharacterOriginPoint);
	Player->parentedLocalRightKnee = QJointRot(Player->parentedLocalRightKnee, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalRightKnee = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalRightKnee);
	//OriginRFoot
	Player->AxisRFoot = QJointRot(Player->AxisRFoot, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalRightFoot = V3Ops.v3Sub(Player->parentedLocalRightFoot, Player->localCharacterOriginPoint);
	Player->parentedLocalRightFoot = QJointRot(Player->parentedLocalRightFoot, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalRightFoot = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalRightFoot);
	//OriginLPelvis
	Player->AxisLPelvis = QJointRot(Player->AxisLPelvis, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalLeftPelvis = V3Ops.v3Sub(Player->parentedLocalLeftPelvis, Player->localCharacterOriginPoint);
	Player->parentedLocalLeftPelvis = QJointRot(Player->parentedLocalLeftPelvis, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalLeftPelvis = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalLeftPelvis);
	//OriginLKnee
	Player->AxisLKnee = QJointRot(Player->AxisLKnee, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalLeftKnee = V3Ops.v3Sub(Player->parentedLocalLeftKnee, Player->localCharacterOriginPoint);
	Player->parentedLocalLeftKnee = QJointRot(Player->parentedLocalLeftKnee, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalLeftKnee = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalLeftKnee);
	//OriginLFoot
	Player->AxisLFoot = QJointRot(Player->AxisLFoot, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalLeftFoot = V3Ops.v3Sub(Player->parentedLocalLeftFoot, Player->localCharacterOriginPoint);
	Player->parentedLocalLeftFoot = QJointRot(Player->parentedLocalLeftFoot, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalLeftFoot = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalLeftFoot);
	/***********************/
	Player->AxisHead = QJointRot(Player->AxisHead, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalHead = V3Ops.v3Sub(Player->parentedLocalHead, Player->localCharacterOriginPoint);
	Player->parentedLocalHead = QJointRot(Player->parentedLocalHead, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalHead = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalHead);
	//
	Player->AxisChest = QJointRot(Player->AxisChest, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalChest = V3Ops.v3Sub(Player->parentedLocalChest, Player->localCharacterOriginPoint);
	Player->parentedLocalChest = QJointRot(Player->parentedLocalChest, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalChest = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalChest);
	/***********************/
	Player->AxisRShoulder = QJointRot(Player->AxisRShoulder, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalRightShoulder = V3Ops.v3Sub(Player->parentedLocalRightShoulder, Player->localCharacterOriginPoint);
	Player->parentedLocalRightShoulder = QJointRot(Player->parentedLocalRightShoulder, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalRightShoulder = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalRightShoulder);
	//
	Player->AxisRElbow = QJointRot(Player->AxisRElbow, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalRightElbow = V3Ops.v3Sub(Player->parentedLocalRightElbow, Player->localCharacterOriginPoint);
	Player->parentedLocalRightElbow = QJointRot(Player->parentedLocalRightElbow, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalRightElbow = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalRightElbow);
	//
	Player->AxisRHand = QJointRot(Player->AxisRHand, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalRightHand = V3Ops.v3Sub(Player->parentedLocalRightHand, Player->localCharacterOriginPoint);
	Player->parentedLocalRightHand = QJointRot(Player->parentedLocalRightHand, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalRightHand = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalRightHand);
	//
	Player->AxisLShoulder = QJointRot(Player->AxisLShoulder, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalLeftShoulder = V3Ops.v3Sub(Player->parentedLocalLeftShoulder, Player->localCharacterOriginPoint);
	Player->parentedLocalLeftShoulder = QJointRot(Player->parentedLocalLeftShoulder, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalLeftShoulder = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalLeftShoulder);
	//
	Player->AxisLElbow = QJointRot(Player->AxisLElbow, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalLeftElbow = V3Ops.v3Sub(Player->parentedLocalLeftElbow, Player->localCharacterOriginPoint);
	Player->parentedLocalLeftElbow = QJointRot(Player->parentedLocalLeftElbow, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalLeftElbow = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalLeftElbow);
	//
	Player->AxisLHand = QJointRot(Player->AxisLHand, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalLeftHand = V3Ops.v3Sub(Player->parentedLocalLeftHand, Player->localCharacterOriginPoint);
	Player->parentedLocalLeftHand = QJointRot(Player->parentedLocalLeftHand, Player->originRotAmount, Player->AxisOrigin);
	Player->parentedLocalLeftHand = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalLeftHand);
	//
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!LPELVIS
	//
	/***********************************************************************************************Left pelvis*/
	//???????????????????????????????????????????????????????????????????????

	real32 leftFootYBeforeParenting = Player->parentedLocalLeftFoot.y;

	//???????????????????????????????????????????????????????????????????????
	Player->parentedLocalLeftPelvis = V3Ops.v3Sub(Player->parentedLocalLeftPelvis, Player->localCharacterOriginPoint);
	Player->parentedLocalLeftPelvis = QJointRot(Player->parentedLocalLeftPelvis, Player->leftPelvisRotAmount, Player->AxisLPelvis);
	Player->parentedLocalLeftPelvis = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalLeftPelvis);
	//Children/////////////////////////////////////////////////////////////////////////////////////////////////
	//
	Player->AxisLKnee = QJointRot(Player->AxisLKnee, Player->leftPelvisRotAmount, Player->AxisLPelvis);
	Player->parentedLocalLeftKnee = V3Ops.v3Sub(Player->parentedLocalLeftKnee, Player->localCharacterOriginPoint);
	Player->parentedLocalLeftKnee = QJointRot(Player->parentedLocalLeftKnee, Player->leftPelvisRotAmount, Player->AxisLPelvis);
	Player->parentedLocalLeftKnee = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalLeftKnee);
	//
	Player->AxisLFoot = QJointRot(Player->AxisLFoot, Player->leftPelvisRotAmount, Player->AxisLPelvis);
	Player->parentedLocalLeftFoot = V3Ops.v3Sub(Player->parentedLocalLeftFoot, Player->localCharacterOriginPoint);
	Player->parentedLocalLeftFoot = QJointRot(Player->parentedLocalLeftFoot, Player->leftPelvisRotAmount, Player->AxisLPelvis);
	Player->parentedLocalLeftFoot = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalLeftFoot);
	//
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!LKNEE
	//
	/**************************************************************************************************Left knee*/
	Player->parentedLocalLeftKnee = V3Ops.v3Sub(Player->parentedLocalLeftKnee, Player->parentedLocalLeftPelvis);
	Player->parentedLocalLeftKnee = QJointRot(Player->parentedLocalLeftKnee, Player->leftKneeRotAmount, Player->AxisLKnee);
	Player->parentedLocalLeftKnee = V3Ops.v3Add(Player->parentedLocalLeftPelvis, Player->parentedLocalLeftKnee);
	//Children////////////////////////////////////////////////////////////////////////////
	//
	Player->AxisLFoot = QJointRot(Player->AxisLFoot, Player->leftKneeRotAmount, Player->AxisLKnee);
	Player->parentedLocalLeftFoot = V3Ops.v3Sub(Player->parentedLocalLeftFoot, Player->parentedLocalLeftPelvis);
	Player->parentedLocalLeftFoot = QJointRot(Player->parentedLocalLeftFoot, Player->leftKneeRotAmount, Player->AxisLKnee);
	Player->parentedLocalLeftFoot = V3Ops.v3Add(Player->parentedLocalLeftPelvis, Player->parentedLocalLeftFoot);
	//
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!LFOOT
	//
	/**************************************************************************************************Left foot*/
	Player->parentedLocalLeftFoot = V3Ops.v3Sub(Player->parentedLocalLeftFoot, Player->parentedLocalLeftKnee);
	Player->parentedLocalLeftFoot = QJointRot(Player->parentedLocalLeftFoot, Player->leftFootRotAmount, Player->AxisLFoot);
	Player->parentedLocalLeftFoot = V3Ops.v3Add(Player->parentedLocalLeftKnee, Player->parentedLocalLeftFoot);
	//????????????????????????????????????????
	/********************************************************************Colliding elevating origin.****************************************************/
	if (
		Player->isLeftFootColliding 
		&& Player->parentedLocalLeftFoot.y > Player->parentedLocalRightFoot.y
		)
	{
		real32 y = leftFootYBeforeParenting - Player->parentedLocalLeftFoot.y;
		Player->originDirection = V3Ops.v3Add(Player->originDirection, v3{ 0, y, 0 });
		//LocalOriginMovement(Player, 1);
	}
	//??????????????????????????????????????????????
	//
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!RPELVIS
	//
	/***********************************************************************************************Right pelvis*/

	//???????????????????????????????????????????????????????????????????????

	real32 rightFootYBeforeParenting = Player->parentedLocalRightFoot.y;

	//???????????????????????????????????????????????????????????????????????

	Player->parentedLocalRightPelvis = V3Ops.v3Sub(Player->parentedLocalRightPelvis, Player->localCharacterOriginPoint);
	Player->parentedLocalRightPelvis = QJointRot(Player->parentedLocalRightPelvis, Player->rightPelvisRotAmount, Player->AxisRPelvis);
	Player->parentedLocalRightPelvis = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalRightPelvis);
	//Children/////////////////////////////////////////////////////////////////////////////////////////////////
	//
	Player->AxisRKnee = QJointRot(Player->AxisRKnee, Player->rightPelvisRotAmount, Player->AxisRPelvis);
	Player->parentedLocalRightKnee = V3Ops.v3Sub(Player->parentedLocalRightKnee, Player->localCharacterOriginPoint);
	Player->parentedLocalRightKnee = QJointRot(Player->parentedLocalRightKnee, Player->rightPelvisRotAmount, Player->AxisRPelvis);
	Player->parentedLocalRightKnee = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalRightKnee);
	//
	Player->AxisRFoot = QJointRot(Player->AxisRFoot, Player->rightPelvisRotAmount, Player->AxisRPelvis);
	Player->parentedLocalRightFoot = V3Ops.v3Sub(Player->parentedLocalRightFoot, Player->localCharacterOriginPoint);
	Player->parentedLocalRightFoot = QJointRot(Player->parentedLocalRightFoot, Player->rightPelvisRotAmount, Player->AxisRPelvis);
	Player->parentedLocalRightFoot = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalRightFoot);
	//
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!LKNEE
	//

	//Player->parentedLocalLeftElbow = V3Ops.v3Sub(Player->parentedLocalLeftElbow, Player->parentedLocalLeftShoulder);
	//Player->parentedLocalLeftElbow = QJointRot(Player->parentedLocalLeftElbow, Player->leftElbowRotAmount, Player->AxisLElbow);
	//Player->parentedLocalLeftElbow = V3Ops.v3Add(Player->parentedLocalLeftShoulder, Player->parentedLocalLeftElbow);
	//
	//Player->AxisLHand = QJointRot(Player->AxisLHand, Player->leftElbowRotAmount, Player->AxisLElbow);
	//Player->parentedLocalLeftHand = V3Ops.v3Sub(Player->parentedLocalLeftHand, Player->parentedLocalLeftElbow);
	//Player->parentedLocalLeftHand = QJointRot(Player->parentedLocalLeftHand, Player->leftElbowRotAmount, Player->AxisLElbow);
	//Player->parentedLocalLeftHand = V3Ops.v3Add(Player->parentedLocalLeftElbow, Player->parentedLocalLeftHand);

	//A HANDET IS SHOULDERBOL KNENE KIVONNI HÉLÓ TE!!!!!!!!!!
	//!!!!!!!!!!!!!!!!!

	/**************************************************************************************************Right knee*/
	Player->parentedLocalRightKnee = V3Ops.v3Sub(Player->parentedLocalRightKnee, Player->parentedLocalRightPelvis);
	Player->parentedLocalRightKnee = QJointRot(Player->parentedLocalRightKnee, Player->rightKneeRotAmount, Player->AxisRKnee);
	Player->parentedLocalRightKnee = V3Ops.v3Add(Player->parentedLocalRightPelvis, Player->parentedLocalRightKnee);
	//Children////////////////////////////////////////////////////////////////////////////
	//
	Player->AxisRFoot = QJointRot(Player->AxisRFoot, Player->rightKneeRotAmount, Player->AxisRKnee);
	Player->parentedLocalRightFoot = V3Ops.v3Sub(Player->parentedLocalRightFoot, Player->parentedLocalRightPelvis);
	Player->parentedLocalRightFoot = QJointRot(Player->parentedLocalRightFoot, Player->rightKneeRotAmount, Player->AxisRKnee);
	Player->parentedLocalRightFoot = V3Ops.v3Add(Player->parentedLocalRightPelvis, Player->parentedLocalRightFoot);
	//
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!LFOOT
	//
	/**************************************************************************************************Right foot*/
	Player->parentedLocalRightFoot = V3Ops.v3Sub(Player->parentedLocalRightFoot, Player->parentedLocalRightKnee);
	Player->parentedLocalRightFoot = QJointRot(Player->parentedLocalRightFoot, Player->rightFootRotAmount, Player->AxisRFoot);
	Player->parentedLocalRightFoot = V3Ops.v3Add(Player->parentedLocalRightKnee, Player->parentedLocalRightFoot);
	//


	//????????????????????????????????????????
	/********************************************************************Colliding elevating origin.****************************************************/
	if ( 
		Player->isRightFootColliding 
		&& Player->parentedLocalRightFoot.y > Player->parentedLocalLeftFoot.y
		)
	{
		real32 y = rightFootYBeforeParenting - Player->parentedLocalRightFoot.y;
		Player->originDirection = V3Ops.v3Add(Player->originDirection, v3{ 0, y, 0 });
		//LocalOriginMovement(Player, 1);
	}
	//??????????????????????????????????????????????


	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!CHEST
	//
	Player->parentedLocalChest = V3Ops.v3Sub(Player->parentedLocalChest, Player->localCharacterOriginPoint);
	Player->parentedLocalChest = QJointRot(Player->parentedLocalChest, Player->chestRotAmount, Player->AxisChest);
	Player->parentedLocalChest = V3Ops.v3Add(Player->localCharacterOriginPoint, Player->parentedLocalChest);
	//Children////////////////////////////////////////////////////////////////////////////
	//
	Player->AxisHead = QJointRot(Player->AxisHead, Player->chestRotAmount, Player->AxisChest);
	Player->parentedLocalHead = V3Ops.v3Sub(Player->parentedLocalHead, Player->parentedLocalChest);
	Player->parentedLocalHead = QJointRot(Player->parentedLocalHead, Player->chestRotAmount, Player->AxisChest);
	Player->parentedLocalHead = V3Ops.v3Add(Player->parentedLocalChest, Player->parentedLocalHead);
	//
	Player->AxisRShoulder = QJointRot(Player->AxisRShoulder, Player->chestRotAmount, Player->AxisChest);
	Player->parentedLocalRightShoulder = V3Ops.v3Sub(Player->parentedLocalRightShoulder, Player->parentedLocalChest);
	Player->parentedLocalRightShoulder = QJointRot(Player->parentedLocalRightShoulder, Player->chestRotAmount, Player->AxisChest);
	Player->parentedLocalRightShoulder = V3Ops.v3Add(Player->parentedLocalChest, Player->parentedLocalRightShoulder);
	//
	Player->AxisRElbow = QJointRot(Player->AxisRElbow, Player->chestRotAmount, Player->AxisChest);
	Player->parentedLocalRightElbow = V3Ops.v3Sub(Player->parentedLocalRightElbow, Player->parentedLocalChest);
	Player->parentedLocalRightElbow = QJointRot(Player->parentedLocalRightElbow, Player->chestRotAmount, Player->AxisChest);
	Player->parentedLocalRightElbow = V3Ops.v3Add(Player->parentedLocalChest, Player->parentedLocalRightElbow);
	//
	Player->AxisRHand = QJointRot(Player->AxisRHand, Player->chestRotAmount, Player->AxisChest);
	Player->parentedLocalRightHand = V3Ops.v3Sub(Player->parentedLocalRightHand, Player->parentedLocalChest);
	Player->parentedLocalRightHand = QJointRot(Player->parentedLocalRightHand, Player->chestRotAmount, Player->AxisChest);
	Player->parentedLocalRightHand = V3Ops.v3Add(Player->parentedLocalChest, Player->parentedLocalRightHand);
	//
	Player->AxisLShoulder = QJointRot(Player->AxisLShoulder, Player->chestRotAmount, Player->AxisChest);
	Player->parentedLocalLeftShoulder = V3Ops.v3Sub(Player->parentedLocalLeftShoulder, Player->parentedLocalChest);
	Player->parentedLocalLeftShoulder = QJointRot(Player->parentedLocalLeftShoulder, Player->chestRotAmount, Player->AxisChest);
	Player->parentedLocalLeftShoulder = V3Ops.v3Add(Player->parentedLocalChest, Player->parentedLocalLeftShoulder);
	//
	Player->AxisLElbow = QJointRot(Player->AxisLElbow, Player->chestRotAmount, Player->AxisChest);
	Player->parentedLocalLeftElbow = V3Ops.v3Sub(Player->parentedLocalLeftElbow, Player->parentedLocalChest);
	Player->parentedLocalLeftElbow = QJointRot(Player->parentedLocalLeftElbow, Player->chestRotAmount, Player->AxisChest);
	Player->parentedLocalLeftElbow = V3Ops.v3Add(Player->parentedLocalChest, Player->parentedLocalLeftElbow);
	//
	Player->AxisLHand = QJointRot(Player->AxisLHand, Player->chestRotAmount, Player->AxisChest);
	Player->parentedLocalLeftHand = V3Ops.v3Sub(Player->parentedLocalLeftHand, Player->parentedLocalChest);
	Player->parentedLocalLeftHand = QJointRot(Player->parentedLocalLeftHand, Player->chestRotAmount, Player->AxisChest);
	Player->parentedLocalLeftHand = V3Ops.v3Add(Player->parentedLocalChest, Player->parentedLocalLeftHand);
	//
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!HEAD
	//
	Player->parentedLocalHead = V3Ops.v3Sub(Player->parentedLocalHead, Player->parentedLocalChest);
	Player->parentedLocalHead = QJointRot(Player->parentedLocalHead, Player->headRotAmount, Player->AxisHead);
	Player->parentedLocalHead = V3Ops.v3Add(Player->parentedLocalChest, Player->parentedLocalHead);
	//
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!LSHOULDER
	//
	Player->parentedLocalLeftShoulder = V3Ops.v3Sub(Player->parentedLocalLeftShoulder, Player->parentedLocalChest);
	Player->parentedLocalLeftShoulder = QJointRot(Player->parentedLocalLeftShoulder, Player->leftShoulderRotAmount, Player->AxisLShoulder);
	Player->parentedLocalLeftShoulder = V3Ops.v3Add(Player->parentedLocalChest, Player->parentedLocalLeftShoulder);
	//
	Player->AxisLElbow = QJointRot(Player->AxisLElbow, Player->leftShoulderRotAmount, Player->AxisLShoulder);
	Player->parentedLocalLeftElbow = V3Ops.v3Sub(Player->parentedLocalLeftElbow, Player->parentedLocalChest);
	Player->parentedLocalLeftElbow = QJointRot(Player->parentedLocalLeftElbow, Player->leftShoulderRotAmount, Player->AxisLShoulder);
	Player->parentedLocalLeftElbow = V3Ops.v3Add(Player->parentedLocalChest, Player->parentedLocalLeftElbow);
	//
	Player->AxisLHand = QJointRot(Player->AxisLHand, Player->leftShoulderRotAmount, Player->AxisLShoulder);
	Player->parentedLocalLeftHand = V3Ops.v3Sub(Player->parentedLocalLeftHand, Player->parentedLocalChest);
	Player->parentedLocalLeftHand = QJointRot(Player->parentedLocalLeftHand, Player->leftShoulderRotAmount, Player->AxisLShoulder);
	Player->parentedLocalLeftHand = V3Ops.v3Add(Player->parentedLocalChest, Player->parentedLocalLeftHand);
	//
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!LELBOW
	//
	Player->parentedLocalLeftElbow = V3Ops.v3Sub(Player->parentedLocalLeftElbow, Player->parentedLocalLeftShoulder);
	Player->parentedLocalLeftElbow = QJointRot(Player->parentedLocalLeftElbow, Player->leftElbowRotAmount, Player->AxisLElbow);
	Player->parentedLocalLeftElbow = V3Ops.v3Add(Player->parentedLocalLeftShoulder, Player->parentedLocalLeftElbow);
	//
	Player->AxisLHand = QJointRot(Player->AxisLHand, Player->leftElbowRotAmount, Player->AxisLElbow);
	Player->parentedLocalLeftHand = V3Ops.v3Sub(Player->parentedLocalLeftHand, Player->parentedLocalLeftShoulder);
	Player->parentedLocalLeftHand = QJointRot(Player->parentedLocalLeftHand, Player->leftElbowRotAmount, Player->AxisLElbow);
	Player->parentedLocalLeftHand = V3Ops.v3Add(Player->parentedLocalLeftShoulder, Player->parentedLocalLeftHand);
	//
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!LHAND
	//
	Player->parentedLocalLeftHand = V3Ops.v3Sub(Player->parentedLocalLeftHand, Player->parentedLocalLeftElbow);
	Player->parentedLocalLeftHand = QJointRot(Player->parentedLocalLeftHand, Player->leftHandRotAmount, Player->AxisLHand);
	Player->parentedLocalLeftHand = V3Ops.v3Add(Player->parentedLocalLeftElbow, Player->parentedLocalLeftHand);
	//
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!RSHOULDER
	//?????????????????????????????????????????????????????????????????????
	Player->parentedLocalRightShoulder = V3Ops.v3Sub(Player->parentedLocalRightShoulder, Player->parentedLocalChest);
	Player->parentedLocalRightShoulder = QJointRot(Player->parentedLocalRightShoulder, Player->rightShoulderRotAmount, Player->AxisRShoulder);
	Player->parentedLocalRightShoulder = V3Ops.v3Add(Player->parentedLocalChest, Player->parentedLocalRightShoulder);
	//
	Player->AxisRElbow = QJointRot(Player->AxisRElbow, Player->rightShoulderRotAmount, Player->AxisRShoulder);
	Player->parentedLocalRightElbow = V3Ops.v3Sub(Player->parentedLocalRightElbow, Player->parentedLocalChest);
	Player->parentedLocalRightElbow = QJointRot(Player->parentedLocalRightElbow, Player->rightShoulderRotAmount, Player->AxisRShoulder);
	Player->parentedLocalRightElbow = V3Ops.v3Add(Player->parentedLocalChest, Player->parentedLocalRightElbow);
	//
	Player->AxisRHand = QJointRot(Player->AxisRHand, Player->rightShoulderRotAmount, Player->AxisRShoulder);
	Player->parentedLocalRightHand = V3Ops.v3Sub(Player->parentedLocalRightHand, Player->parentedLocalChest);
	Player->parentedLocalRightHand = QJointRot(Player->parentedLocalRightHand, Player->rightShoulderRotAmount, Player->AxisRShoulder);
	Player->parentedLocalRightHand = V3Ops.v3Add(Player->parentedLocalChest, Player->parentedLocalRightHand);
	//
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!RELBOW
	// SUBBBBBBBBBBBBBBBSTRAAAAAAAAAAAAAAACT FROM CHEST
	Player->parentedLocalRightElbow = V3Ops.v3Sub(Player->parentedLocalRightElbow, Player->parentedLocalRightShoulder);
	Player->parentedLocalRightElbow = QJointRot(Player->parentedLocalRightElbow, Player->rightElbowRotAmount, Player->AxisRElbow);
	Player->parentedLocalRightElbow = V3Ops.v3Add(Player->parentedLocalRightShoulder, Player->parentedLocalRightElbow);
	//
	Player->AxisRHand = QJointRot(Player->AxisRHand, Player->rightElbowRotAmount, Player->AxisRElbow);
	Player->parentedLocalRightHand = V3Ops.v3Sub(Player->parentedLocalRightHand, Player->parentedLocalRightShoulder);
	Player->parentedLocalRightHand = QJointRot(Player->parentedLocalRightHand, Player->rightElbowRotAmount, Player->AxisRElbow);
	Player->parentedLocalRightHand = V3Ops.v3Add(Player->parentedLocalRightShoulder, Player->parentedLocalRightHand);
	//
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!RHAND
	//
	Player->parentedLocalRightHand = V3Ops.v3Sub(Player->parentedLocalRightHand, Player->parentedLocalRightElbow);
	Player->parentedLocalRightHand = QJointRot(Player->parentedLocalRightHand, Player->rightHandRotAmount, Player->AxisRHand);
	Player->parentedLocalRightHand = V3Ops.v3Add(Player->parentedLocalRightElbow, Player->parentedLocalRightHand);
}
/***********************************************************************************************/
void AnimationRun(player_instance *Player, cat_model *CatModel, real32 originRotAmount)
{
	Player->AnimSpeed = 2.0f;
	if (Player->isLeftFootAhead == false)
	{
		/********************************************I*/
		//I:LENDÜLETVÉTEL,ALAPHELYZET
		//Pelvis
		//Player->leftKneeRotAmount = 0.0f;
		//Player->leftFootRotAmount = 0.0f;
		//Pelvis
		//Player->rightKneeRotAmount = -30.0f;
		//Player->rightFootRotAmount = 80.0f;
		if (Player->isLeftRunI == false)
		{
			Player->rightKneeRotAmount = -1.5f * Player->AnimSpeed;
			Player->rightFootRotAmount = 4.0f * Player->AnimSpeed;

			Player->runStateTimer += 1;
			if (Player->runStateTimer == 10)
			{
				Player->isLeftRunI = true;
				Player->isLeftRunII = true;
				Player->runStateTimer = 0;
			}
		}
		/******************************************II*/
		//II:ELRUGASZKODÁS
		//pelvis
		//Player->leftKneeRotAmount = 20.0f;
		//Player->leftFootRotAmount = 0.0f;
		//pelvis
		//Player->rightKneeRotAmount = -60.0f;
		//Player->rightFootRotAmount = 80.0f;	
		if (Player->isLeftRunII == true)
		{
			Player->leftKneeRotAmount = 1.5f * Player->AnimSpeed;
			Player->rightKneeRotAmount = -3.0f * Player->AnimSpeed;
			Player->rightFootRotAmount = 4.0f * Player->AnimSpeed;
			//ellök iamgát
			if (
				Player->isRightFootColliding || Player->isLeftFootColliding
				)
			{
				Player->originDirection = Player->originDirection + v3{ 0, -0.5f/Player->AnimSpeed, 0 };
			}

			Player->runStateTimer += 1;
			if (Player->runStateTimer == 10)
			{
				Player->isLeftRunII = false;
				Player->isLeftRunIII = true;
				Player->runStateTimer = 0;
			}
		}
		/*****************************************III*/
		//III:REPÜLÉS
		//pelvis
		//Player->leftKneeRotAmount = 60.0f;
		//Player->leftFootRotAmount = 0.0f;
		//pelvis	
		//Player->rightKneeRotAmount = -60.0f;
		//Player->rightFootRotAmount = 0.0f;	
		if (Player->isLeftRunIII == true)
		{
			Player->leftKneeRotAmount = 1.5f * Player->AnimSpeed;

			Player->leftFootRotAmount = 3.0f * Player->AnimSpeed;

			Player->rightFootRotAmount = -4.0f * Player->AnimSpeed;

			Player->runStateTimer += 1;
			if (Player->runStateTimer == 10)
			{
				Player->isLeftRunIII = false;
				Player->isLeftRunIV = true;
				Player->runStateTimer = 0;
			}
		}
		/*****************************************IV*/
		//IV:FÖLDETÉRÉS
		//Player->leftPelvisRotAmount = -0.0f;
		//Player->leftKneeRotAmount = 50.0f; 0 !!!!!!!!0
		//Player->leftFootRotAmount = 30.0f; 0 !!!!!!!!0
		//Player->rightPelvisRotAmount = 0.0f;
		//Player->rightKneeRotAmount = -30.0f; -30 !!!!0
		//Player->rightFootRotAmount = 0.0f; 80 !!!!!!!0
		if (Player->isLeftRunIV == true)
		{
			Player->leftKneeRotAmount = -1.0f * Player->AnimSpeed;

			Player->leftFootRotAmount = -3.0f * Player->AnimSpeed;

			Player->rightKneeRotAmount = +4.0f * Player->AnimSpeed;
			Player->rightFootRotAmount = -4.0f * Player->AnimSpeed;//-4

			Player->runStateTimer += 1;
			if (Player->runStateTimer == 11)
			{
				Player->isLeftRunIV = false;

				//Player->originRotAmount = 0.0f;

				Player->leftPelvisRotAmount = 0.0f;
				Player->leftKneeRotAmount = 0.0f;
				Player->leftFootRotAmount = 0.0f;
				Player->rightPelvisRotAmount = 0.0f;
				Player->rightKneeRotAmount = 0.0f;
				Player->rightFootRotAmount = 0.0f;
				Player->runStateTimer = 0;

				//itt is csináld meg a méréseket, és insert keyframe :D
				//v3 OriginSub = v3{ 115.64451f, 0.66463f, 0 };
				//Player->parentedLocalRightKnee = Player->localCharacterOriginPoint - (OriginSub - v3{ 120.22768f, 20.55472f, -2.48611f });
				//Player->parentedLocalRightFoot = Player->localCharacterOriginPoint - (OriginSub - v3{ 119.77739f, 30.31753f, -4.60384f });
				//Player->parentedLocalLeftKnee = Player->localCharacterOriginPoint -  (OriginSub - v3{ 112.00582f, 18.64975f, 6.92804f });//???
				//Player->parentedLocalLeftFoot = Player->localCharacterOriginPoint -  (OriginSub - v3{ 113.25779f, 26.63487f, 12.81620f });

				Player->isLeftFootAhead = true;
				Player->isLeftRunI = false;
				Player->isLeftRunII = false;
				Player->isLeftRunIII = false;
				Player->isLeftRunIV = false;
			}
		}
	}
	else if (Player->isLeftFootAhead == true)//!!!VALAHOL AZ ELÕJELLEL VAN GOND? ABSZOLÚT ÉRTÉKET KELL VENNI::::
	{
		/********************************************I*/
		//I:LENDÜLETVÉTEL,ALAPHELYZET
		//Pelvis
		//Player->leftKneeRotAmount = 0.0f; 50
		//Player->leftFootRotAmount = 0.0f; 30
		//Pelvis
		//Player->rightKneeRotAmount = -30.0f; -30
		//Player->rightFootRotAmount = 80.0f; 0
		if (Player->isLeftRunI == false)
		{
			Player->leftKneeRotAmount = -3.5f * Player->AnimSpeed;
			Player->leftFootRotAmount = 4.0f * Player->AnimSpeed;
			Player->rightKneeRotAmount = 0.5f * Player->AnimSpeed;

			Player->runStateTimer += 1;
			if (Player->runStateTimer == 10)
			{
				Player->isLeftRunI = true;
				Player->isLeftRunII = true;
				Player->runStateTimer = 0;
			}
		}
		/******************************************II*/
		//II:ELRUGASZKODÁS
		//pelvis
		//Player->leftKneeRotAmount = 20.0f;
		//Player->leftFootRotAmount = 0.0f;
		//pelvis
		//Player->rightKneeRotAmount = -60.0f;
		//Player->rightFootRotAmount = 80.0f;	
		if (Player->isLeftRunII == true)
		{
			Player->rightKneeRotAmount = 1.5f * Player->AnimSpeed;
			Player->leftKneeRotAmount = -3.0f * Player->AnimSpeed;
			Player->leftFootRotAmount = 4.0f * Player->AnimSpeed;

			if (
				Player->isRightFootColliding || Player->isLeftFootColliding
				)
			{
				Player->originDirection = Player->originDirection + v3{ 0, -0.5f/Player->AnimSpeed, 0 };
			}

			Player->runStateTimer += 1;
			if (Player->runStateTimer == 10)
			{
				Player->isLeftRunII = false;
				Player->isLeftRunIII = true;
				Player->runStateTimer = 0;
			}
		}
		/*****************************************III*/
		//III:REPÜLÉS
		//pelvis
		//Player->leftKneeRotAmount = 60.0f;
		//Player->leftFootRotAmount = 0.0f;
		//pelvis	
		//Player->rightKneeRotAmount = -60.0f;
		//Player->rightFootRotAmount = 0.0f;	
		if (Player->isLeftRunIII == true)
		{
			Player->rightKneeRotAmount = 1.5f * Player->AnimSpeed;

			Player->rightFootRotAmount = 3.0f * Player->AnimSpeed;

			Player->leftFootRotAmount = -4.0f * Player->AnimSpeed;

			Player->runStateTimer += 1;
			if (Player->runStateTimer == 10)
			{
				Player->isLeftRunIII = false;
				Player->isLeftRunIV = true;
				Player->runStateTimer = 0;
			}
		}
		/*****************************************IV*/
		//IV:FÖLDETÉRÉS
		//pelvis
		//Player->leftKneeRotAmount = 50.0f;
		//Player->leftFootRotAmount = 30.0f;
		//pelvis
		//Player->rightKneeRotAmount = -30.0f;
		//Player->rightFootRotAmount = 0.0f;
		if (Player->isLeftRunIV == true)
		{
			//itt a bal foot kicsit beesik ....
			Player->rightKneeRotAmount = -1.0f * Player->AnimSpeed;//-3û

			Player->rightFootRotAmount = -3.0f * Player->AnimSpeed;

			Player->leftKneeRotAmount = 4.0f * Player->AnimSpeed;//4.5 4.0
			Player->leftFootRotAmount = -3.5f * Player->AnimSpeed;//-4

			////Player->leftPelvisRotAmount = -0.0f;
			//Player->rightKneeRotAmount = -3.0f;
			////Player->rightFootRotAmount = 1.5f;
			////Player->rightPelvisRotAmount = 0.0f;
			//Player->leftKneeRotAmount = 4.5f;
			//Player->leftFootRotAmount = -4.0f;

			Player->runStateTimer += 1;
			if (Player->runStateTimer == 10)
			{
				Player->isLeftRunIV = false;
				Player->isLeftRunV = true;
				Player->runStateTimer = 0;			
			}
		}
		/*************************************************************V:Lábváltás*/
		if (Player->isLeftRunV == true)
		{
			Player->rightKneeRotAmount = -3.36f * Player->AnimSpeed;//+-1.5 -3.5volt
			Player->rightFootRotAmount = 4.0f * Player->AnimSpeed;//x
			Player->leftKneeRotAmount = 0.5f * Player->AnimSpeed;
			Player->leftFootRotAmount = 0.05f * Player->AnimSpeed;
			//Player->leftKneeWasRotatedBy += 0.5f;

			Player->runStateTimer += 1;
			if (
				//Player->leftKneeWasRotatedBy == 10.00f
				Player->runStateTimer == 11
				)
			{
				//Player->isLeftRunV = false;
				//Player->originRotAmount = 0.0f;

				Player->leftPelvisRotAmount = 0.0f;
				Player->leftKneeRotAmount = 0.0f;
				Player->leftFootRotAmount = 0.0f;
				Player->rightPelvisRotAmount = 0.0f;
				Player->rightKneeRotAmount = 0.0f;
				Player->rightFootRotAmount = 0.0f;
				Player->runStateTimer = 0;

				/*if (Player->originRotAmount > 0)
				{
					originRotAmount = 60.0f;
				}
				else if (Player->originRotAmount < 0)
				{
					originRotAmount = -60.0f;
				}
				else if (Player->originRotAmount == 0)
				{
				}*/

				//The animation cycle has to be made as punctual as possible, 
				//and then the correction keyframe is to be applied
				//only periodically, and under certain conditions to avoid bugs.
				
				//Player->runKeyFrameCounter += 1;
				if (
					Player->runKeyFrameCounter >= 1 
					//&& Player->originRotAmount == Player->lastOriginRotAMount
					)
				{
					v3 OriginSub = v3{ -125.06898, 1.20799, 0 };
					Player->parentedLocalRightKnee = Player->localCharacterOriginPoint - (OriginSub - v3{ -120.06898f, 20.02029f, -4.72688f });
					Player->parentedLocalRightFoot = Player->localCharacterOriginPoint - (OriginSub - v3{ -120.06898f, 26.73553f, 2.68294f });
					Player->parentedLocalLeftKnee = Player->localCharacterOriginPoint - (OriginSub - v3{ -130.06898f, 21.18933f, -0.61068f });//???
					Player->parentedLocalLeftFoot = Player->localCharacterOriginPoint - (OriginSub - v3{ -130.06898f, 31.10372f, -1.91635f });

					/*****************************************************************************************************/
					//OriginRKnee
					//Player->AxisRKnee = QJointRot(Player->AxisRKnee, originRotAmount, Player->AxisOrigin);
					Player->parentedLocalRightKnee = Player->parentedLocalRightKnee - Player->localCharacterOriginPoint;
					Player->parentedLocalRightKnee = QJointRot(Player->parentedLocalRightKnee, originRotAmount, Player->AxisOrigin);
					Player->parentedLocalRightKnee = Player->localCharacterOriginPoint + Player->parentedLocalRightKnee;
					//OriginRFoot
					//Player->AxisRFoot = QJointRot(Player->AxisRFoot, originRotAmount, Player->AxisOrigin);
					Player->parentedLocalRightFoot = Player->parentedLocalRightFoot - Player->localCharacterOriginPoint;
					Player->parentedLocalRightFoot = QJointRot(Player->parentedLocalRightFoot, originRotAmount, Player->AxisOrigin);
					Player->parentedLocalRightFoot = Player->localCharacterOriginPoint + Player->parentedLocalRightFoot;
					/*******************************************************************************************************/
					////OriginLKnee
					//Player->AxisLKnee = QJointRot(Player->AxisLKnee, originRotAmount, Player->AxisOrigin);
					Player->parentedLocalLeftKnee = Player->parentedLocalLeftKnee - Player->localCharacterOriginPoint;
					Player->parentedLocalLeftKnee = QJointRot(Player->parentedLocalLeftKnee, originRotAmount, Player->AxisOrigin);
					Player->parentedLocalLeftKnee = Player->localCharacterOriginPoint + Player->parentedLocalLeftKnee;
					//OriginLFoot
					//Player->AxisLFoot = QJointRot(Player->AxisLFoot, originRotAmount, Player->AxisOrigin);
					Player->parentedLocalLeftFoot = Player->parentedLocalLeftFoot - Player->localCharacterOriginPoint;
					Player->parentedLocalLeftFoot = QJointRot(Player->parentedLocalLeftFoot, originRotAmount, Player->AxisOrigin);
					Player->parentedLocalLeftFoot = Player->localCharacterOriginPoint + Player->parentedLocalLeftFoot;
					/******************************************************************************************************/
					Player->runKeyFrameCounter = 0;
				}
				Player->runKeyFrameCounter += 1;
				/******************************************/
				Player->isLeftFootAhead = false;
				//Player->isLeftRunI = false;
				Player->isLeftRunII = true;
				Player->isLeftRunIII = false;
				Player->isLeftRunIV = false;
				Player->isLeftRunV = false;
			}
		}
		/*************************************************************************/
	}
	/*QParentedJointMovement(Player, CatModel);
	
	Player->originRotAmount = 0.0f;

	Player->rightPelvisRotAmount = 0.0f;
	Player->rightKneeRotAmount = 0.0f;
	Player->rightFootRotAmount = 0.0f;
	Player->leftPelvisRotAmount = 0.0f;
	Player->leftKneeRotAmount = 0.0f;
	Player->leftFootRotAmount = 0.0f;*/
}
void Idle(player_instance *Player, cat_model *CatModel)
{
	/*Player->originRotAmount = -0.05f * Player->originWasRotatedBy;
	QParentedJointMovement(Player, CatModel);
	Player->originWasRotatedBy = Player->originWasRotatedBy * 0.95f;*/

	//az originrot amountot itt nullázzuk le csak

	//Player->originRotAmount = 0;

	//if (Player->originWasRotatedBy == 0)
	//Player->isReadyForRunCycle = false;
}
void RunLeft(player_instance *Player, cat_model *CatModel)
{
	if (Player->originDirection.x > -0.96f*Player->AnimSpeed)
	{
		Player->originDirection = Player->originDirection + v3{ -0.02f*Player->AnimSpeed, 0, 0 };
	}
	//assume running pose
	if ( Player->isReadyForRunCycle == false 
		|| Player->isReadyForRunCycle
		)
	{
		Player->originRotAmount = 6.0f;
		if (Player->originRotAmount != Player->lastOriginRotAmount)
		{
			Player->runKeyFrameCounter = 0;
		}
		Player->lastOriginRotAmount = 6.0f;

		//wasrotetedbyt le kell nulláznivalahol
		Player->originWasRotatedBy = Player->originWasRotatedBy + 6.0f;

		if (Player->originWasRotatedBy == 66.0f
			)
		{
			Player->originWasRotatedBy = 60.0f;
			Player->isReadyForRunCycle = true;
			Player->originRotAmount = 0.0f;
		}
	}
	else
	{
		//Player->originRotAmount = 0.0f;
	}
	AnimationRun(Player, CatModel, Player->originWasRotatedBy);
	//
}
void RunRight(player_instance *Player, cat_model *CatModel)
{
	if (Player->originDirection.x < +0.96f*Player->AnimSpeed)
	{
		Player->originDirection = Player->originDirection + v3{ +0.02f*Player->AnimSpeed, 0, 0 };
	}
	//assume running pose
	if (Player->isReadyForRunCycle == false 
		|| Player->isReadyForRunCycle
		)
	{
		Player->originRotAmount = -6.0f;
		/********************************************************************/
		if (Player->originRotAmount != Player->lastOriginRotAmount)
		{
			Player->runKeyFrameCounter = 0;
		}
		//ezt nem szabad folyton mérni, csa kegyszer
		Player->lastOriginRotAmount = -6.0f;
		/*******************************************************************/
		Player->originWasRotatedBy = Player->originWasRotatedBy - 6.0f;

		if (Player->originWasRotatedBy == -66.0f			
			)
		{
			Player->originWasRotatedBy = -60.0f;
			Player->isReadyForRunCycle = true;
			Player->originRotAmount = 0.0f;
		}
	}
	else
	{
		//Player->originRotAmount = 0.0f;
	}
	//Ide nme 60/-60 at kell átadni, hane ma wasrotated by t sztem..........
	AnimationRun(Player, CatModel, Player->originWasRotatedBy);
	//
}
void Duck(player_instance *Player)
{

}
void Jump(player_instance *Player, cat_model *CatModel)
{
	if (Player->isRightFootColliding || Player->isLeftFootColliding)
	{
		Player->originDirection = Player->originDirection + v3{ 0.0f, -1.0f, 0 };
	}

	Player->leftPelvisRotAmount = -0.0f;
	Player->leftKneeRotAmount = -0.0f;
	Player->leftFootRotAmount = -0.0f;

	Player->rightPelvisRotAmount = 0.0f;
	Player->rightKneeRotAmount = 0.0f;
	Player->rightFootRotAmount = 0.0f;
}
void AttackPrimary(player_instance *Player, cat_model *CatModel)
{
	//Player->leftShoulderRotAmount = 0.10f;
	Player->leftElbowRotAmount = 4.0f;
	Player->leftHandRotAmount = 0.0f;
	//a rotation is lehetne egy helyen csak a lenullázások miatt kell több?
}
void AttackSecondary(player_instance *Player, cat_model *CatModel)
{
	//Player->leftShoulderRotAmount = -0.10f;
	Player->leftElbowRotAmount = -4.0f;
	Player->leftHandRotAmount = 0.0f;
}
void OtherClientCharacterSync()
{

}
void OtherCLientAnimationPrediction()
{
}
/***********************************************************************************CALL*/
internal_function void
LocalCharacterMovement(game_controller_input *Controller, 
	player_instance *Player, cat_model *CatModel,
	game_input *Input, camera_buffer *CameraBuffer
	)
{
	//if (Controller->Down.EndedDown)
	if (
			Player->originDirection.x < 0.1f &&
			Player->originDirection.x > -0.1f
		)
	{
		//Idle(Player, CatModel);
	}
	if (Controller->Left.EndedDown)
	{
		RunLeft(Player, CatModel);
		Player->localCharacterOriginPoint = 
			Player->localCharacterOriginPoint - v3{0.4f,0,0};
		CameraBuffer->PlayerBuffer = v3{
			CameraBuffer->PlayerBuffer.x - 4.0f,
			CameraBuffer->PlayerBuffer.y,
			CameraBuffer->PlayerBuffer.z
		};
	}	
	else if (Controller->Right.EndedDown)
	{
		RunRight(Player, CatModel);//!!!be volt kapcsolva
		Player->localCharacterOriginPoint = 
			Player->localCharacterOriginPoint + v3{0.4f,0,0};
		CameraBuffer->PlayerBuffer = v3{
			CameraBuffer->PlayerBuffer.x + 4.0f,
			CameraBuffer->PlayerBuffer.y,
			CameraBuffer->PlayerBuffer.z
		};
	}
	if (Controller->Up.EndedDown)
	{
		CameraBuffer->PlayerBuffer = v3{
			CameraBuffer->PlayerBuffer.x,
			CameraBuffer->PlayerBuffer.y,
			CameraBuffer->PlayerBuffer.z + 4.0f
		};
	}
	else if (Controller->Down.EndedDown)
	{
		CameraBuffer->PlayerBuffer = v3{
			CameraBuffer->PlayerBuffer.x,
			CameraBuffer->PlayerBuffer.y,
			CameraBuffer->PlayerBuffer.z - 4.0f
		};
	}
	if (Controller->Space.EndedDown)
	{
	//Jump(Player, CatModel);
		CameraBuffer->PlayerBuffer = v3{
			CameraBuffer->PlayerBuffer.x,
			CameraBuffer->PlayerBuffer.y + 4.0f,
			CameraBuffer->PlayerBuffer.z
		};
	}
	if (Input->MouseButtons[0].EndedDown)
	{
	AttackPrimary(Player, CatModel);
	CameraBuffer->PlayerBuffer = v3{
		CameraBuffer->PlayerBuffer.x,
		CameraBuffer->PlayerBuffer.y + 4.0f,
		CameraBuffer->PlayerBuffer.z
	};
	}
	if (Input->MouseButtons[2].EndedDown)
	{
	AttackSecondary(Player, CatModel);
	}
}
internal_function void MouseScreenScroll(game_input *Input, camera_buffer *CameraBuffer)
{
	if (Input->MouseX >= 620)
	{	
		//CameraBuffer->CamX = CameraBuffer->CamX - CameraBuffer->PlayerBuffer;
		CameraBuffer->CamZ = QJointRot(CameraBuffer->CamZ, 0.4f, v3{ 0,1,0 });
		CameraBuffer->CamX = QJointRot(CameraBuffer->CamX, 0.4f, v3{ 0,1,0 });
		CameraBuffer->CamY = QJointRot(CameraBuffer->CamY, 0.4f, v3{ 0,1,0 });
	}
	else if (Input->MouseX <= 20)
	{
		//CameraBuffer->CamX = CameraBuffer->CamX - CameraBuffer->PlayerBuffer;
		CameraBuffer->CamZ = QJointRot(CameraBuffer->CamZ, -0.4f, v3{ 0,1,0 });
		CameraBuffer->CamX = QJointRot(CameraBuffer->CamX, -0.4f, v3{ 0,1,0 });
		CameraBuffer->CamY = QJointRot(CameraBuffer->CamY, -0.4f, v3{ 0,1,0 });
	}
	//else if (Input->MouseY >= 460)
	//{	
	//	//CameraBuffer->CamY = CameraBuffer->CamY - CameraBuffer->PlayerBuffer;
	//	CameraBuffer->CamZ = QJointRot(CameraBuffer->CamZ, -0.4f, v3{ 1,0,0 });
	//	CameraBuffer->CamY = QJointRot(CameraBuffer->CamY, -0.4f, v3{ 1,0,0 });
	//	CameraBuffer->CamX = QJointRot(CameraBuffer->CamX, -0.4f, v3{ 1,0,0 });
	//}
	//else if (Input->MouseY <= 20)
	//{	
	//	//CameraBuffer->CamY = CameraBuffer->CamY - CameraBuffer->PlayerBuffer;
	//	CameraBuffer->CamZ = QJointRot(CameraBuffer->CamZ, 0.4f, v3{ 1,0,0 });
	//	CameraBuffer->CamY = QJointRot(CameraBuffer->CamY, 0.4f, v3{ 1,0,0 });
	//	CameraBuffer->CamX = QJointRot(CameraBuffer->CamX, 0.4f, v3{ 1,0,0 });
	//}
	if (Input->MouseButtons[0].EndedDown)
	{
		
	}
}
internal_function void
InitializeMemoryPlace
(
	memory_place *MemoryPlace, uint64 Size,
	uint8 *Base//get memory starting after gamestate
)
{
	MemoryPlace->Size = Size;
	MemoryPlace->Base = Base;
	MemoryPlace->Used = 0;
}
#define PushStruct(MemoryPlace, type)(type*)PushStruct_(MemoryPlace, sizeof(type)) 
void *
PushStruct_(memory_place *MemoryPlace, uint64 Size)
{
	void * Result = MemoryPlace->Base + MemoryPlace->Used;
	MemoryPlace->Used += Size;
	return (Result);
}


internal_function void 
GameUpdateAndRender(
	game_memory *Memory,
	game_input *Input,
	game_offscreen_buffer *Buffer,
	//game_sound_output_buffer *SoundBuffer,
	client_parts *ClientParts,
	player_instance *Player,
	local_player_parts *LocalPlayerParts
	//client parts???
)
{	
	/******************************************************Write into created memory I\O*/
	Assert(sizeof(game_state) <= Memory->PermanentStorageSize);
	game_state *GameState = (game_state*)Memory->PermanentStorage;
	/********************************************************************************/
	if (!Memory->IsInitialized)
	{
		char *Filename = "Icon.ico";//__FILE__;

		/*uint32 FileSize = GetFileSize(Filename);
		void *BitmapMemory = ReserveMemory(Memory, FileSize);
		ReadEntireFileIntoMemory(Filename, BitmapMemory);*///for final version
		//void *BitmapMemory = DEBUGPlatformReadEntireFile(Filename);//dont on the platform because code cycles too much
		//
		//not permanent solution
		debug_read_file_result File = DEBUGPlatformReadEntireFile(Filename);
		if (File.Contents)
		{
			DEBUGPlatformWriteEntireFile("d:/gameengine/build/writetest.out", File.ContentsSize, File.Contents);
			DEBUGPlatformFreeFileMemory(File.Contents);
		}

		GameState->Hz = 256;
		/****************************************Player INITIALIZATION*/
		GameState->PlayerX = 0.0f;
		//GameState->PlayerX = 100.0f;
		//GameState->PlayerY = 0.0f;
		//GameState->PlayerZ = 0.0f;
		/*****************************************************************************************MemStackPointer*/
		InitializeMemoryPlace(
			&GameState->MapPlace, Memory->PermanentStorageSize - sizeof(game_state),
			(uint8*)Memory->PermanentStorage + sizeof(game_state)//get memory starting after gamestate
			);
		/*************************************************************************************************/
		GameState->Map = PushStruct(&GameState->MapPlace, map);
		//on to memory//IT PUSHES ONLY SIZE NOT else 8but blabla[10] will be 10 * 8size
		map *Map;
		Map = GameState->Map;

		GameState->Player = PushStruct(&GameState->MapPlace, player_instance);
		//player_instance *Player;
		Player = GameState->Player;

		/**********************Õk jelenleg globális változókként õrzõdnek meg a memoriában valahol*/
		//valamiért a memhelyet le kell foglalni nekik így is

		GameState->LocalPlayerParts = PushStruct(&GameState->MapPlace, local_player_parts);
		//IT VOLT A NETWORKOS HIBA
		//local_player_parts *LocalPlayerParts;
		LocalPlayerParts = GameState->LocalPlayerParts;

		////fordítva?
		//
		////clientparts?

		GameState->ClientParts = PushStruct(&GameState->MapPlace, client_parts);
		//client_parts *ClientParts;
		ClientParts = GameState->ClientParts;
		/*****************************************************************************************/

		GameState->CameraBuffer = PushStruct(&GameState->MapPlace, camera_buffer);
		camera_buffer *CameraBuffer;
		CameraBuffer = GameState->CameraBuffer;

		GameState->CatModel = PushStruct(&GameState->MapPlace, cat_model);
		cat_model *CatModel;
		CatModel = GameState->CatModel;

		GameState->WriteCursor = PushStruct(&GameState->MapPlace, write_cursor);
		write_cursor *WriteCursor;
		WriteCursor = GameState->WriteCursor;

		/******************************************Initialize GameState Memory*/
		LocalCharacterInitialization(GameState->Player, CameraBuffer);
		MapInit(GameState->Map);
		CatModelInit(GameState->CatModel, GameState->Player);
		WriteCursor->WriteCursorX = 40;
		WriteCursor->WriteCursorY = 40;
		CameraBuffer->CamZ = { 0,0,1 };
		CameraBuffer->CamX = { 1,0,0 };
		CameraBuffer->CamY = { 0,1,0 };
		/**********************************/
		////LocalCharacterParenting(Player);//has to be put in memory??
		////LocalPlayerParts->OriginX = 300;
		////LocalPlayerParts->OriginY = 300;
		Memory->IsInitialized = true;
	}
	/***********************************************/
	Player = GameState->Player;
	//LocalPlayerParts = GameState->LocalPlayerParts;
	//FORDÍTVA

	GameState->CameraBuffer->PlayerBuffer = GameState->Player->playerBuffer;
	/***********************************************/

	/******************************************************************************************GameUpdate*/

	//GameOutputSound(SoundBuffer, GameState->Hz);

	DrawSky(Buffer, GameState->XOffset, GameState->YOffset);

	DrawPolygons(Buffer, GameState->CameraBuffer, GameState->Map, GameState->CatModel);

	//FillPolygons(Buffer, GameState->CameraBuffer, GameState->Map);//map will be like polygons....

	////DrawCall(Buffer);

	//DrawGameCursor(Buffer, Input->MouseX, Input->MouseY);

	LocalPlayerAssembleClients(Buffer, ClientParts);

	VectorCollectionForPixelTinting(Buffer, GameState->Player, GameState->CameraBuffer);

	LocalPlayerAssembleLocalPlayer(LocalPlayerParts, GameState->Player);//!!!

	MouseScreenScroll(Input, GameState->CameraBuffer);

	/*****************************************************************************************INPUTRELATED*/

	//Physics(GameState->Player);

	//LocalOriginMovement(GameState->Player, GameState->CatModel, 1);
	DrawRectangle(Buffer, LocalPlayerParts->OriginX, LocalPlayerParts->OriginY);

	//Collisions(GameState->Player, GameState->Map);

	RenderingPipeline(Buffer, GameState->CameraBuffer, GameState->CatModel);
	//A vonalakat a kitöltõ felé rajzoljuk, mert a kitöltõ a vonalakat használja...
	//POLYTÖLTÕ

	//DrawCall(Buffer);

	//canvas újrarajzoló
	//KÉNE DEBUGHOZ A RÉGI RENDERSKY

	DrawGameCursor(Buffer, Input->MouseX, Input->MouseY);

    /*******************************************************************************************GET INPUT*/
	//!!!!!!! ez hasznos lesz playerke megkülönböztetéséhzez
	for (
		int ControllerIndex = 0;
		ControllerIndex < ArrayCount(Input->Controllers);
		++ControllerIndex
		)
	{
		game_controller_input *Controller = &Input->Controllers[ControllerIndex];//ez is hasznos lehet NETWORK!!
		if (Controller->isAnalog)//this is for gamepad STICK only
		{

		}
		else
		{

		}	

		LocalCharacterMovement(Controller, GameState->Player, GameState->CatModel, Input, GameState->CameraBuffer);
		/***************************************************************************************INPUT*/
		if (Controller->Enter.EndedDown)
		{
			if (GlobalInterfaceOn == false)
			{
				GlobalInterfaceOn = true;
			}
			else
			{
				GlobalInterfaceOn = false;//process message....
			}
		}
		if (Controller->Left.EndedDown)
		{
			//Player->isLeftRun0 = true;
			//RunLeft(Player, CatModel);
			GameState->LocalPlayerParts->OriginX += 1;
			GameState->LocalPlayerParts->OriginY += 1;
			//a gamestatehet referencia kell h ne torlodjon memorybol
		}
		if (GlobalInterfaceOn
			//|| !GlobalInterfaceOn
			)
		{
			UserInterface(Buffer, Input, Controller, GameState->WriteCursor);
		}
	}

	QParentedJointMovement(GameState->Player, GameState->CatModel);
	Player->originRotAmount = 0.0f;
	Player->rightPelvisRotAmount = 0.0f;
	Player->rightKneeRotAmount = 0.0f;
	Player->rightFootRotAmount = 0.0f;
	Player->leftPelvisRotAmount = 0.0f;
	Player->leftKneeRotAmount = 0.0f;
	Player->leftFootRotAmount = 0.0f;

	Player->leftElbowRotAmount = 0.0f;
	Player->leftHandRotAmount = 0.0f;

	//omiatta  abetu meg nme lácik
	/**********************************************************CUSTOM KEYBOARD INTERFACE*/
	letter_codes LetterCode = {};
	letter_color_array Code = {};

	int WriteCursorX = 10;
	int WriteCursorY = 10;

	Code = LetterCode.LetterA();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterB();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterC();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterD();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterE();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterF();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterG();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterH();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterI();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterJ();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterK();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterL();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterM();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterN();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterO();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterP();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterQ();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterR();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterS();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterT();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterU();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterV();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterW();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterX();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterY();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.LetterZ();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	//
	WriteCursorY += 12;
	//
	Code = LetterCode.Letter0();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.Letter1();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.Letter2();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.Letter3();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.Letter4();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.Letter5();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.Letter6();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.Letter7();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.Letter8();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;
	Code = LetterCode.Letter9();
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY);//!!!!!!!!
	DrawLetter(Buffer, Code, WriteCursorX, WriteCursorY + 1);
	WriteCursorX += 12;	
}
