#include <iostream>
#include <vector>

// 测试类，包含各种控制流结构
class TestControlFlow {
public:
  // 测试寄存器与立即数比较
  int testRegisterImmediate(int x) {
    if (x > 10) {
      return 1;
    } else if (x == 0) {
      return 0;
    } else {
      return -1;
    }
  }

  // 测试两个寄存器比较
  int testRegisterRegister(int a, int b) {
    if (a > b) {
      return a;
    } else if (a < b) {
      return b;
    } else {
      return 0;
    }
  }

  // 测试 switch 语句
  int testSwitch(int value) {
    switch (value) {
      case 1:
        return 10;
      case 2:
        return 20;
      case 3:
        return 30;
      default:
        return -1;
    }
  }

  // 测试直接跳转（无条件分支）
  void testDirectJump(bool flag) {
    if (flag) {
      std::cout << "Flag is true" << std::endl;
      return; // 直接跳转到函数返回
    }
    std::cout << "Flag is false" << std::endl;
  }

  // 测试复杂控制流
  int testComplexFlow(int x, int y) {
    if (x > 5) {
      if (y > 10) {
        return 100;
      } else {
        switch (y) {
          case 1:
            return 101;
          case 2:
            return 102;
          default:
            return 103;
        }
      }
    } else {
      if (x == 0) {
        return 200;
      } else {
        return 201;
      }
    }
  }
};

// 模糊测试入口点
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  TestControlFlow test;
  
  // 使用输入数据生成测试值
  int x = data[0] % 20;
  int y = data[1] % 15;
  
  // 测试各种控制流结构
  test.testRegisterImmediate(x);
  test.testRegisterRegister(x, y);
  test.testSwitch(x % 5);
  test.testDirectJump(x % 2 == 0);
  test.testComplexFlow(x, y);
  
  return 0;
}
