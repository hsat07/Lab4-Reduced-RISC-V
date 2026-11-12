module instr_mem (
    input  logic [31:0] addr,
    output logic [31:0] dout
);

    logic [7:0] rom [255:0];
    logic [7:0] a;

    initial begin
        $readmemh("../rtl/program.hex", rom);
    end

    assign a = addr[7:0];
    assign dout = {rom[a + 8'd3], rom[a + 8'd2], rom[a + 8'd1], rom[a]};

endmodule
