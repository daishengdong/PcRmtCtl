/**************************
*´«ÊäÐ­Òé
***************************/
#define EnterMouseMode 0x01
#define LeaveMouseMode 0x02
#define MouseLeftButton 0x03
#define MouseRightButton 0x04
#define EnterPaintButton 0x05 //½øÈë»­±ÊÄ£Ê½
#define LeavePaintButton 0x06	//ÍË³ö»­±ÊÄ£Ê½
#define ErasePaintButton 0x07	//²Á³ý
#define EnterFullScreen 0x08	//È«ÆÁ
#define ExitFillScreen 0x09		//ÍË³öÈ«ÆÁ
#define PageUpButton 0x0A			//ÉÏ·­Ò³
#define PageDownButton 0x0B		//ÏÂ·­Ò³
#define ModeSwitchButton 0x0C
#define CtrlAltDelete    0x0D 
#define InputPassWord    0x0E

//unsigned char USB_MouseBuffer[5]={0,0,0,0,0};
//unsigned char USB_KeyBoradBuffer[9]={0,0,0,0,0,0,0,0,0};

/**********Êó±êÏà¹Ø***********/
#define USB_MouseLeftButton 0x01
#define USB_MouseRightButton 0x02
#define USB_MouseMidButton    0x04

/*********¼üÅÌÏà¹Ø**********/
#define USB_MSEnterPaintButton  0x01,0x13,0 
#define USB_WPSEnterPaintButton 0x01,0x05,0
#define USB_LeavePaintButton  0x01,0x04,0
#define USB_ErasePaintButton  0x01,0x08,0 
#define USB_EnterFullScreen   0,0x3E,0        //F5
#define USB_ExitFillScreen    0,0x29,0         //ESC
#define USB_PageUpButton 		  0,0x52,0 
#define USB_PageDownButton 	  0,0x51,0 
#define USB_CtrlAltDelete     0x01|0x04,0x4C,0
//#define USB_ModeSwitchButton 0x0C

//#define USB_ESC  0x29          //
//#define USB_F5    0x3E         //

#define CURSOR_STEP     10	//Êó±êÒÆ¶¯ÏñËØ

typedef unsigned char      u8;
typedef signed char      s8;
#define uchar unsigned char
#define schar signed char
typedef struct {
		uchar SS;//'s'
		uchar FB;
	  uchar mouse_enanle;
		short mouse_x;
		short mouse_y;
		schar reserve[5];
    uchar End;
}PackFormat;


/************¶ÔÓ¦µÄUSBÂë*********************
#define 0x00 //Reserved (no event indicated)9 
#define 0x01 //Keyboard ErrorRollOver9 
#define 0x02 //Keyboard POSTFail9
#define 0x03 //Keyboard ErrorUndefined9
#define 0x04 //Keyboard a and A 
#define 0x05 //Keyboard b and B 
#define 0x06 //Keyboard c and c
#define 0x07 //Keyboard d and D
8 08 Keyboard e and E 19 ¡Ì ¡Ì ¡Ì 4/101/104
9 09 Keyboard f and F 34 ¡Ì ¡Ì ¡Ì 4/101/104
10 0A Keyboard g and G 35 ¡Ì ¡Ì ¡Ì 4/101/104
11 0B Keyboard h and H 36 ¡Ì ¡Ì ¡Ì 4/101/104
12 0C Keyboard i and I 24 ¡Ì ¡Ì ¡Ì 4/101/104
13 0D Keyboard j and J 37 ¡Ì ¡Ì ¡Ì 4/101/104
14 0E Keyboard k and K 38 ¡Ì ¡Ì ¡Ì 4/101/104
15 0F Keyboard l and L 39 ¡Ì ¡Ì ¡Ì 4/101/104
16 10 Keyboard m and M4 52 ¡Ì ¡Ì ¡Ì 4/101/104
17 11 Keyboard n and N 51 ¡Ì ¡Ì ¡Ì 4/101/104
18 12 Keyboard o and O
19 13 Keyboard p and P
20 14 Keyboard q and Q
21 15 Keyboard r and R 
22 16 Keyboard s and S
23 17 Keyboard t and T 21 ¡Ì ¡Ì ¡Ì 4/101/104
24 18 Keyboard u and U 23 ¡Ì ¡Ì ¡Ì 4/101/104
25 19 Keyboard v and V 49 ¡Ì ¡Ì ¡Ì 4/101/104
26 1A Keyboard w and W4 18 ¡Ì ¡Ì ¡Ì 4/101/104
27 1B Keyboard x and X4 47 ¡Ì ¡Ì ¡Ì 4/101/104
28 1C Keyboard y and Y4 22 ¡Ì ¡Ì ¡Ì 4/101/104
29 1D Keyboard z and Z4 46 ¡Ì ¡Ì ¡Ì 4/101/104
30 1E Keyboard 1 and !4 2 ¡Ì ¡Ì ¡Ì 4/101/104
31 1F Keyboard 2 and @4 3 ¡Ì ¡Ì ¡Ì 4/101/104
32 20 Keyboard 3 and #4 4 ¡Ì ¡Ì ¡Ì 4/101/104
33 21 Keyboard 4 and $4 5 ¡Ì ¡Ì ¡Ì 4/101/104
34 22 Keyboard 5 and %4 6 ¡Ì ¡Ì ¡Ì 4/101/104
35 23 Keyboard 6 and ^4 7 ¡Ì ¡Ì ¡Ì 4/101/104
36 24 Keyboard 7 and &4 8 ¡Ì ¡Ì ¡Ì 4/101/104
37 25 Keyboard 8 and *4 9 ¡Ì ¡Ì ¡Ì 4/101/104
38 26 Keyboard 9 and (4 10 ¡Ì ¡Ì ¡Ì 4/101/104
39 27 Keyboard 0 and )4 11 ¡Ì ¡Ì ¡Ì 4/101/104
40 28 Keyboard Return (ENTER)5 43 ¡Ì ¡Ì ¡Ì 4/101/104
41 29 Keyboard ESCAPE 110 ¡Ì ¡Ì ¡Ì 4/101/104
42 2A Keyboard DELETE (Backspace)13 15 ¡Ì ¡Ì ¡Ì 4/101/104
43 2B Keyboard Tab 16 ¡Ì ¡Ì ¡Ì 4/101/104
44 2C Keyboard Spacebar 61 ¡Ì ¡Ì ¡Ì 4/1/104
45 2D Keyboard - and (underscore)4 12 ¡Ì ¡Ì ¡Ì 4/101/104
46 2E Keyboard = and +4 13 ¡Ì ¡Ì ¡Ì 4/101/104
47 2F Keyboard [ and {4 27 ¡Ì ¡Ì ¡Ì 4/101/104
48 30 Keyboard ] and }4 28 ¡Ì ¡Ì ¡Ì 4/101/104
49 31 Keyboard \ and | 29 ¡Ì ¡Ì ¡Ì 4/101/104
50 32 Keyboard Non-US # and ~2 42 ¡Ì ¡Ì ¡Ì 4/101/104
51 33 Keyboard ; and :4 40 ¡Ì ¡Ì ¡Ì 4/101/104
52 34 Keyboard ¡® and ¡°4 41 ¡Ì ¡Ì ¡Ì 4/101/104
53 35 Keyboard Grave Accent and Tilde4 1 ¡Ì ¡Ì ¡Ì 4/101/104
54 36 Keyboard, and <4 53 ¡Ì ¡Ì ¡Ì 4/101/104
55 37 Keyboard . and >4 54 ¡Ì ¡Ì ¡Ì 4/101/104
56 38 Keyboard / and ?4 55 ¡Ì ¡Ì ¡Ì 4/101/104
57 39 Keyboard Caps Lock11 30 ¡Ì ¡Ì ¡Ì 4/101/104
58 3A Keyboard F1 112 ¡Ì ¡Ì ¡Ì 4/101/104
59 3B Keyboard F2 113 ¡Ì ¡Ì ¡Ì 4/101/104
60 3C Keyboard F3 114 ¡Ì ¡Ì ¡Ì 4/101/104
61 3D Keyboard F4 115 ¡Ì ¡Ì ¡Ì 4/101/104
62 3E Keyboard F5 116 ¡Ì ¡Ì ¡Ì 4/101/104
63 3F Keyboard F6 117 ¡Ì ¡Ì ¡Ì 4/101/104
64 40 Keyboard F7 118 ¡Ì ¡Ì ¡Ì 4/101/104
65 41 Keyboard F8 119 ¡Ì ¡Ì ¡Ì 4/101/104
66 42 Keyboard F9 120 ¡Ì ¡Ì ¡Ì 4/101/104
67 43 Keyboard F10 121 ¡Ì ¡Ì ¡Ì 4/101/104
68 44 Keyboard F11 122 ¡Ì ¡Ì ¡Ì 101/104
69 45 Keyboard F12 123 ¡Ì ¡Ì ¡Ì 101/104
70 46 Keyboard PrintScreen1 124 ¡Ì ¡Ì ¡Ì 101/104
71 47 Keyboard Scroll Lock11 125 ¡Ì ¡Ì ¡Ì 4/101/104
72 48 Keyboard Pause1 126 ¡Ì ¡Ì ¡Ì 101/104
73 49 Keyboard Insert1 75 ¡Ì ¡Ì ¡Ì 101/104
74 4A Keyboard Home1 80 ¡Ì ¡Ì ¡Ì 101/104
75 4B Keyboard PageUp1 85 ¡Ì ¡Ì ¡Ì 101/104
76 4C Keyboard Delete Forward1;14 76 ¡Ì ¡Ì ¡Ì 101/104
77 4D Keyboard End1 81 ¡Ì ¡Ì ¡Ì 101/104
78 4E Keyboard PageDown1 86 ¡Ì ¡Ì ¡Ì 101/104
79 4F Keyboard RightArrow1 89 ¡Ì ¡Ì ¡Ì 101/104
80 50 Keyboard LeftArrow1 79 ¡Ì ¡Ì ¡Ì 101/104

81 51 Keyboard DownArrow1 84 ¡Ì ¡Ì ¡Ì 101/10
82 52 Keyboard UpArrow1 83 ¡Ì ¡Ì ¡Ì 101/104
83 53 Keypad Num Lock and Clear11 90 ¡Ì ¡Ì ¡Ì 101/104
84 54 Keypad /
85 55 Keypad * 
86 56 Keypad -
87 57 Keypad +
88 58 Keypad ENTER
89 59 Keypad 1 and End 
90 5A Keypad 2 and Down Arrow 
91 5B Keypad 3 and PageDn 
92 5C Keypad 4 and Left Arrow 
93 5D Keypad 5 
94 5E Keypad 6 and Right Arrow 
95 5F Keypad 7 and Home 91 ¡Ì ¡Ì ¡Ì 4/101/104
96 60 Keypad 8 and Up Arrow 96 ¡Ì ¡Ì ¡Ì 4/101/104
97 61 Keypad 9 and PageUp 101 ¡Ì ¡Ì ¡Ì 4/101/104
98 62 Keypad 0 and Insert 99 ¡Ì ¡Ì ¡Ì 4/101/104
99 63 Keypad . and Delete 104 ¡Ì ¡Ì ¡Ì 4/101/104
100 64 Keyboard Non-US \ and |3;6 45 ¡Ì ¡Ì ¡Ì 4/101/104
101 65 Keyboard Application10 129 ¡Ì ¡Ì 104
102 66 Keyboard Power9 ¡Ì ¡Ì
103 67 Keypad = ¡Ì
104 68 Keyboard F13 ¡Ì
105 69 Keyboard F14 ¡Ì
106 6A Keyboard F15 ¡Ì
107 6B Keyboard F16
108 6C Keyboard F17
109 6D Keyboard F18
110 6E Keyboard F19
111 6F Keyboard F20
112 70 Keyboard F21
113 71 Keyboard F22
114 72 Keyboard F23
115 73 Keyboard F24
116 74 Keyboard Execute ¡Ì
117 75 Keyboard Help ¡Ì
118 76 Keyboard Menu ¡Ì
119 77 Keyboard Select ¡Ì
120 78 Keyboard Stop ¡Ì
121 79 Keyboard Again ¡Ì
122 7A Keyboard Undo ¡Ì

123 7B Keyboard Cut ¡

124 7C Keyboard Copy¡Ì
125 7D Keyboard Paste ¡Ì
126 7E Keyboard Find ¡Ì
127 7F Keyboard Mute ¡Ì
128 80 Keyboard Volume Up ¡Ì
129 81 Keyboard Volume Down ¡Ì
130 82 Keyboard Locking Caps Lock12 ¡Ì
131 83 Keyboard Locking Num Lock12 ¡Ì
132 84 Keyboard Locking Scroll Lock12 ¡Ì
133 85 Keypad Comma27 107
134 86 Keypad Equal Sign29
135 87 Keyboard International115,28 56
136 88 Keyboard International216
137 89 Keyboard International317
138 8A Keyboard International41
139 8B Keyboard International519
140 8C Keyboard International620
141 8D Keyboard International721
142 8E Keyboard International822
143 8F Keyboard International922
144 90 Keyboard LANG125
145 91 Keyboard LANG226
146 92 Keyboard LANG330
147 93 Keyboard LANG431
148 94 Keyboard LANG532
149 95 Keyboard LANG68
150 96 Keyboard LANG78
151 97 Keyboard LANG88
152 98 Keyboard LANG98
153 99 Keyboard Alternate Erase7
154 9A Keyboard SysReq/Attention1
155 9B Keyboard Cancel
156 9C Keyboard Clear
157 9D Keyboard Prior
158 9E Keyboard Return
159 9F Keyboard Separator
160 A0 Keyboard Out
161 A1 Keyboard Oper
162 A2 Keyboard Clear/Again
163 A3 Keyboard CrSel/Props
164 A4 Keyboard ExSel
165-175 A5-CF Reserved
176 B0 Keypad 00
177 B1 Keypad 000
178 B2 Thousands Separator 33
179 B3 Decimal Separator 33
180 B4 Currency Unit 34
181 B5 Currency Sub-unit 34
182 B6 Keypad (
183 B7 Keypad )
184 B8 Keypad {
185 B9 Keypad }
186 BA Keypad Tab
187 BB Keypad Backspace
188 BC Keypad A
189 BD Keypad 
190 BE KeypadC
191 BF Keypad D
192 C0 Keypad E
193 C1 Keypad F
194 C2 Keypad XOR
195 C3 Keypad ^
196 C4 Keypad %

197 C5 Keypad 
198 C6 Keypad >
199 C7 Keypad &
200 C8 Keypad &&
201 C9 Keypad |
202 CA Keypad ||
203 CB Keypad £º
204 CC Keypad #
205 CD Keypad Space
206 CE Keypad @
207 CF Keypad !
208 D0 Keypad Memory Store
209 D1 Keypad Memory Recall
210 D2 Keypad Memory Clear
211 D3 Keypad Memory Add
212 D4 Keypad Memory Subtract
213 D5 Keypad Memory Multiply
214 D6 Keypad Memory Divide
215 D7 Keypad +/-
216 D8 Keypad Clear
217 D9 Keypad Clear Entry
218 DA Keypad Binary
219 DB Keypad Octal
220 DC Keypad Decimal
221 DD Keypad Hexadecimal
222-223 DE-DF Reserved
224 E0 Keyboard LeftControl 58 ¡Ì ¡Ì ¡Ì 4/101/104
225 E1 Keyboard LeftShift 44 ¡Ì ¡Ì ¡Ì 4/101/104
226 E2 Keyboard LeftAlt 60 ¡Ì ¡Ì ¡Ì 4/101/104
227 E3 Keyboard Left GUI10;23 127 ¡Ì ¡Ì ¡Ì 104
228 E4 Keyboard RightControl 64 ¡Ì ¡Ì ¡Ì 101/104
229 E5 Keyboard RightShift 57 ¡Ì ¡Ì ¡Ì 4/101/104
230 E6 Keyboard RightAlt 62 ¡Ì ¡Ì ¡Ì 101/104
231 E7 Keyboard Right GUI10;24 128 ¡Ì ¡Ì ¡Ì 104         

*/
