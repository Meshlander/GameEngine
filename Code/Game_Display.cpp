inline void ParticleDraw(game_offscreen_buffer *Buffer, int x, int y, int z)
{
	uint8 *PixelToColor = (
		(uint8 *)Buffer->Memory
		+ x * Buffer->BytesPerPixel
		+ y * Buffer->Pitch);
	uint32 *Pixel = (uint32 *)PixelToColor;
	uint32 Color = 0xFFFFFF00;
	if (z != 0)
	{
		Color = z * 1;
	}
	*Pixel++ = Color;
}
//Use particles for drawing, particles might be able to mold together, and give interesting effects
//takes up more memory but might be less calculation heavy
//particles should "bleed" over the edges
//each individual particle has to be moved when the object is moving, which can be calculation heavy,
//but with the vector based rot algorhythm you might be a ble to pull it off.
//maybe you could use vertices and then "shoot" thwe filler particles which would "reproduce in a way
//Like bacteria they would reproduce until the whole area of the rect is filled"
inline void PixelColorB(game_offscreen_buffer *Buffer, int x, int y, int z)
{
	uint8 *PixelToColor = (
		(uint8 *)Buffer->Memory
		+ x * Buffer->BytesPerPixel
		+ y * Buffer->Pitch);
	uint32 *Pixel = (uint32 *)PixelToColor;
	uint32 Color = 0xFFFFFF00;
	if (z != 0)
	{
		Color = z * 1;
	}
	*Pixel++ = Color;
}
internal_function void
DrawCall(game_offscreen_buffer *Buffer
	//, draw_buffer_poly_info *PolyInfo
)
{//bal->jobb, lent->fent
	//uint8 *Row = (uint8 *)Buffer->Memory;//256 os négyzet, row writtne into memory

	uint8 *Column = (
		(uint8 *)Buffer->Memory
		//*Buffer->BytesPerPixel
		//*Buffer->Pitch
		);

	for (int Y = 0; Y < Buffer->Height; ++Y)
	{
		//uint8 *Pixel = (uint8 *)Row;

		Buffer->IsColoring = false;

		for (int X = 0; X < Buffer->Width; ++X)
		{//BB GG RR xx //4byte/pixel
			uint8 *PixelToColor = (
				(uint8 *)Buffer->Memory
				+ X * Buffer->BytesPerPixel
				+ Y * Buffer->Pitch);
			uint32 *Pixel32 = (uint32 *)PixelToColor;

			//*Pixel32 = 0xFFFF00;

			//*Pixel = 0;//(uint8)(X*4 + XOffset);//the gradient is the 8 bits of y going up
			//Pixel++;
			//*Pixel = 0;//(uint8)(Y*4 + YOffset);
			//Pixel++;
			//*Pixel = 0;
			//Pixel++;
			//*Pixel = 255;
			//Pixel++;

			//Buffer->IsColoring = true;
			v3 A;
			v3 B;
			//B = v3{ 0,0,0 };
			if (*Pixel32 != 0x000000 && *Pixel32 != 0xFFFFFF 
				&& *Pixel32 != 0xFFFFFF00
				//&& !Buffer->IsColoring
				)
			{
				Buffer->IsColoring = true;

				A = v3{ (real64)X,(real64)Y,(real64)*Pixel32 };

				for (int n = (X+1); n < (X+1) + (400 - X); n++)
				{
					uint8 *PixelToColorB = (
						(uint8 *)Buffer->Memory
						+ n * Buffer->BytesPerPixel
						+ Y * Buffer->Pitch);
					uint32 *Pixel32B = (uint32 *)PixelToColorB;
					//ITT TALÁN ELÉG AZ X EL OPERÁLNI::::::::NEM KELL EZT A SOKAT UJRA SZÁMOLNI....

					if (*Pixel32B != 0x000000 && *Pixel32B != 0xFFFFFF 
						//&& *Pixel32 != 0xFFFFFF00
						//&& B.x == 0.00f
						)
					{
						B = v3{ (real64)n,(real64)Y,(real64)*Pixel32B };
					}	
					
				}
				//real32 deltaY;
				real32 deltaX;
				real32 deltaZ;
				//deltaY = B.y - A.y;//y,y
				deltaX = B.x - A.x;//x,x
				deltaZ = B.z - A.z;//x,x
				real32 ZToAdd = deltaZ / (deltaX);

				int x;
				int y = (int)A.y;
				int z = (int)A.z;

				if (
					(int)A.x > 640 && (int)B.x > 640 ||
					A.x < 0 && B.x < 0 					
					)//ÍRD ÁT ne returnnel hanem foglald bele az alsó részt és true nak kelljen lennie
				{
					//return;
				}

				for (int n = (int)A.x; n < (int)B.x; n++)
				{
					z += (uint32)ZToAdd;
					x = n;
					X = n;
					PixelColorB(Buffer, x, y, z);
				}

				//

				//B = v3{0,0,0};
				//ki kell számolni a z értéket mennyivel kell csökkenteni vagy növelni lépésenként..
				//itt elég a pixelcolorB ami addig színez a z értékkel
				//ha beleütközik egy másik polygonba akkor kell mást csinálni
			}			
		}
		//Row += Buffer->Pitch;//pitch is what gives markings to 1d representation 1234PITCH567P...
		Column += Buffer->Pitch;
	}
}
internal_function void
DrawSky(game_offscreen_buffer *Buffer, int XOffset, int YOffset)
{//bal->jobb, lent->fent
	uint8 *Row = (uint8 *)Buffer->Memory;//256 os négyzet, row writtne into memory

	uint8 *Column = (
		(uint8 *)Buffer->Memory
		+ 0 * Buffer->BytesPerPixel
		+ 0 * Buffer->Pitch);

	for (int Y = 0; Y < Buffer->Height; ++Y)
	{
		uint8 *Pixel = (uint8 *)Row;//pixel writtne into row
		//uint32 *Pixel32 = (uint32 *)Column;
		
		//az egész sort színezi valahogy...
		Buffer->IsColoring = false;
		for (int X = 0; X < Buffer->Width; ++X)
		{//BB GG RR xx
		 //4byte/pixel
			uint8 *PixelToColor = (
				(uint8 *)Buffer->Memory
				+ X * Buffer->BytesPerPixel
				+ Y * Buffer->Pitch);
			uint32 *Pixel32 = (uint32 *)PixelToColor;
			

			*Pixel32++ = 0x00000000;

		}
		Row += Buffer->Pitch;//pitch is what gives markings to 1d representation 1234PITCH567P...
		Column += Buffer->Pitch;
	}
}
internal_function void
DrawRectangle(game_offscreen_buffer *Buffer, int LeftX, int UpY)
{
	//ifoutofbounds
	if (
		LeftX > Buffer->Width - 10 || UpY > Buffer->Height - 10
		|| LeftX < 0 || UpY < 0
		)
	{
		return;
	}
	uint32 Color = 0xFFFFFFFF;
	//color Col = {};
	//Color = Col.Color(255, 0, 0, 255);

	uint8 *Column = (
		(uint8 *)Buffer->Memory
		+ LeftX * Buffer->BytesPerPixel
		+ UpY * Buffer->Pitch);
	for (int X = LeftX; X < LeftX + 10; ++X)
	{
		uint32 *Pixel = (uint32 *)Column;
		for (int Y = UpY; Y < UpY + 10; ++Y)
		{
			*Pixel++ = Color;
		}
		Column += Buffer->Pitch;
	}
}
internal_function void
DrawGameCursor(game_offscreen_buffer *Buffer, int LeftX, int UpY)
{
	//ifoutofbounds
	if (
		LeftX > Buffer->Width - 10 || UpY > Buffer->Height - 10
		|| LeftX < 0 || UpY < 0
		)
	{
		return;
	}
	uint32 Color = 0xFFFFFFFF;
	//color Col = {};
	//Color = Col.Color(255, 0, 0, 255);

	uint8 *Column = (
		(uint8 *)Buffer->Memory
		+ LeftX * Buffer->BytesPerPixel
		+ UpY * Buffer->Pitch);
	for (int X = LeftX; X < LeftX + 10; ++X)
	{
		uint32 *Pixel = (uint32 *)Column;
		for (int Y = UpY; Y < UpY + 10; ++Y)
		{
			*Pixel++ = Color;
		}
		Column += Buffer->Pitch;
	}
}
inline uint32 Color(uint8 B, uint8 G, uint8 R, uint8 A)
{
	uint32 Result;
	Result = (B << 24) | (G << 16) | (R << 8) | (A << 0);
	return Result;
}
internal_function void
DrawLetter(game_offscreen_buffer *Buffer, letter_color_array LetterArray, int LetterX, int LetterY)
{//bbggrrxx	
	real32 nrow = 0;
	real32 ncol = 0;
	int8 n = 0;
	uint32 Color = LetterArray.LetterColorCode[0];

	uint8 *Column = (
		(uint8 *)Buffer->Memory
		+ LetterX * Buffer->BytesPerPixel
		+ LetterY * Buffer->Pitch);
	for (int X = LetterX; X < LetterX + 4; X = X + 1)//4/8 mukszik 8/4 nem mert a for belépései mások...
	{
		uint32 *Pixel = (uint32 *)Column;
		for (int Y = LetterY; Y < LetterY + 8; ++Y)
		{
			n = (int8)nrow;
			uint32 Color = LetterArray.LetterColorCode[n];
			*Pixel++ = Color;
			nrow += 0.5f;	//itt kell az X el operálni		
		}
		Column = Column + 2 * Buffer->Pitch;
	}
}
//Line drawing function pipeline.
/**************************************************************************Line drawer*/
inline void PixelColor(game_offscreen_buffer *Buffer, int x, int y, int z)
{
	uint8 *PixelToColor = (
		(uint8 *)Buffer->Memory
		+ x * Buffer->BytesPerPixel
		+ y * Buffer->Pitch);
	uint32 *Pixel = (uint32 *)PixelToColor;
	uint32 Color;
	if (
		//*Pixel == 0x00000000 &&
		z != 0
		)
	{
		z *= 1000;
		Color = (uint32)z;
		uint32 ColorB = *Pixel;
		if (ColorB != 0 && ColorB != Color)
		{
			ColorB = ColorB;
		}
		if (Color > ColorB)//az a baj h a pixel színe nincs is eltárolva
			//egyszerûen kikeresed a bufferben a HELYET
			//pixel mindig 0... csak a mem címet keressük k iahova írunk
		{
			*Pixel++ = Color;
		}
		//*Pixel++ = Color;
	}
	//*Pixel++ = Color;

	uint8 *PixelToColorB = (
		(uint8 *)Buffer->Memory
		+ (x - 1) * Buffer->BytesPerPixel
		+ y * Buffer->Pitch);
	uint32 *PixelB = (uint32 *)PixelToColorB;
	//Color = 0xFFFFFFFF;
	if (
		*PixelB == 0x00000000 &&
		x > 0 && x < 640 && y > 0 && y < 480 && z > 0 
		&& z < 10000 * 1000
		)
	{		
		//*PixelB++ = 0x00FFFFFF;
	}
}
void DrawVectorLine
	(
	game_offscreen_buffer *Buffer,
	v3 A,
	v3 B
	)
{
	//ezt a részt a sok linedrawerbe?
	v3 BADiff = B-A;
	real32 ZtoAdd = BADiff.z / (BADiff.x + BADiff.y);

	real64 deltaY;//ezeket fel
	real64 deltaX;
	deltaY = B.y - A.y;//y,y
	deltaX = B.x - A.x;//x,x

	real64 slopeAB = AbsVal(deltaY / deltaX);
	real64 errorAB = slopeAB - 0.5;

	int x;
	int y;
	real32 z;

	for (
		x = x;
		x < (int)B.x;
		x ++
		)
	{
		z += (int)ZtoAdd;
		if (x > 0 && x < 640 && y > 0 && y < 480 && z > 0 && z < 10000)
		{
			PixelColor(Buffer, x, y, z);
		}	
		errorAB = errorAB + slopeAB;
		if (errorAB > 0.5)//az egészet át kell írni?
		{
			y = y + 1;	//we have an Y wise upside-down buffer		
			errorAB -= 1.0;
		}
	}
}
void LineDrawerI(
	game_offscreen_buffer *Buffer,
	int x, int y, real32 z,
	real64 errorAB, real64 slopeAB,
	real64 Bx,
	real32 ZToAdd,
	int PolyNumber
)
{//msdn color list https://msdn.microsoft.com/en-us/library/system.drawing.color(v=vs.110).aspx	
	for (
		x = x;
		x < (int)Bx;
		x = x + 1
		)
	{
		z += ZToAdd;
		//ezekre a helyekre be lehetne építeni a szomszédos feltételeket, 
		//hogy ki tudjon lépni félúton, de lehet nem éri meg ezzle lassítan ia függvényt
		if (x > 0 && x < 640 && y > 0 && y < 480
			//&& z > 0 && z < 10000
			)
		{
			PixelColor(Buffer, x, y, z);
		}
		errorAB = errorAB + slopeAB;
		if (errorAB > 0.5)//az egészet át kell írni?
		{
			y = y + 1;	//we have an Y wise upside-down buffer		
			errorAB -= 1.0;
		}
	}
}
void LineDrawerII(
	game_offscreen_buffer *Buffer,
	int y, int x, real32 z,
	real64 errorBA, real64 slopeBA,
	real64 By,
	real32 ZToAdd,
	int PolyNumber
)
{
	for (
		y = y;
		y < (int)By;
		y = y + 1
		)
	{
		z += ZToAdd;
		if (x > 0 && x < 640 && y > 0 && y < 480 
			//&& z > 0 && z < 10000
			)
		{
			PixelColor(Buffer, x, y, z);
		}
		errorBA = errorBA + slopeBA;
		if (errorBA > 0.5)//az egészet át kell írni?
		{
			x = x + 1;	//we have an Y wise upside-down buffer		
			errorBA -= 1.0;
		}
	}
}
void LineDrawerIII(
	game_offscreen_buffer *Buffer,
	int y, int x, real32 z,
	real64 errorBA, real64 slopeBA,
	real64 By,
	real32 ZToAdd,
	int PolyNumber
)
{
	for (
		y = y;
		y < (int)By;
		y = y + 1
		)
	{
		z += ZToAdd;
		if ((-x) > 0 && (-x) < 640 && y > 0 && y < 480 
			//&& z > 0 && z < 10000
			)
		{
			PixelColor(Buffer, (-x), y, z);
		}
		errorBA = errorBA + slopeBA;
		if (errorBA > 0.5)//az egészet át kell írni?
		{
			x = x + 1;	//we have an Y wise upside-down buffer		
			errorBA -= 1.0;
		}
	}
}
void LineDrawerIV(
	game_offscreen_buffer *Buffer,
	int x, int y, real32 z,
	real64 errorAB, real64 slopeAB,
	real64 Bx,
	real32 ZToAdd,
	int PolyNumber
)
{
	for (
		x = x;
		x < (int)Bx;
		x = x + 1
		)
	{
		z += ZToAdd;
		if ((-x) > 0 && (-x) < 640 && y > 0 && y < 480 
			//&& z > 0 && z < 10000
			)
		{
			PixelColor(Buffer, (-x), y, z);
		}
		errorAB = errorAB + slopeAB;
		if (errorAB > 0.5)//az egészet át kell írni?
		{
			y = y + 1;	//we have an Y wise upside-down buffer		
			errorAB -= 1.0;
		}
	}
}
void LineDrawerV(
	game_offscreen_buffer *Buffer,
	int x, int y, real32 z,
	real64 errorAB, real64 slopeAB,
	real64 Bx,
	real32 ZToAdd,
	int PolyNumber
)
{
	for (
		x = x;
		x < (int)Bx;
		x = x + 1
		)
	{
		z += ZToAdd;
		if ((-x) > 0 && (-x) < 640 && (-y) > 0 && (-y) < 480 
			//&& z > 0 && z < 10000
			)
		{
			PixelColor(Buffer, (-x), (-y), z);
		}
		errorAB = errorAB + slopeAB;
		if (errorAB > 0.5)//az egészet át kell írni?
		{
			y = y + 1;	//we have an Y wise upside-down buffer		
			errorAB -= 1.0;
		}
	}
}
void LineDrawerVI(
	game_offscreen_buffer *Buffer,
	int y, int x, real32 z,
	real64 errorBA, real64 slopeBA,
	real64 By,
	real32 ZToAdd,
	int PolyNumber
)
{
	z += ZToAdd;
	for (
		y = y;
		y < (int)By;
		y = y + 1
		)
	{
		z += ZToAdd;
		if ((-x) > 0 && (-x) < 640 && (-y) > 0 && (-y) < 480
			//&& z > 0 && z < 10000
			)
		{
			PixelColor(Buffer, (-x), (-y), z);
		}
		errorBA = errorBA + slopeBA;
		if (errorBA > 0.5)//az egészet át kell írni?
		{
			x = x + 1;	//we have an Y wise upside-down buffer		
			errorBA -= 1.0;
		}
	}
}
void LineDrawerVII(
	game_offscreen_buffer *Buffer,
	int y, int x, real32 z,
	real64 errorBA, real64 slopeBA,
	real64 By,
	real32 ZToAdd,
	int PolyNumber
)
{
	for (
		y = y;
		y < (int)By;
		y = y + 1
		)
	{
		z += ZToAdd;
		if (x > 0 && x < 640 && (-y) > 0 && (-y) < 480 
			//&& z > 0 && z < 10000
			)
		{
			PixelColor(Buffer, x, (-y), z);
		}
		errorBA = errorBA + slopeBA;
		if (errorBA > 0.5)//az egészet át kell írni?
		{
			x = x + 1;	//we have an Y wise upside-down buffer		
			errorBA -= 1.0;
		}
	}
}
void LineDrawerVIII(
	game_offscreen_buffer *Buffer,
	int x, int y, real32 z,
	real64 errorAB, real64 slopeAB,
	real64 Bx,
	real32 ZToAdd,
	int PolyNumber
)
{//msdn color list https://msdn.microsoft.com/en-us/library/system.drawing.color(v=vs.110).aspx	
	for (
		x = x;
		x < (int)Bx;
		x = x + 1
		)
	{
		z += ZToAdd;
		if (x > 0 && x < 640 && (-y) > 0 && (-y) < 480 
			//&& z > 0 && z < 10000
			)
		{
			PixelColor(Buffer, x, (-y), z);
		}
		errorAB = errorAB + slopeAB;
		if (errorAB > 0.5)//az egészet át kell írni?
		{
			y = y + 1;	//we have an Y wise upside-down buffer		
			errorAB -= 1.0;
		}
	}
}
internal_function void
PreparationForLineDrawer(
	game_offscreen_buffer *Buffer,
	v3 pointA, v3 pointB,
	int PolyNumber,
	camera_buffer *CameraBuffer
)
{
	v3 inA = pointA;
	v3 inB = pointB;	
	/*************************************************************************ROTORIENTATION*/
	inA.x = dot(CameraBuffer->PlayerBuffer - pointA, CameraBuffer->CamX);
	inA.y = dot(CameraBuffer->PlayerBuffer - pointA, CameraBuffer->CamY);
	inA.z = dot(CameraBuffer->PlayerBuffer - pointA, CameraBuffer->CamZ);
	inB.x = dot(CameraBuffer->PlayerBuffer - pointB, CameraBuffer->CamX);
	inB.y = dot(CameraBuffer->PlayerBuffer - pointB, CameraBuffer->CamY);
	inB.z = dot(CameraBuffer->PlayerBuffer - pointB, CameraBuffer->CamZ);
	/*************************************************************************PROJECTION*/
	inA.z -= 1000; //+CameraBuffer->PlayerBuffer.z;
	inB.z -= 1000; //+CameraBuffer->PlayerBuffer.z;

	inA.x = (inA.x / (1000.00f + inA.z))*1000.00f;//fel voltak cserélv
	inA.y = (inA.y / (1000.00f + inA.z))*1000.00f;
	inB.x = (inB.x / (1000.00f + inB.z))*1000.00f;
	inB.y = (inB.y / (1000.00f + inB.z))*1000.00f;

	inA.x += 320;//fel voltak cserélv//ide a buffer size kell
	inA.y += 240;
	inB.x += 320;
	inB.y += 240;
	/***********************************************************************************/
	real64 deltaY;
	real64 deltaX;
	real32 deltaZ;
	deltaY = inB.y - inA.y;//y,y
	deltaX = inB.x - inA.x;//x,x
	deltaZ = inB.z - inA.z;//x,x

	int y = (int)inA.y;//y
	int x = (int)inA.x;//x
	real32 z = inA.z;

	//int Bx = (int)inB.x;
	//int By = (int)inB.y;

	real64 slopeAB = AbsVal(deltaY / deltaX);
	real64 slopeBA = AbsVal(deltaX / deltaY);
	real64 errorAB = slopeAB - 0.5;
	real64 errorBA = slopeBA - 0.5;

	real32 ZToAdd = deltaZ / (deltaX + deltaY);

	//real64 distanceAB = mathFunctions.DistanceBetweenTwoPoints(inA, inB);
	/*******************************************************************************/
	uint32 Color = 0xFFFFFFFF;
	//A z koordinátát át kell vinni a rajzolónak A 2Z koordináta közt is lesz egy slope,
	//a line drawer aszerint a slope szerint kell szinezze a pixeleket másképp
	// ALFA channelbe lesz a polgon kódolva, RGB channelbe rakjuk a Z távot a kamerától
	// A polifiller az RGB alapján fog dönteni, hogy melyik POLiGON fedi a másikat...
	/*******************************************************************************/

	//Bresenham's line algorithm. 

	if (
		inA.x > 640 && inB.x > 640 ||
		inA.x < 0 && inB.x < 0 ||
		inA.y > 480 && inB.x > 480 ||
		inA.y < 0 && inB.y < 0
		|| inA.z < 0.008f && inB.z < 0.008f
		|| inA.z > 10000 && inB.z > 10000
		)//ÍRD ÁT ne returnnel hanem foglald bele az alsó részt és true nak kelljen lennie
	{
		return;
	}

	//DrawVectorLine(Buffer, inA, inB);
	if (deltaY >= 0 && deltaX >= deltaY && deltaX >= 0)//AZ ELLENTÉTES OLDALON KI LEHET VENNI AZ =-t H NE KEZELJÜK KÉTSZER A FELTÉTELT
	{ //I
	  //LineDrawer(Buffer, x, y, errorAB, slopeAB, inB.x, x, Bx, By);
		LineDrawerI(Buffer, x, y, z, errorAB, slopeAB, inB.x, ZToAdd, PolyNumber);
	}
	if (deltaY >= 0 && deltaX <= deltaY && deltaX >= 0)
	{ //II
	  //LineDrawer(Buffer, y, x, errorBA, slopeBA, inB.y, -x, Bx, By);
		LineDrawerII(Buffer, y, x, z, errorBA, slopeBA, inB.y, ZToAdd, PolyNumber);
	}
	if (deltaY >= 0 && AbsVal(deltaX) <= deltaY && deltaX <= 0)
	{ //III
	  //LineDrawer(Buffer, y, -x, errorBA, slopeBA, inB.y, -x, -Bx, By);
		LineDrawerIII(Buffer, y, -x, z, errorBA, slopeBA, inB.y, ZToAdd, PolyNumber);
	}
	if (deltaY >= 0 && AbsVal(deltaX) >= deltaY && deltaX <= 0)
	{ //IV
	  //LineDrawer(Buffer, -x, y, errorAB, slopeAB, -inB.x, -x, -Bx, By);
		LineDrawerIV(Buffer, -x, y, z, errorAB, slopeAB, -inB.x, ZToAdd, PolyNumber);
	}
	if (deltaY <= 0 && AbsVal(deltaX) >= AbsVal(deltaY) && deltaX <= 0)
	{ //V
	  //LineDrawer(Buffer, -x, -y, errorAB, slopeAB, -inB.x, -x, -Bx, -By);
		LineDrawerV(Buffer, -x, -y, z, errorAB, slopeAB, -inB.x, ZToAdd, PolyNumber);
	}
	if (deltaY <= 0 && AbsVal(deltaX) <= AbsVal(deltaY) && deltaX <= 0)
	{ //VI
	  //LineDrawer(Buffer, -y, -x, errorBA, slopeBA, -inB.y, x, -Bx, -By);
		LineDrawerVI(Buffer, -y, -x, z, errorBA, slopeBA, -inB.y, ZToAdd, PolyNumber);
	}
	if (deltaY <= 0 && deltaX <= AbsVal(deltaY) && deltaX >= 0)
	{ //VII
	  //LineDrawer(Buffer, -y, x, errorBA, slopeBA, -inB.y, x, Bx, -By);
		LineDrawerVII(Buffer, -y, x, z, errorBA, slopeBA, -inB.y, ZToAdd, PolyNumber);
	}
	if (deltaY <= 0 && deltaX >= AbsVal(deltaY) && deltaX >= 0)
	{ //VIII
	  //LineDrawer(Buffer, x, -y, errorAB, slopeAB, inB.x, x, Bx, -By);
		LineDrawerVIII(Buffer, x, -y, z, errorAB, slopeAB, inB.x, ZToAdd, PolyNumber);
	}
}
internal_function void
DrawPolygons(
	game_offscreen_buffer *Buffer, camera_buffer *CameraBuffer
	, map *Map, cat_model *CatModel
)
{
	for (uint32 m = 0; m < Map->MapPolyCount; m = m + 1)
	{
		for (uint8 n = 0; n < 3; n = n + 1)
		{
			PreparationForLineDrawer(
				Buffer,
				Map->MapPolygons[m].Points[n],
				Map->MapPolygons[m].Points[n + 1],
				m,
				CameraBuffer
			);
		}
		PreparationForLineDrawer(
			Buffer,
			Map->MapPolygons[m].Points[0],
			Map->MapPolygons[m].Points[3],
			m,
			CameraBuffer
		);
	}
	/*for (uint8 n = 0; n < 5; n = n + 1)
	{
	PreparationForLineDrawer(
	Buffer,
	CatModel->RightPelvis[n],
	CatModel->RightPelvis[n + 1],
	CameraBuffer
	);
	}
	for (uint8 n = 0; n < 5; n = n + 1)
	{
	PreparationForLineDrawer(
	Buffer,
	CatModel->RightKnee[n],
	CatModel->RightKnee[n + 1],
	CameraBuffer
	);
	}
	for (uint8 n = 0; n < 5; n = n + 1)
	{
	PreparationForLineDrawer(
	Buffer,
	CatModel->RightFoot[n],
	CatModel->RightFoot[n + 1],
	CameraBuffer
	);
	}*/
}
/*******************************************************************************************PolyFilling*/
//inline void PolyFillColor(game_offscreen_buffer *Buffer, int x, int y, uint32 Color)
//{
//	if (x > 0 && x < 640 && y > 0 && y < 480)
//	{
//		uint8 *PixelToColor = (
//			(uint8 *)Buffer->Memory
//			+ x * Buffer->BytesPerPixel
//			+ y * Buffer->Pitch);
//		uint32 *Pixel = (uint32 *)PixelToColor;
//		uint32 Col = Color;//0xFFFFFFFF;
//
//						   //azt kell kezelni amikor több pixelen van x tengelyen a vonal
//
//		if (*Pixel == 0xFFFFFFFF && !Buffer->IsColoring)
//		{
//			Buffer->IsColoring = true;
//		}
//		else if (*Pixel == 0xFFFFFFFF && Buffer->IsColoring)
//		{
//			Buffer->IsColoring = false;
//		}
//
//		if (*Pixel == Col)
//		{
//			return;
//			//*Pixel++ = Color;
//		}
//		else if (*Pixel != 0xFFFFFFFF && Buffer->IsColoring)
//		{
//			*Pixel++ = Col;
//			//a colort is eleve szétszedve kell küldeni, ha nem struc pointerezést akarok!!!
//		}
//	}
//}
//void PolyFillerI(
//	game_offscreen_buffer *Buffer,
//	int x, int y,
//	int endX, int endY,
//	uint32 ColorIN
//)
//{
//	int StartX = x;
//	int StartY = y;
//	for (y = y; y < (int)endY; y = y + 1)
//	{
//		int xChange = x;
//		Buffer->IsColoring = false;
//		//PolyMarkEmptyBehind(Buffer, x, y);
//		for (xChange = xChange; xChange < (int)endX; xChange = xChange + 1)
//		{
//			//PolyMarkEmptyBelow(Buffer, x, y);		
//			//if (uint32 *Pixel == 0xFFFFFFFF;)
//			//{			
//			PolyFillColor(Buffer, xChange, y, ColorIN);
//			//}
//		}
//	}
//	//here you fill out the places that remained empty after the first fill
//}
//void PolyFillerChooser(
//	game_offscreen_buffer *Buffer,
//	v3 A, v3 B, v3 C, v3 D,
//	camera_buffer *CameraBuffer,
//	uint32 Color
//)
//{
//	if (A.x <= B.x && A.x <= C.x && A.x <= D.x
//		&& A.y <= B.y && A.y <= C.y && A.y <= D.y
//		)
//	{
//		int x = (int)A.x;
//		int y = (int)A.y;
//		int endX = (int)C.x;
//		int endY = (int)C.y;
//		x = x - CameraBuffer->PlayerBuffer.x;//fel voltak cserélv
//		y = y - CameraBuffer->PlayerBuffer.y;
//		endX = endX - CameraBuffer->PlayerBuffer.x;
//		endY = endY - CameraBuffer->PlayerBuffer.y;
//		PolyFillerI(
//			Buffer, x, y, endX, endY, Color
//		);
//	}
//	else if (B.x <= C.x && B.x <= D.x && B.x <= A.x
//		&& B.y <= C.y && B.y <= D.y && B.y <= A.y
//		)
//	{
//		int x = (int)B.x;
//		int y = (int)B.y;
//		int endX = (int)D.x;
//		int endY = (int)D.y;
//		x = x - CameraBuffer->PlayerBuffer.x;//fel voltak cserélv
//		y = y - CameraBuffer->PlayerBuffer.y;
//		endX = endX - CameraBuffer->PlayerBuffer.x;
//		endY = endY - CameraBuffer->PlayerBuffer.y;
//		PolyFillerI(
//			Buffer, x, y, endX, endY, Color
//		);
//	}
//	else if (C.x <= D.x && C.x <= A.x && C.x <= B.x
//		&& C.y <= D.y && C.y <= A.y && C.y <= B.y
//		)
//	{
//		int x = (int)C.x;
//		int y = (int)C.y;
//		int endX = (int)A.x;
//		int endY = (int)A.y;
//		x = x - CameraBuffer->PlayerBuffer.x;//fel voltak cserélv
//		y = y - CameraBuffer->PlayerBuffer.y;
//		endX = endX - CameraBuffer->PlayerBuffer.x;
//		endY = endY - CameraBuffer->PlayerBuffer.y;
//		PolyFillerI(
//			Buffer, x, y, endX, endY, Color
//		);
//	}
//	else if (D.x <= A.x && D.x <= B.x && D.x <= C.x
//		&& D.y <= A.y && D.y <= B.y && D.y <= C.y
//		)
//	{
//		int x = (int)D.x;
//		int y = (int)D.y;
//		int endX = (int)B.x;
//		int endY = (int)B.y;
//		x = x - CameraBuffer->PlayerBuffer.x;//fel voltak cserélv
//		y = y - CameraBuffer->PlayerBuffer.y;
//		endX = endX - CameraBuffer->PlayerBuffer.x;
//		endY = endY - CameraBuffer->PlayerBuffer.y;
//		PolyFillerI(
//			Buffer, x, y, endX, endY, Color
//		);
//	}
//}

//internal_function void
//FillPolygons(
//	game_offscreen_buffer *Buffer, camera_buffer *CameraBuffer
//	, map *Map
//)
//{
//	for (uint32 m = 0; m < Map->MapPolyCount; m = m + 1)
//	{
//
//		uint8 n = 0;
//		PolyFillerChooser(
//			Buffer,
//			Map->MapPolygons[m].Points[n],
//			Map->MapPolygons[m].Points[n + 1],
//			Map->MapPolygons[m].Points[n + 2],
//			Map->MapPolygons[m].Points[n + 3],
//			CameraBuffer,
//			Map->MapPolygons[m].Color
//		);
//	}
//}
internal_function void RenderingPipeline(
	game_offscreen_buffer *Buffer, camera_buffer *CameraBuffer,
	cat_model *CatModel
)
{

	//0
	polygon CatPolys[128];
	uint32 PolyCol = Color(200, 200, 200, 255);
	/*********************************************************************************/
	CatPolys[0] = polygon{
		CatModel->RightPelvis[0], CatModel->RightPelvis[1],
		CatModel->RightKnee[0], CatModel->RightKnee[1],
		PolyCol
	};
	CatPolys[1] = polygon{
		CatModel->RightPelvis[1], CatModel->RightPelvis[2],
		CatModel->RightKnee[1], CatModel->RightKnee[2],
		PolyCol
	};
	CatPolys[2] = polygon{
		CatModel->RightPelvis[2], CatModel->RightPelvis[3],
		CatModel->RightKnee[2], CatModel->RightKnee[3],
		PolyCol
	};
	CatPolys[3] = polygon{
		CatModel->RightPelvis[3], CatModel->RightPelvis[4],
		CatModel->RightKnee[3], CatModel->RightKnee[4],
		PolyCol
	};
	CatPolys[4] = polygon{
		CatModel->RightPelvis[4], CatModel->RightPelvis[5],
		CatModel->RightKnee[4], CatModel->RightKnee[5],
		PolyCol
	};
	CatPolys[5] = polygon{
		CatModel->RightPelvis[5], CatModel->RightPelvis[6],
		CatModel->RightKnee[5], CatModel->RightKnee[6],
		PolyCol
	};
	CatPolys[6] = polygon{
		CatModel->RightPelvis[6], CatModel->RightPelvis[7],
		CatModel->RightKnee[6], CatModel->RightKnee[7],
		PolyCol
	};
	CatPolys[7] = polygon{
		CatModel->RightPelvis[7], CatModel->RightPelvis[8],
		CatModel->RightKnee[7], CatModel->RightKnee[8],
		PolyCol
	};
	/*******************************************************************************************/
	CatPolys[8] = polygon{
		CatModel->LeftPelvis[0], CatModel->LeftPelvis[1],
		CatModel->LeftKnee[0], CatModel->LeftKnee[1],
		PolyCol
	};
	CatPolys[9] = polygon{
		CatModel->LeftPelvis[1], CatModel->LeftPelvis[2],
		CatModel->LeftKnee[1], CatModel->LeftKnee[2],
		PolyCol
	};
	CatPolys[10] = polygon{
		CatModel->LeftPelvis[2], CatModel->LeftPelvis[3],
		CatModel->LeftKnee[2], CatModel->LeftKnee[3],
		PolyCol
	};
	CatPolys[11] = polygon{
		CatModel->LeftPelvis[3], CatModel->LeftPelvis[4],
		CatModel->LeftKnee[3], CatModel->LeftKnee[4],
		PolyCol
	};
	CatPolys[12] = polygon{
		CatModel->LeftPelvis[4], CatModel->LeftPelvis[5],
		CatModel->LeftKnee[4], CatModel->LeftKnee[5],
		PolyCol
	};
	CatPolys[13] = polygon{
		CatModel->LeftPelvis[5], CatModel->LeftPelvis[6],
		CatModel->LeftKnee[5], CatModel->LeftKnee[6],
		PolyCol
	};
	CatPolys[14] = polygon{
		CatModel->LeftPelvis[6], CatModel->LeftPelvis[7],
		CatModel->LeftKnee[6], CatModel->LeftKnee[7],
		PolyCol
	};
	CatPolys[15] = polygon{
		CatModel->LeftPelvis[7], CatModel->LeftPelvis[8],
		CatModel->LeftKnee[7], CatModel->LeftKnee[8],
		PolyCol
	};
	/****************************************************************************/
	CatPolys[16] = polygon{
		CatModel->RightKnee[0], CatModel->RightKnee[1],
		CatModel->RightFoot[0], CatModel->RightFoot[1],
		PolyCol
	};
	CatPolys[17] = polygon{
		CatModel->RightKnee[1], CatModel->RightKnee[2],
		CatModel->RightFoot[1], CatModel->RightFoot[2],
		PolyCol
	};
	CatPolys[18] = polygon{
		CatModel->RightKnee[2], CatModel->RightKnee[3],
		CatModel->RightFoot[2], CatModel->RightFoot[3],
		PolyCol
	};
	CatPolys[19] = polygon{
		CatModel->RightKnee[3], CatModel->RightKnee[4],
		CatModel->RightFoot[3], CatModel->RightFoot[4],
		PolyCol
	};
	CatPolys[20] = polygon{
		CatModel->RightKnee[4], CatModel->RightKnee[5],
		CatModel->RightFoot[4], CatModel->RightFoot[5],
		PolyCol
	};
	CatPolys[21] = polygon{
		CatModel->RightKnee[5], CatModel->RightKnee[6],
		CatModel->RightFoot[5], CatModel->RightFoot[6],
		PolyCol
	};
	CatPolys[22] = polygon{
		CatModel->RightKnee[6], CatModel->RightKnee[7],
		CatModel->RightFoot[6], CatModel->RightFoot[7],
		PolyCol
	};
	CatPolys[23] = polygon{
		CatModel->RightKnee[7], CatModel->RightKnee[8],
		CatModel->RightFoot[7], CatModel->RightFoot[8],
		PolyCol
	};
	/******************************************************************************/
	CatPolys[24] = polygon{
		CatModel->LeftKnee[0], CatModel->LeftKnee[1],
		CatModel->LeftFoot[0], CatModel->LeftFoot[1],
		PolyCol
	};
	CatPolys[25] = polygon{
		CatModel->LeftKnee[1], CatModel->LeftKnee[2],
		CatModel->LeftFoot[1], CatModel->LeftFoot[2],
		PolyCol
	};
	CatPolys[26] = polygon{
		CatModel->LeftKnee[2], CatModel->LeftKnee[3],
		CatModel->LeftFoot[2], CatModel->LeftFoot[3],
		PolyCol
	};
	CatPolys[27] = polygon{
		CatModel->LeftKnee[3], CatModel->LeftKnee[4],
		CatModel->LeftFoot[3], CatModel->LeftFoot[4],
		PolyCol
	};
	CatPolys[28] = polygon{
		CatModel->LeftKnee[4], CatModel->LeftKnee[5],
		CatModel->LeftFoot[4], CatModel->LeftFoot[5],
		PolyCol
	};
	CatPolys[29] = polygon{
		CatModel->LeftKnee[5], CatModel->LeftKnee[6],
		CatModel->LeftFoot[5], CatModel->LeftFoot[6],
		PolyCol
	};
	CatPolys[30] = polygon{
		CatModel->LeftKnee[6], CatModel->LeftKnee[7],
		CatModel->LeftFoot[6], CatModel->LeftFoot[7],
		PolyCol
	};
	CatPolys[31] = polygon{
		CatModel->LeftKnee[7], CatModel->LeftKnee[8],
		CatModel->LeftFoot[7], CatModel->LeftFoot[8],
		PolyCol
	};
	/**********************************************************************************/
	for (uint8 o = 0; o < 7; o = o + 1)
	{

		/*PreparationForLineDrawer(
			Buffer,
			CatPolys[o].Points[0],
			CatPolys[o].Points[1],
			CameraBuffer
		);
		PreparationForLineDrawer(
			Buffer,
			CatPolys[o].Points[1],
			CatPolys[o].Points[2],
			CameraBuffer
		);
		PreparationForLineDrawer(
			Buffer,
			CatPolys[o].Points[2],
			CatPolys[o].Points[3],
			CameraBuffer
		);
		PreparationForLineDrawer(
			Buffer,
			CatPolys[o].Points[3],
			CatPolys[o].Points[0],
			CameraBuffer
		);*/
	}
	/*******************************************************************************/
	//I
	real32 CatPolysAverageZ[128];
	for (int n = 0; n < 128; n++)
	{
		CatPolysAverageZ[n] = polyAverageZ(CatPolys[n]);
	}
	//I
	//Make a new poly list for fewer polys, which are actually close enough to one another so that they can
	//actually collapse, and then send two lists for polydrawer, one untouched, and one sorted...
	//II
	CatPolysAverageZ[5] = 5;
	polygon PrevCatPolys[128];
	for (int n = 4; n > 0; n--)
	{
		for (int m = 5; m > 0; m--)
		{
			if (CatPolysAverageZ[n] > CatPolysAverageZ[m])
			{
				//a = a + b;
				//b = a - b;
				//a = a - b;
				CatPolysAverageZ[n] = CatPolysAverageZ[n] + CatPolysAverageZ[m];
				CatPolysAverageZ[m] = CatPolysAverageZ[n] - CatPolysAverageZ[m];
				CatPolysAverageZ[n] = CatPolysAverageZ[n] - CatPolysAverageZ[m];
				//itt nem lehet algebra mágiával felcserélni õket
				//ez itt nem jó?
				//PrevCatPolys[n] = CatPolys[n];
				//CatPolys[n] = CatPolys[m];
				//CatPolys[m] = PrevCatPolys[n];
			}
		}
	}
	//II
	for (uint8 x = 0; x < 7; x = x + 1)
	{

		uint8 n = 0;
	/*	PolyFillerChooser(
			Buffer,
			CatPolys[x].Points[n],
			CatPolys[x].Points[n + 1],
			CatPolys[x].Points[n + 2],
			CatPolys[x].Points[n + 3],
			CameraBuffer,
			0
		);*/
		//there are NO lines
	}

	/*v3 RightPelvis[8];
	v3 RightKnee[8];
	v3 RightFoot[8];
	v3 RightToe[8];
	v3 LeftPelvis[8];
	v3 LeftThigh[8];
	v3 LeftShin[8];
	v3 LeftFoot[8];
	v3 Chest[8];
	v3 Neck[8];
	v3 Head[8];
	v3 RightShoulder[8];
	v3 RightUpperArm[8];
	v3 RightLowerArm[8];
	v3 RightHand[8];
	v3 LeftShoulder[8];
	v3 LeftUpperArm[8];
	v3 LeftLowerArm[8];
	v3 LeftHand[8];*/

	//0: MAKE POLYGONS
	//I: check for overlapping polys
	//II: decide rendering order of overlapping polys
}
/********************************************************************************************************/