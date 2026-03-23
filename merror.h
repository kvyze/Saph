#pragma once

#define error(m) fprintf(stderr, m); exit(1);
#define merror(p, m) if (!p) { fprintf(stderr, "%s memory allocation error.", m); exit(1); }
