module top_datapath (
    input  logic        clk,
    input  logic [4:0]  rs1,
    input  logic [4:0]  rs2,
    input  logic [4:0]  rd,
    input  logic        RegWrite,
    input  logic        ALUsrc,
    input  logic [2:0]  ALUctrl,
    input  logic [31:0] ImmOp,

    output logic [31:0] a0,
    output logic        EQ
);

    logic [31:0] RD1, regOp2, ALUop2, ALUout;

    regfile rf (
        .AD1 (rs1),
        .AD2 (rs2),
        .AD3 (rd),
        .clk (clk),
        .WE3 (RegWrite),
        .WD3 (ALUout),
        .RD1 (RD1),
        .RD2 (regOp2),
        .a0  (a0)
    );

    mux #(.DATA_WIDTH(32)) alu_mux (
        .in0 (regOp2),
        .in1 (ImmOp),
        .sel (ALUsrc),
        .out (ALUop2)
    );

    alu alu0 (
        .ALUop1  (RD1),
        .ALUop2  (ALUop2),
        .ALUctrl (ALUctrl),
        .SUM     (ALUout),
        .EQ      (EQ)
    );

endmodule
