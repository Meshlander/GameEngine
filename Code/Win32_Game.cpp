/* ======================================================
SFile: S
SDate: S
SRevision: S
SCreator: Gergely Zara S
SNotice: S
======================================================*/

/*
save game locations
getting a handle to our own exe
asset loading path
threading l a thread??
raw input multiple keybord
sleep/timebegin period
clipcursor 
full screen support
wm setcursor
querycancelautoplay
wm activeate app
blit speed improvement BITBLIT
opengl direct3d?
getkeyboardlayout french international keyboard

not everything
*/

#include <windows.h>
#include <stdint.h>
#include <xinput.h>
#include <dsound.h>
#include <math.h>//not this math but own
/*************************************************************NETWORK*/
#pragma comment(lib, "ws2_32.lib")
#include <WinSock.h>
/**************************************************************/
//mymath.h
#define internal_function static 
#define local_persist static 
#define global_variable static

#define Pi32 3.14259265359f

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;

#include "game.cpp"
/************************************************************************For video buffer*/
struct win32_draw_buff//or establish pipeline
{
	BITMAPINFO Info;//bitmap
	void *Memory;
	int Width;
	int Height;
	int Pitch;
	int BytesPerPixel;
};

struct win32_window_dimension//_ convention
{
	int Width;
	int Height;
};
/*****************************************************************************GLOBAL VARS*/
global_variable bool isRunning;
global_variable win32_draw_buff GlobalBackBuffer;
global_variable LPDIRECTSOUNDBUFFER GlobalSecondaryBuffer;
global_variable int32 GlobalPerfCountFrequency;//int64?
global_variable bool32 GlobalIsConnected;
global_variable bool32 GlobalIsNetworkInitialized;
global_variable bool32 GlobalIsHideCursor;
global_variable client_parts GlobalClientParts;
global_variable local_player_parts GlobalLocalPlayerParts;
/*********************************************************WRITEREADMEMORY for indep layer*/

/*******************************Due to this the game runs even if gamepad fails to load.*/
#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dxUserIndex, XINPUT_STATE *pState)//ref it to down there
typedef X_INPUT_GET_STATE(x_input_get_state);
X_INPUT_GET_STATE(xInputGetStateStub)
{
	return(ERROR_DEVICE_NOT_CONNECTED);//!!!
}
global_variable x_input_get_state *XInputGetState_ = xInputGetStateStub;
#define XInputGetState XInputGetState_
//Here we manually load with windows
//load sound
#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter)
typedef DIRECT_SOUND_CREATE(direct_sound_create);
internal_function void
Win32LoadXInput(void)
{
	HMODULE XInputLibrary = LoadLibrary("xinput1_4.dll");
	if (!XInputLibrary)
	{
		HMODULE XInputLibrary = LoadLibrary("xinput1_3.dll");
	}
	if (XInputLibrary)
	{
		XInputGetState = (x_input_get_state *) GetProcAddress(XInputLibrary, "XInputGetState");
	}
}
/***********************************************************************************SOUND*/
internal_function void
Win32InitDirectSound(HWND Window, int32 SamplesPerSecond, int32 BufferSize)
{	
	HMODULE DSoundLibrary = LoadLibraryA("dsound.dll");
	if (DSoundLibrary)
	{
		direct_sound_create *DirectSoundCreate = (direct_sound_create *)
			GetProcAddress(DSoundLibrary, "DirectSoundCreate");
		LPDIRECTSOUND DirectSound;//XP directsound 87
		if (DirectSoundCreate && SUCCEEDED(DirectSoundCreate(0, &DirectSound, 0)))
		{
			WAVEFORMATEX WaveFormat = {};
			WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
			WaveFormat.nChannels = 2;//stereo
			WaveFormat.nSamplesPerSec = SamplesPerSecond;
			WaveFormat.wBitsPerSample = 16;//16 too
			WaveFormat.nBlockAlign = (WaveFormat.nChannels * WaveFormat.wBitsPerSample) / 8;//16,8??
			WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;
			WaveFormat.cbSize = 0;			

			if(SUCCEEDED(DirectSound->SetCooperativeLevel(Window, DSSCL_PRIORITY)))
			{
				DSBUFFERDESC BufferDescription = {};
				BufferDescription.dwSize = sizeof(BufferDescription);
				BufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;

				LPDIRECTSOUNDBUFFER PrimaryBuffer;
				if (SUCCEEDED(DirectSound->CreateSoundBuffer(&BufferDescription, &PrimaryBuffer, 0)))
				{		
					HRESULT Error = PrimaryBuffer->SetFormat(&WaveFormat);
					if (SUCCEEDED(Error))
					{
					}
					else {}
				}
			}
			else {}
			DSBUFFERDESC BufferDescription = {};
			BufferDescription.dwSize = sizeof(BufferDescription);
			BufferDescription.dwFlags = DSBCAPS_GETCURRENTPOSITION2;
			BufferDescription.dwBufferBytes = BufferSize;
			BufferDescription.lpwfxFormat = &WaveFormat;
			HRESULT Error = DirectSound->CreateSoundBuffer(&BufferDescription, &GlobalSecondaryBuffer, 0);
			if (SUCCEEDED(Error))
			{
			}
		}
		else {}		
	}
}
/******************************************************************MEMORY IOs for machine*/
internal_function debug_read_file_result
DEBUGPlatformReadEntireFile(char *Filename)
{
	debug_read_file_result Result = {};

	HANDLE FileHandle = CreateFile(
		Filename, GENERIC_READ, 
		0, 0, OPEN_EXISTING, 0, 0
	);
	if (FileHandle != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER FileSize;
		if (GetFileSizeEx(FileHandle, &FileSize))
		{
			//safe truncate
			Assert(FileSize.QuadPart <= 0xFFFFFFFF);
			uint32 FileSize32 = (uint32)FileSize.QuadPart;
			Result.Contents = VirtualAlloc (0, FileSize32, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE); //lehet elég a low 32biteseknél
			DWORD BytesRead;
			if (ReadFile(FileHandle, Result.Contents, FileSize.QuadPart, &BytesRead, 0))
			{
				Result.ContentsSize = FileSize32;
			}
			else
			{
				DEBUGPlatformFreeFileMemory(Result.Contents);
				Result.Contents = 0;
			}
		}
		CloseHandle(FileHandle);
	}
	return(Result);
}
internal_function void
DEBUGPlatformFreeFileMemory(void *Memory)
{
	if (Memory)
	{
		VirtualFree(Memory, 0, MEM_RELEASE);
	}
}
internal_function bool32
DEBUGPlatformWriteEntireFile(char *Filename, uint32 MemorySize, void *Memory)
{
	bool32 Result = false;
	HANDLE FileHandle = CreateFile(
		Filename, GENERIC_WRITE,
		0, 0, CREATE_ALWAYS, 0, 0
	);
	if (FileHandle != INVALID_HANDLE_VALUE)
	{		
		DWORD BytesWritten;
		if (WriteFile(FileHandle, Memory, MemorySize, &BytesWritten, 0))
		{
			Result = (BytesWritten == MemorySize);
		}
		else
		{			
		}
		CloseHandle(FileHandle);
	}
	return(Result);
}
/*********************************************************************************NETWORK*/
SOCKET Connection;
void ClientThread()//receive data/send data
{//talán itt lesz a baj...... Innen kéne kivenni és itt eltárolni a cuccot	
	//memcopy a legjobb
	//recv(Connection, (char*)&GlobalClientParts.OriginX, sizeof(GlobalClientParts.OriginX), 0);
	while(true)
	{
	char Packet[6];
	recv(Connection, Packet, sizeof(Packet), 0);

	int16 XToRec = ((int16)Packet[0] << 8) | ((int16)Packet[1] & 0x00FF);
	int16 YToRec = ((int16)Packet[2] << 8) | ((int16)Packet[3] & 0x00FF);
	int16 ZToRec = ((int16)Packet[4] << 8) | ((int16)Packet[5] & 0x00FF);

	GlobalClientParts.OriginX = (XToRec / 100.00f);
	GlobalClientParts.OriginY = (YToRec / 100.00f);
	}
}

int Client(local_player_parts *LocalPlayerParts)
{
	//egy double 8 byte
	//a socketet gondolom elég egyszer inicializálni?
	if (GlobalIsNetworkInitialized == false)
	{
		WSAData WsaData;
		WORD DllVersion = MAKEWORD(2, 1);
		if (WSAStartup(DllVersion, &WsaData) != 0)
		{
			MessageBoxA(NULL, "Winsock startup failed", "ERROR", MB_OK | MB_ICONERROR);
			exit(1);
		}
		else
		{
			GlobalIsNetworkInitialized = true;
		}
	}
	if (GlobalIsConnected == false)//boolok helyett majd szétszedni? client initialize/client update
	{//ez az idõigényes rész
		SOCKADDR_IN address;
		int addresslength = sizeof(address);
		address.sin_addr.s_addr = inet_addr("127.0.0.1");//local host
		address.sin_port = htons(1111);
		address.sin_family = AF_INET;//use ip			
		Connection = socket(AF_INET, SOCK_STREAM, NULL);

		CreateThread
		(0, 0, (LPTHREAD_START_ROUTINE)ClientThread, 0, 0, 0);

		if (connect(Connection, (SOCKADDR*)&address, addresslength) != 0)
		{
			//MessageBoxA(NULL, "connection failed", "ERROR", MB_OK | MB_ICONERROR);
			GlobalIsConnected = true;//itt másféle kell majd
			//
			
			//
			return 0;			
		}
		else
		{
			GlobalIsConnected = true;
		}
	}	
	/**********************************************************Variables received*/
		//CreateThread
		//(0, 0, (LPTHREAD_START_ROUTINE)ClientThread, 0, 0, 0);
		/*********************************************************Variables sent*/	
		int16 XTrunced = trunc(LocalPlayerParts->OriginX * 100);
		int16 YTrunced = trunc(LocalPlayerParts->OriginY * 100);
		int16 ZTrunced = trunc(LocalPlayerParts->OriginZ * 100);
		int8 X1ToSend = (int8)((XTrunced & 0xFF00) >> 8);
		int8 X2ToSend = (int8)((XTrunced & 0x00FF) >> 0);
		int8 Y1ToSend = (int8)((YTrunced & 0xFF00) >> 8);
		int8 Y2ToSend = (int8)((YTrunced & 0x00FF) >> 0);
		int8 Z1ToSend = (int8)((ZTrunced & 0xFF00) >> 8);
		int8 Z2ToSend = (int8)((ZTrunced & 0x00FF) >> 0);

		char Packet[6];
		Packet[0] = X1ToSend;
		Packet[1] = X2ToSend;
		Packet[2] = Y1ToSend;
		Packet[3] = Y2ToSend;
		Packet[4] = Z1ToSend;
		Packet[5] = Z2ToSend;
		if(Packet > 0)
		{
			send(Connection, Packet, sizeof(Packet), 0);
		}
		//Sleep(10);
		//system("pause");
		/************************************************************************************/
		return 0;
}
/***********************************************************************************PAINT*/
win32_window_dimension
GetWindowDimension(HWND Window)
{
	win32_window_dimension Result;

	RECT ClientRect;
	GetClientRect(Window, &ClientRect);
	Result.Width = ClientRect.right - ClientRect.left;//ezt már kiszámoltuk máshol
	Result.Height = ClientRect.bottom - ClientRect.top;

	return(Result);
}
//
internal_function void 
Win32DrawBuffInit(win32_draw_buff *Buffer, int Width, int Height)//DIBits section is what windows uses for buffer
{	
	if (Buffer->Memory)//-> return helyett kinda? to preserve it in memory about.
	{
		VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
	}
	//640x480
	Buffer->Width = Width;
	Buffer->Height = Height;
	Buffer->BytesPerPixel = 4;

	Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
	Buffer->Info.bmiHeader.biWidth = Buffer->Width;//ha a te módszeredet akarnád -w +h
	Buffer->Info.bmiHeader.biHeight = -Buffer->Height;//itt kéne a te loopolásodat --
	Buffer->Info.bmiHeader.biPlanes = 1;
	Buffer->Info.bmiHeader.biBitCount = 32;
	Buffer->Info.bmiHeader.biCompression = BI_RGB;
	
	int BitmapMemorySize = (Buffer->Width * Buffer->Height)*Buffer->BytesPerPixel;
	Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);//bytes RGB 3x8 bits

	Buffer->Pitch = Width * Buffer->BytesPerPixel;//
}
internal_function void 
Win32CopyBufferToWindow(HDC DeviceContext, 
	int WindowWidth, int WindowHeight,
	win32_draw_buff *Buffer
	)
{
	StretchDIBits(
		//copy from buffer to buffer in, size bitblits. palette colors, aspect ratio correction needed
		DeviceContext, 
		//0, 0, WindowWidth, WindowHeight,
		0, 0, Buffer->Width*2, Buffer->Height*2,
		0, 0, Buffer->Width, Buffer->Height,//az ablakra rakja rá a buffert
		Buffer->Memory,
		&Buffer->Info,
		DIB_RGB_COLORS, SRCCOPY);		
}
/*****************************************************************************************/
LRESULT CALLBACK
MainWindowCallback(HWND Window,
	UINT Message,
	WPARAM WParam,
	LPARAM LParam)
{
	LRESULT Result = 0;
	switch (Message)
	{
		case WM_SIZE:
		{			
		} break;
		case WM_DESTROY:
		{
			isRunning = false;
		} break;
		case WM_CLOSE:
		{
			isRunning = false;
		} break;
		case WM_ACTIVATEAPP:
		{ 
		} break;
		/***************************************************************************INPUT*/
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		{		
		} break;
		/***************************************************************************PAINT*/
		case WM_PAINT:
		{//using gdi; windows' graphics
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(Window, &Paint);
			int X = Paint.rcPaint.left;
			int Y = Paint.rcPaint.top;//top
			int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;//bot-top
			int Width = Paint.rcPaint.right - Paint.rcPaint.left;

			win32_window_dimension Dimension = GetWindowDimension(Window);
			Win32CopyBufferToWindow(DeviceContext,
				Dimension.Width, Dimension.Height,
				&GlobalBackBuffer
			);	
			EndPaint(Window, &Paint);
		} break;
		/*********************************************************************************/
		default:
		{
			OutputDebugStringA("default/n");
			Result = DefWindowProc(Window, Message, WParam, LParam);
		} break;
	}
	return(Result);
}
/***********************************************************************************SOUND*/
struct win32_sound_output
{
	int SamplesPerSecond;
	int Hz;
	int16 Volume;
	uint32 RunningSampleIndex;
	int SquareWavePeriod;
	//int HalfSquareWavePeriod;
	int BytesPerSample;
	int SecondaryBufferSize;
	real32 tSine;
	int LatencySampleCount;
};
internal_function void Win32ClearBuffer(win32_sound_output *SoundOutput)
{
	VOID *Region1;
	DWORD Region1Size;
	VOID *Region2;
	DWORD Region2Size;
	if (SUCCEEDED(GlobalSecondaryBuffer->Lock(
		0,
		SoundOutput->SecondaryBufferSize,
		&Region1, &Region1Size,
		&Region2, &Region2Size,
		0
	)))
	{
		uint8 *DestSample = (uint8*)Region1;
		for (DWORD ByteIndex = 0; ByteIndex < Region1Size; ++ByteIndex)
		{
			*DestSample++ = 0;
		}
		DestSample = (uint8*)Region2;
		for (DWORD ByteIndex = 0; ByteIndex < Region2Size; ++ByteIndex)
		{
			*DestSample++ = 0;
		}
		GlobalSecondaryBuffer->Unlock(Region1, Region1Size, Region2, Region2Size);
	}
}
internal_function void
Win32FillSoundBuffer(
	win32_sound_output *SoundOutput,
	DWORD ByteToLock,
	DWORD BytesToWrite,
	game_sound_output_buffer *SourceBuffer
)
{
	VOID *Region1;
	DWORD Region1Size;
	VOID *Region2;
	DWORD Region2Size;
	if (SUCCEEDED(GlobalSecondaryBuffer->Lock(
		ByteToLock,
		BytesToWrite,
		&Region1, &Region1Size,
		&Region2, &Region2Size,
		0
	)))
	{
		DWORD Region1SampleCount = Region1Size / SoundOutput->BytesPerSample;
		int16 *DestSample = (int16 *)Region1;
		int16 *SourceSample = SourceBuffer->Samples;
		for (DWORD SampleIndex = 0;
			SampleIndex < Region1SampleCount;
			++SampleIndex)
		{
			//real32 t = 2.0f * Pi32 *(real32)SoundOutput->RunningSampleIndex / (real32)SoundOutput->SquareWavePeriod;
			real32 SineValue = sinf(SoundOutput->tSine);//truncation, fixed point math....
			int16 SampleValue = (int16)(SineValue * SoundOutput->Volume);
			//int8 SampleValue = ((RunningSampleIndex / HalfSquareWavePeriod) % 2) ? Volume : -Volume;
			*DestSample++ = *SourceSample++;
			*DestSample++ = *SourceSample++;

			//SoundOutput->tSine += 2.0f*Pi32 / 1.0f / (real32)SoundOutput->SquareWavePeriod;
			++SoundOutput->RunningSampleIndex;
		}
		DestSample = (int16 *)Region2;
		DWORD Region2SampleCount = Region2Size / SoundOutput->BytesPerSample;
		for (DWORD SampleIndex = 0;
			SampleIndex < Region2SampleCount;
			++SampleIndex)
		{
			*DestSample++ = *SourceSample++;
			*DestSample++ = *SourceSample++;

			//SoundOutput->tSine += 2.0f*Pi32 / 1.0f / (real32)SoundOutput->SquareWavePeriod;
			++SoundOutput->RunningSampleIndex;
		}
		GlobalSecondaryBuffer->Unlock(Region1, Region1Size, Region2, Region2Size);
	}
}
/****************************************************INPUT for platform independent layer*/
internal_function void
Win32ProcessKeyboardMessage(
	game_button_state *NewState,
	bool32 IsDown
)
{
	NewState->EndedDown = IsDown;
	//++NewState->HalfTransitionCount;	
}
//
internal_function void
Win32ProcessXInputDigitalButton(
	DWORD XinputButtonState,
	game_button_state *OldState,
	DWORD ButtonBit,
	game_button_state *NewState
)
{
	NewState->EndedDown = ((XinputButtonState & ButtonBit) == ButtonBit);
	//NewState->HalfTransitionCount = (OldState->EndedDown != NewState->EndedDown) ? 1 : 0;
}
/*****************************************************************************************/
internal_function void
Win32ProcessPendingMessages(game_controller_input *KeyboardController)
{
	MSG Message;
	while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
	{
		if (Message.message == WM_QUIT)
		{
			isRunning = false;
		}
		/****************/
		switch (Message.message)/*******************************INPUT*/
		{
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			uint32 VKCode = (uint32)Message.wParam;
			bool32 WasDown = ((Message.lParam & (1 << 30)) != 0);//if key was down
			bool32 IsDown = ((Message.lParam & (1 << 31)) == 0);//if key is down
			if (WasDown != IsDown)
			{
				if (VKCode == VK_RETURN)//enter
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Enter, IsDown);
				}
				if (VKCode == 'W')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Up, IsDown);
				}
				if (VKCode == 'A')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Left, IsDown);
				}
				if (VKCode == 'S')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Down, IsDown);
				}				
				if (VKCode == 'D')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Right, IsDown);
				}
				if (VKCode == VK_SPACE)
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Space, IsDown);
				}
				if (VKCode == VK_ESCAPE)
				{
					if (IsDown)
					{
						SetCursor(0);
						GlobalIsHideCursor = true;
					}
					//SetCursor(0);
					//Win32ProcessKeyboardMessage(&KeyboardController->, IsDown);
				}
			}			
			//some of the messages should be handled only if enter was pressed....
			if (
				GlobalInterfaceOn
				&& WasDown != IsDown
				//KeyboardController->Enter.EndedDown
				)
			{
				if (VKCode == 'Q')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Q, IsDown);
				}
				/**************************************************************************/
			/*	else if (VKCode == 'W')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Up, IsDown);
				}
				else if (VKCode == 'A')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Left, IsDown);
				}
				else if (VKCode == 'S')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Down, IsDown);
				}
				else if (VKCode == 'D')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Right, IsDown);
				}*/
				/**************************************************************************/
				else if (VKCode == 'E')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->E, IsDown);
				}
				else if (VKCode == 'R')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->R, IsDown);
				}
				else if (VKCode == 'T')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->T, IsDown);
				}
				else if (VKCode == 'Z')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Z, IsDown);
				}
				else if (VKCode == 'U')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->U, IsDown);
				}
				else if (VKCode == 'I')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->I, IsDown);
				}
				else if (VKCode == 'O')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->O, IsDown);
				}
				else if (VKCode == 'P')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->P, IsDown);
				}
				else if (VKCode == 'F')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->F, IsDown);
				}
				else if (VKCode == 'G')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->G, IsDown);
				}
				else if (VKCode == 'H')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->H, IsDown);
				}
				else if (VKCode == 'J')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->J, IsDown);
				}
				else if (VKCode == 'K')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->K, IsDown);
				}
				else if (VKCode == 'L')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->L, IsDown);
				}
				else if (VKCode == 'Y')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Y, IsDown);
				}
				else if (VKCode == 'X')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->X, IsDown);
				}
				else if (VKCode == 'C')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->C, IsDown);
				}
				else if (VKCode == 'V')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->V, IsDown);
				}
				else if (VKCode == 'B')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->B, IsDown);
				}
				else if (VKCode == 'N')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->N, IsDown);
				}
				else if (VKCode == 'M')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->M, IsDown);
				}
				else if (VKCode == '0')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Up, IsDown);
				}
				else if (VKCode == '1')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Up, IsDown);
				}
				else if (VKCode == '2')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Up, IsDown);
				}
				else if (VKCode == '3')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Up, IsDown);
				}
				else if (VKCode == '4')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Up, IsDown);
				}
				else if (VKCode == '5')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Up, IsDown);
				}
				else if (VKCode == '6')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Up, IsDown);
				}
				else if (VKCode == '7')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Up, IsDown);
				}
				else if (VKCode == '8')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Up, IsDown);
				}
				else if (VKCode == '9')
				{
					Win32ProcessKeyboardMessage(&KeyboardController->Up, IsDown);
				}
			}		
			bool32 AltKeyWasDown = (Message.lParam & (1 << 29));
			if ((VKCode == VK_F4) && AltKeyWasDown)
			{
				isRunning = false;
			}
		}break;

		default:
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}break;
		}
		/********************/
	}
}
/**********************************************************************************SCHEDULER*/
inline LARGE_INTEGER Win32GetWallClock()
{
	LARGE_INTEGER Result;
	QueryPerformanceCounter(&Result);
	return(Result);
}
inline real32 Win32GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End)
{
	real32 Result = ((real32)End.QuadPart - (real32)Start.QuadPart) / (real32)GlobalPerfCountFrequency;
	return(Result);
}
struct win32_scheduler
{
	//int MonitorRefreshHz = 60;
	//int GameUpdateHz = 60;
	//real32 TargetSecondsPerFrame = 1.0f / (real32)GameUpdateHz;
};
internal_function void
Win32Scheduler()
{
	//target
}
//internal_function HICON Win32LoadIco(HINSTANCE hInstance)
//{
//	HBITMAP BitMap = LoadBitmap(
//		hInstance,
//		"Icon.ico"
//	);
//	//loadimage
//	HICON hIcon = (HICON)LoadImage(hInstance, "Icon.ico", IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE);
//	return hIcon;
//}
internal_function void
Win32CreateIcon(HINSTANCE hInst, HICON *hIcon)
{
	BYTE ANDmaskIcon[] = { 
		0xFF, 0xFF, 0xFF, 0xFF,   // line 1 
		0xFF, 0xFF, 0xC3, 0xFF,   // line 2 
		0xFF, 0xFF, 0x00, 0xFF,   // line 3 
		0xFF, 0xFE, 0x00, 0x7F,   // line 4 

		0xFF, 0xFC, 0x00, 0x1F,   // line 5 
		0xFF, 0xF8, 0x00, 0x0F,   // line 6 
		0xFF, 0xF8, 0x00, 0x0F,   // line 7 
		0xFF, 0xF0, 0x00, 0x07,   // line 8 

		0xFF, 0xF0, 0x00, 0x03,   // line 9 
		0xFF, 0xE0, 0x00, 0x03,   // line 10 
		0xFF, 0xE0, 0x00, 0x01,   // line 11 
		0xFF, 0xE0, 0x00, 0x01,   // line 12 

		0xFF, 0xF0, 0x00, 0x01,   // line 13 
		0xFF, 0xF0, 0x00, 0x00,   // line 14 
		0xFF, 0xF8, 0x00, 0x00,   // line 15 
		0xFF, 0xFC, 0x00, 0x00,   // line 16 

		0xFF, 0xFF, 0x00, 0x00,   // line 17 
		0xFF, 0xFF, 0x80, 0x00,   // line 18 
		0xFF, 0xFF, 0xE0, 0x00,   // line 19 
		0xFF, 0xFF, 0xE0, 0x01,   // line 20 

		0xFF, 0xFF, 0xF0, 0x01,   // line 21 
		0xFF, 0xFF, 0xF0, 0x01,   // line 22 
		0xFF, 0xFF, 0xF0, 0x03,   // line 23 
		0xFF, 0xFF, 0xE0, 0x03,   // line 24 

		0xFF, 0xFF, 0xE0, 0x07,   // line 25 
		0xFF, 0xFF, 0xC0, 0x0F,   // line 26 
		0xFF, 0xFF, 0xC0, 0x0F,   // line 27 
		0xFF, 0xFF, 0x80, 0x1F,   // line 28 

		0xFF, 0xFF, 0x00, 0x7F,   // line 29 
		0xFF, 0xFC, 0x00, 0xFF,   // line 30 
		0xFF, 0xF8, 0x03, 0xFF,   // line 31 
		0xFF, 0xFC, 0x3F, 0xFF };  // line 32 

								   // Yang icon XOR bitmask 

	BYTE XORmaskIcon[] = {
		0x00, 0x00, 0x00, 0x00,   // line 1 
		0x00, 0x00, 0x00, 0x00,   // line 2 
		0x00, 0x00, 0x00, 0x00,   // line 3 
		0x00, 0x00, 0x00, 0x00,   // line 4 

		0x00, 0x00, 0x00, 0x00,   // line 5 
		0x00, 0x00, 0x00, 0x00,   // line 6 
		0x00, 0x00, 0x00, 0x00,   // line 7 
		0x00, 0x00, 0x38, 0x00,   // line 8 

		0x00, 0x00, 0x00, 0x00,   // line 1 
		0x00, 0x00, 0x00, 0x00,   // line 2 
		0x00, 0x00, 0x00, 0x00,   // line 3 
		0x00, 0x00, 0x00, 0x00,   // line 4 

		//0x00, 0x00, 0x7C, 0x00,   // line 9 
		//0x00, 0x00, 0x7C, 0x00,   // line 10 
		//0x00, 0x00, 0x7C, 0x00,   // line 11 
		//0x00, 0x00, 0x38, 0x00,   // line 12 

		0x00, 0x00, 0x00, 0x00,   // line 13 
		0x00, 0x00, 0x00, 0x00,   // line 14 
		0x00, 0x00, 0x00, 0x00,   // line 15 
		0x00, 0x00, 0x00, 0x00,   // line 16 

		0x00, 0x00, 0x00, 0x00,   // line 17 
		0x00, 0x00, 0x00, 0x00,   // line 18 
		0x00, 0x00, 0x00, 0x00,   // line 19 
		0x00, 0x00, 0x00, 0x00,   // line 20 

		0x00, 0x00, 0x00, 0x00,   // line 21 
		0x00, 0x00, 0x00, 0x00,   // line 22 
		0x00, 0x00, 0x00, 0x00,   // line 23 
		0x00, 0x00, 0x00, 0x00,   // line 24 

		0x00, 0x00, 0x00, 0x00,   // line 25 
		0x00, 0x00, 0x00, 0x00,   // line 26 
		0x00, 0x00, 0x00, 0x00,   // line 27 
		0x00, 0x00, 0x00, 0x00,   // line 28 

		0x00, 0x00, 0x00, 0x00,   // line 29 
		0x00, 0x00, 0x00, 0x00,   // line 30 
		0x00, 0x00, 0x00, 0x00,   // line 31 
		0x00, 0x00, 0x00, 0x00 };  // line 32 

	*hIcon = CreateIcon(hInst,    // application instance  
		32,              // icon width 
		32,              // icon height 
		1,               // number of XOR planes 
		1,               // number of bits per pixel 
		ANDmaskIcon,     // AND bitmask  
		XORmaskIcon);    // XOR bitmask 
}
int CALLBACK
WinMain
(
	HINSTANCE Instance,
	HINSTANCE prevInstance,
	LPSTR CommandLine,
	int ShowCode
)
{
	LARGE_INTEGER PerfCountFrequencyResult;
	QueryPerformanceFrequency(&PerfCountFrequencyResult);
	GlobalPerfCountFrequency = (int32)PerfCountFrequencyResult.QuadPart;//valahol int64 volt?

	//set windwos scheduler granularity to 1ms instead
	UINT DesiredSchedulerMS = 1;
	bool32 SleepIsGranular = (timeBeginPeriod(DesiredSchedulerMS) == TIMERR_NOERROR);

	MMRESULT timeBeginPeriod(DesiredSchedulerMS);

	WNDCLASS WindowClass = {};

	Win32DrawBuffInit(&GlobalBackBuffer, 640, 480);

	WindowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
  	WindowClass.lpfnWndProc = MainWindowCallback;
  	WindowClass.hInstance = Instance;

  	//WindowClass.hIcon = LoadIcon(Instance, MAKEINTRESOURCE(402));//0, IDI_APPLICATION

	//HICON hIcon;

	//Win32CreateIcon(Instance, &hIcon);
	//hIcon = Win32LoadIco(Instance);

	//WindowClass.hIcon = hIcon;//0, IDI_APPLICATION

	WindowClass.hCursor = LoadCursor(0, IDC_HAND);

  	//hbrBackground;
  	//lpszMenuName;
  	WindowClass.lpszClassName = "MyWindowClass";
	/**************************************************MONITOR REFRESH RATE FOR VERT SYNC*/
	int MonitorRefreshHz = 60;
	int GameUpdateHz = 60;
#define FramesOfAudioLatency 3
	real32 TargetSecondsPerFrame = 1.0f / (real32)GameUpdateHz;
	/*************************************************************************************/
	if (RegisterClass(&WindowClass))
	{
		HWND Window = CreateWindowEx(
			0,
			WindowClass.lpszClassName,
			"MyGame",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			0,
			0,
			Instance,
			0);
		if (Window)
		{
			HDC DeviceContext = GetDC(Window);
			/***********************************************************************SOUND*/
			//win32_sound_output SoundOutput = {};
			//SoundOutput.SamplesPerSecond = 48000;

			//SoundOutput.Hz = 256;
			//SoundOutput.Volume = 3000;
			//SoundOutput.SquareWavePeriod = SoundOutput.SamplesPerSecond / SoundOutput.Hz;//!!	

			//SoundOutput.BytesPerSample = sizeof(int16) * 2;//túl kicsi...
			//SoundOutput.HalfSquareWavePeriod = SoundOutput.SquareWavePeriod / 2;
			//SoundOutput.LatencySampleCount = FramesOfAudioLatency*(SoundOutput.SamplesPerSecond*SoundOutput.BytesPerSample);
			//SoundOutput.SecondaryBufferSize = SoundOutput.SamplesPerSecond*SoundOutput.BytesPerSample;
			//Win32InitDirectSound(Window, SoundOutput.SamplesPerSecond, SoundOutput.SecondaryBufferSize);
			//Win32FillSoundBuffer(&SoundOutput, 0, SoundOutput.LatencySampleCount * SoundOutput.BytesPerSample);
			//Win32ClearBuffer(&SoundOutput);
			//GlobalSecondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);
			//bool32 SoundIsPlaying = false;
			//alloc for sound
			/*int16 *Samples =
				(int16*)VirtualAlloc
				(
					0, SoundOutput.SecondaryBufferSize,//buffer size more optimal? 
					MEM_RESERVE | MEM_COMMIT,
					PAGE_READWRITE
				);*/
			/**************************************************PLATFORMINDEPENDENT MEMORY*/
			game_memory GameMemory = {};
			GameMemory.PermanentStorageSize = Megabytes(256);
			GameMemory.PermanentStorage = VirtualAlloc
			(
				0, (uint32)GameMemory.PermanentStorageSize,
				MEM_RESERVE | MEM_COMMIT,
				PAGE_READWRITE
			);
			/*********************************************TRANSIENT!!!NETWORK!!!!!!!!!!!!*/
			GameMemory.TransientStorageSize = Megabytes(512);
			GameMemory.TransientStorage = VirtualAlloc
			(
				0, (uint32)GameMemory.TransientStorageSize,
				MEM_RESERVE | MEM_COMMIT,
				PAGE_READWRITE
			);
			/*****************************************************************************/
			isRunning = true;
			/*************************************************************************FPS*/
			LARGE_INTEGER LastCounter = Win32GetWallClock();
			int32 LastCycleCount = __rdtsc();//dont ship
			/**********************************************************************SOUND*/
			//DWORD LastPlayCursor = 0;
			//bool32 SoundIsValid = false;
			//DWORD AudioLatencyBytes = 0;
			//real32 AudioLatencySeconds = 0;
			/***************************************************PLATFORMINDEPENDENT INPUT*/
			game_input Input[2] = {};
			game_input *NewInput = &Input[0];
			game_input *OldInput = &Input[1];
			/**********************************************************************UPDATE*/
			while (isRunning)
			{
				//windows messages	
				game_controller_input *OldKeyboardController = &OldInput->Controllers[0];
				game_controller_input *NewKeyboardController = &NewInput->Controllers[0];
				game_controller_input ZeroController = {};
				*NewKeyboardController = ZeroController;
				for (
					int ButtonIndex = 0;
					ButtonIndex < ArrayCount(NewKeyboardController->Buttons);
					++ButtonIndex
					)
				{
					NewKeyboardController->Buttons[ButtonIndex].EndedDown =
						OldKeyboardController->Buttons[ButtonIndex].EndedDown;
				}
				Win32ProcessPendingMessages(NewKeyboardController);
				if (GlobalIsHideCursor)
				{
					SetCursor(0);
				}
				//MSG Message;
				/*******************************************************************INPUT*/
				//game_input Input = {};
				/*****************************************************************WINDOWS*/
				/*********************************************************(GAMEPAD) INPUT*/
				/*******************************************************************Mouse*/
				POINT MouseP;
				GetCursorPos(&MouseP);
				ScreenToClient(Window, &MouseP);
				NewInput->MouseX = MouseP.x/2;
				NewInput->MouseY = MouseP.y/2;//ismouseinverted?
				NewInput->MouseZ = 0;

				Win32ProcessKeyboardMessage(
					&NewInput->MouseButtons[0],
					GetKeyState(VK_LBUTTON) &
					(1 << 15)
				);
				Win32ProcessKeyboardMessage(
					&NewInput->MouseButtons[1],
					GetKeyState(VK_MBUTTON) &
					(1 << 15)
				);
				Win32ProcessKeyboardMessage(
					&NewInput->MouseButtons[2],
					GetKeyState(VK_RBUTTON) &
					(1 << 15)
				);
				/*************************/
				DWORD MaxControllerCount = XUSER_MAX_COUNT/*keyboard*/;
				if (MaxControllerCount > (ArrayCount(NewInput->Controllers) - 1))
				{
					MaxControllerCount = (ArrayCount(NewInput->Controllers) - 1);
				}
				for (DWORD ControllerIndex = 0;
					ControllerIndex < MaxControllerCount;
					++ControllerIndex)//input
				{
					/*******************************************PLATFORMINDEPENDENT INPUT*/
					DWORD OurControllerIndex = ControllerIndex + 1;
					game_controller_input *OldController = &OldInput->Controllers[OurControllerIndex];
					game_controller_input *NewController = &NewInput->Controllers[OurControllerIndex];
					/*********************************************************************/
					XINPUT_STATE ControllerState;
					if (XInputGetState_(ControllerIndex, &ControllerState) == ERROR_SUCCESS)
					{//if controller plugged CHECK WHEN PLUGGED
						XINPUT_GAMEPAD *Pad = &ControllerState.Gamepad;

						bool32 DPadUp = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
						bool32 DPadDown = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
						bool32 DPadLeft = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
						bool32 DPadRight = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);

						//bool32 DPadA = (Pad->wButtons & XINPUT_GAMEPAD_A);
						bool32 DPadB = (Pad->wButtons & XINPUT_GAMEPAD_B);
						bool32 DPadStart = (Pad->wButtons & XINPUT_GAMEPAD_START);
						bool32 DPadBack = (Pad->wButtons & XINPUT_GAMEPAD_BACK);

						/***************************************PLATFORMINDEPENDENT INPUT*/
						Win32ProcessXInputDigitalButton(
							Pad->wButtons,
							&OldController->Down, XINPUT_GAMEPAD_A,
							&NewController->Down
						);
						/*****************************************************************/
					}
					else
					{
					}
				}
				/********************************SOUND*/
				/*DWORD ByteToLock = 0;
				DWORD TargetCursor = 0;
				DWORD BytesToWrite = 0;
				if (SoundIsValid)
				{
					//hmh 009 1:30 .. introduced a bug with the target cursor
					//hmh 010 maybe that way you can catch up to yourself...
					ByteToLock = ((SoundOutput.RunningSampleIndex*SoundOutput.BytesPerSample)
						% SoundOutput.SecondaryBufferSize);//%remainder...

					TargetCursor = ((LastPlayCursor +
						(SoundOutput.LatencySampleCount * SoundOutput.BytesPerSample))
						% SoundOutput.SecondaryBufferSize)
						;
					BytesToWrite;
					if (ByteToLock > TargetCursor)
					{
						BytesToWrite = (SoundOutput.SecondaryBufferSize - ByteToLock);
						BytesToWrite += TargetCursor;
					}
					else
					{
						BytesToWrite = TargetCursor - ByteToLock;
					}	//ezt kell máshova tenni
					//Win32FillSoundBuffer(&SoundOutput, ByteToLock, BytesToWrite, &SoundBuffer);
				}*/
				/*******************************************************************SOUND*/
				//int16 Samples[48000 / 60 * 2];
				//game_sound_output_buffer SoundBuffer = {};
				//SoundBuffer.SamplesPerSecond = SoundOutput.SamplesPerSecond;
				//SoundBuffer.SampleCount = BytesToWrite / SoundOutput.BytesPerSample;
				//SoundBuffer.Samples = Samples;
				/*******************************************************************PAINT*/
				game_offscreen_buffer Buffer = {};
				Buffer.Memory = GlobalBackBuffer.Memory;
				Buffer.Width = GlobalBackBuffer.Width;
				Buffer.Height = GlobalBackBuffer.Height;
				Buffer.Pitch = GlobalBackBuffer.Pitch;
				Buffer.BytesPerPixel = GlobalBackBuffer.BytesPerPixel;
				/*****************************************************************NETWORK*/
				//A player itt nem is létezik
				player_instance Player = {};
				//... playeres dolgok
				local_player_parts LocalPlayerParts = {};
				//LocalPlayerAssembleLocalPlayer(&LocalPlayerParts, Player);//(!!!!!)
				/*********************************************************************MAP*/
				/***********************************************************GAME.CPP_CALL*/
				Client(&GlobalLocalPlayerParts);//elég egyszer gondolom
				GameUpdateAndRender(
					&GameMemory, NewInput, &Buffer, /*&SoundBuffer,*/ &GlobalClientParts, &Player, &GlobalLocalPlayerParts
				);
				//Client(&GlobalLocalPlayerParts);//elég egyszer gondolom
				/*if (SoundIsValid)
				{
					DWORD PlayCursor;
					DWORD WriteCursor;
					GlobalSecondaryBuffer->GetCurrentPosition(&PlayCursor, &WriteCursor);
					DWORD UnwrappedWriteCursor = WriteCursor;
					if (UnwrappedWriteCursor < PlayCursor)
					{
						UnwrappedWriteCursor += SoundOutput.SecondaryBufferSize;
					}
					AudioLatencyBytes = UnwrappedWriteCursor - PlayCursor;
					AudioLatencySeconds = ((AudioLatencyBytes / SoundOutput.BytesPerSample)
						/ SoundOutput.SamplesPerSecond);
						;

					Win32FillSoundBuffer(&SoundOutput, ByteToLock, BytesToWrite, &SoundBuffer);
				}*/
				//	
				//ezt kell a gameben ...				
				/*******************************************************************SOUND*/
				//if (SoundIsValid)
				//{
				//	//hmh 009 1:30 .. introduced a bug with the target cursor
				//	//hmh 010 maybe that way you can catch up to yourself...
				//	DWORD ByteToLock = ((SoundOutput.RunningSampleIndex*SoundOutput.BytesPerSample)
				//		% SoundOutput.SecondaryBufferSize);//%remainder...

				//	DWORD TargetCursor = ((LastPlayCursor + 
				//		(SoundOutput.LatencySampleCount * SoundOutput.BytesPerSample))
				//		% SoundOutput.SecondaryBufferSize)
				//		;
				//	DWORD BytesToWrite;
				//	if (ByteToLock > TargetCursor)
				//	{
				//		BytesToWrite = (SoundOutput.SecondaryBufferSize - ByteToLock);
				//		BytesToWrite += TargetCursor;
				//	}
				//	else
				//	{
				//		BytesToWrite = TargetCursor - ByteToLock;
				//	}	//ezt kell máshova tenni
				//	Win32FillSoundBuffer(&SoundOutput, ByteToLock, BytesToWrite, &SoundBuffer);
				//}
				/*********************************************************************FPS*/
				LARGE_INTEGER WorkCounter = Win32GetWallClock();
				real32 WorkSecondsElapsed = Win32GetSecondsElapsed(LastCounter, WorkCounter);
				real32 SecondsElapsedForFrame = WorkSecondsElapsed;

				//low part vs quad part, now we never ever hit the target.. coz its too low
				//and the secelapsed is more than 1??? not possible
				if (SecondsElapsedForFrame < TargetSecondsPerFrame)
				{				
						while (SecondsElapsedForFrame < TargetSecondsPerFrame)
						{
							if (SleepIsGranular)
							{
								DWORD SleepMS = (DWORD)(1000.0f * (TargetSecondsPerFrame - SecondsElapsedForFrame));
								if (SleepMS > 0)
								{
									Sleep(SleepMS);
								}
							}
							SecondsElapsedForFrame = Win32GetSecondsElapsed(LastCounter, Win32GetWallClock());
							//lastcounter egy wallclock
						}
				}
				else
				{
				}
				LARGE_INTEGER EndCounter = Win32GetWallClock();
				LastCounter = EndCounter;
					//missed framerate
				/*****************************************************************************BUFFER*/
				win32_window_dimension Dimension = GetWindowDimension(Window);
				Win32CopyBufferToWindow(DeviceContext,
					Dimension.Width, Dimension.Height,
					&GlobalBackBuffer
				);
				/*******************************************************************************SOUND*/
				/*DWORD PlayCursor;
				DWORD WriteCursor;
				if (SUCCEEDED(GlobalSecondaryBuffer->GetCurrentPosition(&PlayCursor, &WriteCursor)))
				{
					LastPlayCursor = PlayCursor;
					if(!SoundIsValid)
					{ 
						SoundOutput.RunningSampleIndex = WriteCursor / SoundOutput.BytesPerSample;
						SoundIsValid = true;
					}
				}
				else
				{
					SoundIsValid = false;
				}*/
				/************************************************************************/
				//LARGE_INTEGER EndCounter = Win32GetWallClock();
				//LastCounter = EndCounter;

				int32 FPS = Win32GetSecondsElapsed(LastCounter, EndCounter);
				int32 MSPerFrame = 1000.0f*Win32GetSecondsElapsed(LastCounter, EndCounter);

				//debug fps values

				int32 EndCycleCount = __rdtsc();
				int32 CyclesElapsed = EndCycleCount - LastCycleCount;
				LastCycleCount = EndCycleCount;
				/***********************************************PLATFORMINDEPENDENT INPUT*/
				game_input *Temp = NewInput;
				NewInput = OldInput;
				OldInput = Temp;
				/*************************************************************************/
			}
		}
	}
	else
	{
	}
	return(0);
}
