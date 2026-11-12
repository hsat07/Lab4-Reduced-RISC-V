module next_pc #(
    parameter int XLEN = 32
)(
    input  logic [1:0]      pc_src,
    input  logic [XLEN-1:0] pc,
    input  logic [XLEN-1:0] imm_ext,
    input  logic [XLEN-1:0] alu_res,
    output logic [XLEN-1:0] pc_plus4,
    output logic [XLEN-1:0] pc_target,
    output logic [XLEN-1:0] pc_next
);
    assign pc_plus4  = pc + 4;          // adder 1
    assign pc_target = pc + imm_ext;    // adder 2

    always_comb
        case (pc_src)
            2'b00:   pc_next = pc_plus4;
            2'b01:   pc_next = pc_target;
            2'b10:   pc_next = {alu_res[XLEN-1:1], 1'b0};   // jalr
            default: pc_next = pc_plus4;
        endcase
endmodule
