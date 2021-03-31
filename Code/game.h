#if !defined(GAME_H)
/* ======================================================
SFile: S
SDate: S
SRevision: S
SCreator: Gergely Zara S
SNotice: S
======================================================*/
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}

#define Kilobytes(Value) ((Value)*1024)
#define Megabytes(Value) (Kilobytes(Value)*1024)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array[0])))

#define Abs(Value) if((Value)<(0)) {Value = Value * -1;}
/******************************************************************Basic math definitions*/
//#define Abs(Value) if(Value < 0) {Value = Value * -1;}

//services game provides for platform like win32
/*********************************************************************************MATH.H*/
struct color
{
	uint32 Color(uint8 B, uint8 G, uint8 R, uint8 A)
	{
		uint32 Result;
		Result = (B << 24) | (G << 16) | (R << 8) | (A /*<< 0*/);
		return Result;
	}
};
struct v3
{
	real64 x;
	real64 y;
	real64 z;
};
/*********************************************V3Operators*/
inline real32 Distance(v3 A, v3 B)
{
	real32 Distance =
		sqrt(pow((A.x - B.x), 2)
			+ pow((A.y - B.y), 2)
			+ pow((A.z - B.z), 2));
	return Distance;
}
inline v3 operator+(real32 A, v3 B)
{
	v3 Result;
	Result = v3{B.x + A, B.y +A, B.z +A};
	return Result;
}
inline v3 operator+(v3 B, real32 A)
{
	v3 Result;
	Result = v3{ B.x + A, B.y + A, B.z + A };
	return Result;
}
inline v3 operator+(v3 A, v3 B)
{
	v3 Result;
	Result = v3{ B.x + A.x, B.y + A.y, B.z + A.z };
	return Result;
}
inline v3 operator-(real32 A, v3 B)
{
	v3 Result;
	Result = v3{ A - B.x, A-B.y, A-B.z };
	return Result;
}
inline v3 operator-(v3 A, real32 B)
{
	v3 Result;
	Result = v3{ A.x - B, A.y - B, A.z - B };
	return Result;
}
inline v3 operator-(v3 A, v3 B)
{
	v3 Result;
	Result = v3{ A.x - B.x, A.y - B.y, A.z - B.z };//itt + ok voltak
	return Result;
}
inline v3 operator*(v3 A, real32 B)
{
	v3 Result;
	Result = v3{ A.x * B, A.y * B, A.z * B };
	return Result;
}
inline v3 operator*(real32 B, v3 A)
{
	v3 Result;
	Result = v3{ A.x * B, A.y * B, A.z * B };
	return Result;
}
inline v3 operator*(v3 A, v3 B)
{
	v3 Result;
	Result = v3{ A.x * B.x, A.y * B.y, A.z * B.z };
	return Result;
}
inline bool32 operator==(v3 A, v3 B)
{
	bool32 Result;
	if (A.x == B.x && 
		A.y == B.y && 
		A.z == B.z)
	{
		Result = true;
	}
	else
	{
		Result = false;
	}
}
inline bool32 operator>(v3 A, v3 B)
{
	bool32 Result;
	if (A.x > B.x &&
		A.y > B.y &&
		A.z > B.z)
	{
		Result = true;
	}
	else
	{
		Result = false;
	}
}
inline bool32 operator>=(v3 A, v3 B)
{
	bool32 Result;
	if (A.x >= B.x &&
		A.y >= B.y &&
		A.z >= B.z)
	{
		Result = true;
	}
	else
	{
		Result = false;
	}
}
inline bool32 operator<(v3 A, v3 B)
{
	bool32 Result;
	if (A.x < B.x &&
		A.y < B.y &&
		A.z < B.z)
	{
		Result = true;
	}
	else
	{
		Result = false;
	}
}
inline bool32 operator<=(v3 A, v3 B)
{
	bool32 Result;
	if (A.x <= B.x &&
		A.y <= B.y &&
		A.z <= B.z)
	{
		Result = true;
	}
	else
	{
		Result = false;
	}
}
inline real32 dot(v3 A, v3 B)
{
	real32 Result;
	Result = A.x * B.x + A.y * B.y + A.z * B.z ;
	return Result;
}
inline v3 cross(v3 V1, v3 V2)
{
	v3 Result;
	Result.x = (V1.y * V2.z) - (V1.z * V2.y);
	Result.y = (V1.z * V2.x) - (V1.x * V2.z);
	Result.z = (V1.x * V2.y) - (V1.y * V2.x);
	return Result;
}
inline v3 normal(v3 Pstart, v3 Pend)
{
	v3 Result;
	Result.x = Result.x / Distance(Pstart, Pend);
	Result.y = Result.y / Distance(Pstart, Pend);
	Result.z = Result.z / Distance(Pstart, Pend);
	return Result;
}
/***************************************************************/
struct v3Ops
{
	bool32 v3IsEqual(v3 V1, v3 V2)
	{
		bool32 Result;
		if (V1.x == V2.x && V1.y == V2.y && V1.z == V2.z)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	v3 v3Add(v3 V1,v3 V2)
	{
		v3 Result;
		Result.x = V1.x + V2.x;
		Result.y = V1.y + V2.y;
		Result.z = V1.z + V2.z;
		return Result;
	}
	v3 v3Sub(v3 V1, v3 V2)
	{
		v3 Result;
		Result.x = V1.x - V2.x;
		Result.y = V1.y - V2.y;
		Result.z = V1.z - V2.z;
		return Result;
	}
	v3 v3Cross(v3 V1, v3 V2)
	{
		v3 Result;
		Result.x = (V1.y * V2.z) - (V1.z * V2.y);
		Result.y = (V1.z * V2.x) - (V1.x * V2.z);
		Result.z = (V1.x * V2.y) - (V1.y * V2.x);
		return Result;
	}
	real32 v3Dot(v3 V1, v3 V2)
	{
		v3 Res;
		Res.x = V1.x * V2.x;
		Res.y = V1.y * V2.y;
		Res.z = V1.z * V2.z;
		real32 Result = Res.x + Res.y + Res.z;
		return Result;
	}
};
inline real32 AbsVal(real32 A)
{
	if (A < 0)
	{
		A = A * -1;
		return A;
	}
	else
	{
		return A;
	}
	return A;
}
struct distance
{
	real64 Distance;
	real64 DistanceBetweenTwoPoints(v3 thisPoint, v3 thatPoint)
	{
		Distance =
			sqrt(pow((thisPoint.x - thatPoint.x), 2)
			+ pow((thisPoint.y - thatPoint.y), 2)
			+ pow((thisPoint.z - thatPoint.z), 2));
		return Distance;
	}
};
struct sine
{
	real32 Sine;
	real32 AngleSine(v3 A, v3 B, v3 C)//opposite/hypotenuse
	{
		distance Dist = {};
		real32 opp = Dist.DistanceBetweenTwoPoints(B, C);
		real32 hyp = Dist.DistanceBetweenTwoPoints(A, C);
		Sine = opp / hyp;
		return Sine;
	}
};
struct cosine
{
	real32 Cosine;
	real32 AngleCosine(v3 A, v3 B, v3 C)//adjacent/hypotenuse
	{
		distance Dist = {};
		real32 adj = Dist.DistanceBetweenTwoPoints(A, B);
		real32 hyp = Dist.DistanceBetweenTwoPoints(A, C);
		Cosine = adj / hyp;
		return Cosine;
	}
};
struct intersection_of_2lines
{
	v3 IntersectionPoint;
	v3 IntersectionOfTwoLinesIn3d(v3 A, v3 B, v3 P)
	{
		distance Dist = {};
		real64 distAB = Dist.DistanceBetweenTwoPoints(A, B);
		real64 distAP = Dist.DistanceBetweenTwoPoints(A, P);
		real64 distBP = Dist.DistanceBetweenTwoPoints(B, P);
		//real32 PAyDiff = P.y - A.y;
		//real32 PAxDiff = P.x - A.x;
		//real32 BAyDiff = B.y - A.y;
		//real32 BAxDiff = B.x - A.x;
		if (
			distAB - (distAP + distBP) < 0.8f
			&& distAB - (distAP + distBP) > -0.8f
			//PAyDiff/PAxDiff - BAyDiff/BAxDiff <= 0.02f
			//&& PAyDiff / PAxDiff - BAyDiff / BAxDiff >= -0.02f
			)
		{
			IntersectionPoint = P;
			return IntersectionPoint;
		}
		else
		{
			IntersectionPoint = P;
			//return IntersectionPoint;
			return v3{ 0, 0, 0 };
		}
	}
};
/*************************************************************************Naive rotations*/
struct point_rotation
{
	v3 PointRotation(v3 A, v3 B, real32 angle)
	{//rotation around pivot using x' = x * (cos)angle - y * (sin)angle, y' = y * (cos)angle + x * (sin)angle. real32 angle is given in radians.
		sine Sine = {};
		cosine Cosine = {};
		v3 pivotPointA = A;
		v3 C = {};
		C.x = A.x;
		C.y = B.y;
		real32 sinAngle = Sine.AngleSine(A, B, C);
		sinAngle = sin(angle);
		real32 cosinAngle = Cosine.AngleCosine(A, B, C);
		cosinAngle = cos(angle);
		real32 x = B.x - pivotPointA.x;//parenting?
		real32 y = B.y - pivotPointA.y;
		real32 rotatedX = x * cosinAngle - y * sinAngle;
		real32 rotatedY = x * sinAngle + y * cosinAngle;
		real32 newX = rotatedX + pivotPointA.x;
		real32 newY = rotatedY + pivotPointA.y;

		v3 RotatedB = {};
		RotatedB.x = newX;
		RotatedB.y = newY;
		return RotatedB;
	}
};
/*****************************************************************************Quaternions*/
struct quaternion
{//b,c,d are imaginary numbers -> b^2 = -1	
	real32 w;//scalar part for rotation around axis
	v3 v;
	//real32 x;//
	//real32 y;//axis orientation parts
	//real32 z;//
};
struct quaternion_operations
{	
	quaternion QtoRads(real32 w, v3 v)
	{
		w = w / 360 * (real32)Pi32 * 2;
		quaternion Result;
		Result = 
		{
			(real32)cos(w / 2),
			v3
		{
			(real32)v.x * sin(w / 2),
			(real32)v.y * sin(w / 2),
			(real32)v.z * sin(w / 2),
		}
		};
		return Result;
	}
	quaternion QInverse(quaternion RotatedQuaternion)
	{
		quaternion Result;	
		Result = quaternion{
			RotatedQuaternion.w,
			v3
		{
			-RotatedQuaternion.v.x,
			-RotatedQuaternion.v.y,
			-RotatedQuaternion.v.z,
		}
		};
		return Result;
	}
	quaternion QMultiplication(quaternion A, quaternion B)
	{
		v3Ops V3Ops;
		quaternion Result;
		//Result = {
		//	A.w * B.w - A.x * B.x - A.y * B.y - A.z * B.z,//w
		//	A.w * B.x + A.x * B.w + A.y * B.z - A.z * B.y,//x
		//	A.w * B.y - A.x * B.z + A.y * B.w + A.z * B.x,//y
		//	A.w * B.z + A.x * B.y - A.y * B.x + A.z + B.w //z
		//};
		Result.w = B.w * A.w - V3Ops.v3Dot(B.v,A.v);
		Result.v = 
			V3Ops.v3Add
			(V3Ops.v3Add
			(v3{B.w * A.v.x, B.w * A.v.y, B.w * A.v.z }, 
				v3{A.w * B.v.x, A.w * B.v.y, A.w * B.v.z }), 
				V3Ops.v3Cross(B.v, A.v));
		return Result;
	}
	v3 QuaternionPointRotation(v3 Point, real32 Angle, v3 Axis)//v3 axisorientation
	{
		distance Dist;
		v3Ops V3Ops;
		v3 Result;
		//Axis = { 0.0f, 0.0f, 1.0f };

		real32 AxDist = Dist.DistanceBetweenTwoPoints(v3{ 0,0,0 }, Axis);
		Axis = v3{ Axis.x/AxDist, Axis.y / AxDist, Axis.z / AxDist };

		quaternion Q = QtoRads(Angle, Axis);
		//quaternion Q = {Angle, Axis};
		//quaternion InvQ = QInverse(Q);
		//Result = Q * P * InvQ;

		//Result = p + 2w(v × p) + 2(v ×(v × p));

		//Vector vcV = v.Cross(V);
		//return V + vcV*(2 * w) + v.Cross(vcV) * 2;
	https://blog.molecular-matters.com/2013/05/24/a-faster-quaternion-vector-multiplication/
		//t = 2 * cross(q.xyz, v)
		//v' = v + q.w * t + cross(q.xyz, t)

		v3 VxP = V3Ops.v3Cross(Q.v, Point);
		v3 VxPa = v3{ (VxP.x * 2), (VxP.y * 2), (VxP.z * 2)};
		v3 VxPb = v3{ (VxP.x * 2) * Q.w, (VxP.y * 2) * Q.w, (VxP.z *2) * Q.w };
		v3 PplusVxP = V3Ops.v3Add(Point, VxPb);
		v3 VxVxP = V3Ops.v3Cross(Q.v, VxPa);
		//VxVxP = { VxVxP.x * 2, VxVxP.y * 2, VxVxP.z * 2 };

		Result = V3Ops.v3Add
		(PplusVxP, VxVxP);
		return Result;

		//void rotate_vector_by_quaternion(const Vector3& v, const Quaternion& q, Vector3& vprime)
		//{
		//	// Extract the vector part of the quaternion
		//	Vector3 u(q.x, q.y, q.z);

		//	// Extract the scalar part of the quaternion
		//	float s = q.w;

		//	// Do the math
		//	vprime = 2.0f * dot(u, v) * u
		//		+ (s*s - dot(u, u)) * v
		//		+ 2.0f * s * cross(u, v);
		//}
	}
};

struct polygon
{
	v3 Points[4];
	uint32 Color;
};

struct child
{
	// A PARENTINGNÉL EZEKNEK A POZICIOIT KELL KIVONOGATNI ÍGY
	//RELAÍTÍVVÉ TENNI AM OZGÁST
	v3 Position;
	quaternion Rotation;
	polygon Polys[80000];
};

struct parent
{
	v3 Position;
	quaternion Rotation;
	child Children[255];
};

inline real32 polyAverageZ(polygon A)
{
	real32 Result;
	Result = (A.Points[0].z + A.Points[1].z + A.Points[2].z + A.Points[3].z)/4;
	return Result;
}
/******************************************************************************LERP,SLERP*/
struct lerp
{

};
struct slerp
{

};
/***********************************************************************************PAINT*/
struct game_offscreen_buffer//or establish pipeline
{
	//pixels 32bit wide, BBGGRRxx
	void *Memory;
	int Width;
	int Height;
	int Pitch;
	int BytesPerPixel;
	bool IsColoring;
};
struct draw_poly_info//EHELYETT 2 DB GAME OFFSCREEN BUFFER?
{
	uint32 DrawBufferCoordinate;
	uint32 PolyArrayNumber;//legjobb ha ez a buffer a másik bufferhez teljesen hasonló felosztásban van
};
struct draw_poly_info_buffer
{
	draw_poly_info PolyInfo[640*480];
};
/***********************************************************************************SOUND*/
struct game_sound_output_buffer
{
	int SamplesPerSecond;
	int SampleCount;
	int16 *Samples;
};
/***********************************************************************************INPUT*/
struct game_button_state
{
	int HalfTransitionCount;
	bool32 EndedDown;
};
struct game_controller_input
{
	bool32 isAnalog;
	real32 StartX;
	real32 StartY;
	real32 MinX;
	real32 MinY;
	real32 MaxX;
	real32 MaxY;
	real32 EndX;
	real32 EndY;	

	union //stored in onep lace in memory
	{
		game_button_state Buttons[30];
		//kéne még egy tömb, egyik a kontrolloknak mási ka chatnek
		struct
		{
			game_button_state Up;
			game_button_state Down;
			game_button_state Left;
			game_button_state Right;
			game_button_state Space;

			game_button_state Q; game_button_state W; game_button_state E; game_button_state R;
			game_button_state T; game_button_state Z; game_button_state U; game_button_state I;
			game_button_state O; game_button_state P; game_button_state A; game_button_state S;
			game_button_state D; game_button_state F; game_button_state G; game_button_state H;
			game_button_state J; game_button_state K; game_button_state L; game_button_state Y;
			game_button_state X; game_button_state C; game_button_state V; game_button_state B;
			game_button_state N; game_button_state M; game_button_state n0; game_button_state n1; 
			game_button_state n2; game_button_state n3; game_button_state n4; game_button_state n5; 
			game_button_state n6; game_button_state n7; game_button_state n8; game_button_state n9; 
			game_button_state Enter; game_button_state Escape; game_button_state Shift;

			game_button_state zero; game_button_state one; game_button_state two; game_button_state three;
			game_button_state four; game_button_state five; game_button_state six; game_button_state seven;
			game_button_state eight; game_button_state nine;
		};
	};	
};
struct game_input
{
	game_button_state MouseButtons[3];
	int32 MouseX, MouseY, MouseZ;

	game_controller_input Controllers[5];
};
/**********************************************************************************MEMORY*/
struct game_memory
{
	bool32 IsInitialized;
	uint64 PermanentStorageSize;
	void *PermanentStorage;
	uint64 TransientStorageSize;
	void *TransientStorage;
};
/**********************************************************************IO READWRITEMEMORY*/
//we ask the platform to do stuff which we wont do
struct debug_read_file_result
{
	uint32 ContentsSize;
	void *Contents;
};
internal_function debug_read_file_result DEBUGPlatformReadEntireFile(char *Filename);
internal_function void DEBUGPlatformFreeFileMemory(void *Memory);
internal_function bool32 DEBUGPlatformWriteEntireFile(char *Filename, uint32 MemorySize, void *Memory);
/*****************************************************************************************/

struct camera_buffer
{
	//v3 GlobalOrigin = { 0,0,0 };
	//v3 Camera;
	v3 PlayerBuffer;
	v3 CamZ;
	v3 CamX;
	v3 CamY;
};
struct player_instance
{	
	/*********************************************************************************Variables for syncing.*/
	char playerName;
	v3 playerBuffer;
	v3 playerCam;
	/********************************************************************************************************/
	v3 localCharacterOriginPoint;//0
    /********************************************************************************************************/
	v3 parentedLocalRightPelvis;//1
	v3 parentedLocalRightKnee;//2
	v3 parentedLocalRightFoot;//3
	v3 parentedLocalLeftPelvis;//4
	v3 parentedLocalLeftKnee;//5
	v3 parentedLocalLeftFoot;//6
	/********************************************************************************************************/
	v3 parentedLocalHead;//7
	v3 parentedLocalChest;//8
	v3 parentedLocalRightShoulder;//8
	v3 parentedLocalRightElbow;//9
	v3 parentedLocalRightHand;//10
	v3 parentedLocalLeftShoulder;//11
	v3 parentedLocalLeftElbow;//12
	v3 parentedLocalLeftHand;//13
	/*************************************************************************************Segmented rotation*/
	v3 AxisOrigin;
	v3 AxisRPelvis;//1
	v3 AxisRKnee;//2
	v3 AxisRFoot;//3
	v3 AxisLPelvis;//4
	v3 AxisLKnee;//5
	v3 AxisLFoot;//6
	/********************************************************************************************************/
	v3 AxisHead;//7
	v3 AxisChest;//8
	v3 AxisRShoulder;//8
	v3 AxisRElbow;//9
	v3 AxisRHand;//10
	v3 AxisLShoulder;//11
	v3 AxisLElbow;//12
	v3 AxisLHand;//13
	/********************************************************************************************************/
	//bool32 isMyOriginMoving;
	/***********************************************************************Variables for physics simulation*/
	v3 originDirection;
	real32 originForce;
	real32 originRotAmount;
	real32 lastOriginRotAmount;
	real32 leftPelvisRotAmount;
	real32 leftKneeRotAmount;
	real32 leftFootRotAmount;
	real32 rightPelvisRotAmount;
	real32 rightKneeRotAmount;
	real32 rightFootRotAmount;
	real32 headRotAmount;
	real32 chestRotAmount;
	real32 leftShoulderRotAmount;
	real32 leftElbowRotAmount;
	real32 leftHandRotAmount;
	real32 rightShoulderRotAmount;
	real32 rightElbowRotAmount;
	real32 rightHandRotAmount;
	/********************************/
	real32 originWasRotatedBy;
	real32 leftPelvisWasRotatedBy;
	real32 leftKneeWasRotatedBy;
	real32 leftFootWasRotatedBy;
	real32 rightPelvisWasRotatedBy;
	real32 rightKneeWasRotatedBy;
	real32 rightFootWasRotatedBy;
	real32 headWasRotatedBy;
	real32 chestWasRotatedBy;
	real32 leftShoulderWasRotatedBy;
	real32 leftElbowWasRotatedBy;
	real32 leftHandWasRotatedBy;
	real32 rightShoulderWasRotatedBy;
	real32 rightElbowWasRotatedBy;
	real32 rightHandWasRotatedBy;
	/**************************************************************************Variables for animation logic*/
	bool32 isLeftFootAhead;
	bool32 isReadyForRunCycle;

	bool32 isLeftRun0;
	bool32 isLeftRunI;
	bool32 isLeftRunII;
	bool32 isLeftRunIII;
	bool32 isLeftRunIV;
	bool32 isLeftRunV;

	/*************************************************************************************************Animation*/
	real32 AnimSpeed;
	real32 runStateTimer;
	real32 kneeRotatedBy;
	real32 runKeyFrameCounter;
	/**************************************************************************************boolsForCollision*/
	bool32 isAnyPartColliding;
	bool32 isLeftFootColliding;
	bool32 isRightFootColliding;
	bool32 isHeadColliding;
	bool32 isLeftHandColliding;
	bool32 isRightHandFootColliding;
	int8 nOfPartsColliding;
	/********************************************************************************************************/
};
struct cat_animation
{
	bool32 isLeftRunI;
	bool32 isLeftRunII;
	bool32 isLeftRunIII;
	bool32 isLeftRunIV;
};
struct cat_model
{
	//polygon CatPolys[128];
	v3 RightPelvis[8];
	v3 RightKnee[8];
	v3 RightFoot[8];
	v3 RightToe[8];
	v3 LeftPelvis[8];
	v3 LeftKnee[8];
	v3 LeftFoot[8];
	v3 LeftToe[8];
	v3 Chest[8];
	v3 Neck[8];
	v3 Head[8];
	v3 RightShoulder[8];
	v3 RightElbow[8];
	v3 RightHand[8];
	v3 RightFinger[8];
	v3 LeftShoulder[8];
	v3 LeftElbow[8];
	v3 LeftHand[8];
	v3 LeftFinger[8];
};
struct client_parts
{//ami rajzolás az elég int, !U!int32 foo = OriginX;
	real64 OriginX; real64 OriginY; real64 OriginZ;
	real64 LPelvisX; real64 LPelvisY; real64 LPelvisZ;
	real64 LKneeX; real64 LKneeY; real64 LKneeZ;
	real64 LFootX; real64 LFootY; real64 LFootZ;
	real64 RPelvisX; real64 RPelvisY; real64 RPelvisZ;
	real64 RKneeX; real64 RKneeY; real64 RKneeZ;
	real64 RFootX; real64 RFootY; real64 RFootZ;
	real64 HeadX; real64 HeadY; real64 HeadZ;
	real64 ChestX; real64 ChestY; real64 ChestZ;
	real64 LShoulderX; real64 LShoulderY; real64 LShoulderZ;
	real64 LElbowX; real64 LElbowY; real64 LElbowZ;
	real64 LHandX; real64 LHandY; real64 LHandZ;
	real64 RShoulderX; real64 RShoulderY; real64 RShoulderZ;
	real64 RElbowX; real64 RElbowY; real64 RElbowZ;
	real64 RHandX; real64 RHandY; real64 RHandZ;
};
struct local_player_parts
{
	real64 OriginX; real64 OriginY; real64 OriginZ;
	real64 LPelvisX; real64 LPelvisY; real64 LPelvisZ;
	real64 LKneeX; real64 LKneeY; real64 LKneeZ;
	real64 LFootX; real64 LFootY; real64 LFootZ;
	real64 RPelvisX; real64 RPelvisY; real64 RPelvisZ;
	real64 RKneeX; real64 RKneeY; real64 RKneeZ;
	real64 RFootX; real64 RFootY; real64 RFootZ;
	real64 HeadX; real64 HeadY; real64 HeadZ;
	real64 ChestX; real64 ChestY; real64 ChestZ;
	real64 LShoulderX; real64 LShoulderY; real64 LShoulderZ;
	real64 LElbowX; real64 LElbowY; real64 LElbowZ;
	real64 LHandX; real64 LHandY; real64 LHandZ;
	real64 RShoulderX; real64 RShoulderY; real64 RShoulderZ;
	real64 RElbowX; real64 RElbowY; real64 RElbowZ;
	real64 RHandX; real64 RHandY; real64 RHandZ;
};
/**********************************************************************************MAP OBJECTS*/
struct sphere
{
	v3 Center;
	real32 Radius;
};
struct cube
{
	v3 Center;
	real32 Radius;
};
struct triangle_roof
{

};
struct ball
{

};
struct collider
{

};

struct memory_place
{
	uint64 Size;
	uint8 *Base;
	uint64 Used;
};
struct map
{
	uint32 MapPolyCount;
	polygon MapPolygons[4000000];
	v3 PolyEmptyPlacesBehind[256];//? i think I dont need this 
	v3 PolyEmptyPlacesBelow[256];

	v3 MapRectCorners[32];
	v3 BackGroundRectCorners[64];
};
struct write_cursor
{
	int WriteCursorX;
	int WriteCursorY;
	int LetterBuffer[256];
};
//services that the platform provides for game
struct game_state
{
	int Hz;
	int XOffset;
	int YOffset;

	real64 PlayerX, PlayerY, PlayerZ;//player origin..
	/******************************************************/
	//this must be general for the stact pointer to be at right place
	memory_place MapPlace;
	/******************************************************/
	write_cursor *WriteCursor;
	player_instance *Player;
	local_player_parts *LocalPlayerParts;
	client_parts *ClientParts;
	camera_buffer *CameraBuffer;
	map *Map;
	cat_model *CatModel;
};
/************************************************************************************CUSTOM INTERFACE HEADER*/
struct letter_color_array
{
	uint32 LetterColorCode[16];
	uint32 C0 = 0xFF000000;
	uint32 C1 = 0xFFFFFFFF;
};
struct letter_codes
{	
	letter_color_array GetLetter(int *LetterCode)
	{
		if (*LetterCode == 'Q')
		{
			return LetterQ();
		}
		else if (*LetterCode == 'W')	
		{
			return LetterW();
		}
		else if (*LetterCode == 'E')
		{
			return LetterE();
		}
		else if (*LetterCode == 'R')
		{
			return LetterR();
		}
		else if (*LetterCode == 'T')
		{
			return LetterT();
		}
		else if (*LetterCode == 'Z')
		{
			return LetterZ();
		}
		else if (*LetterCode == 'U')
		{
			return LetterU();
		}
		else if (*LetterCode == 'I')
		{
			return LetterI();
		}
		else if (*LetterCode == 'O')
		{
			return LetterO();
		}
		else if (*LetterCode == 'P')
		{
			return LetterP();
		}
		else if (*LetterCode == 'A')
		{
			return LetterA();
		}
		else if (*LetterCode == 'S')
		{
			return LetterS();
		}
		else if (*LetterCode == 'D')
		{
			return LetterD();
		}
	}
	letter_color_array LetterA()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C0; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C1; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C1; Array.LetterColorCode[9] = C0; Array.LetterColorCode[10] = C1; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C1;

		return Array;
	}
	letter_color_array LetterB()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C1; Array.LetterColorCode[1] = C0; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C1; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C1; Array.LetterColorCode[9] = C0; Array.LetterColorCode[10] = C1; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterC()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C0; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C1; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C1; Array.LetterColorCode[9] = C0; Array.LetterColorCode[10] = C0; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterD()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C0; Array.LetterColorCode[2] = C1; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C1; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C1; Array.LetterColorCode[9] = C0; Array.LetterColorCode[10] = C1; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterE()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C1; Array.LetterColorCode[2] = C1; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C1; Array.LetterColorCode[9] = C1; Array.LetterColorCode[10] = C0; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterF()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C1; Array.LetterColorCode[1] = C1; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C0; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C1; Array.LetterColorCode[9] = C1; Array.LetterColorCode[10] = C0; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C0; Array.LetterColorCode[14] = C0; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterG()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C1; Array.LetterColorCode[1] = C1; Array.LetterColorCode[2] = C1; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C1; Array.LetterColorCode[9] = C1; Array.LetterColorCode[10] = C1; Array.LetterColorCode[11] = C1;
		Array.LetterColorCode[12] = C0; Array.LetterColorCode[13] = C0; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C1;

		return Array;
	}
	letter_color_array LetterH()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C1; Array.LetterColorCode[1] = C0; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C1; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C1; Array.LetterColorCode[9] = C0; Array.LetterColorCode[10] = C1; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C0; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterI()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C1; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C0; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C0; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C0; Array.LetterColorCode[9] = C1; Array.LetterColorCode[10] = C0; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C0; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C0; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterJ()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C1; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C0; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C0; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C0; Array.LetterColorCode[9] = C1; Array.LetterColorCode[10] = C0; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C0; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterK()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C1; Array.LetterColorCode[1] = C0; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C1; Array.LetterColorCode[9] = C1; Array.LetterColorCode[10] = C0; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C0; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterL()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C1; Array.LetterColorCode[1] = C0; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C0; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C1; Array.LetterColorCode[9] = C0; Array.LetterColorCode[10] = C0; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C0; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterM()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C0; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C0; Array.LetterColorCode[5] = C1; Array.LetterColorCode[6] = C0; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C1; Array.LetterColorCode[9] = C0; Array.LetterColorCode[10] = C1; Array.LetterColorCode[11] = C1;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C0; Array.LetterColorCode[14] = C0; Array.LetterColorCode[15] = C1;

		return Array;
	}
	letter_color_array LetterN()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C0; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C0; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C1; Array.LetterColorCode[9] = C1; Array.LetterColorCode[10] = C1; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C0; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterO()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C0; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C1; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C1; Array.LetterColorCode[9] = C0; Array.LetterColorCode[10] = C1; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterP()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C1; Array.LetterColorCode[1] = C1; Array.LetterColorCode[2] = C1; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C1; Array.LetterColorCode[9] = C1; Array.LetterColorCode[10] = C1; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C0; Array.LetterColorCode[14] = C0; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterQ()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C1; Array.LetterColorCode[1] = C1; Array.LetterColorCode[2] = C1; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C1; Array.LetterColorCode[9] = C1; Array.LetterColorCode[10] = C1; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C0; Array.LetterColorCode[13] = C0; Array.LetterColorCode[14] = C0; Array.LetterColorCode[15] = C1;

		return Array;
	}
	letter_color_array LetterR()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C0; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C0; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C0; Array.LetterColorCode[9] = C1; Array.LetterColorCode[10] = C0; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C0; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C0; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterS()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C1; Array.LetterColorCode[1] = C0; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C0; Array.LetterColorCode[5] = C1; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C0; Array.LetterColorCode[9] = C0; Array.LetterColorCode[10] = C1; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterT()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C1; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C1; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C0; Array.LetterColorCode[9] = C1; Array.LetterColorCode[10] = C0; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C0; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterU()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C0; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C1; Array.LetterColorCode[9] = C0; Array.LetterColorCode[10] = C1; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterV()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C0; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C1; Array.LetterColorCode[9] = C0; Array.LetterColorCode[10] = C1; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C0; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C0; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterW()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C0; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C0; Array.LetterColorCode[7] = C1;
		Array.LetterColorCode[8] = C1; Array.LetterColorCode[9] = C0; Array.LetterColorCode[10] = C1; Array.LetterColorCode[11] = C1;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C0; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterX()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C0; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C0; Array.LetterColorCode[9] = C1; Array.LetterColorCode[10] = C0; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C0; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterY()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C0; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C0; Array.LetterColorCode[9] = C1; Array.LetterColorCode[10] = C0; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C0; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C0; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array LetterZ()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C0; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C1; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C0; Array.LetterColorCode[9] = C1; Array.LetterColorCode[10] = C0; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C1;

		return Array;
	}
	letter_color_array Letter0()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C1; Array.LetterColorCode[1] = C1; Array.LetterColorCode[2] = C1; Array.LetterColorCode[3] = C1;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C1;
		Array.LetterColorCode[8] = C1; Array.LetterColorCode[9] = C1; Array.LetterColorCode[10] = C0; Array.LetterColorCode[11] = C1;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C1;

		return Array;
	}
	letter_color_array Letter1()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C0; Array.LetterColorCode[2] = C1; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C0; Array.LetterColorCode[5] = C1; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C0; Array.LetterColorCode[9] = C0; Array.LetterColorCode[10] = C1; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C0; Array.LetterColorCode[13] = C0; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array Letter2()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C1; Array.LetterColorCode[2] = C1; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C0; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C0; Array.LetterColorCode[9] = C1; Array.LetterColorCode[10] = C0; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C0; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array Letter3()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C1; Array.LetterColorCode[2] = C1; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C0; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C0; Array.LetterColorCode[9] = C0; Array.LetterColorCode[10] = C0; Array.LetterColorCode[11] = C1;
		Array.LetterColorCode[12] = C0; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C1;

		return Array;
	}
	letter_color_array Letter4()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C1; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C0; Array.LetterColorCode[5] = C1; Array.LetterColorCode[6] = C0; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C0; Array.LetterColorCode[9] = C1; Array.LetterColorCode[10] = C1; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C0; Array.LetterColorCode[13] = C0; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array Letter5()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C1; Array.LetterColorCode[2] = C1; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C0; Array.LetterColorCode[5] = C1; Array.LetterColorCode[6] = C0; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C0; Array.LetterColorCode[9] = C0; Array.LetterColorCode[10] = C1; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C0; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array Letter6()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C0; Array.LetterColorCode[2] = C0; Array.LetterColorCode[3] = C1;
		Array.LetterColorCode[4] = C0; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C0; Array.LetterColorCode[9] = C1; Array.LetterColorCode[10] = C0; Array.LetterColorCode[11] = C1;
		Array.LetterColorCode[12] = C0; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array Letter7()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C1; Array.LetterColorCode[1] = C1; Array.LetterColorCode[2] = C1; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C0; Array.LetterColorCode[5] = C0; Array.LetterColorCode[6] = C1; Array.LetterColorCode[7] = C0;
		Array.LetterColorCode[8] = C0; Array.LetterColorCode[9] = C1; Array.LetterColorCode[10] = C0; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C0; Array.LetterColorCode[14] = C0; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array Letter8()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C1; Array.LetterColorCode[2] = C1; Array.LetterColorCode[3] = C1;
		Array.LetterColorCode[4] = C1; Array.LetterColorCode[5] = C1; Array.LetterColorCode[6] = C0; Array.LetterColorCode[7] = C1;
		Array.LetterColorCode[8] = C1; Array.LetterColorCode[9] = C0; Array.LetterColorCode[10] = C1; Array.LetterColorCode[11] = C1;
		Array.LetterColorCode[12] = C1; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C1; Array.LetterColorCode[15] = C0;

		return Array;
	}
	letter_color_array Letter9()
	{
		letter_color_array Array;
		uint32 C0 = Array.C0;
		uint32 C1 = Array.C1;
		Array.LetterColorCode[0] = C0; Array.LetterColorCode[1] = C1; Array.LetterColorCode[2] = C1; Array.LetterColorCode[3] = C0;
		Array.LetterColorCode[4] = C0; Array.LetterColorCode[5] = C1; Array.LetterColorCode[6] = C0; Array.LetterColorCode[7] = C1;
		Array.LetterColorCode[8] = C0; Array.LetterColorCode[9] = C0; Array.LetterColorCode[10] = C1; Array.LetterColorCode[11] = C0;
		Array.LetterColorCode[12] = C0; Array.LetterColorCode[13] = C1; Array.LetterColorCode[14] = C0; Array.LetterColorCode[15] = C0;

		return Array;
	}
};
internal_function void GameUpdateAndRender
(
	game_memory *Memory,
	game_input *Input,
	game_offscreen_buffer *Buffer,
	game_sound_output_buffer *SoundBuffer,

	client_parts *ClientParts,
	player_instance *Player,
	local_player_parts *LocalPlayerParts
);
#define GAME_H
#endif
