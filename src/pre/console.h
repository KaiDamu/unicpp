#pragma once

#define ConWrite(...) { printf(__VA_ARGS__); printf("\n"); }
#define ConWriteInfo(...) { printf("[INFO] "); ConWrite(__VA_ARGS__); }
#define ConWriteErr(...) { printf("[ERROR] "); ConWrite(__VA_ARGS__); }

dfa ER ConPause();
