#include "base_testbench.h"

Vdut *top;
VerilatedVcdC *tfp;
unsigned int ticks = 0;

enum pc_src_e { 
    PLUS4 = 0b00, 
    BRANCH = 0b01, 
    JALR = 0b10 
};

class NextPCTestbench : public BaseTestbench {
    protected:
        void initializeInputs() override {
            top->pc_src = PLUS4;
            top->pc = 0;
            top->imm_ext = 0;
            top->alu_res = 0;
        }

        void set(uint8_t src, uint32_t pc, uint32_t imm = 0, uint32_t alu = 0) {
            top->pc_src = src;
            top->pc = pc;
            top->imm_ext = imm;
            top->alu_res = alu;
            
            top->eval();
        }
};

TEST_F(NextPCTestbench, Plus4) {
    set(PLUS4, 0x100);
    EXPECT_EQ(top->pc_plus4, 0x104u);
    EXPECT_EQ(top->pc_next,  0x104u);
}

TEST_F(NextPCTestbench, BranchForward) {
    set(BRANCH, 0x100, 0x20);
    EXPECT_EQ(top->pc_target, 0x120u);
    EXPECT_EQ(top->pc_next,   0x120u);
}

TEST_F(NextPCTestbench, BranchBackward) {
    set(BRANCH, 0x100, (uint32_t) -4);
    EXPECT_EQ(top->pc_next, 0xFCu);
}

// TEST_F(NextPCTestbench, JalrClearsBit0) {
//     set(JALR, 0x100, 0, 0x205);
//     EXPECT_EQ(top->pc_next, 0x204u);
// }

// TEST_F(NextPCTestbench, JalrIgnoresImmAdder) {
//     set(JALR, 0x100, 0x40, 0x300);
//     EXPECT_EQ(top->pc_next, 0x300u);
// }

TEST_F(NextPCTestbench, WrapsAround) {
    set(PLUS4, 0xFFFFFFFC);
    EXPECT_EQ(top->pc_next, 0u);
}
