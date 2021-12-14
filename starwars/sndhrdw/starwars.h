
int  starwars_sh_start(void);
void starwars_sh_stop(void);
void starwars_sh_update(void);

void starwars_pokey_sound_w(int offset,int data);
void starwars_pokey_ctl_w(int offset,int data);

int main_read_r(int);
int main_ready_flag_r(int);
void main_wr_w(int, int);
void soundrst(int, int);

int sin_r(int);
int m6532_r(int);

void sout_w(int, int);
void m6532_w(int, int);

int starwars_snd_interrupt(void);

