#pragma once

class hud
{
public:

	hud(void);
	~hud(void);

	void run();
	void draw();

	void drawStatusPanel(bool glowPass);
	void drawRadarPanel(bool glowPass);
	void drawLivesPanel(bool glowPass);
	void drawReticule(bool glowPass, bool isTargeted);

private:

	float mRadarHeading;
	float mBlipBrightness;
};

