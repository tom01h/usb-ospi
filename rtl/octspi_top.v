module octspi_top(
    input  wire clkin,
    input  wire reset_n,
    (* mark_debug = "true" *) input  wire ncs,
    inout  wire dqs,
    inout  wire [7:0] data,
    output reg [1:0] ledout
);

(* mark_debug = "true" *) wire dqs_i;
(* mark_debug = "true" *) wire dqs_o;
wire dqs_t;
(* mark_debug = "true" *) wire [7:0] data_i;
reg [7:0] data_o;
wire data_t;

IOBUF dqs_io (.IO(dqs), .O(dqs_i), .I(dqs_o), .T(dqs_t));
IOBUF dqs_d0 (.IO(data[0]), .O(data_i[0]), .I(data_o[0]), .T(data_t));
IOBUF dqs_d1 (.IO(data[1]), .O(data_i[1]), .I(data_o[1]), .T(data_t));
IOBUF dqs_d2 (.IO(data[2]), .O(data_i[2]), .I(data_o[2]), .T(data_t));
IOBUF dqs_d3 (.IO(data[3]), .O(data_i[3]), .I(data_o[3]), .T(data_t));
IOBUF dqs_d4 (.IO(data[4]), .O(data_i[4]), .I(data_o[4]), .T(data_t));
IOBUF dqs_d5 (.IO(data[5]), .O(data_i[5]), .I(data_o[5]), .T(data_t));
IOBUF dqs_d6 (.IO(data[6]), .O(data_i[6]), .I(data_o[6]), .T(data_t));
IOBUF dqs_d7 (.IO(data[7]), .O(data_i[7]), .I(data_o[7]), .T(data_t));



(* mark_debug = "true" *) wire clkin_int;
wire clk;

IBUFG userbuf0 (.I(clkin), .O(clkin_int));
BUFG userbuf1 (.I(clkin_int), .O(clk));

wire reset;
assign reset = ~reset_n;

(* mark_debug = "true" *) reg [2:0] cmd_cnt;
(* mark_debug = "true" *) reg [1:0] dmy_cnt;
(* mark_debug = "true" *) reg [7:0] data_cnt;

(* mark_debug = "true" *) reg [7:0] cmd;
(* mark_debug = "true" *) reg [7:0] size;
(* mark_debug = "true" *) reg [23:0] address;

localparam CMD_LEN = 5;
localparam DMY_LEN = 2;
localparam DATA_LEN = 512*512;

localparam COMMAND_WRITE = 'hA0;
localparam COMMAND_READ = 'h20;

always @(posedge clk) begin
    if(~reset_n) begin
        cmd_cnt <= 0;
        dmy_cnt <= 0;
        data_cnt <= 0;
    end else if(ncs) begin
    end else if(cmd_cnt != CMD_LEN) begin
        cmd_cnt <= cmd_cnt + 1;
        casez(cmd_cnt)
            0: cmd <= data_i;
            1: size <= data_i;
            2: address[23:16] <= data_i;
            3: address[15: 8] <= data_i;
            4: address[ 7: 0] <= data_i;
        endcase
    end else if((dmy_cnt != DMY_LEN) && (cmd == COMMAND_READ)) begin
        dmy_cnt <= dmy_cnt + 1;
    //end else if((data_cnt != size+1) && (cmd == COMMAND_READ) || (data_cnt != size) && (cmd == COMMAND_WRITE)) begin
    end else if((data_cnt != size) && (cmd == COMMAND_READ) || (data_cnt != size) && (cmd == COMMAND_WRITE)) begin
        data_cnt <= data_cnt + 1;
        address <= address + 1;
    end else begin
        cmd_cnt <= 0;
        dmy_cnt <= 0;
        data_cnt <= 0;
    end
end

wire write;
wire read;
wire dummy;
(* mark_debug = "true" *) reg hold_state;
assign write = (cmd_cnt == CMD_LEN) && (cmd == COMMAND_WRITE);
assign read = (cmd_cnt == CMD_LEN) && (dmy_cnt == DMY_LEN) && (cmd == COMMAND_READ);
assign dummy = (cmd_cnt == CMD_LEN) && (dmy_cnt != DMY_LEN) && (cmd == COMMAND_READ);

assign dqs_t = ~(read || dummy || hold_state);
//assign dqs_o = (read) & data_cnt[0];
assign dqs_o = ~clk;
assign data_t = ~(read || hold_state);

always @(posedge clk or posedge ncs) begin
    if(ncs) begin
        hold_state <= 1'b0;
    end else if(dummy) begin
        hold_state <= 1'b1;
    end    
end

reg [7:0] ram [0:DATA_LEN-1];

always @(posedge clk) begin
    if(write) begin
        ram[address] <= data_i;
    end
    data_o <= ram[address];
end
    
always @(posedge clk) begin
    if(reset) begin
        ledout <= 2'b0;
    end
end

endmodule