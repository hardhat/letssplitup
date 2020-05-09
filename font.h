#ifndef FONT_H
#define FONT_H

enum FontId {
	FONT_HEADLINE,
	FONT_BODY,
	FONT_LABEL,
	FONT_NOTICE,
};

void initFont();
void extentMessage(FontId fontId,const char *message,int &w,int &h);
void drawMessage(FontId fontId,const char *message,int x,int y);

#endif
