﻿#ifndef MOAIKEYCODEENUM_H
#define MOAIKEYCODEENUM_H

// This is a C/C++ language header with no dependencies on Moai internals

// winnt.h has an unfortunate "DELETE" define that conflicts with the key name
#undef DELETE

#define MOAI_KEY_NONCHAR_KEY_OFFSET 0x100

//================================================================//
// MOAIKeyCodes
//================================================================//
// These are the codes used to represent keyboard keys.
// For most keys that typically generate a character, the key code is the ASCII character generated when pressing that key on a *US* keyboard.
// For all other keys, the key code is the key's virtual-key code (msdn.microsoft.com/library/windows/desktop/dd375731) plus an offset to prevent overlaps.
enum MOAIKeyCodes {

	MOAI_KEY_BACKSPACE			= MOAI_KEY_NONCHAR_KEY_OFFSET + 8,		// VK_BACK
	MOAI_KEY_TAB				= MOAI_KEY_NONCHAR_KEY_OFFSET + 9,		// VK_TAB
	MOAI_KEY_RETURN				= MOAI_KEY_NONCHAR_KEY_OFFSET + 13,		// VK_RETURN
	MOAI_KEY_SHIFT				= MOAI_KEY_NONCHAR_KEY_OFFSET + 16,		// VK_SHIFT
	MOAI_KEY_CONTROL			= MOAI_KEY_NONCHAR_KEY_OFFSET + 17,		// VK_CONTROL
	MOAI_KEY_ALT				= MOAI_KEY_NONCHAR_KEY_OFFSET + 18,		// VK_MENU
	MOAI_KEY_PAUSE				= MOAI_KEY_NONCHAR_KEY_OFFSET + 19,		// VK_PAUSE
	MOAI_KEY_CAPS_LOCK			= MOAI_KEY_NONCHAR_KEY_OFFSET + 20,		// VK_CAPITAL
	MOAI_KEY_ESCAPE				= MOAI_KEY_NONCHAR_KEY_OFFSET + 27,		// VK_ESCAPE
	MOAI_KEY_SPACE				= ' ',									// VK_SPACE
	MOAI_KEY_PAGE_UP			= MOAI_KEY_NONCHAR_KEY_OFFSET + 33,		// VK_PRIOR
	MOAI_KEY_PAGE_DOWN			= MOAI_KEY_NONCHAR_KEY_OFFSET + 34,		// VK_NEXT
	MOAI_KEY_END				= MOAI_KEY_NONCHAR_KEY_OFFSET + 35,		// VK_END
	MOAI_KEY_HOME				= MOAI_KEY_NONCHAR_KEY_OFFSET + 36,		// VK_HOME
	MOAI_KEY_LEFT				= MOAI_KEY_NONCHAR_KEY_OFFSET + 37,		// VK_LEFT
	MOAI_KEY_UP					= MOAI_KEY_NONCHAR_KEY_OFFSET + 38,		// VK_UP
	MOAI_KEY_RIGHT				= MOAI_KEY_NONCHAR_KEY_OFFSET + 39,		// VK_RIGHT
	MOAI_KEY_DOWN				= MOAI_KEY_NONCHAR_KEY_OFFSET + 40,		// VK_DOWN
	MOAI_KEY_PRINT_SCREEN		= MOAI_KEY_NONCHAR_KEY_OFFSET + 44,		// VK_SNAPSHOT
	MOAI_KEY_INSERT				= MOAI_KEY_NONCHAR_KEY_OFFSET + 45,		// VK_INSERT
	MOAI_KEY_DELETE				= MOAI_KEY_NONCHAR_KEY_OFFSET + 46,		// VK_DELETE
	MOAI_KEY_DIGIT_0			= '0',
	MOAI_KEY_DIGIT_1			= '1',
	MOAI_KEY_DIGIT_2			= '2',
	MOAI_KEY_DIGIT_3			= '3',
	MOAI_KEY_DIGIT_4			= '4',
	MOAI_KEY_DIGIT_5			= '5',
	MOAI_KEY_DIGIT_6			= '6',
	MOAI_KEY_DIGIT_7			= '7',
	MOAI_KEY_DIGIT_8			= '8',
	MOAI_KEY_DIGIT_9			= '9',
	MOAI_KEY_A					= 'a',
	MOAI_KEY_B					= 'b',
	MOAI_KEY_C					= 'c',
	MOAI_KEY_D					= 'd',
	MOAI_KEY_E					= 'e',
	MOAI_KEY_F					= 'f',
	MOAI_KEY_G					= 'g',
	MOAI_KEY_H					= 'h',
	MOAI_KEY_I					= 'i',
	MOAI_KEY_J					= 'j',
	MOAI_KEY_K					= 'k',
	MOAI_KEY_L					= 'l',
	MOAI_KEY_M					= 'm',
	MOAI_KEY_N					= 'n',
	MOAI_KEY_O					= 'o',
	MOAI_KEY_P					= 'p',
	MOAI_KEY_Q					= 'q',
	MOAI_KEY_R					= 'r',
	MOAI_KEY_S					= 's',
	MOAI_KEY_T					= 't',
	MOAI_KEY_U					= 'u',
	MOAI_KEY_V					= 'v',
	MOAI_KEY_W					= 'w',
	MOAI_KEY_X					= 'x',
	MOAI_KEY_Y					= 'y',
	MOAI_KEY_Z					= 'z',
	MOAI_KEY_GUI				= MOAI_KEY_NONCHAR_KEY_OFFSET + 91,		// VK_LWIN
	MOAI_KEY_APPLICATION		= MOAI_KEY_NONCHAR_KEY_OFFSET + 93,		// VK_APPS
	MOAI_KEY_NUM_0				= MOAI_KEY_NONCHAR_KEY_OFFSET + 96,		// VK_NUMPAD0
	MOAI_KEY_NUM_1				= MOAI_KEY_NONCHAR_KEY_OFFSET + 97,		// VK_NUMPAD1
	MOAI_KEY_NUM_2				= MOAI_KEY_NONCHAR_KEY_OFFSET + 98,		// VK_NUMPAD2
	MOAI_KEY_NUM_3				= MOAI_KEY_NONCHAR_KEY_OFFSET + 99,		// VK_NUMPAD3
	MOAI_KEY_NUM_4				= MOAI_KEY_NONCHAR_KEY_OFFSET + 100,	// VK_NUMPAD4
	MOAI_KEY_NUM_5				= MOAI_KEY_NONCHAR_KEY_OFFSET + 101,	// VK_NUMPAD5
	MOAI_KEY_NUM_6				= MOAI_KEY_NONCHAR_KEY_OFFSET + 102,	// VK_NUMPAD6
	MOAI_KEY_NUM_7				= MOAI_KEY_NONCHAR_KEY_OFFSET + 103,	// VK_NUMPAD7
	MOAI_KEY_NUM_8				= MOAI_KEY_NONCHAR_KEY_OFFSET + 104,	// VK_NUMPAD8
	MOAI_KEY_NUM_9				= MOAI_KEY_NONCHAR_KEY_OFFSET + 105,	// VK_NUMPAD9
	MOAI_KEY_NUM_MULTIPLY		= MOAI_KEY_NONCHAR_KEY_OFFSET + 106,	// VK_MULTIPLY
	MOAI_KEY_NUM_PLUS			= MOAI_KEY_NONCHAR_KEY_OFFSET + 107,	// VK_ADD
	MOAI_KEY_NUM_MINUS			= MOAI_KEY_NONCHAR_KEY_OFFSET + 109,	// VK_SUBTRACT
	MOAI_KEY_NUM_DECIMAL		= MOAI_KEY_NONCHAR_KEY_OFFSET + 110,	// VK_DECIMAL
	MOAI_KEY_NUM_DIVIDE			= MOAI_KEY_NONCHAR_KEY_OFFSET + 111,	// VK_DIVIDE
	MOAI_KEY_F1					= MOAI_KEY_NONCHAR_KEY_OFFSET + 112,	// VK_F1
	MOAI_KEY_F2					= MOAI_KEY_NONCHAR_KEY_OFFSET + 113,	// VK_F2
	MOAI_KEY_F3					= MOAI_KEY_NONCHAR_KEY_OFFSET + 114,	// VK_F3
	MOAI_KEY_F4					= MOAI_KEY_NONCHAR_KEY_OFFSET + 115,	// VK_F4
	MOAI_KEY_F5					= MOAI_KEY_NONCHAR_KEY_OFFSET + 116,	// VK_F5
	MOAI_KEY_F6					= MOAI_KEY_NONCHAR_KEY_OFFSET + 117,	// VK_F6
	MOAI_KEY_F7					= MOAI_KEY_NONCHAR_KEY_OFFSET + 118,	// VK_F7
	MOAI_KEY_F8					= MOAI_KEY_NONCHAR_KEY_OFFSET + 119,	// VK_F8
	MOAI_KEY_F9					= MOAI_KEY_NONCHAR_KEY_OFFSET + 120,	// VK_F9
	MOAI_KEY_F10				= MOAI_KEY_NONCHAR_KEY_OFFSET + 121,	// VK_F10
	MOAI_KEY_F11				= MOAI_KEY_NONCHAR_KEY_OFFSET + 122,	// VK_F11
	MOAI_KEY_F12				= MOAI_KEY_NONCHAR_KEY_OFFSET + 123,	// VK_F12
	MOAI_KEY_NUM_LOCK			= MOAI_KEY_NONCHAR_KEY_OFFSET + 144,	// VK_NUMLOCK
	MOAI_KEY_SCROLL_LOCK		= MOAI_KEY_NONCHAR_KEY_OFFSET + 145,	// VK_SCROLL
	MOAI_KEY_OEM_1				= ';',									// VK_OEM_1
	MOAI_KEY_OEM_PLUS			= '=',									// VK_OEM_PLUS
	MOAI_KEY_OEM_COMMA			= ',',									// VK_OEM_COMMA
	MOAI_KEY_OEM_MINUS			= '-',									// VK_OEM_MINUS
	MOAI_KEY_OEM_PERIOD			= '.',									// VK_OEM_PERIOD
	MOAI_KEY_OEM_2				= '/',									// VK_OEM_2
	MOAI_KEY_OEM_3				= '`',									// VK_OEM_3
	MOAI_KEY_OEM_4				= '[',									// VK_OEM_4
	MOAI_KEY_OEM_5				= '\\',									// VK_OEM_5
	MOAI_KEY_OEM_6				= ']',									// VK_OEM_6
	MOAI_KEY_OEM_7				= '\'',									// VK_OEM_7
	MOAI_KEY_OEM_8				= MOAI_KEY_NONCHAR_KEY_OFFSET + 223,	// VK_OEM_8
	MOAI_KEY_OEM_102			= MOAI_KEY_NONCHAR_KEY_OFFSET + 226,	// VK_OEM_102

	MOAI_KEY_TOTAL				= MOAI_KEY_NONCHAR_KEY_OFFSET + 0x100,
	MOAI_KEY_INVALID			= MOAI_KEY_TOTAL,
};

#endif
