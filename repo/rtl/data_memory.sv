module data_memory #(
    parameter ADDRESS_WIDTH = 15,
              DATA_WIDTH = 32
)(

    input logic clk,
    input logic [3:0] wr_en,
    input logic rst,
    input logic [ADDRESS_WIDTH-1:0] wr_addr,
    input logic [ADDRESS_WIDTH-1:0] rd_addr,
    input logic [DATA_WIDTH-1:0] din,
    output logic [DATA_WIDTH-1:0] dout

);

    logic [DATA_WIDTH-1:0] ram_array [2**ADDRESS_WIDTH-1:0];
    
    initial begin
        $readmemh("../rtl/data.hex", ram_array);
    end
    
    always_ff @(posedge clk) begin
        if (rst) begin
            dout <= {DATA_WIDTH{1'b0}};
        end
        else begin
            dout <= ram_array[rd_addr];
            
            for(int i = 0; i < 4; i++) begin
                if (wr_en[i]) begin
                    ram_array[wr_addr][8*i +: 8] <= din[8*i +: 8];
                end    
            end
        end
    end
endmodule

