module pc_reg #(
    parameter int XLEN = 32,
    parameter logic [XLEN-1:0] RESET_ADDR = 32'hBFC00000
)(
    input  logic            clk,
    input  logic            rst,
    input  logic [XLEN-1:0] pc_next,
    output logic [XLEN-1:0] pc
);
    always_ff @(posedge clk)
        if (rst) pc <= RESET_ADDR;
        else     pc <= pc_next;
endmodule
