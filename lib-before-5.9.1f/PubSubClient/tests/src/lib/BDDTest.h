#ifndef bddtest_h
#define bddtest_h

void bddtest_suite(const char* name);
int bddtest_test(const char*, int, const char*, int);
void bddtest_start(const char*);
void bddtest_end();
int bddtest_summary();

#define SUITE(x) { bddtest_suite(x); }
#define TEST(x) { if (!bddtest_test(__FILE__, __LINE__, #x, (x))) return false;  }

#define IT(x) { bddtest_start(x); }
#define END_IT { bddtest_end();return true;}

#define FINISH { return bddtest_summary(); }

#define IS_TRUE(x) TEST(x)
#define IS_FALSE(x) TEST(!(x))
#define IS_EQUAL(x,y) TEST(x==y)
#define IS_NOT_EQUAL(x,y) TEST(x!=y)

#endif
