#include "base_testbench.h"
#include <fstream>
#include <vector>

unsigned int ticks = 0;

static const char *HEX_PATH = "../rtl/program.hex";

static const std::vector<uint32_t> program = {
    0x0ff00313,   // addi t1, zero, 0xff
    0x00000513,   // addi a0, zero, 0
    0x00000593,   // addi a1, zero, 0
    0x00058513,   // addi a0, a1, 0
    0x00158593,   // addi a1, a1, 1
    0xfe659ce3,   // bne  a1, t1, -8
    0xDEADBEEF,
    0x01234567,
};

class InstrMemTestbench : public BaseTestbench {
    protected:
        void initializeInputs() override {
            std::ofstream f(HEX_PATH);
            for (uint32_t word : program)
                for (int b = 0; b < 4; b++)
                    f << std::hex << ((word >> (8 * b)) & 0xFF) << "\n";

            f.close();

            top->addr = 0;
        }

        uint32_t fetch(uint32_t addr) {
            top->addr = addr;

            top->eval();
            return top->dout;
        }
};

TEST_F(InstrMemTestbench, ReadsEachWord) {
    for (size_t i = 0; i < program.size(); i++) {
        SCOPED_TRACE("word " + std::to_string(i));
        EXPECT_EQ(fetch(4 * i), program[i]);
    }
}

TEST_F(InstrMemTestbench, LittleEndian) {
    EXPECT_EQ(fetch(0), 0x0ff00313u);    // bytes in file: 13 03 f0 0f
}

TEST_F(InstrMemTestbench, AllBytesDistinct) {
    EXPECT_EQ(fetch(4 * 7), 0x01234567u);
}

TEST_F(InstrMemTestbench, ResetVectorAddress) {
    // compile.sh links the program at 0xBFC00000, so the PC starts there
    EXPECT_EQ(fetch(0xBFC00000), program[0]);
    EXPECT_EQ(fetch(0xBFC00004), program[1]);
    EXPECT_EQ(fetch(0xBFC00014), program[5]);
}

TEST_F(InstrMemTestbench, ReadIsCombinational) {
    fetch(0);
    EXPECT_EQ(fetch(8), program[2]);
    EXPECT_EQ(fetch(4), program[1]);
}
