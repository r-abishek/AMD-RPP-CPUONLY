#define RPPABS(a)       ((a < 0) ? -(a) : a)
#define RPPMIN2(a,b)    ((a < b) ? a : b)
#define RPPMIN3(a,b,c)  ((a < b) && (a < c) ?  a : ((b < c) ? b : c))
#define RPPMAX2(a,b)    ((a > b) ? a : b)
#define RPPMAX3(a,b,c)  ((a > b) && (a > c) ?  a : ((b > c) ? b : c))