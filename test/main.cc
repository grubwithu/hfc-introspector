#include <iostream>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <cstdint>

// =====================================================================
// 模拟一个复杂的哈希/加密函数 (用于测试 COMPLEX_BITWISE 约束)
// =====================================================================
uint32_t dummy_hash(const uint8_t* data, size_t len) {
    uint32_t hash = 5381;
    for (size_t i = 0; i < len; ++i) {
        // 密集的位操作：左移 (Shl)、加法、异或 (Xor)
        hash = ((hash << 5) + hash) ^ data[i]; 
    }
    return hash;
}

// =====================================================================
// 模糊测试目标函数 (Fuzz Target)
// =====================================================================
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // 保证输入长度足够，避免越界
    if (size < 24) {
        return 0;
    }

    // ---------------------------------------------------------
    // 噪音测试 (Noise): STL 库展开和异常处理
    // 你的 LLVM Pass 应该通过 DebugLoc 和 isEHPad() 过滤掉它们
    // ---------------------------------------------------------
    try {
        // 这行代码会在 LLVM IR 中生成几十个基本块 (内存分配、循环等)
        std::vector<std::vector<int>> noise_matrix(5, std::vector<int>(5, 0));
        
        if (data[0] == 0xFF) {
            throw std::runtime_error("Exception noise!"); // 会生成 invoke 和 landingpad
        }
    } catch (const std::exception& e) {
        // 异常处理基本块
    }

    // ---------------------------------------------------------
    // 类别 1: 精确匹配 (EXACT_MATCH) - CMPLOG 的最爱
    // ---------------------------------------------------------
    
    // 1.1 Magic Bytes 比较 (直接 Load 后与常量比较)
    uint32_t magic = *(uint32_t*)(data + 1);
    if (magic == 0xDEADBEEF) {
        std::cout << "Magic Bytes matched!\n";
    }

    // 1.2 字符串精确比较 (调用 strcmp)
    const char* str_payload = (const char*)(data + 5);
    if (std::strncmp(str_payload, "FUZZ", 4) == 0) {
        std::cout << "String matched!\n";
    }

    // 1.3 Switch 语句精确匹配
    uint8_t cmd = data[9];
    switch (cmd) {
        case 0x10: std::cout << "Command 10\n"; break;
        case 0x20: std::cout << "Command 20\n"; break;
        default: break;
    }

    // ---------------------------------------------------------
    // 类别 2: 算术与代数运算 (ARITHMETIC) - 符号执行的最爱
    // ---------------------------------------------------------
    
    int32_t x = data[10];
    int32_t y = data[11];
    // 包含乘法 (Mul)、减法 (Sub)，且最终进行 ICmp 比较
    if ((x * 3) - y == 42) {
        std::cout << "Arithmetic constraint solved!\n";
    }

    // ---------------------------------------------------------
    // 类别 3: 浮点数运算 (FLOATING_POINT) - 梯度下降的最爱
    // ---------------------------------------------------------
    
    // 将输入转换为浮点数并比较 (会生成 FCmpInst)
    float f_val = *(float*)(data + 12);
    if (f_val > 3.14159f && f_val < 3.15f) {
        std::cout << "Floating point constraint solved!\n";
    }

    // ---------------------------------------------------------
    // 类别 4: 位操作与哈希计算 (COMPLEX_BITWISE) - 全体 Fuzzer 短板
    // ---------------------------------------------------------
    
    // 调用复杂的位运算哈希函数，比较结果
    uint32_t target_hash = dummy_hash(data + 16, 8);
    if (target_hash == 0x12345678) {
        std::cout << "Complex Hash constraint solved!\n";
    }
    
    // 直接在判断里的位运算 (按位与 And)
    if ((data[20] & 0x0F) == 0x0A) {
        std::cout << "Bitwise AND constraint solved!\n";
    }

    return 0;
}
