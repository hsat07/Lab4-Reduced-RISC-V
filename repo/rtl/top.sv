module top #(
    parameter int DATA_WIDTH = 32
) (
    input  logic                  clk,
    input  logic                  rst,
    output logic [DATA_WIDTH-1:0] a0
);

    logic [DATA_WIDTH-1:0] pc, pc_next;
    logic [DATA_WIDTH-1:0] instr;
    logic [DATA_WIDTH-1:0] imm_ext;
    logic                  pc_src, reg_write, alu_src, imm_src;
    logic [2:0]            alu_ctrl;
    logic                  eq;

    /* verilator lint_off UNUSEDSIGNAL */
    logic [DATA_WIDTH-1:0] pc_plus4, pc_target;
    /* verilator lint_on UNUSEDSIGNAL */

    pc_reg #(.XLEN(DATA_WIDTH)) pc_register (
        .clk(clk), .rst(rst), .pc_next(pc_next), .pc(pc)
    );

    next_pc #(.XLEN(DATA_WIDTH)) next_pc_logic (
        .pc_src   ({1'b0, pc_src}),
        .pc       (pc),
        .imm_ext  (imm_ext),
        .alu_res  ('0),
        .pc_plus4 (pc_plus4),
        .pc_target(pc_target),
        .pc_next  (pc_next)
    );

    instr_mem imem (
        .addr(pc), .dout(instr)
    );

    ctrl_unit control (
        .op       (instr[6:0]),
        .funct3   (instr[14:12]),
        .zero     (eq),
        .pc_src   (pc_src),
        .reg_write(reg_write),
        .alu_src  (alu_src),
        .imm_src  (imm_src),
        .alu_ctrl (alu_ctrl)
    );

    sign_extend sext (
        .din(instr), .immsrc(imm_src), .immOp(imm_ext)
    );

    top_datapath datapath (
        .clk     (clk),
        .rs1     (instr[19:15]),
        .rs2     (instr[24:20]),
        .rd      (instr[11:7]),
        .RegWrite(reg_write),
        .ALUsrc  (alu_src),
        .ALUctrl (alu_ctrl),
        .ImmOp   (imm_ext),
        .a0      (a0),
        .EQ      (eq)
    );

endmodule
