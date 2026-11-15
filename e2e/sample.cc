// Each line marked "// expect:" states the access roles scip-clang should
// emit for the reference(s) of the named symbol on that line.
struct Point {
  int x;
  int y;
  static int count;
  Point &operator+=(const Point &o) { return *this; }
};


int global = 0;

int run(Point p, Point q) {
  int a = 0;
  int b = a;      // expect: a read
  a = b;          // expect: a write, b read
  a++;            // expect: a read+write
  --b;            // expect: b read+write
  a += b;         // expect: a read+write, b read
  (a) = 1;        // expect: a write (parentheses ignored)
  p.x = q.y;      // expect: x write, y read
  p.y *= 2;       // expect: y read+write
  Point::count = 3; // expect: count write (declared, not defined: forward-decl path)
  global = a + b; // expect: global write, a read, b read
  p += q;         // expect: p read+write (overloaded operator)
  int *ptr = &a;  // expect: a unknown (address taken)
  *ptr = 2;       // expect: ptr read
  return global;  // expect: global read
}

void takesConst(const int &v);
void takesMut(int &v);
void takesPoint(Point v);

struct Holder {
  int v;
  Holder(int x) : v(x) {} // expect: v write, x read
  int get() const { return v; }
};

int more(Point p, Holder h) {
  int i = 0;
  int arr[3] = {i, i, i}; // expect: i read (braced initializer)
  arr[i] = 1;             // expect: arr write, i read
  p.x = 2;                // expect: p write, x write
  takesConst(i);          // expect: i read (const reference)
  takesMut(i);            // expect: i unknown (non-const reference)
  takesPoint(p);          // expect: p read (copy constructor)
  Point q = p;            // expect: p read
  int n = h.get();        // expect: h read (const method)
  Point r = {i, arr[0]};  // expect: i read, arr read
  return n + q.x + r.y;
}
