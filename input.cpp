#include <windows.h>
class input {
private:
	bool keyCurrState[255] = {0};
	bool keyPrevState[255] = {0};
	struct keyState
	{
		bool pressed=false;
		bool released = false;
		bool down = false;
	};
	keyState keyboard[255];

	struct mousePosition {
		int x = 0;
		int y = 0;
	} mousePos;
	


public:
	enum key {
		LMB = 0x01,
		RMB = 0x02,//	Right mouse button
		BREAK = 0x03,//	Control - break processing
		MMB = 0x04,//	Middle mouse button(three - button mouse)
		X1MB = 0x05,//	X1 mouse button
		X2MB = 0x06,//	X2 mouse button

		BACK = 0x08,//	BACKSPACE key
		TAB = 0x09,//	TAB key

		CLEAR = 0x0C,//	CLEAR key
		ENTER = 0x0D,//	ENTER key

		SHIFT = 0x10,//	SHIFT key
		CTRL = 0x11,//	CTRL key
		ALT = 0x12,//	ALT key
		PAUSE = 0x13,//	PAUSE key
		CAPSLOCK = 0x14,//	CAPS LOCK key

		ESC = 0x1B,//	ESC key
		SPACE = 0x20,//	SPACEBAR
		PG_UP = 0x21,//	PAGE UP key
		PG_DOWN = 0x22,//	PAGE DOWN key
		END = 0x23,//	END key
		HOME = 0x24,//	HOME key
		LEFT = 0x25,//	LEFT ARROW key
		UP = 0x26,//	UP ARROW key
		RIGHT = 0x27,//	RIGHT ARROW key
		DOWN = 0x28,//	DOWN ARROW key
		SELECT = 0x29,//	SELECT key
		PRINT = 0x2A,//	PRINT key
		EXECUTE = 0x2B,//	EXECUTE key
		PRNT_SCRN = 0x2C,//	PRINT SCREEN key
		INSERT = 0x2D,//	INS key
		DEL = 0x2E,//	DEL key
		HELP = 0x2F,//	HELP key
		K0 = 0x30,//	0 key
		K1 = 0x31,//	1 key
		K2 = 0x32,//	2 key
		K3 = 0x33,//	3 key
		K4 = 0x34,//	4 key
		K5 = 0x35,//	5 key
		K6 = 0x36,//	6 key
		K7 = 0x37,//	7 key
		K8 = 0x38,//	8 key
		K9 = 0x39,//	9 key

		A = 0x41,//	A key
		B = 0x42,//	B key
		C = 0x43,//	C key
		D = 0x44,//	D key
		E = 0x45,//	E key
		F = 0x46,//	F key
		G = 0x47,//	G key
		H = 0x48,//	H key
		I = 0x49,//	I key
		J = 0x4A,//	J key
		K = 0x4B,//	K key
		L = 0x4C,//	L key
		M = 0x4D,//	M key
		N = 0x4E,//	N key
		O = 0x4F,//	O key
		P = 0x50,//	P key
		Q = 0x51,//	Q key
		R = 0x52,//	R key
		S = 0x53,//	S key
		T = 0x54,//	T key
		U = 0x55,//	U key
		V = 0x56,//	V key
		W = 0x57,//	W key
		X = 0x58,//	X key
		Y = 0x59,//	Y key
		Z = 0x5A,//	Z key

		N0 = 0x60,//	Numeric keypad 0 key
		N1 = 0x61,//	Numeric keypad 1 key
		N2 = 0x62,//	Numeric keypad 2 key
		N3 = 0x63,//	Numeric keypad 3 key
		N4 = 0x64,//	Numeric keypad 4 key
		N5 = 0x65,//	Numeric keypad 5 key
		N6 = 0x66,//	Numeric keypad 6 key
		N7 = 0x67,//	Numeric keypad 7 key
		N8 = 0x68,//	Numeric keypad 8 key
		N9 = 0x69,//	Numeric keypad 9 key
		MUL = 0x6A,//	Multiply key
		ADD = 0x6B,//	Add key
		SEPARATOR = 0x6C,//	Separator key
		SUBTRACT = 0x6D,//	Subtract key
		DECIMAL = 0x6E,//	Decimal key
		DIVIDE = 0x6F,//	Divide key
		F1 = 0x70,//	F1 key
		F2 = 0x71,//	F2 key
		F3 = 0x72,//	F3 key
		F4 = 0x73,//	F4 key
		F5 = 0x74,//	F5 key
		F6 = 0x75,//	F6 key
		F7 = 0x76,//	F7 key
		F8 = 0x77,//	F8 key
		F9 = 0x78,//	F9 key
		F10 = 0x79,//	F10 key
		F11 = 0x7A,//	F11 key
		F12 = 0x7B,//	F12 key
		NUMLOCK = 0x90,//	NUM LOCK key
		SCROLLLOCK = 0x91,//	SCROLL LOCK key
		LSHIFT = 0xA,//	Left SHIFT key
		RSHIFT = 0xA1,//	Right SHIFT key
		LCTRL = 0xA2,//	Left CONTROL key
		RCTRL = 0xA3,//	Right CONTROL key
		LALT = 0xA4,//	Left ALT key
		RALT = 0xA5,//	Right ALT key

		VOLUME_MUTE = 0xAD,//	Volume Mute key
		VOLUME_DOWN = 0xAE,//	Volume Down key
		VOLUME_UP = 0xAF,//	Volume Up key

		SEMICOLON = 0xBA,//Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the ';:' key
		PLUS = 0xBB,//For any country / region, the '+' key
		COMMA = 0xBC,//For any country / region, the ',' key
		MINUS = 0xBD,//	For any country / region, the '-' key
		PERIOD = 0xBE,//For any country / region, the '.' key
		QUESTION = 0xBF,//	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the '/?' key
		TILDA = 0xC0,//	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the '`~' key

		SQBRL = 0xDB,//	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the '[{' key
		LSLASH = 0xDC,//	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the '\|' key
		SQBRR = 0xDD,//	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the ']}' key
		SQUOTE = 0xDE,//	Used for miscellaneous characters; it can vary by keyboard.For the US standard keyboard, the 'single-quote/double-quote' key
		OEM = 0xDF,//	Used for miscellaneous characters; it can vary by keyboard.
		LSLASH2 = 0xE2,//	The <> keys on the US standard keyboard, or the \\ | key on the non - US 102 - key keyboard

		PACK = 0xE7,//	Used to pass Unicode characters as if they were keystrokes.The VK_PACKET key is the low word of a 32 - bit Virtual Key value used for non - keyboard input methods.For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP


		OEMCLR = 0xFE//	Clear key



	};
	void update() {
		for (int i = 0; i < 255; i++)
		{
			keyboard[i].pressed = false;
			keyboard[i].released = false;
			if (keyCurrState[i] != keyPrevState[i])
			{
				if (keyCurrState[i])
				{
					keyboard[i].pressed = !keyboard[i].down;
					keyboard[i].down = true;
				}
				else
				{
					keyboard[i].released = true;
					keyboard[i].down = false;

				}
				keyPrevState[i] = keyCurrState[i];
			}

		}
		
	};
	void updateKEY(int key, bool state) {
		keyCurrState[key] = state;

	};

	keyState KEYState(key one) {
		return keyboard[one];
	
	}
	bool isKeyDown(key one) {
		return keyboard[one].down;
	};
	void updateMousePos(LPARAM lowW) {
		mousePos.x = LOWORD(lowW), mousePos.y = HIWORD(lowW);//needs to be clamped to content size
	}
	mousePosition getMousePos() {
		return mousePos;
	};
	mousePosition getMousePos(int WWidth, int WHeight, int cwidth, int cheight) {
		mousePosition p;
		p.x = (int)(((float)mousePos.x / (float)WWidth * (float)cwidth));
		p.y = (int)(((float)mousePos.y / (float)WHeight * (float)cheight));
		return p;
	
	}

};