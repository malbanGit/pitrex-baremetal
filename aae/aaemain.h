#ifndef AAEMAIN_H
#define AAEMAIN_H
int mystrcmp(const char *s1, const char *s2);
void sort_games(void);
void run_game(void);
void reset_to_default_keys();
void ListDisplaySettings(void);
void SetGammaRamp(double gamma, double bright, double contrast);
void reset_for_new_game();
void test_keys();
#endif