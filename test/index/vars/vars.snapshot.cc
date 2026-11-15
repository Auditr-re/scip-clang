  // extra-args: -std=c++17
//^^^^^^^^^^^^^^^^^^^^^^^^^ definition [..] `<file>/vars.cc`/
  
  int MyGlobal = 3;
//    ^^^^^^^^ definition [..] MyGlobal.
  
  namespace n {
//          ^ definition [..] n/
    int otherGlobal = 0;
//      ^^^^^^^^^^^ definition [..] n/otherGlobal.
  }
  
  int f(int x_, int y_);
//    ^ reference [..] f(9b79fb6aee4c0440).
//          ^^ definition local 0
//                  ^^ definition local 1
  
  int f(int x, int y) {
//    ^ definition [..] f(9b79fb6aee4c0440).
//          ^ definition local 2
//                 ^ definition local 3
    static int z = x + y;
//             ^ definition local 4
//                 ^ reference local 2
//                     ^ reference local 3
    int arr[2] = {x, y};
//      ^^^ definition local 5
//                ^ reference local 2
//                   ^ reference local 3
    auto [a, b] = arr;
//        ^ definition local 6
//           ^ definition local 7
//                ^^^ reference local 5
    return z + a + b + MyGlobal + n::otherGlobal;
//         ^ reference local 4
//             ^ reference local 6
//                 ^ reference local 7
//                     ^^^^^^^^ reference [..] MyGlobal.
//                                ^ reference [..] n/
//                                   ^^^^^^^^^^^ reference [..] n/otherGlobal.
  }
  
  struct S {
//       ^ definition [..] S#
    int x;
//      ^ definition [..] S#x.
    static int y;
//             ^ definition [..] S#y.
  };
  
  int f(S s) {
//    ^ definition [..] f(6871c211ea8bb0a1).
//      ^ reference [..] S#
//        ^ definition local 8
    return s.x + S::y;
//         ^ reference local 8
//           ^ reference [..] S#x.
//               ^ reference [..] S#
//                  ^ reference [..] S#y.
  }
  
  void lambdas() {
//     ^^^^^^^ definition [..] lambdas(49f6e7a06ebc5aa8).
    int x = 0;
//      ^ definition local 9
    int y = 1;
//      ^ definition local 10
    auto add = [&x, y](int z) mutable {
//       ^^^ definition local 11
//               ^ reference local 9
//                  ^ reference local 10
//                         ^ definition local 12
      y += z;
//    ^ reference (read+write) local 10
//         ^ reference local 12
      x += y;
//    ^ reference (read+write) local 9
//         ^ reference local 10
    };
  }
  
  void access() {
//     ^^^^^^ definition [..] access(49f6e7a06ebc5aa8).
    int a = 0;
//      ^ definition local 13
    int b = a;
//      ^ definition local 14
//          ^ reference local 13
    a = b;
//  ^ reference (write) local 13
//      ^ reference local 14
    a++;
//  ^ reference (read+write) local 13
    --b;
//    ^ reference (read+write) local 14
    (a) = 1;
//   ^ reference (write) local 13
    int *p = &a;
//       ^ definition local 15
//            ^ reference local 13
    *p = 2;
//   ^ reference local 15
    S *ps = nullptr;
//  ^ reference [..] S#
//     ^^ definition local 16
    ps->x = a;
//  ^^ reference local 16
//      ^ reference (write) [..] S#x.
//          ^ reference local 13
    ps->x += 1;
//  ^^ reference local 16
//      ^ reference (read+write) [..] S#x.
    S::y = 3;
//  ^ reference [..] S#
//     ^ reference (write) [..] S#y.
  }
