#include "testbench.h"

unsigned int ticks = 0;

static const uint32_t RESET_ADDR = 0xBFC00000;

class PcRegTestbench : public Testbench {
    protected:
        void initializeInputs() override {
            top->clk = 0;
            top->rst = 0;
            top->pc_next = 0;
        }

        void reset() {
            top->rst = 1;
            runSimulation(1);
            top->rst = 0;
        }
};

TEST_F(PcRegTestbench, ResetLoadsResetVector) {
    top->pc_next = 0x1234;
    reset();
    EXPECT_EQ(top->pc, RESET_ADDR);
}

TEST_F(PcRegTestbench, LoadsPcNextOnClock) {
    reset();
    top->pc_next = RESET_ADDR + 4;
    runSimulation(1);
    EXPECT_EQ(top->pc, RESET_ADDR + 4);
}

TEST_F(PcRegTestbench, HoldsWithoutClockEdge) {
    reset();
    top->pc_next = 0xDEADBEEC;
    top->eval();
    EXPECT_EQ(top->pc, RESET_ADDR);
}

TEST_F(PcRegTestbench, SequenceOfUpdates) {
    reset();
    for (uint32_t i = 1; i <= 5; i++) {
        top->pc_next = top->pc + 4;
        runSimulation(1);
        EXPECT_EQ(top->pc, RESET_ADDR + 4 * i);
    }
}

TEST_F(PcRegTestbench, ResetOverridesPcNext) {
    reset();
    top->pc_next = 0x100;
    runSimulation(1);
    ASSERT_EQ(top->pc, 0x100u);

    top->rst = 1;
    top->pc_next = 0x200;
    runSimulation(1);
    EXPECT_EQ(top->pc, RESET_ADDR);
}

TEST_F(PcRegTestbench, ResetIsSynchronous) {
    reset();
    top->pc_next = 0x100;
    runSimulation(1);
    ASSERT_EQ(top->pc, 0x100u);

    top->rst = 1;
    top->eval();
    EXPECT_EQ(top->pc, 0x100u);
}
