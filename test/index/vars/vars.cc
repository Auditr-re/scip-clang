// extra-args: -std=c++17

int MyGlobal = 3;

namespace n {
  int otherGlobal = 0;
}

int f(int x_, int y_);

int f(int x, int y) {
  static int z = x + y;
  int arr[2] = {x, y};
  auto [a, b] = arr;
  return z + a + b + MyGlobal + n::otherGlobal;
}

struct S {
  int x;
  static int y;
};

int f(S s) {
  return s.x + S::y;
}

void lambdas() {
  int x = 0;
  int y = 1;
  auto add = [&x, y](int z) mutable {
    y += z;
    x += y;
  };
}

void access() {
  int a = 0;
  int b = a;
  a = b;
  a++;
  --b;
  (a) = 1;
  int *p = &a;
  *p = 2;
  S *ps = nullptr;
  ps->x = a;
  ps->x += 1;
  S::y = 3;
}
