/*Animation sheet would have form:
ANIM%
PARAM:framewidth:frameheight:xdistancebetweensprites:ydistancebetweensprites:xstartoffset:ystartoffset%
SPRT:sprite1:sprite2:sprite3:sprite4:sprite5:sprite6:sprite7:sprite8:sprite9%
ANIMHEADER%
Animation1:sprite1:sprite2:sprite3%
Animation2:sprite1:sprite4:sprite5%
Animation3:sprite6:sprite1:sprite9:sprite8:sprite1%


it would iterarte through one entire animation before going to next

we need this class because we would only have one sprite but can have multiple animations in different steps

*/

#ifndef _ANIMATION_
#define _ANIMATION_
#include <windows.h>
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include "sprite.cpp"


class animation{
private:
	std::map<std::string, int> spriteList;
	std::map<std::string, std::vector<int>> AnimationList;
	sprite* sprited;

	int framewidth, frameheight, xdistancebetweensprites, ydistancebetweensprites, xstartoffset, ystartoffset;
	std::vector<std::string> split(const std::string& s, char delim) {
		std::vector<std::string> result;
		std::stringstream ss(s);
		std::string item;

		while (std::getline(ss, item, delim)) {
			result.push_back(item);
		}

		return result;
	}

public:
	int getAnimationsListFromFile(const char* filename, std::map<std::string, sprite*> collection, const char* spritename){
		std::string path;
		path = filename;
		sprited = collection[std::string(spritename)];
		if (sprited == NULL)
			return -1;
		std::ifstream list;
		list.open(filename, std::ios_base::binary);
		std::string sbuffer, pbuffer;
		list.seekg(0, list.end);
		unsigned int size = list.tellg();
		list.seekg(0, list.beg);
		std::getline(list,sbuffer, '%');
		if (sbuffer != "ANIM")
			return -1;
		
		std::getline(list, sbuffer, '%');
		sbuffer.erase(remove(sbuffer.begin(), sbuffer.end(), '\r'), sbuffer.end());
		sbuffer.erase(remove(sbuffer.begin(), sbuffer.end(), '\n'), sbuffer.end());
		std::vector<std::string> params = split(sbuffer, ':');//
		if (params.at(0) != "PARAM")
			return -1;

		framewidth = atoi(params.at(1).c_str());
		frameheight = atoi(params.at(2).c_str());
		xdistancebetweensprites = atoi(params.at(3).c_str());
		ydistancebetweensprites = atoi(params.at(4).c_str());
		xstartoffset = atoi(params.at(5).c_str());
		ystartoffset = atoi(params.at(6).c_str());

		std::getline(list, sbuffer , '%');
		sbuffer.erase(remove(sbuffer.begin(), sbuffer.end(), '\r'), sbuffer.end());
		sbuffer.erase(remove(sbuffer.begin(), sbuffer.end(), '\n'), sbuffer.end());
		std::vector<std::string> sprites = split(sbuffer, ':');//
		if (sprites.at(0) != "SPRT")
			return -1;
		sprites.erase(sprites.begin());
		int framenum = 0;
		for (auto& sprtname : sprites) {
			spriteList[sprtname] = framenum;
			framenum++;
		}

		std::getline(list, sbuffer, '%');
		sbuffer.erase(remove(sbuffer.begin(), sbuffer.end(), '\r'), sbuffer.end());
		sbuffer.erase(remove(sbuffer.begin(), sbuffer.end(), '\n'), sbuffer.end());
		if (sbuffer != "ANIMHEADER")
			return -1;


		int spriteListNum = 0;
		std::getline(list, sbuffer);

		while (std::getline(list, sbuffer, '%')) {
			sbuffer.erase(remove(sbuffer.begin(), sbuffer.end(), '\r'), sbuffer.end());
			sbuffer.erase(remove(sbuffer.begin(), sbuffer.end(), '\n'), sbuffer.end());
			std::vector<std::string> AnimationSheet = split(sbuffer, ':');// Animation1:sprite1:sprite2:sprite3:
			for (auto& animname : AnimationSheet) {
				AnimationList[AnimationSheet.at(0)].push_back(spriteList[animname]);
			}
			AnimationList[AnimationSheet.at(0)].erase(AnimationList[AnimationSheet.at(0)].begin());
			
		}
		



		return 0;
	}
	

};

#endif