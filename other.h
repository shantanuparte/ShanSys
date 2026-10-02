#ifndef OTHER
#define OTHER

int get_battery_percentage();
double get_load_avg();
void get_uptime(char *buff, size_t size);
void get_net_bytes(const char *iface, unsigned long *rx, unsigned long *tx);
void draw_bar(int y, int x, int width, double percentage);
void draw_header_sec(int y, const char *title, int width);
#endif