#ifndef _TEXTFIELD_
#define _TEXTFIELD_
#include <iostream>
#include <fstream>
#include "renderer.cpp"
#include "clocker.cpp"
#include "input.cpp"


class textfield {
private:
	std::string context = " ";
	bool showText = {true};
	bool showCursor = {false};
	bool write = { false };//if able to write
	bool active = { false };//if selected to  input
	bool cycle = { false };//if we want to cycle through text
	int cyclespeed = 400;//in milliseconds
	int crblink = 300;
	unsigned int selector = {0};
	unsigned int cyclestep = { 0 };
	int cursorPos = { 0 };
	int scopestart = { 0 };//scope is scope start +xnumchar*ynumchar
	clocker timerms;
	clocker cursorms;
	int charw = { 15 }, charh = {15 };//
	int xpadding = {charw/2}, ypadding=0;
	int xnumchar =5, ynumchar = 2;
	int xPos=30, yPos =100;
	uint32_t bgColor = 0x00000000;
	uint32_t crColor = 0x0F000000;
	sprite* font;
	input* in;
	std::vector<std::tuple<input::key, std::string, std::string>> kMap =//some keys arent working properly
	{
		{input::key::A, "a", "A"}, {input::key::B, "b", "B"}, {input::key::C, "c", "C"}, {input::key::D, "d", "D"}, {input::key::E, "e", "E"},
		{input::key::F, "f", "F"}, {input::key::G, "g", "G"}, {input::key::H, "h", "H"}, {input::key::I, "i", "I"}, {input::key::J, "j", "J"},
		{input::key::K, "k", "K"}, {input::key::L, "l", "L"}, {input::key::M, "m", "M"}, {input::key::N, "n", "N"}, {input::key::O, "o", "O"},
		{input::key::P, "p", "P"}, {input::key::Q, "q", "Q"}, {input::key::R, "r", "R"}, {input::key::S, "s", "S"}, {input::key::T, "t", "T"},
		{input::key::U, "u", "U"}, {input::key::V, "v", "V"}, {input::key::W, "w", "W"}, {input::key::X, "x", "X"}, {input::key::Y, "y", "Y"},
		{input::key::Z, "z", "Z"},

		{input::key::K0, "0", ")"}, {input::key::K1, "1", "!"}, {input::key::K2, "2", "@"}, {input::key::K3, "3", "#"},	{input::key::K4, "4", "$"},
		{input::key::K5, "5", "%"}, {input::key::K6, "6", "^"}, {input::key::K7, "7", "&"}, {input::key::K8, "8", "*"},	{input::key::K9, "9", "("},

		{input::key::N0, "0", "0"}, {input::key::N1, "1", "1"}, {input::key::N2, "2", "2"}, {input::key::N3, "3", "3"},	{input::key::N4, "4", "4"},
		{input::key::N5, "5", "5"}, {input::key::N6, "6", "6"}, {input::key::N7, "7", "7"}, {input::key::N8, "8", "8"},	{input::key::N9, "9", "9"},
		{input::key::MUL, "*", "*"}, {input::key::DIVIDE, "/", "/"}, {input::key::SUBTRACT, "-", "-"},	{input::key::DECIMAL, ".", "."},

		{input::key::PERIOD, ".", ">"}, {input::key::ADD, "=", "+"}, {input::key::COMMA, ",", "<"}, {input::key::MINUS, "-", "_"}, {input::key::SPACE, " ", " "},

		{input::key::SEMICOLON, ";", ":"}, {input::key::QUESTION, "/", "?"}, {input::key::SQBRL, "[", "{"},
		{input::key::LSLASH, "\\", "|"}, {input::key::SQBRR, "]", "}"}, {input::key::TILDA, "#", "~"}
	};
	std::vector<std::string> previous = {""};
		public:
			enum RW {
				READ = false,
				WRITE = true
			};


			
			textfield(const char* str, RW rw, sprite& fonts, input& inp) {
				context=str;
				write = rw;
				if (context.length()>(xnumchar * ynumchar))
				scopestart = context.length() - (xnumchar * ynumchar);
				cursorPos = context.length();
				font = &fonts;
				in = &inp;

			};
			textfield(const char* str, RW rw, sprite& fonts) {
				context = str;
				write = rw;
				cursorPos = context.length();
				if (context.length() > (xnumchar * ynumchar))
					scopestart = context.length() - (xnumchar * ynumchar);
				font = &fonts;
};
			int put(std::string str){
				context = str;
				if (context.length() > (xnumchar * ynumchar))
					scopestart = context.length() - (xnumchar * ynumchar);
				cursorPos = context.length();
				return 0;
			}




			int draw(Renderer& render) {
				if (!showText)
					return 0;
				int len = context.length();
				if (write) {
					if (active) {
						if (cursorms.millis() > crblink) {

							showCursor = !showCursor;

							cursorms.refresh();
						}
					}
					else showCursor = false;
				}
				else 
					showCursor = false;

				if (cycle == true) {
					if (timerms.millis() > cyclespeed) {

						if (cursorPos <= len)
						{
							scopestart++;
						cursorPos=scopestart;
						}
						if (cursorPos > len)
						{
							scopestart=0;
							cursorPos = 0;
						}
						timerms.refresh();
					}

				} 
				if (bgColor > 0x00FFFFFF)
					render.fillcoordRect((xPos - charw / 2), (yPos - charh / 2), (charw * xnumchar + xPos - charw / 2 + xpadding), (charh * ynumchar + yPos - charh / 2 + ypadding), bgColor);
				unsigned int scopelength = xnumchar * ynumchar;
				if (cursorPos > (scopelength + scopestart))
					scopestart = cursorPos - scopelength;
				if (cursorPos < scopestart)
					scopestart = cursorPos;

				int scopeCursor=cursorPos-scopestart;
						
					for (std::string::size_type i = 0; i < scopelength; i++){
					
					if ((i+scopestart > len))
						break;

						font->setFrame(context[i+scopestart]);
						font->resizeNNOrigin(charw, charh);
						int x = i % xnumchar;
						int y = i / xnumchar;
						render.drawSprite(*font, xPos + (((charw / 2 + xpadding) * x)), yPos + (y * charh), 0, 0, 0, 0, 0);

					
				}
					if (showCursor) {
						font->setFrame("|");
						font->resizeNNOrigin(charw, charh);

						int x = (scopeCursor-1) % xnumchar;//10 x=0;y=2. willing x=5, y=1
						int y = (scopeCursor-1) / xnumchar;
						render.drawSprite(*font, xPos + (charw / 2) + (((charw / 2 + xpadding) * (x))), yPos + ((y)*charh), 0, 0, 0, 0, 0);
					}




				return 0;
			};
			int setposition(int x, int y) {
				xPos = x; yPos = y;
			
				return 0;
			}
			int activate() {
				if (in == NULL)
					return -1;
				active = true;
				return 0;
			};
			int deactivate() {
				active = false;
				return 0;
			};
			int clear() {
				context = "";
				cursorPos = 0;
				return 0;
			};
			int show() {
				showText = true;
				return 0;
			}
			int hide() {
				showText = false;
				return 0;
			}
			



			int call() {
				if (in == NULL)
					return -1;
				if (!showText)
					return -1;
				if (!write)
					return -1;
				if (!active)
					return -1;
				if (in->isKeyPressed(input::key::ENTER))
				{
					if (context != "")
					{
						previous.push_back(context);
						selector = previous.size();
					}
					//in this place to insert event handler
					this->deactivate();
					this->clear();
				}


				for (const auto& k : kMap)
					if (in->isKeyPressed(std::get<0>(k)))
					{
						if (in->isKeyDown(input::key::SHIFT))
						{
							context.insert(cursorPos, std::get<2>(k));
							cursorPos++;
						}

						else
						{
							context.insert(cursorPos, std::get<1>(k));
							cursorPos++;
						}

					}
				if (in->isKeyPressed(input::key::RIGHT))
					if (cursorPos<context.length())
						cursorPos++;
				if (in->isKeyPressed(input::key::LEFT))
					if (cursorPos > 0)
						cursorPos--;

				if (in->isKeyPressed(input::key::UP))
				{
					if (selector > 0)
					{	selector--;
					context = previous.at(selector);
					cursorPos = context.length();
				}

				}
				if (in->isKeyPressed(input::key::DOWN))
				{
					if (selector < previous.size()-1)
					{
						selector++;
						context = previous.at(selector);
						cursorPos = context.length();
					}
					else
					{
						context = "";
						cursorPos = context.length();
					}

				}

				
				if (in->isKeyPressed(input::key::BACK))
				if ((context.length() > 0)&& (cursorPos > 0))
				{
					context.erase(cursorPos-1,1);
					cursorPos--;
				}
				if (in->isKeyPressed(input::key::DEL))
					if ((context.length() > 0) && (cursorPos > 0))
					{
						context.erase(cursorPos, 1);
					}
				return 0;


			};

		};

#endif